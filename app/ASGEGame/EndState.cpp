
#include "EndState.hpp"
EndState::EndState(Player* player_1, Player* player_2, ASGE::Font* font) :
  State(player_1, player_2, font)
{
  state_id   = END;
  next_state = END;
}

EndState::~EndState() {}

void EndState::init()
{
  initFixText(
    "Winner",
    5,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.5F,
      static_cast<float>(renderer->resolution().base[1]) * 0.25F));

  initModText(
    "Winning_Player",
    2,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.5F,
      static_cast<float>(renderer->resolution().base[1]) * 0.5F));

  initModText(
    "Winning_Score",
    2,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.5F,
      static_cast<float>(renderer->resolution().base[1]) * 0.7F));

  initModText(
    "Losing_Player",
    1,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.85F,
      static_cast<float>(renderer->resolution().base[1]) * 0.8F));

  initModText(
    "Losing_Score",
    1,
    ASGE::Point2D(
      static_cast<float>(renderer->resolution().base[0]) * 0.85F,
      static_cast<float>(renderer->resolution().base[1]) * 0.9F));
}

void EndState::initModText(std::string string, float scale, ASGE::Point2D position)
{
  auto& add_string = modular_strings.emplace_back(std::make_unique<std::string>());
  *add_string      = string;
  auto& text       = modular_texts.emplace_back(std::make_unique<ASGE::Text>());
  renderable_texts.emplace_back(text.get());
  text->setFont(*default_font).setString(*add_string).setScale(scale);
  auto new_position =
    ASGE::Point2D(position.x - text->getWidth() / 2, position.y - text->getHeight() / 2);
  Logging::DEBUG(std::to_string(new_position.x));
  text->setPositionX(new_position.x);
  text->setPositionY(new_position.y);
}

void EndState::initFixText(std::string string, float scale, ASGE::Point2D position)
{
  auto& text = fixed_texts.emplace_back(std::make_unique<ASGE::Text>());
  renderable_texts.emplace_back(text.get());
  text->setFont(*default_font).setString(string).setScale(scale);
  auto new_position =
    ASGE::Point2D(position.x - text->getWidth() / 2, position.y - text->getHeight() / 2);
  Logging::DEBUG(std::to_string(new_position.x));
  text->setPositionX(new_position.x);
  text->setPositionY(new_position.y);
}

void EndState::update(const ASGE::GameTime& us) {}

void EndState::fixedUpdate(const ASGE::GameTime& us) {}

void EndState::updateTexts()
{
  int winner_id     = 99;
  int winning_score = -1;

  int loser_id     = 99;
  int losing_score = 99;

  int player_id    = -1;
  int player_score = -1;

  for (auto& player : players)
  {
    player_id    = player->getPlayerID();
    player_score = player->getScore();

    if (winner_id != player_id && winning_score < player_score)
    {
      winner_id     = player_id;
      winning_score = player_score;
    }
    if (loser_id != player_id && losing_score >= player_score)
    {
      loser_id     = player_id;
      losing_score = player_score;
    }
  }

  *modular_strings[0] = "Player " + std::to_string(winner_id + 1);
  *modular_strings[1] = "Score: " + std::to_string(winning_score);
  *modular_strings[2] = "Player " + std::to_string(loser_id + 1);
  *modular_strings[3] = "Score: " + std::to_string(losing_score);

  for (unsigned long long i = 0; i < modular_texts.size(); i++)
  {
    modular_texts[i]->setString(*modular_strings[i]);
  }
  initialized = true;
}

void EndState::inputHandler(ASGE::GamePadData& gamepad)
{
  if (gamepad_state.contains(gamepad.idx))
  {
    if (
      inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_A) ||
      inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_CROSS) ||
      inputClickCondition(gamepad, ASGE::GAMEPAD::BUTTON_START))
    {
      exit(0);
    }
  }
}

std::vector<ASGE::Text*>* EndState::getRenderableText()
{
  return &renderable_texts;
}