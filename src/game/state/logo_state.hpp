#include "game/state/state.hpp"

class LogoState : public State {
public:
	LogoState(Game& game);

	void update() override;
	void render(f64 deltaTime) override;
};
