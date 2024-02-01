#include "ASGEGame.hpp"
#include "EndState.hpp"
#include <Engine/FileIO.hpp>
#include <Engine/Logger.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

/**
 * @brief Initialises the game.
 * Setup your game and initialise the core components.
 * @param settings
 */
ASGENetGame::ASGENetGame(const ASGE::GameSettings& settings) :
  OGLGame(settings), game_font(renderer->loadFont("/data/fonts/cour.ttf", 42, 5)) /*,
   game_width(static_cast<float>(settings.window_width)),
   game_height(static_cast<float>(settings.window_height))*/
{
  renderer->setBaseResolution(1920, 1080, ASGE::Resolution::Policy::MAINTAIN);
  key_callback_id     = inputs->addCallbackFnc(ASGE::E_KEY, &ASGENetGame::keyHandler, this);
  inputs->use_threads = false;

  players.reserve(3);

  auto& player1 = players.emplace_back(std::make_unique<Player>());
  player1       = std::make_unique<Player>();
  player1->createNewCharacter(Character::HUMAN);
  player1->getCharacter()->sprite = renderer->createUniqueSprite();
  player1->getCharacter()->sprite->loadTexture("/data/sprites/playerSpriteSheet.png");
  player1->getCharacter()->sprite->srcRect()[0] = 0;
  player1->getCharacter()->sprite->srcRect()[1] = 0;
  player1->getCharacter()->sprite->srcRect()[2] = 32;
  player1->getCharacter()->sprite->srcRect()[3] = 32;
  player1->getCharacter()->sprite->width(32);
  player1->getCharacter()->sprite->height(32);
  player1->setScale(static_cast<float>(SCALE) * 1.2F);
  player1->setPosition(ASGE::Point2D(
    static_cast<float>(64 * SCALE) * static_cast<float>(SCALE),
    static_cast<float>(64 * SCALE) * static_cast<float>(SCALE) * 2));
  player1->getCharacter()->sprite->setGlobalZOrder(100);

  auto& player2 = players.emplace_back(std::make_unique<Player>());
  player2       = std::make_unique<Player>();
  player2->createNewCharacter(Character::ANIMAL);
  player2->getCharacter()->sprite = renderer->createUniqueSprite();
  player2->getCharacter()->sprite->loadTexture("/data/sprites/animalSpriteSheet.png");
  player2->getCharacter()->sprite->srcRect()[0] = 0;
  player2->getCharacter()->sprite->srcRect()[1] = 0;
  player2->getCharacter()->sprite->srcRect()[2] = 32;
  player2->getCharacter()->sprite->srcRect()[3] = 32;
  player2->getCharacter()->sprite->width(32);
  player2->getCharacter()->sprite->height(32);
  player2->setScale(static_cast<float>(SCALE) * 1.2F);
  player2->setPosition(
    ASGE::Point2D(64 * 3 * static_cast<float>(SCALE), 64 * 2 * static_cast<float>(SCALE) * 2));
  player2->getCharacter()->sprite->setGlobalZOrder(100);

  camera_one_label.setFont(*game_font).setString("Camera 1").setPosition({ 0, 55 }).setScale(1.5);
  camera_two_label.setFont(*game_font).setString("Camera 2").setPosition({ 960, 55 }).setScale(1.5);

  main_Camera.lookAt(ASGE::Point2D(1920 / 2, 1080 / 2));
  initAudio();
  loadBackground();
  tilemap        = std::make_unique<TileMap>();
  tilemap->scale = SCALE;
  loadTileMap();

  player1->setCenter(*tilemap->spawnpoints[0]);
  player2->setCenter(*tilemap->spawnpoints[1]);

  /*WallUP();*/
  game_states.reserve(4);

  ASGE::Font* new_font = const_cast<ASGE::Font*>(game_font);
  auto& menu           = game_states.emplace_back(
    std::make_unique<MenuState>(players[0].get(), players[1].get(), new_font));
  menu->renderer = renderer.get();
  menu->init();

  auto& pause = game_states.emplace_back(
    std::make_unique<PauseState>(players[0].get(), players[1].get(), new_font));
  pause->renderer = renderer.get();
  pause->init();

  auto& gameplay = game_states.emplace_back(
    std::make_unique<GameplayState>(players[0].get(), players[1].get(), new_font));
  gameplay->renderer                                    = renderer.get();
  dynamic_cast<GameplayState*>(gameplay.get())->tilemap = tilemap.get();

  auto* dynamic_gameplay = dynamic_cast<GameplayState*>(gameplay.get());
  auto& ui_overlay =
    dynamic_gameplay->renderable_sprites.emplace_back(renderer->createUniqueSprite());
  ui_overlay->loadTexture("/data/img/GamePlay_UI.png");
  auto& human_icon =
    dynamic_gameplay->renderable_sprites.emplace_back(renderer->createUniqueSprite());
  human_icon->loadTexture("/data/img/Human_Sprite.png");
  auto& animal_icon =
    dynamic_gameplay->renderable_sprites.emplace_back(renderer->createUniqueSprite());
  animal_icon->loadTexture("/data/img/Creature_Sprite.png");

  gameplay->init();

  auto& end = game_states.emplace_back(
    std::make_unique<EndState>(players[0].get(), players[1].get(), new_font));
  end->renderer = renderer.get();
  end->init();
}

