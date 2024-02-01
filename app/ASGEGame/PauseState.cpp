#include "PauseState.hpp"
#include "iostream"
#include <Engine/FileIO.hpp>

PauseState::PauseState(Player* player_1, Player* player_2, ASGE::Font* font) :
  State(player_1, player_2, font)
{
  state_id   = PAUSE;
  next_state = PAUSE;
  modular_texts.reserve(10);
  modular_strings.reserve(10);
}

PauseState::~PauseState() {}

void PauseState::init()
{
  initialized = true;
  loadText();
  initAudio();
}

void PauseState::initAudio()
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
}

void PauseState::initModText(std::string string, float scale, ASGE::Point2D position)
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

void PauseState::initFixText(std::string string, float scale, ASGE::Point2D position)
{
  auto& text = fixed_texts.emplace_back(std::make_unique<ASGE::Text>());
  renderables.emplace_back(text.get());
  text->setFont(*default_font).setString(string).setScale(scale);
  auto new_position =
    ASGE::Point2D(position.x - text->getWidth() / 2, position.y - text->getHeight() / 2);
  Logging::DEBUG(std::to_string(new_position.x));
  text->setPositionX(new_position.x);
  text->setPositionY(new_position.y);
}

void PauseState::loadText()
{
  initFixText(
    "Bear Station 14",
    2.0F,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.5F,
      static_cast<float>(renderer->resolution().base[1]) * 0.25F));
  initFixText(
    "Pause",
    1.5F,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.5F,
      static_cast<float>(renderer->resolution().base[1]) * 0.35F));
  initModText(
    "Resume",
    1.2F,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.5F,
      static_cast<float>(renderer->resolution().base[1]) * 0.65F));
  initModText(
    "Options",
    1.2F,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.5F,
      static_cast<float>(renderer->resolution().base[1]) * 0.75F));
  initModText(
    "Exit",
    1.2F,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.5F,
      static_cast<float>(renderer->resolution().base[1]) * 0.85F));
}

/**
 * @brief \InputClickCondition
 *  Takes a reference to a gamepad and the id of a button input.
 * \n Returns whether the corresponding button is being pressed.
 * \n Use whenever wanting to check if a button is clicked.
 * \n\n (Probably was not necessary, but I found it helps with readability)*/
bool PauseState::inputClickCondition(ASGE::GamePadData& gamepad, int gamepad_input)
{
  return ((gamepad.buttons[gamepad_input]) != 0U) &&
         ((gamepad_state.at(gamepad.idx).buttons[gamepad_input]) == 0U);
}

void PauseState::inputHandler(ASGE::SharedEventData data) {}

void PauseState::inputHandler(ASGE::GamePadData& gamepad)
{
  if (gamepad.is_connected)
  {
    if (gamepad_state.contains(gamepad.idx))
    {
      if (inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_START))
      {
        audio_engine.play(option_audio);
        inputConnect(gamepad);
      }
      if (
        inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_DPAD_RIGHT) ||
        inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_DPAD_DOWN))
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
      if (
        inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_DPAD_LEFT) ||
        inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_DPAD_UP))
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
      /// Can also add playstation inputs
      if (inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_A))
      {
        audio_engine.play(option_audio);
        handleTransitions();
      }
    }
  }
}

void PauseState::inputConnect(ASGE::GamePadData& gamepad)
{
  if (players_connected < player_limit && !checkControllerConnected(gamepad.idx))
  {
    players[static_cast<unsigned long long>(players_connected)]->setController(gamepad.idx);
    Logging::DEBUG("Player connected with controller " + std::to_string(gamepad.idx));
    players_connected++;
  }
}

void PauseState::fixedUpdate(const ASGE::GameTime& us) {}

void PauseState::update(const ASGE::GameTime& us)
{
  updateTexts();
}

void PauseState::updateTexts()
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
    modular_texts[text_select_index]->setString("-" + *modular_strings[previous_index] + "-");
    modular_texts[text_select_index]->setPositionX(
      center.x - modular_texts[text_select_index]->getWidth() / 2);
    modular_texts[text_select_index]->setPositionY(
      center.y - modular_texts[text_select_index]->getHeight() / 2);
  }
}

void PauseState::handleTransitions()
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

std::vector<ASGE::Text*>* PauseState::getRenderableText()
{
  return &renderables;
}

std::vector<std::unique_ptr<ASGE::Sprite>>* PauseState::getRenderableSprites()
{
  return nullptr;
}
