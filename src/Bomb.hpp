#pragma once
/**
 * The players' bomb
 */

#include "FixedEntity.hpp"
#include "Animated.hpp"

namespace Game {

class Bomb : public Game::FixedEntity, public Game::Animated {
	sf::Clock fuseClock;
	float fuseTime;
	unsigned short radius;
	bool ignited = false;
public:
	constexpr static float DEFAULT_FUSE = 5; // seconds
	constexpr static unsigned short DEFAULT_RADIUS = 2;

	Bomb(const sf::Vector2f& pos, const float fuseTime = DEFAULT_FUSE, const unsigned short radius = DEFAULT_RADIUS);

	void draw(sf::RenderTarget& window) override;
	void setOrigin(const sf::Vector2f& pos) override { Game::Animated::setOrigin(pos); }

	void ignite() { fuseClock.restart(); ignited = true; }

	inline bool exploded() const { return ignited && fuseClock.getElapsedTime().asSeconds() >= fuseTime; }
};

}