/**
 * Destroys the game.
 */
ASGENetGame::~ASGENetGame()
{
  this->inputs->unregisterCallback(key_callback_id);
  audio_engine.deinit();
}

void ASGENetGame::initAudio()
{
  audio_engine.init();
  ASGE::FILEIO::File bg_audio_file;
  if (bg_audio_file.open("/data/audio/Stellardrone - Light Years.mp3"))
  {
    auto buffer = bg_audio_file.read();
    auto length = static_cast<unsigned int>(buffer.length);
    background_audio.loadMem(buffer.as_unsigned_char(), length, false, false);
    background_audio.setLooping(true);
    background_audio.setVolume(0.3F);
    audio_engine.play(background_audio);

    /* Music is used under creative commons and is available from:
     * https://freemusicarchive.org/music/Stellardrone/Light_Years_1227/04_Light_Years*/
  }
}

void ASGENetGame::loadBackground()
{
  background = renderer->createUniqueSprite();
  background->loadTexture(("/data/img/background.png"));
  background->xPos(-400);
  background->yPos(-400);
  background->setGlobalZOrder(-60);
}

void ASGENetGame::loadTileMap()
{
  ASGE::FILEIO::File tile_map;
  if (tile_map.open(tilemap->filepath))
  {
    ASGE::FILEIO::IOBuffer buffer = tile_map.read();
    std::string file_string(buffer.as_char(), buffer.length);

    map.loadFromString(file_string, ".");

    for (const auto& layer : map.getLayers())
    {
      if (layer->getType() == tmx::Layer::Type::Tile)
      {
        storeTiles(*layer);
      }

      if (layer->getType() == tmx::Layer::Type::Object)
      {
        if (layer->getName() == "Collision")
        {
          tilemap->storeCollisions(*layer);
        }
        else if (layer->getName() == "Spawns")
        {
          tilemap->storeSpawns(*layer);
        }
        else if (layer->getName() == "Objective")
        {
          tilemap->storeObjectives(*layer);
        }
        else
        {
          tilemap->handleObjects(*layer);
        }
      }
    }
  }
}

void ASGENetGame::storeTiles(tmx::Layer& layer)
{
  auto tile_layer = layer.getLayerAs<tmx::TileLayer>();
  int y           = 0;
  for (unsigned int row = 0; row < layer.getSize().y; ++row)
  {
    int x = 0;
    for (unsigned int col = 0; col < layer.getSize().x; ++col)
    {
      auto tile_info   = tile_layer.getTiles()[row * tile_layer.getSize().x + col];
      const auto* tile = map.getTilesets()[0].getTile(tile_info.ID);
      if (tile != nullptr)
      {
        auto& sprite = tilemap->tiles.emplace_back(renderer->createUniqueSprite());
        if (sprite->loadTexture(tile->imagePath))
        {
          sprite->srcRect()[0] = static_cast<float>(tile->imagePosition.x);
          sprite->srcRect()[1] = static_cast<float>(tile->imagePosition.y);
          sprite->srcRect()[2] = static_cast<float>(tile->imageSize.x);
          sprite->srcRect()[3] = static_cast<float>(tile->imageSize.y);

          sprite->width(static_cast<float>(tile->imageSize.x));
          sprite->height(static_cast<float>(tile->imageSize.y));

          sprite->scale(static_cast<float>(tilemap->scale));
          sprite->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);

          sprite->yPos(static_cast<float>(y));
          sprite->xPos(static_cast<float>(x));
        }
      }
      x += 32 * tilemap->scale;
    }
    y += 32 * tilemap->scale;
  }
}

