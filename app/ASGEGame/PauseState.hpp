#ifndef ASGEGAME_PAUSESTATE_HPP
#define ASGEGAME_PAUSESTATE_HPP
#include "State.hpp"
#include <soloud.h>
#include <soloud_wav.h>

class PauseState : public State
{
 public:
  PauseState(Player* player_1, Player* player_2, ASGE::Font* font);
  ~PauseState() override;

  void update(const ASGE::GameTime& us) override;
  /*void render(const ASGE::GameTime& us) override;*/
  void fixedUpdate(const ASGE::GameTime& us) override;
  void inputHandler(ASGE::GamePadData& gamepad) override;
  void inputHandler(ASGE::SharedEventData data) override;
  void init() override;

  virtual std::vector<ASGE::Text*>* getRenderableText() override;
  virtual std::vector<std::unique_ptr<ASGE::Sprite>>* getRenderableSprites() override;

 private:
  SoLoud::Soloud audio_engine;
  SoLoud::Wav option_audio;
  void initAudio();

 protected:
  void initModText(std::string string, float scale, ASGE::Point2D position);
  void initFixText(std::string string, float scale, ASGE::Point2D position);
  bool inputClickCondition(ASGE::GamePadData& gamepad, int gamepad_input);
  void inputConnect(ASGE::GamePadData& gamepad);
  void loadText();
  void updateTexts();
  void handleTransitions();

  int players_connected = 0;
  int player_limit      = 2;
  int font_idx          = 0;

  std::vector<std::unique_ptr<ASGE::Text>> modular_texts;
  std::vector<std::unique_ptr<ASGE::Text>> fixed_texts;
  std::vector<ASGE::Text*> renderables;
  std::vector<std::unique_ptr<std::string>> modular_strings;
  ASGE::Text title_text    = {};
  ASGE::Text subtitle_text = {};

  unsigned long long text_select_index = 0;
  unsigned long long previous_index    = 9;
  bool play                            = true;
};
#endif // ASGEGAME_PAUSESTATE_HPP
