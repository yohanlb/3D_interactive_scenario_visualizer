#include <engine/StopState.hpp>

namespace engine {
	
	/* Constructors & Destructor */
	StopState::StopState() {}

	StopState::~StopState() {}

	/* Other methods */
	const ApplicationState* StopState::enter(Game& game) {
		return nullptr;
	}
	
	ApplicationState* StopState::loop(Game& game) {
		exit(EXIT_FAILURE);
	}

	ApplicationState* StopState::processInput(Game& game) {
        return nullptr;
	}

	void StopState::update(Game& game) const {}

	void StopState::render(Game& game) const {}

}
