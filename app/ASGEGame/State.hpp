
#ifndef ASGEGAME_STATE_HPP
#define ASGEGAME_STATE_HPP

#include "Player.hpp"
#include <Engine/GameTime.hpp>
#include <Engine/InputEvents.hpp>
#include <Engine/Logger.hpp>
#include <Engine/OGLGame.hpp>
#include <Engine/Renderer.hpp>
#include <map>
class State
{
 public:
  enum StateID
  {
    INVALID  = -1,
    MENU     = 0,
    GAMEPLAY = 1,
    PAUSE    = 2,
    END      = 3
  };
  State(Player* player_1, Player* player_2, ASGE::Font* font);
  virtual ~State();

  virtual void update(const ASGE::GameTime& us) {}
  /*virtual void render(const ASGE::GameTime& us) {}*/

  virtual void fixedUpdate(const ASGE::GameTime& us) {}
  virtual void inputHandler(ASGE::SharedEventData data) {}
  virtual void inputHandler(ASGE::GamePadData& gamepad) {}

  virtual std::vector<ASGE::Text*>* getRenderableText();
  virtual std::vector<std::unique_ptr<ASGE::Sprite>>* getRenderableSprites();

  virtual void init() {}

  StateID getStateID();
  void updateGamepadState(ASGE::GamePadData& gamepad);
  bool checkControllerConnected(int idx);
  void updateConnected(int num_controllers);
  ASGE::Renderer* renderer;

  StateID getNextState();
  void resetNextState();

  bool initialized = false;

 protected:
  bool inputClickCondition(ASGE::GamePadData& gamepad, int gamepad_input);

  StateID state_id = INVALID;
  std::vector<Player*> players;
  std::map<int, ASGE::GamePadData&> gamepad_state{};
  int controllers_connected = 0;
  StateID next_state        = INVALID;
  ASGE::Font* default_font;
};

#endif // ASGEGAME_STATE_HPP
