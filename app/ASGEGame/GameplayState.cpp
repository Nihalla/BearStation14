
#include "GameplayState.hpp"
#include <Engine/FileIO.hpp>

GameplayState::GameplayState(Player* player_1, Player* player_2, ASGE::Font* font) :
  State(player_1, player_2, font)
{
  state_id   = GAMEPLAY;
  next_state = GAMEPLAY;

  camera_vector.reserve(10);
  modular_texts.reserve(10);
  modular_strings.reserve(10);
}

GameplayState::~GameplayState() {}

void GameplayState::init()
{
  initialized = true;
  setUpCamera();
  cameraUpdate();

  initModText(
    "0:00",
    2.8F,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.5F,
      static_cast<float>(renderer->resolution().base[1]) * 0.13F));

  initModText(
    "0",
    2.8F,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.1F,
      static_cast<float>(renderer->resolution().base[1]) * 0.13F));

  initModText(
    "0",
    2.8F,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.9F,
      static_cast<float>(renderer->resolution().base[1]) * 0.13F));

  initHPText(
    "-1",
    2.8F,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.25F,
      static_cast<float>(renderer->resolution().base[1]) * 0.13F));

  initHPText(
    "-1",
    2.8F,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.75F,
      static_cast<float>(renderer->resolution().base[1]) * 0.13F));

  renderable_sprites[0]->setGlobalZOrder(110);
  renderable_sprites[1]->setGlobalZOrder(120);
  renderable_sprites[2]->setGlobalZOrder(120);

  updateIcons();
  initAudio();
}

void GameplayState::initModText(std::string string, float scale, ASGE::Point2D position)
{
  auto& add_string = modular_strings.emplace_back(std::make_unique<std::string>());
  *add_string      = string;
  auto& text       = modular_texts.emplace_back(std::make_unique<ASGE::Text>());
  renderable_text.emplace_back(text.get());
  text->setFont(*default_font).setString(*add_string).setScale(scale);
  auto new_position =
    ASGE::Point2D(position.x - text->getWidth() / 2, position.y - text->getHeight() / 2);
  text->setPositionX(new_position.x);
  text->setPositionY(new_position.y);
  text->setZOrder(150);
}

void GameplayState::initHPText(std::string string, float scale, ASGE::Point2D position)
{
  auto& add_string = hp_strings.emplace_back(std::make_unique<std::string>());
  *add_string      = string;
  auto& text       = hp_texts.emplace_back(std::make_unique<ASGE::Text>());
  renderable_text.emplace_back(text.get());
  text->setFont(*default_font).setString(*add_string).setScale(scale);
  auto new_position =
    ASGE::Point2D(position.x - text->getWidth() / 2, position.y - text->getHeight() / 2);
  text->setPositionX(new_position.x);
  text->setPositionY(new_position.y);
  text->setColour(ASGE::COLOURS::INDIANRED);
  text->setZOrder(150);
}

void GameplayState::initAudio()
{
  audio_engine.init();
  ASGE::FILEIO::File human_audio_file;
  if (human_audio_file.open("/data/audio/hit.mp3"))
  {
    auto buffer = human_audio_file.read();
    auto length = static_cast<unsigned int>(buffer.length);
    human_audio.loadMem(buffer.as_unsigned_char(), length, false, false);
    human_audio.setLooping(false);
    human_audio.setVolume(0.5F);
  }

  ASGE::FILEIO::File bear_audio_file;
  if (bear_audio_file.open("/data/audio/beast_roar.wav"))
  {
    auto buffer = bear_audio_file.read();
    auto length = static_cast<unsigned int>(buffer.length);
    bear_audio.loadMem(buffer.as_unsigned_char(), length, false, false);
    bear_audio.setLooping(false);
    bear_audio.setVolume(0.8F);
  }

  ASGE::FILEIO::File success_audio_file;
  if (success_audio_file.open("/data/audio/success.wav"))
  {
    auto buffer = success_audio_file.read();
    auto length = static_cast<unsigned int>(buffer.length);
    success_audio.loadMem(buffer.as_unsigned_char(), length, false, false);
    success_audio.setLooping(false);
    success_audio.setVolume(0.5F);
  }

  ASGE::FILEIO::File failure_audio_file;
  if (human_audio_file.open("/data/audio/failure.wav"))
  {
    auto buffer = failure_audio_file.read();
    auto length = static_cast<unsigned int>(buffer.length);
    failure_audio.loadMem(buffer.as_unsigned_char(), length, false, false);
    failure_audio.setLooping(false);
    failure_audio.setVolume(0.5F);
  }
}