/**
 * @brief Processes key inputs.
 * This function is added as a callback to handle the game's
 * keyboard input. For this game, calls to this function
 * are not thread safe, so you may alter the game's state
 * but your code needs to designed to prevent data-races.
 *
 * @param data
 * @see KeyEvent
 */
void ASGENetGame::keyHandler(ASGE::SharedEventData data)
{
  const auto* key  = dynamic_cast<const ASGE::KeyEvent*>(data.get());
  keymap[key->key] = key->action != ASGE::KEYS::KEY_RELEASED;

  if (key->key == ASGE::KEYS::KEY_ESCAPE)
  {
    signalExit();
  }
}

/**
 * @brief FixedUpdate
 * Calls to this function use the same fixed timestep
 * irrespective of how much time is passed. If you want
 * deterministic behaviour on clients, this is the place
 * to go.
 *
 * https://gamedev.stackexchange.com/questions/1589/when-should-i-use-a-fixed-or-variable-time-step
 * "Use variable timesteps for your fixed steps for physics"
 * @param us
 */
void ASGENetGame::fixedUpdate(const ASGE::GameTime& us)
{
  Game::fixedUpdate(us);

  game_states[state_index]->fixedUpdate(us);
}

/**
 * @brief Updates your game and all it's components.
 * @param us
 */
void ASGENetGame::update(const ASGE::GameTime& us)
{
  for (auto& gamepad : inputs->getGamePads())
  {
    if (gamepad.is_connected)
    {
      if (gamepad_state.contains(gamepad.idx))
      {
        game_states[static_cast<unsigned long long>(state_index)]->inputHandler(gamepad);
        /*if ((gamepad.idx == 0 && player_1) || (gamepad.idx == 1 && !player_1))
        {
          if (
            gamepad.axis[0] > 0.1F || gamepad.axis[0] < -0.1F || gamepad.axis[1] > 0.1F ||
            gamepad.axis[1] < -0.1F)
          {
            velocity.x = gamepad.axis[ASGE::GAMEPAD::AXIS_LEFT_X];
            velocity.y = gamepad.axis[ASGE::GAMEPAD::AXIS_LEFT_Y];
          }
          else
          {
            velocity.x = 0;
            velocity.y = 0;
          }
        }
        else if ((gamepad.idx == 1 && player_1) || (gamepad.idx == 0 && !player_1))
        {
          if (
            gamepad.axis[0] > 0.1F || gamepad.axis[0] < -0.1F || gamepad.axis[1] > 0.1F ||
            gamepad.axis[1] < -0.1F)
          {
            velocity2.x = gamepad.axis[ASGE::GAMEPAD::AXIS_LEFT_X];
            velocity2.y = gamepad.axis[ASGE::GAMEPAD::AXIS_LEFT_Y];
          }
          else
          {
            velocity2.x = 0;
            velocity2.y = 0;
          }
        }*/

        /*if (
          ((gamepad.buttons[ASGE::GAMEPAD::BUTTON_X]) == 0U) &&
          ((gamepad_state.at(gamepad.idx).buttons[ASGE::GAMEPAD::BUTTON_X]) != 0U))
        {
          Logging::DEBUG("X PRESSED: " + std::to_string(gamepad.idx));

          player_1 = !player_1;

          velocity.x  = 0;
          velocity.y  = 0;
          velocity2.x = 0;
          velocity2.y = 0;
          // velocity.y *= 10;
        }*/
      }
    }
  }

  game_states[static_cast<unsigned long long>(state_index)]->update(us);

  int connected = 0;
  // retrieve all connected gamepads and store their states
  for (auto& gamepad : inputs->getGamePads())
  {
    connected++;
    gamepad_state.insert_or_assign(gamepad.idx, gamepad);
    game_states[static_cast<unsigned long long>(state_index)]->updateGamepadState(gamepad);
  }
  game_states[static_cast<unsigned long long>(state_index)]->updateConnected(connected);

  stateTransition();

  if (!game_states[state_index]->initialized)
  {
    if (game_states[state_index]->getStateID())
    {
      dynamic_cast<EndState*>(game_states[state_index].get())->updateTexts();
    }
  }
}

