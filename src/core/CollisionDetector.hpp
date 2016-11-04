#pragma once

#ifndef RELEASE
#	include "Stats.hpp"
#endif

namespace Game {

class EntityGroup;

/**
 * Abstract class for a collision detector.
 */
class CollisionDetector {
protected:
	Game::EntityGroup& group;
#ifndef RELEASE
	Game::Debug::Stats dbgStats;
#endif

public:
	explicit CollisionDetector(Game::EntityGroup& group);

	virtual void update() = 0;

#ifndef RELEASE
	const Game::Debug::Stats& getStats() const { return dbgStats; }
#else
#	error("CollisionDetector::getStats() is not available in RELEASE mode!")
#endif
};

}