void GameplayState::initMinigame(Player* player)
{
  minigame_solution.clear();
  for (unsigned long long i = 0; i < 6; ++i)
  {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 3);
    minigame_solution.emplace_back(static_cast<Character::GuessColours>(dist6(rng)));
  }

  std::string output;
  for (Character::GuessColours i : minigame_solution)
  {
    output += std::to_string(i);
  }
  Logging::DEBUG(output);

  auto& minigame_background = minigame_sprites.emplace_back(renderer->createUniqueSprite());
  minigame_background->loadTexture("/data/img/brassy_frame.png");
  minigame_background->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);
  minigame_background->width(800);
  minigame_background->height(600);
  minigame_background->xPos(player->getPosition().x - 400);
  minigame_background->yPos(player->getPosition().y - 300);
  minigame_background->setGlobalZOrder(500);

  auto& button_one = minigame_sprites.emplace_back(renderer->createUniqueSprite());
  button_one->loadTexture("/data/sprites/arcade_button.png");
  button_one->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);
  button_one->srcRect()[0] = 191;
  button_one->srcRect()[1] = 32;
  button_one->srcRect()[2] = 32;
  button_one->srcRect()[3] = 32;
  button_one->xPos(player->getPosition().x - 48);
  button_one->yPos(player->getPosition().y - 200);
  button_one->width(96);
  button_one->height(96);
  button_one->setGlobalZOrder(501);

  auto& button_two = minigame_sprites.emplace_back(renderer->createUniqueSprite());
  button_two->loadTexture("/data/sprites/arcade_button.png");
  button_two->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);
  button_two->srcRect()[0] = 191;
  button_two->srcRect()[1] = 32;
  button_two->srcRect()[2] = 32;
  button_two->srcRect()[3] = 32;
  button_two->xPos(player->getPosition().x - 144);
  button_two->yPos(player->getPosition().y - 152);
  button_two->width(96);
  button_two->height(96);
  button_two->setGlobalZOrder(501);

  auto& button_three = minigame_sprites.emplace_back(renderer->createUniqueSprite());
  button_three->loadTexture("/data/sprites/arcade_button.png");
  button_three->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);
  button_three->srcRect()[0] = 191;
  button_three->srcRect()[1] = 32;
  button_three->srcRect()[2] = 32;
  button_three->srcRect()[3] = 32;
  button_three->xPos(player->getPosition().x + 48);
  button_three->yPos(player->getPosition().y - 152);
  button_three->width(96);
  button_three->height(96);
  button_three->setGlobalZOrder(501);

  auto& button_four = minigame_sprites.emplace_back(renderer->createUniqueSprite());
  button_four->loadTexture("/data/sprites/arcade_button.png");
  button_four->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);
  button_four->srcRect()[0] = 191;
  button_four->srcRect()[1] = 32;
  button_four->srcRect()[2] = 32;
  button_four->srcRect()[3] = 32;
  button_four->xPos(player->getPosition().x - 48);
  button_four->yPos(player->getPosition().y - 104);
  button_four->width(96);
  button_four->height(96);
  button_four->setGlobalZOrder(501);

  std::thread sequence_thread([&] { playMinigameSequence(); });
  sequence_thread.detach();
}

void GameplayState::setUpCamera()
{
  int camera_id = 0;
  for (auto& player : players)
  {
    camera_vector.emplace_back(std::make_unique<ASGE::Camera>(960, 1080));
    player->setCamera(camera_id);
    camera_id++;
  }
}

