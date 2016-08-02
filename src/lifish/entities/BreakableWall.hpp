#pragma once

#include "Entity.hpp"
#include "Animated.hpp"
#include "Killable.hpp"
#include "Collider.hpp"

namespace Game {

/**
 * A breakable wall
 */
class BreakableWall : public Game::Entity {
	Game::Killable *killable = nullptr;

protected:
	/** Constructor used by TransparentWall */
	BreakableWall(const sf::Vector2f& pos, const std::string& texture_name, 
			unsigned short life, unsigned int score);

	Animation& _setupAnimations(const std::string& texture_name);
	void _setupComponents(unsigned short life, unsigned int score);
	void _checkCollision(Game::Collider& cld);

public:
	constexpr static unsigned int SCORE = 10;

	explicit BreakableWall(const sf::Vector2f& pos, const unsigned short id, unsigned short life = 1);
};

}
