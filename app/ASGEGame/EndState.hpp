
#ifndef ASGEGAME_ENDSTATE_HPP
#define ASGEGAME_ENDSTATE_HPP
#include "State.hpp"

class EndState : public State
{
 public:
  EndState(Player* player_1, Player* player_2, ASGE::Font* font);
  ~EndState() override;

  void update(const ASGE::GameTime& us) override;
  void init() override;
  void fixedUpdate(const ASGE::GameTime& us) override;
  void inputHandler(ASGE::GamePadData& gamepad) override;
  void updateTexts();
  virtual std::vector<ASGE::Text*>* getRenderableText() override;

 private:
  void initModText(std::string string, float scale, ASGE::Point2D position);
  void initFixText(std::string string, float scale, ASGE::Point2D position);

  std::vector<std::unique_ptr<ASGE::Text>> fixed_texts;
  std::vector<std::unique_ptr<ASGE::Text>> modular_texts;
  std::vector<std::unique_ptr<std::string>> modular_strings;
  std::vector<ASGE::Text*> renderable_texts;
};

#endif // ASGEGAME_ENDSTATE_HPP