void GameplayState::inputHandler(ASGE::GamePadData& gamepad)
{
  if (gamepad.is_connected)
  {
    if (gamepad_state.contains(gamepad.idx))
    {
      ASGE::Point2D velocity = ASGE::Point2D(0, 0);
      if (
        gamepad.axis[ASGE::GAMEPAD::AXIS_LEFT_X] > GAMEPAD_DEADZONE ||
        gamepad.axis[ASGE::GAMEPAD::AXIS_LEFT_X] < GAMEPAD_DEADZONE)
      {
        velocity.x = gamepad.axis[ASGE::GAMEPAD::AXIS_LEFT_X];

        if (velocity.x > GAMEPAD_DEADZONE)
        {
          players[findPlayerWithController(gamepad.idx)]->getCharacter()->sprite->setFlipFlags(
            ASGE::Sprite::NORMAL);
        }
        else if (velocity.x < -GAMEPAD_DEADZONE)
        {
          players[findPlayerWithController(gamepad.idx)]->getCharacter()->sprite->setFlipFlags(
            ASGE::Sprite::FLIP_X);
        }
      }
      if (
        gamepad.axis[ASGE::GAMEPAD::AXIS_LEFT_Y] > GAMEPAD_DEADZONE ||
        gamepad.axis[ASGE::GAMEPAD::AXIS_LEFT_Y] < GAMEPAD_DEADZONE)
      {
        velocity.y = gamepad.axis[ASGE::GAMEPAD::AXIS_LEFT_Y];
      }

      auto index = findPlayerWithController(gamepad.idx);
      if (index != FAIL_INDEX)
      {
        players[index]->setVelocity(velocity);
      }

      if (!players[index]->getCharacter()->isInMinigame())
      {
        if (
          inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_X) ||
          inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_SQUARE))
        {
          attemptAttack(gamepad.idx);
        }

        if (
          inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_Y) ||
          inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_TRIANGLE))
        {
          /*playerSwap(players[0], players[1]);*/
          if (
            players[index]->getCharacter()->getCharacterType() == Character::CharacterType::HUMAN &&
            players[index]->getCharacter()->isNearObjective())
          {
            players[index]->getCharacter()->setInMinigame(true);
            initMinigame(players[index]);
          }
        }
      }

      else
      {
        auto human_character = players[index]->getCharacter();
        players[index]->setVelocity(0);

        if (
          inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_A) ||
          inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_CROSS))
        {
          human_character->addGuess(Character::GuessColours::YELLOW);

          auto& green_guess = minigame_sprites.emplace_back(renderer->createUniqueSprite());
          green_guess->loadTexture("/data/sprites/arcade_button.png");
          green_guess->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);
          green_guess->srcRect()[0] = 192;
          green_guess->srcRect()[1] = 0;
          green_guess->srcRect()[2] = 32;
          green_guess->srcRect()[3] = 32;
          green_guess->xPos(
            human_character->getPosition().x - 424 +
            (static_cast<float>(106 * human_character->getGuesses().size())));
          green_guess->yPos(human_character->getPosition().y + 100);
          green_guess->width(96);
          green_guess->height(96);
          green_guess->setGlobalZOrder(501);
        }

        if (
          inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_B) ||
          inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_CIRCLE))
        {
          human_character->addGuess(Character::GuessColours::BLUE);

          auto& red_guess = minigame_sprites.emplace_back(renderer->createUniqueSprite());
          red_guess->loadTexture("/data/sprites/arcade_button.png");
          red_guess->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);
          red_guess->srcRect()[0] = 0;
          red_guess->srcRect()[1] = 0;
          red_guess->srcRect()[2] = 32;
          red_guess->srcRect()[3] = 32;
          red_guess->xPos(
            human_character->getPosition().x - 424 +
            (static_cast<float>(106 * human_character->getGuesses().size())));
          red_guess->yPos(human_character->getPosition().y + 100);
          red_guess->width(96);
          red_guess->height(96);
          red_guess->setGlobalZOrder(501);
        }

        if (
          inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_X) ||
          inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_SQUARE))
        {
          human_character->addGuess(Character::GuessColours::RED);

          auto& blue_guess = minigame_sprites.emplace_back(renderer->createUniqueSprite());
          blue_guess->loadTexture("/data/sprites/arcade_button.png");
          blue_guess->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);
          blue_guess->srcRect()[0] = 128;
          blue_guess->srcRect()[1] = 0;
          blue_guess->srcRect()[2] = 32;
          blue_guess->srcRect()[3] = 32;
          blue_guess->xPos(
            human_character->getPosition().x - 424 +
            (static_cast<float>(106 * human_character->getGuesses().size())));
          blue_guess->yPos(human_character->getPosition().y + 100);
          blue_guess->width(96);
          blue_guess->height(96);
          blue_guess->setGlobalZOrder(501);
        }

        if (
          inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_Y) ||
          inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_TRIANGLE))
        {
          human_character->addGuess(Character::GuessColours::GREEN);

          auto& yellow_guess = minigame_sprites.emplace_back(renderer->createUniqueSprite());
          yellow_guess->loadTexture("/data/sprites/arcade_button.png");
          yellow_guess->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);
          yellow_guess->srcRect()[0] = 64;
          yellow_guess->srcRect()[1] = 0;
          yellow_guess->srcRect()[2] = 32;
          yellow_guess->srcRect()[3] = 32;
          yellow_guess->xPos(
            human_character->getPosition().x - 424 +
            (static_cast<float>(106 * human_character->getGuesses().size())));
          yellow_guess->yPos(human_character->getPosition().y + 100);
          yellow_guess->width(96);
          yellow_guess->height(96);
          yellow_guess->setGlobalZOrder(501);
        }

        if (human_character->getGuesses().size() == 6)
        {
          checkMinigameGuesses(human_character->getGuesses());
          human_character->resetGuesses();
        }

        else if (human_character->getGuesses().size() > 6)
        {
          human_character->resetGuesses();
        }
      }

      if (inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_START))
      {
        next_state = PAUSE;
      }
    }
  }
}

