#include "Shooting.hpp"
#include "Clock.hpp"
#include "Bullet.hpp"
#include "AxisMoving.hpp"
#include "AxisBullet.hpp"
#include "FreeBullet.hpp"
#include "GameCache.hpp"
#include "Sounded.hpp"
#include "utils.hpp"
#include <exception>

using lif::Shooting;

const sf::Time Shooting::SHOOT_FRAME_TIME = sf::milliseconds(250);

Shooting::Shooting(lif::Entity& owner, const Attack& attack)
	: lif::Component(owner)
	, attackAlign(-1.f, -1.f)
	, attack(attack)
{
	_declComponent<Shooting>();
	position = owner.getPosition();
	rechargeClock = addComponent<lif::Clock>(*this);
}

lif::Entity* Shooting::init() {
	// optional
	ownerMoving = owner.get<lif::AxisMoving>();
	return this;
}

lif::AxisBullet* Shooting::shoot(lif::Direction dir) {
	if (attack.type & lif::AttackType::CONTACT) {
		shooting = true;
		lif::cache.playSound(owner.get<lif::Sounded>()->getSoundFile("attack"));
		rechargeClock->restart();
		attackAlign = lif::tile(owner.getPosition());
		if (ownerMoving != nullptr) {
			switch (ownerMoving->getDirection()) {
			case lif::Direction::UP: --attackAlign.y; break;
			case lif::Direction::DOWN: ++attackAlign.y; break;
			case lif::Direction::LEFT: --attackAlign.x; break;
			case lif::Direction::RIGHT: ++attackAlign.x; break;
			default: break;
			}
		}
		if (attack.type & lif::AttackType::RANGED) {
			auto moving = ownerMoving == nullptr ? owner.get<lif::Moving>() : ownerMoving;
			if (moving == nullptr)
				throw std::logic_error("Called shoot() for a dashing attack on a non-Moving owner!");
			moving->setDashing(4);
		}
		return nullptr;
	}

	if (dir == lif::Direction::NONE) {
		if (ownerMoving == nullptr)
			throw std::logic_error("Called shoot(Direction::NONE) on a non-AxisMoving owner!");
		
		if (attack.type & lif::AttackType::BLOCKING) {
			if (ownerMoving != nullptr)
				ownerMoving->block(attack.blockTime);
		}
		return _doShoot(ownerMoving->getDirection());
	}

	if (attack.type & lif::AttackType::BLOCKING) {
		if (ownerMoving != nullptr)
			ownerMoving->block(attack.blockTime);
	}
	return _doShoot(dir);
}

lif::FreeBullet* Shooting::shoot(lif::Angle angle) {
	if (attack.type & lif::AttackType::CONTACT)
		throw std::logic_error("Called shoot(angle) for a CONTACT attack!");
	return _doShoot(angle);
}

bool Shooting::isRecharging() const {
	return attack.fireRate > 0 && 
		rechargeClock->getElapsedTime().asSeconds() < 1. / (fireRateMult * attack.fireRate);
}

void Shooting::update() {
	lif::Component::update();
	if (shooting && rechargeClock->getElapsedTime() > SHOOT_FRAME_TIME) {
		shooting = false;
	}
}

void Shooting::setFireRateMult(float fr) {
	if (fr <= 0)
		throw std::invalid_argument("Fire rate multiplier cannot be <= 0!");
	fireRateMult = fr;
}

void Shooting::setPosition(const sf::Vector2f& pos) {
	lif::Component::setPosition(pos);
	manualPosition = true;
}

sf::Vector2f Shooting::getPosition() const {
	return manualPosition ? position : owner.getPosition() + offset;
}

lif::AxisBullet* Shooting::_doShoot(lif::Direction dir) {
	shooting = true;
	rechargeClock->restart();
	auto bullet = new lif::AxisBullet(getPosition(), dir, attack.bullet, &owner);
	lif::cache.playSound(bullet->get<lif::Sounded>()->getSoundFile("shot"));
	return bullet;
}

lif::FreeBullet* Shooting::_doShoot(lif::Angle angle) {
	shooting = true;
	rechargeClock->restart();
	auto bullet = new lif::FreeBullet(getPosition(), angle, attack.bullet, &owner);
	lif::cache.playSound(bullet->get<lif::Sounded>()->getSoundFile("shot"));
	return bullet;
}
