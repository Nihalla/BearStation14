
#include "MenuState.hpp"
#include "iostream"
#include <Engine/FileIO.hpp>

MenuState::MenuState(Player* player_1, Player* player_2, ASGE::Font* font) :
  State(player_1, player_2, font)
{
  state_id   = MENU;
  next_state = MENU;
  modular_texts.reserve(10);
  modular_strings.reserve(10);
}

MenuState::~MenuState() {}

void MenuState::init()
{
  initialized = true;
  loadMenu();
  initAudio();
}

void MenuState::initAudio()
{
  audio_engine.init();
  ASGE::FILEIO::File bg_audio_file;
  if (bg_audio_file.open("/data/audio/click.wav"))
  {
    auto buffer = bg_audio_file.read();
    auto length = static_cast<unsigned int>(buffer.length);
    option_audio.loadMem(buffer.as_unsigned_char(), length, false, false);
    option_audio.setLooping(false);
    option_audio.setVolume(0.5F);
  }

  ASGE::FILEIO::File two_audio_file;
  if (two_audio_file.open("/data/audio/retro-ping.wav"))
  {
    auto buffer = two_audio_file.read();
    auto length = static_cast<unsigned int>(buffer.length);
    start_audio.loadMem(buffer.as_unsigned_char(), length, false, false);
    start_audio.setLooping(false);
    start_audio.setVolume(0.5F);
  }
}

void MenuState::initModText(std::string string, float scale, ASGE::Point2D position)
{
  auto& add_string = modular_strings.emplace_back(std::make_unique<std::string>());
  *add_string      = string;
  auto& text       = modular_texts.emplace_back(std::make_unique<ASGE::Text>());
  renderables.emplace_back(text.get());
  text->setFont(*default_font).setString(*add_string).setScale(scale);
  auto new_position =
    ASGE::Point2D(position.x - text->getWidth() / 2, position.y - text->getHeight() / 2);
  Logging::DEBUG(std::to_string(new_position.x));
  text->setPositionX(new_position.x);
  text->setPositionY(new_position.y);
}

void MenuState::initFixText(std::string string, float scale, ASGE::Point2D position)
{
  auto& text = fixed_texts.emplace_back(std::make_unique<ASGE::Text>());
  renderables.emplace_back(text.get());
  fixated_texts.emplace_back(text.get());
  text->setFont(*default_font).setString(string).setScale(scale);
  auto new_position =
    ASGE::Point2D(position.x - text->getWidth() / 2, position.y - text->getHeight() / 2);
  Logging::DEBUG(std::to_string(new_position.x));
  text->setPositionX(new_position.x);
  text->setPositionY(new_position.y);
}

void MenuState::loadMenu()
{
  initFixText(
    "Bear Station 14",
    3,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.5F,
      static_cast<float>(renderer->resolution().base[1]) * 0.3F));
  initFixText(
    "Get ready!",
    1,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.5F,
      static_cast<float>(renderer->resolution().base[1]) * 0.45F));
  initModText(
    "Play",
    1,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.20F,
      static_cast<float>(renderer->resolution().base[1]) * 0.7F));
  initModText(
    "Player Select",
    1,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.5F,
      static_cast<float>(renderer->resolution().base[1]) * 0.7F));
  initModText(
    "Exit",
    1,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.8F,
      static_cast<float>(renderer->resolution().base[1]) * 0.7F));

  auto& text = fixed_texts.emplace_back(std::make_unique<ASGE::Text>());
  fixated_texts.emplace_back(text.get());
  text->setFont(*default_font).setString("<Press start to connect>").setScale(1);
  auto new_position = ASGE::Point2D(
    static_cast<float>(renderer->resolution().base[0]) * 0.5F - text->getWidth() / 2,
    static_cast<float>(renderer->resolution().base[1]) * 0.65F - text->getHeight() / 2);

  text->setPositionX(new_position.x);
  text->setPositionY(new_position.y);
}

void MenuState::update(const ASGE::GameTime& us)
{
  updateTexts();
}

void MenuState::updateTexts()
{
  ASGE::Point2D center;
  if (previous_index != text_select_index)
  {
    if (previous_index < modular_texts.size() + 1)
    {
      center   = modular_texts[previous_index]->getPosition();
      center.x = center.x + modular_texts[previous_index]->getWidth() / 2;
      center.y = center.y + modular_texts[previous_index]->getHeight() / 2;
      modular_texts[previous_index]->setString(*modular_strings[previous_index]);
      modular_texts[previous_index]->setPositionX(
        center.x - modular_texts[previous_index]->getWidth() / 2);
      modular_texts[previous_index]->setPositionY(
        center.y - modular_texts[previous_index]->getHeight() / 2);
    }
    previous_index = text_select_index;

    center   = modular_texts[text_select_index]->getPosition();
    center.x = center.x + modular_texts[text_select_index]->getWidth() / 2;
    center.y = center.y + modular_texts[text_select_index]->getHeight() / 2;
    modular_texts[text_select_index]->setString("<" + *modular_strings[previous_index] + ">");
    modular_texts[text_select_index]->setPositionX(
      center.x - modular_texts[text_select_index]->getWidth() / 2);
    modular_texts[text_select_index]->setPositionY(
      center.y - modular_texts[text_select_index]->getHeight() / 2);
  }
}

/*void MenuState::render(const ASGE::GameTime& us) {}*/
void MenuState::fixedUpdate(const ASGE::GameTime& us) {}

void MenuState::inputHandler(ASGE::SharedEventData data) {}

void MenuState::inputHandler(ASGE::GamePadData& gamepad)
{
  if (gamepad.is_connected)
  {
    if (gamepad_state.contains(gamepad.idx))
    {
      if (inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_START))
      {
        inputConnect(gamepad);
      }
      if (inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_DPAD_RIGHT))
      {
        audio_engine.play(option_audio);
        if ((text_select_index < modular_texts.size() - 1))
        {
          text_select_index++;
        }
        else
        {
          text_select_index = 0;
        }
      }
      if (inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_DPAD_LEFT))
      {
        audio_engine.play(option_audio);
        if ((text_select_index > 0))
        {
          text_select_index--;
        }
        else
        {
          text_select_index = modular_texts.size() - 1;
        }
      }
      if (inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_A))
      {
        audio_engine.play(option_audio);
        handleTransitions();
      }
    }
  }
}

void MenuState::handleTransitions()
{
  if (players_connected == controllers_connected)
  {
    switch (text_select_index)
    {
      case (0):
      {
        next_state = GAMEPLAY;
        break;
      }
      case (1):
      {
        Logging::DEBUG("(Insert fun message here)");
        break;
      }
      case (2):
      {
        exit(0);
      }
    }
  }
}

std::vector<ASGE::Text*>* MenuState::getRenderableText()
{
  if (players_connected != controllers_connected)
  {
    return &fixated_texts;
  }
  return &renderables;
}

std::vector<std::unique_ptr<ASGE::Sprite>>* MenuState::getRenderableSprites()
{
  return nullptr;
}

void MenuState::inputConnect(ASGE::GamePadData& gamepad)
{
  if (players_connected < player_limit && !checkControllerConnected(gamepad.idx))
  {
    audio_engine.play(start_audio);
    players[static_cast<unsigned long long>(players_connected)]->setPlayerID(players_connected);
    players[static_cast<unsigned long long>(players_connected)]->setController(gamepad.idx);
    Logging::DEBUG("Player connected with controller " + std::to_string(gamepad.idx));
    players_connected++;
  }
}