void GameplayState::update(const ASGE::GameTime& us)
{
  for (auto& player : players)
  {
    player->updateFrames(us, GAMEPAD_DEADZONE);
  }

  Boundary future_pos;
  for (auto& player : players)
  {
    future_pos.build(
      static_cast<float>(
        player->getPosition().x + player->getScale() *
                                    static_cast<float>(player->getCharacter()->getSpeed()) *
                                    (player->getVelocity().x * us.deltaInSecs())),
      static_cast<float>(
        player->getPosition().y + player->getScale() *
                                    static_cast<float>(player->getCharacter()->getSpeed()) *
                                    (player->getVelocity().y * us.deltaInSecs())),
      player->getGlobalBounds().width * static_cast<float>(player->getScale()),
      player->getGlobalBounds().height * static_cast<float>(player->getScale()));
    if (
      !tilemap->checkCollision(future_pos) &&
      !checkPlayerCollisions(future_pos, player->getPlayerID()))
    {
      player->setPosition(ASGE::Point2D(future_pos.position));
    }

    auto* player_character = player->getCharacter();
    if (player_character->getCharacterType() == Character::CharacterType::HUMAN)
    {
      player_character->setNearObjective(tilemap->checkCollision(future_pos, tilemap->objectives));
    }
  }

  std::thread camera_thread([&] { cameraUpdate(); });
  camera_thread.detach();
  updateTexts();
}

void GameplayState::fixedUpdate(const ASGE::GameTime& us)
{
  timer_display_increment += 1;

  if (timer_display_increment >= 40)
  {
    run_timer -= 1;
    timer_display_increment = 0;
  }
  int minutes_remain = static_cast<int>(run_timer / 60);
  int seconds_remain = static_cast<int>(run_timer - (minutes_remain * 60));

  if (seconds_remain < 10)
  {
    *modular_strings[0] = std::to_string(minutes_remain) + ":0" + std::to_string(seconds_remain);
  }
  else
  {
    *modular_strings[0] = std::to_string(minutes_remain) + ":" + std::to_string(seconds_remain);
  }

  if (run_timer <= 0)
  {
    next_state = END;
  }
  else
  {
    for (auto& player : players)
    {
      if (player->getCharacter()->hasAttacked())
      {
        player->getCharacter()->tickCD(us.deltaInSecs());
      }
    }
  }
}

void GameplayState::cameraUpdate()
{
  for (auto& player : players)
  {
    auto look_at = player->getCenter();
    camera_vector[static_cast<unsigned long long>(player->getCamera())]->lookAt(look_at);
  }
}

void GameplayState::updateTexts()
{
  for (unsigned long long i = 0; i < (modular_texts.size()); ++i)
  {
    modular_texts[i]->setString(*modular_strings[i]);
  }
  for (unsigned long long i = 0; i < players.size(); i++)
  {
    hp_texts[i]->setString(std::to_string(players[i]->getHP()));
  }
}