/**
 * @brief Render your game and its scenes here.
 * @param us
 */
void ASGENetGame::render(const ASGE::GameTime& /*us*/)
{
  // example of split screen. just remove viewports and use
  // a single camera if you don't require the use of split screen

  if (game_states[state_index]->getStateID() == State::GAMEPLAY)
  {
    auto* state = dynamic_cast<GameplayState*>(game_states[state_index].get());
    if (state != nullptr)
    {
      auto amount_of_cameras = state->getNumberOfCameras();
      int step               = 1920 / amount_of_cameras;
      int camera_index       = 0;

      for (auto& camera : *state->getCameras())
      {
        renderer->setViewport(ASGE::Viewport{ camera_index * step, 0, 960, 1080 });
        renderer->setProjectionMatrix(camera->getView());

        renderer->render(*background);
        for (auto& tile : tilemap->tiles)
        {
          renderer->render(*tile);
        }
        for (auto& temp : temp_sprites)
        {
          renderer->render(*temp);
        }
        for (auto& player : players)
        {
          renderer->render(*player->getCharacter()->sprite);
        }

        camera_index++;
      }

      for (auto& player : players)
      {
        if (player->getCharacter()->getCharacterType() == Character::HUMAN)
        {
          auto& camera =
            *state->getCameras()->at(static_cast<unsigned long long>(player->getCamera()));

          renderer->setViewport(ASGE::Viewport{ player->getCamera() * step, 0, 960, 1080 });
          renderer->setProjectionMatrix(camera.getView());
        }
      }

      for (auto& minigame_sprite : *state->getMinigameSprites())
      {
        for (auto& player : players)
        {
          if (
            player->getCharacter()->getCharacterType() == Character::HUMAN &&
            player->getCharacter()->isInMinigame())
          {
            renderer->render(*minigame_sprite);
          }
        }
      }
    }
  }
  renderer->setViewport(ASGE::Viewport{ 0, 0, 1920, 1080 });
  renderer->setProjectionMatrix(main_Camera.getView());

  renderer->render(*background);
  if (game_states[state_index]->getRenderableText() != nullptr)
  {
    for (auto& renderable : *game_states[state_index]->getRenderableText())
    {
      renderer->render(*renderable);
    }
  }
  if (game_states[state_index]->getRenderableSprites() != nullptr)
  {
    for (auto& renderable : *game_states[state_index]->getRenderableSprites())
    {
      renderer->render(*renderable);
    }
  }
}

bool ASGENetGame::collisionCheck(Boundary boundary)
{
  return tilemap->checkCollision(boundary);
}

void ASGENetGame::moveShip(ASGE::Sprite ship_moving, ASGE::GameTime& us, ASGE::Point2D vel)
{
  ship_moving.xPos(static_cast<float>(ship_moving.xPos() + vel.x * us.deltaInSecs()));
  ship_moving.yPos(static_cast<float>(ship_moving.yPos() + vel.y * us.deltaInSecs()));
}

/// Debug function to give a visual display to the wall's collision boxes
/*void ASGENetGame::WallUP()
{
  for (auto& collision : tilemap->collisions)
  {
    auto& sprite = temp_sprites.emplace_back(renderer->createUniqueSprite());
    sprite->loadTexture("/data/sprites/player_ship.png");
    sprite->width(collision->width);
    sprite->height(collision->height);
    sprite->xPos(collision->position.x);
    sprite->yPos(collision->position.y);
    sprite->setGlobalZOrder(999);
  }


}*/

unsigned long long ASGENetGame::FindStateByID(State::StateID idx)
{
  for (unsigned long long i = 0; i < game_states.size(); i++)
  {
    if (game_states[i]->getStateID() == idx)
    {
      return i;
    }
  }
  return 99;
}

void ASGENetGame::stateTransition()
{
  State::StateID new_state         = game_states[state_index]->getNextState();
  unsigned long long current_index = state_index;
  if (new_state != game_states[state_index]->getStateID())
  {
    game_states[state_index]->resetNextState();
    state_index = FindStateByID(new_state);
    if (state_index == 99)
    {
      state_index = current_index;
    }
  }
}