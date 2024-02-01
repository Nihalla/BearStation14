
#ifndef ASGEGAME_GAMEPLAYSTATE_HPP
#define ASGEGAME_GAMEPLAYSTATE_HPP

#include <random>

#include "Character.hpp"
#include "State.hpp"
#include "TileMap.h"
#include <soloud.h>
#include <soloud_wav.h>

class GameplayState : public State
{
 public:
  GameplayState(Player* player_1, Player* player_2, ASGE::Font* font);
  ~GameplayState() override;

  void init() override;

  void update(const ASGE::GameTime& us) override;
  void fixedUpdate(const ASGE::GameTime& us) override;
  void updateTexts();

  void inputHandler(ASGE::GamePadData& gamepad) override;

  std::vector<ASGE::Text*>* getRenderableText() override;
  std::vector<std::unique_ptr<ASGE::Sprite>>* getRenderableSprites() override;
  std::vector<std::unique_ptr<ASGE::Camera>>* getCameras();
  std::vector<std::unique_ptr<ASGE::Sprite>>* getMinigameSprites();
  int getNumberOfCameras();

  unsigned long long findPlayerWithController(int controller_id);
  bool checkPlayerCollisions(Boundary bound, int ignore_id);
  void attemptAttack(int attacker_gamepad_idx);
  unsigned long long findNearestPlayer(ASGE::Point2D attacking_player, int ignore_id = -1);

  std::vector<std::unique_ptr<ASGE::Sprite>> renderable_sprites;
  std::vector<std::unique_ptr<ASGE::Sprite>> minigame_sprites;
  TileMap* tilemap;

 private:
  void initAudio();

  SoLoud::Soloud audio_engine;
  SoLoud::Wav human_audio;
  SoLoud::Wav bear_audio;
  SoLoud::Wav success_audio;
  SoLoud::Wav failure_audio;

 protected:
  void initMinigame(Player* player);
  void initModText(std::string string, float scale, ASGE::Point2D position);
  void initHPText(std::string string, float scale, ASGE::Point2D position);
  void setUpCamera();

  void cameraUpdate();
  void updateScores();
  void updateIcons();

  void playerSwap(Player* attacking_player, Player* defending_player);
  void characterSwap(unsigned long long first_player_id, unsigned long long second_player_id);

  bool checkMinigameGuesses(std::vector<Character::GuessColours> guesses);
  void resetMinigameGuesses();
  void playMinigameSequence();
  void deleteMinigameSprites();

  std::vector<std::unique_ptr<ASGE::Camera>> camera_vector;
  std::vector<std::unique_ptr<ASGE::Text>> modular_texts;
  std::vector<std::unique_ptr<ASGE::Text>> hp_texts;
  std::vector<ASGE::Text*> renderable_text;
  std::vector<std::unique_ptr<std::string>> modular_strings;
  std::vector<std::unique_ptr<std::string>> hp_strings;

  std::vector<Character::GuessColours> minigame_solution;

  int font_idx                = 0;
  double run_timer            = 300;
  int timer_display_increment = 0;

  float GAMEPAD_DEADZONE        = 0.1F;
  unsigned long long FAIL_INDEX = 99;
};

#endif // ASGEGAME_GAMEPLAYSTATE_HPP