void GameplayState::updateScores()
{
  for (unsigned long long i = 0; i < (players.size()); ++i)
  {
    *modular_strings[i + 1] = std::to_string(players[i]->getScore());
  }
}

void GameplayState::updateIcons()
{
  unsigned long long player_index = findPlayerWithController(0);
  if (player_index == FAIL_INDEX)
  {
    player_index = 0;
  }
  if (players[player_index]->getCharacter()->getCharacterType() == Character::HUMAN)
  {
    renderable_sprites[1]->xPos(16);
    renderable_sprites[2]->xPos(1820);
  }
  else
  {
    renderable_sprites[2]->xPos(16);
    renderable_sprites[1]->xPos(1820);
  }
  renderable_sprites[2]->yPos(11);
  renderable_sprites[1]->yPos(11);
}

unsigned long long GameplayState::findPlayerWithController(int controller_id)
{
  for (unsigned long long i = 0; i < players.size(); i++)
  {
    if (players[i]->getController() == controller_id)
    {
      return i;
    }
  }
  return FAIL_INDEX;
}

std::vector<std::unique_ptr<ASGE::Camera>>* GameplayState::getCameras()
{
  return &camera_vector;
}

int GameplayState::getNumberOfCameras()
{
  return static_cast<int>(camera_vector.size());
}

std::vector<ASGE::Text*>* GameplayState::getRenderableText()
{
  return &renderable_text;
}

bool GameplayState::checkPlayerCollisions(Boundary bound, int ignore_id)
{
  Boundary rival_bounds;
  for (auto& player : players)
  {
    if (player->getPlayerID() != ignore_id)
    {
      rival_bounds.build(
        player->getPosition().x,
        player->getPosition().y,
        player->getGlobalBounds().width * static_cast<float>(player->getScale()),
        player->getGlobalBounds().height * static_cast<float>(player->getScale()));

      if (tilemap->checkCollisionBetweenBounds(bound, rival_bounds))
      {
        return true;
      }
    }
  }
  return false;
}

unsigned long long GameplayState::findNearestPlayer(ASGE::Point2D attacking_player, int ignore_id)
{
  int index             = 200;
  float lowest_distance = 99999;
  float dist            = -1;
  for (auto& player : players)
  {
    if (ignore_id != -1 && player->getPlayerID() != ignore_id)
    {
      dist = player->FindDistanceToPoint(attacking_player);
      if (dist < lowest_distance)
      {
        index           = player->getPlayerID();
        lowest_distance = dist;
      }
    }
  }
  return static_cast<unsigned long long>(index);
}

void GameplayState::attemptAttack(int attacker_gamepad_idx)
{
  auto attacker_index    = findPlayerWithController(attacker_gamepad_idx);
  auto& attacking_player = players[attacker_index];

  if (!attacking_player->getCharacter()->hasAttacked() && attacking_player->getCharacter()->getCD() <= 0)
  {
    attacking_player->getCharacter()->setSpeed(attacking_player->getCharacter()->getSpeed() / 2);
    attacking_player->getCharacter()->setHasAttacked(true);
    attacking_player->getCharacter()->setCD(3);

    auto defender_id =
      findNearestPlayer(attacking_player->getCenter(), attacking_player->getPlayerID());
    auto& defending_player = players[defender_id];

    if (
      attacking_player->getRange() * static_cast<float>(tilemap->scale) * tilemap->getTileSize() >=
      attacking_player->FindDistanceToPoint(defending_player->getCenter()))
    {
      if (attacking_player->getCharacter()->getCharacterType() == Character::ANIMAL)
      {
        audio_engine.play(bear_audio);
      }

      else if (attacking_player->getCharacter()->getCharacterType() == Character::HUMAN)
      {
        audio_engine.play(human_audio);
      }
      defending_player->takeDamage(attacking_player->getDamage());

      if (defending_player->getHP() <= 0)
      {
        if (attacking_player->getCharacter()->getCharacterType() == Character::ANIMAL)
        {
          playerSwap(attacking_player, defending_player);
        }

        defending_player->resetCharacter(*tilemap->spawnpoints[1]);

        for (auto& player : players)
        {
          player->setVelocity(ASGE::Point2D(0, 0));
        }
      }
    }
  }
}

