#include "DroppingTextManager.hpp"
#include "DroppingSprite.hpp"
#include "EntityGroup.hpp"
#include "core.hpp"
#include <SFML/System.hpp>
#include <exception>

using lif::DroppingTextManager;

DroppingTextManager::DroppingTextManager()
	: gameOverText(new lif::DroppingSprite(
				lif::getAsset("graphics", "gameover.png"), sf::Vector2i(622, 118)))
	, extraGameText(new lif::DroppingSprite(
				lif::getAsset("graphics", "extragame.png"), sf::Vector2i(446, 312), 500.f))
	, hurryUpText(new lif::DroppingSprite(
				lif::getAsset("graphics", "hurryup.png"), sf::Vector2i(322, 60), 600.f))
{}

void DroppingTextManager::reset() {
	gameOverText->reset();
	extraGameText->reset();
	hurryUpText->reset();
}

void DroppingTextManager::subscribe(lif::EntityGroup& group) {
	group.add(gameOverText);
	group.add(extraGameText);
	group.add(hurryUpText);
}

void DroppingTextManager::trigger(DroppingTextManager::Text text) {
	switch (text) {
		using T = DroppingTextManager::Text;
	case T::GAME_OVER:
		gameOverText->play();
		break;
	case T::EXTRA_GAME:
		extraGameText->play();
		break;
	case T::HURRY_UP:
		hurryUpText->play();
		break;
	}
}

bool DroppingTextManager::isPlaying(DroppingTextManager::Text text) const {
	switch (text) {
		using T = DroppingTextManager::Text;
	case T::GAME_OVER:
		return gameOverText->isPlaying();
	case T::EXTRA_GAME:
		return extraGameText->isPlaying();
	case T::HURRY_UP:
		return hurryUpText->isPlaying();
	}
	throw std::invalid_argument("Unknown text given as argument!");
}
