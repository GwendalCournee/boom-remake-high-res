#include "SHCollisionDetector.hpp"
#include "EntityGroup.hpp"
#include "collision_utils.hpp"
#include "AxisMoving.hpp"
#include "Direction.hpp"

#include "Explosion.hpp"
#include <iostream>

using namespace Game::CollisionUtils;
using Game::SHContainer;
using Game::SHCollisionDetector;

////// SHContainer ///////
SHContainer::SHContainer(const sf::Vector2f& levelSize, unsigned subdivisions)
	: levelSize(levelSize) 
	, cellSize(levelSize.x / subdivisions, levelSize.y / subdivisions)
	, subdivisions(subdivisions)
	, buckets(subdivisions * subdivisions)
{}

void SHContainer::clear() {
	for (auto& b : buckets)
		b.clear();
}

void SHContainer::insert(std::weak_ptr<Game::Collider> obj) {
	if (obj.expired()) return;
	
	auto ids = _getIdFor(*obj.lock().get());
	for (auto id : ids) {
		buckets[id].push_back(obj);
	}
}

std::unordered_set<unsigned> SHContainer::_getIdFor(const Game::Collider& obj) const {
	std::unordered_set<unsigned> ids;
	
	const auto pos = obj.getPosition();
	const auto size = obj.getSize();
	
	const sf::Vector2i upleft(
				(pos.x - Game::TILE_SIZE) / cellSize.x,
				(pos.y - Game::TILE_SIZE) / cellSize.y),
	                   downright(
				(pos.x - Game::TILE_SIZE + size.x) / cellSize.x,
				(pos.y - Game::TILE_SIZE + size.y) / cellSize.y);

	// Insert the object in all the buckets within its vertices
	for (int i = upleft.x; i <= downright.x; ++i)
		for (int j = upleft.y; j <= downright.y; ++j)
			ids.insert(j * subdivisions + i);

	for (auto it = ids.begin(); it != ids.end(); ) {
		if (*it >= buckets.size())
			it = ids.erase(it);
		else
			++it;
	}

	return ids;
}

auto SHContainer::getNearby(const Game::Collider& obj) const -> std::list<std::weak_ptr<Game::Collider>> {
	std::list<std::weak_ptr<Game::Collider>> nearby;

	auto ids = _getIdFor(obj);
	for (auto id : ids) {
		//std::cerr << id << ": " << buckets[id].size() << std::endl;
		for (auto& cld : buckets[id]) {
			// Don't bother to remove invalid objects here, as this list gets regenerated
			// at every frame.
			if (cld.expired()) continue;

			auto oth = cld.lock().get();
			if (oth != &obj)
				nearby.push_back(cld);
		}
	}
	return nearby;
}

////// SHCollisionDetector ///////
SHCollisionDetector::SHCollisionDetector(Game::EntityGroup& group,
		const sf::FloatRect& limit, unsigned subdivisions)
	: Game::CollisionDetector(group, limit)
	, container(sf::Vector2f(limit.width - limit.left, limit.height - limit.top), subdivisions)
{
#ifndef RELEASE
	dbgStats.counter.set("subdivisions", subdivisions);
#endif
}

void SHCollisionDetector::update() {
#ifndef RELEASE
	// Container setup time
	dbgStats.timer.start("setup");
#endif
	container.clear();

	/* For each moving entity, check (towards its direction):
	 * 1) has it reached the level boundaries?
	 * 2) is there another non-trasparent entity occupying the cell ahead?
	 */
	auto& colliding = group.getColliding();
	for (auto& cld : colliding) {
		// No need to check for expired, as EntityGroup prunes them before we're called
		auto collider = cld.lock();
		// reset collider
		collider->reset();
		collider->setAtLimit(false);
		container.insert(cld);
	}

#ifndef RELEASE
	dbgStats.timer.end("setup");
	// Total time taken
	dbgStats.timer.start("tot");
	// Time taken by all narrow checks
	dbgStats.timer.set("tot_narrow", 0);
	// Number of narrow-checked entities
	dbgStats.counter.reset("checked");
#endif

	// Collision detection loop
	for (auto it = colliding.begin(); it != colliding.end(); ++it) {
		auto collider = it->lock();

		// Fixed entities only collide passively
		if (collider->getOwner().get<Game::Fixed>() != nullptr)
			continue;

		const auto moving = collider->getOwner().get<Game::Moving>();
		const auto axismoving = moving ? dynamic_cast<Game::AxisMoving*>(moving) : nullptr;
		if (moving && is_at_boundaries(*collider, axismoving, levelLimit)) {
			collider->setAtLimit(true);
			continue;
		}
	
		for (auto& oth : container.getNearby(*collider.get())) {
			if (oth.expired()) continue;
#ifndef RELEASE
			dbgStats.counter.inc("checked");
			dbgStats.timer.start("single");
#endif	
			auto othcollider = oth.lock().get();
		
			if (axismoving) {
				// Only check entities ahead of this one
				if (!direction_is_viable(*collider, *axismoving, *othcollider))
					continue;

				if (collider->collidesWith(*othcollider)
						&& collide(*collider, *othcollider, axismoving->getDirection()))
				{
					//std::cerr << &collider->getOwner() << " colliding with " << &othcollider->getOwner()<<std::endl;
					collider->addColliding(oth);
					if (collider->requestsForceAck() || othcollider->requestsForceAck()
							|| othcollider->getOwner().get<Game::Moving>() == nullptr) 
					{
						// Let the entity know we collided with it.
						// We only do that for non-moving entities to avoid problems with
						// multiple collisions between two moving entities.
						othcollider->addColliding(*it);
					}
				}
			} else if (collider->contains(*othcollider) && collider->collidesWith(*othcollider)) {
				collider->addColliding(oth);
				othcollider->addColliding(*it);
			}

#ifndef RELEASE
			dbgStats.timer.set("tot_narrow", dbgStats.timer.get("tot_narrow") 
					+ dbgStats.timer.end("single"));
#endif
		}
	}

#ifndef RELEASE
	dbgStats.timer.end("tot");
#endif
}