void GameplayState::playerSwap(Player* attacking_player, Player* defending_player)
{
  auto score = attacking_player->getScore();
  attacking_player->setScore(score + 1);
  updateScores();
  attacking_player->resetCharacter(*tilemap->spawnpoints[1]);
  defending_player->resetCharacter(*tilemap->spawnpoints[0]);
  characterSwap(0, 1);
  deleteMinigameSprites();
  updateIcons();
}

void GameplayState::characterSwap(
  unsigned long long int first_player_id, unsigned long long int second_player_id)
{
  auto* first_char  = players[first_player_id]->getCharacter();
  auto* second_char = players[second_player_id]->getCharacter();

  players[first_player_id]->setCharacter(second_char);
  players[second_player_id]->setCharacter(first_char);
}

std::vector<std::unique_ptr<ASGE::Sprite>>* GameplayState::getRenderableSprites()
{
  return &renderable_sprites;
}

bool GameplayState::checkMinigameGuesses(std::vector<Character::GuessColours> guesses)
{
  std::string output;
  for (Character::GuessColours i : guesses)
  {
    output += std::to_string(i);
  }

  std::string actual;
  for (Character::GuessColours i : minigame_solution)
  {
    actual += std::to_string(i);
  }

  if (guesses == minigame_solution)
  {
    audio_engine.play(success_audio);
    for (unsigned long long i = 0; i < 2; ++i)
    {
      if (players[i]->getCharacter()->getCharacterType() == Character::CharacterType::HUMAN)
      {
        players[i]->getCharacter()->setInMinigame(false);
        players[i]->setScore(players[i]->getScore() + 1);
        updateScores();
        break;
      }
    }
    deleteMinigameSprites();
    return true;
  }

  else
  {
    audio_engine.play(failure_audio);
    std::thread minigame_thread([&] { resetMinigameGuesses(); });
    minigame_thread.detach();

    Logging::DEBUG("INCORRECT: " + output + " (ACTUAL: " + actual + ")");
    std::thread sequence_thread([&] { playMinigameSequence(); });
    sequence_thread.detach();
    return false;
  }
}

std::vector<std::unique_ptr<ASGE::Sprite>>* GameplayState::getMinigameSprites()
{
  return &minigame_sprites;
}

void GameplayState::resetMinigameGuesses()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(800));
  minigame_sprites.erase(minigame_sprites.cend() - 6, minigame_sprites.cend());
  minigame_sprites.shrink_to_fit();
}

void GameplayState::playMinigameSequence()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(300));

  for (Character::GuessColours step : minigame_solution)
  {
    switch (step)
    {
      case Character::GuessColours::GREEN:
        minigame_sprites[1]->srcRect()[0] = 64;
        minigame_sprites[1]->srcRect()[1] = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        minigame_sprites[1]->srcRect()[0] = 191;
        minigame_sprites[1]->srcRect()[1] = 32;
        break;

      case Character::GuessColours::RED:
        minigame_sprites[2]->srcRect()[0] = 128;
        minigame_sprites[2]->srcRect()[1] = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        minigame_sprites[2]->srcRect()[0] = 191;
        minigame_sprites[2]->srcRect()[1] = 32;
        break;

      case Character::GuessColours::BLUE:
        minigame_sprites[3]->srcRect()[0] = 0;
        minigame_sprites[3]->srcRect()[1] = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        minigame_sprites[3]->srcRect()[0] = 191;
        minigame_sprites[3]->srcRect()[1] = 32;
        break;

      case Character::GuessColours::YELLOW:
        minigame_sprites[4]->srcRect()[0] = 192;
        minigame_sprites[4]->srcRect()[1] = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        minigame_sprites[4]->srcRect()[0] = 191;
        minigame_sprites[4]->srcRect()[1] = 32;
        break;

      case Character::GuessColours::DEFAULT:
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }
}

void GameplayState::deleteMinigameSprites()
{
  for (unsigned long long i = 0; i < minigame_sprites.size(); ++i)
  {
    minigame_sprites[i].reset();
  }

  minigame_sprites.clear();
}
