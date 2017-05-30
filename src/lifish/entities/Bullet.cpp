#include <algorithm>
#include "Bullet.hpp"
#include "AxisMoving.hpp"
#include "Animated.hpp"
#include "Clock.hpp"
#include "Sounded.hpp"
#include "GameCache.hpp"
#include "Collider.hpp"
#include "Drawable.hpp"
#include "ZIndexed.hpp"
#include "game.hpp"
#include "Temporary.hpp"
#include "utils.hpp"
#include "conf/zindex.hpp"
#include "conf/bullet.hpp"

using lif::Bullet;

Bullet::Bullet(const sf::Vector2f& pos, const lif::BulletInfo& _info, const lif::Entity *const source)
	: lif::Entity(pos)
	, info(_info)
	, data(lif::conf::bullet::data[info.id])
	, source(source)
	, speed(_info.speed)
{
	addComponent<lif::Sounded>(*this, lif::Sounded::SoundList {
		std::make_pair("hit", lif::getAsset("test", std::string("bullet")
					+ lif::to_string(info.id) + std::string("_hit.ogg"))),
		std::make_pair("shot", lif::getAsset("test", std::string("bullet")
					+ lif::to_string(info.id) + std::string("_shot.ogg")))
	});
	addComponent<lif::ZIndexed>(*this, lif::conf::zindex::BULLETS);
	clock = addComponent<lif::Clock>(*this);
	collider = addComponent<lif::Collider>(*this, [this] (lif::Collider& e) {
		// on collision
		// Note: the default layer doesn't automatically destroy a Bullet for
		// practical reasons: it is typically used as a "catch-all" layer, but
		// it should explicitly tell the bullet to selfdestroy if it's the intended
		// behaviour. The bullet otherwise self-destructs as soon as it collides.
		if (&e.getOwner() == this->source || e.getLayer() == lif::c_layers::DEFAULT)
			return;
		auto klb = get<lif::Killable>();
		if (!klb->isKilled()) {
			klb->kill();
		}
	}, info.id > 100 // FIXME this is an ugly way to set the collision layer, change to something saner.
			? lif::c_layers::BOSS_BULLETS
			: lif::c_layers::ENEMY_BULLETS,
		sf::Vector2f(data.size, data.size));

	addComponent<lif::Temporary>(*this, [this] () {
		// expire condition
		return dealtDamage
			|| (info.range > 0 && get<lif::Moving>()->getDistTravelled() > info.range)
			|| collider->isAtLimit();
	}, [this] () {
		// on kill
		_destroy();
	}, [this] () {
		// is kill in progress
		const auto& animatedSprite = get<lif::Animated>()->getSprite();
		return animatedSprite.isPlaying();
	});
}

lif::Entity* Bullet::init() {
	lif::Entity::init();
	if (collider != nullptr)
		collider->setForceAck(true);
	return this;
}

void Bullet::update() {
	lif::Entity::update();
	if (collider->isAtLimit())
		get<lif::Killable>()->kill();
	if (info.acceleration > 0) {
		const auto delta = clock->getElapsedTime().asSeconds();
		speed = std::min(info.maxSpeed, info.speed + info.acceleration * delta * delta * delta);
		moving->setSpeed(speed, true);
	}
}

void Bullet::_destroy() {
	auto animated = get<lif::Animated>();
	auto moving = get<lif::Moving>();
	auto& animatedSprite = animated->getSprite();
	lif::cache.playSound(get<lif::Sounded>()->getSoundFile("hit"));
	animatedSprite.setLooped(false);
	moving->stop();
	if (data.nDestroyFrames > 0) {
		animatedSprite.stop();
		animated->setAnimation("destroy");
		animatedSprite.play();
		animatedSprite.setPosition(lif::aligned(position));
	}
}
