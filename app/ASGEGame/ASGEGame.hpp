#pragma once

#include "GameplayState.hpp"
#include "MenuState.hpp"
#include "PauseState.hpp"
#include "Player.hpp"
#include "State.hpp"
#include "TileMap.h"
#include <Engine/Gamepad.hpp>
#include <Engine/OGLGame.hpp>
#include <Engine/Sprite.hpp>
#include <map>
#include <soloud.h>
#include <soloud_wav.h>
#include <tmxlite/Map.hpp>

class ASGENetGame : public ASGE::OGLGame
{
 public:
  explicit ASGENetGame(const ASGE::GameSettings& settings);
  ~ASGENetGame() override;

  ASGENetGame(const ASGENetGame&) = delete;
  ASGENetGame& operator=(const ASGENetGame&) = delete;

  void keyHandler(ASGE::SharedEventData data);
  void update(const ASGE::GameTime& us) override;
  void render(const ASGE::GameTime& us) override;
  void fixedUpdate(const ASGE::GameTime& us) override;

  void moveShip(ASGE::Sprite ship_to_move, ASGE::GameTime& delta_t, ASGE::Point2D vel);
  void loadTileMap();
  void storeTiles(tmx::Layer& layer);
  void loadBackground();

  void stateTransition();
  unsigned long long FindStateByID(State::StateID idx);

 private:
  SoLoud::Soloud audio_engine;
  SoLoud::Wav background_audio;

  std::string key_callback_id{}; /**< Key Input Callback ID. */

  std::map<int, bool> keymap{};
  std::map<int, ASGE::GamePadData> gamepad_state{};
  std::unique_ptr<ASGE::Sprite> background{ nullptr };

  ASGE::Point2D velocity{ 0, 0 };
  ASGE::Point2D velocity2{ 0, 0 };

  tmx::Map map;
  int SCALE = 2;
  std::vector<std::unique_ptr<ASGE::Sprite>> tiles;

  // Font
  const ASGE::Font* game_font = nullptr;
  ASGE::Text camera_one_label{};
  ASGE::Text camera_two_label{};

  // some cameras
  ASGE::Camera main_Camera{ 1920, 1080 };

  // float game_width  = 0;
  // float game_height = 0;

  void initAudio();
  /*bool collisionCheck(float, float, float, float);*/
  bool collisionCheck(Boundary boundary);
  std::unique_ptr<TileMap> tilemap;
  /*void WallUP();*/
  std::vector<std::unique_ptr<ASGE::Sprite>> temp_sprites;

  std::vector<std::unique_ptr<Player>> players;
  std::vector<std::unique_ptr<State>> game_states;
  unsigned long long state_index = 0;
};
