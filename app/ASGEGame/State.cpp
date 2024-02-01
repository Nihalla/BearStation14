
#include "State.hpp"

State::State(Player* player_1, Player* player_2, ASGE::Font* font)
{
  players.emplace_back(player_1);
  players.emplace_back(player_2);
  default_font = font;
}

State::~State() {}

State::StateID State::getStateID()
{
  return state_id;
}

std::vector<ASGE::Text*>* State::getRenderableText()
{
  return nullptr;
}

std::vector<std::unique_ptr<ASGE::Sprite>>* State::getRenderableSprites()
{
  return nullptr;
}

void State::updateGamepadState(ASGE::GamePadData& gamepad)
{
  gamepad_state.insert_or_assign(gamepad.idx, gamepad);
}

bool State::checkControllerConnected(int idx)
{
  for (auto& player : players)
  {
    if (player->getController() == idx)
    {
      return true;
    }
  }
  return false;
}

void State::updateConnected(int num_connected)
{
  controllers_connected = num_connected;
}

State::StateID State::getNextState()
{
  return next_state;
}

void State::resetNextState()
{
  next_state = state_id;
}

/**
 * @brief \InputClickCondition
 *  Takes a reference to a gamepad and the id of a button input.
 * \n Returns whether the corresponding button is being pressed.
 * \n Use whenever wanting to check if a button is clicked.
 * \n\n (Probably was not necessary, but I found it helps with readability)*/
bool State::inputClickCondition(ASGE::GamePadData& gamepad, int gamepad_input)
{
  return ((gamepad.buttons[gamepad_input]) != 0U) &&
         ((gamepad_state.at(gamepad.idx).buttons[gamepad_input]) == 0U);
}
