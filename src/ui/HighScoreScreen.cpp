#include "HighScoreScreen.hpp"
#include "HighScoreManager.hpp"
#include "game.hpp"
#include "ShadedText.hpp"
#include "language.hpp"
#include "Drawable.hpp"
#include "Interactable.hpp"
#include "game.hpp"
#include "utils.hpp"

using lif::ui::HighScoreScreen;

HighScoreScreen::HighScoreScreen(const sf::RenderWindow& window, const sf::Vector2u& sz)
	: lif::ui::DynamicScreen(window, sz)
{
	build();
}

void HighScoreScreen::build() {
	name = SCREEN_NAME;
	parent = "home";
	_loadBGSprite(lif::getAsset("graphics", "screenbg1.png"));

	/* Layout:
	 *
	 * HighScores
	 * [name1]...[score1]
	 * [name2]...[score2]
	 * --
	 * [Back]
	 */
	const auto font = lif::getAsset("fonts", lif::fonts::SCREEN);
	const auto win_bounds = sf::FloatRect(0, 0, size.x, size.y);
	const auto size = 24;
	const float ipadx = 25,
	            ipady = 15;

	auto text = new lif::ShadedText(font, lif::getLocalized("high_scores"), sf::Vector2f(ipadx, ipady));
	text->setShadowSpacing(2, 2);
	text->setCharacterSize(size);
	nonInteractables.emplace_back(text);

	// Back
	text = new lif::ShadedText(font, lif::getLocalized("back"), sf::Vector2f(ipadx, ipady));
	text->setCharacterSize(size);
	auto bounds = text->getGlobalBounds();
	text->setPosition(sf::Vector2f(lif::center(bounds, win_bounds).x, win_bounds.height - 3 * bounds.height));
	interactables["back"] = std::make_unique<lif::ui::Interactable>(text);
}

void HighScoreScreen::onLoad() {
	Screen::onLoad();

	const auto font = lif::getAsset("fonts", lif::fonts::SCREEN);
	const auto win_bounds = sf::FloatRect(0, 0, size.x, size.y);
	const auto size = 18;

	sf::Vector2f pos(25, 75);

	for (const auto& entry : lif::getHighScoreManager().getSortedHighScores()) {
		auto text = new lif::ShadedText(font, entry.name, pos);
		text->setShadowSpacing(1, 1);
		text->setCharacterSize(size);
		nonInteractables.emplace_back(text);

		text = new lif::ShadedText(font, std::to_string(entry.score), pos);
		text->setPosition(sf::Vector2f(win_bounds.width - 250, text->getPosition().y));
		text->setShadowSpacing(1, 1);
		text->setCharacterSize(size);
		nonInteractables.emplace_back(text);

		pos.y += 24;
	}
}
