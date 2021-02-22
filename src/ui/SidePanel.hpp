#pragma once

#include "conf/bonus.hpp"
#include "conf/player.hpp"
#include "game.hpp"
#include "utils.hpp"
#include <SFML/Graphics.hpp>
#include <array>

namespace lif {

class LevelManager;
class Player;

/**
 * The side panel with all game information:
 * - Number of remaining lives
 * - Remaining health
 * - EXTRA letters
 * - Bonuses
 * - Score
 * - Time left
 * A side panel always contains info about max 2 players,
 * independently from lif::MAX_PLAYERS.
 */
class SidePanel final : public sf::Drawable, private sf::NonCopyable {
	// Elements' coordinates (in pixel)
	constexpr static unsigned PLAYER_HEAD_WIDTH = 64,
	                          PLAYER_HEAD_HEIGHT = 46,
	                          PLAYER1_HEAD_X = 32,
	                          PLAYER1_HEAD_Y = 118,
	                          PLAYER2_HEAD_Y = 536,
	                          N_LIVES_X = 110,
	                          N_LIVES_Y_1 = 120,
	                          N_LIVES_Y_2 = 536,
	                          HEALTH_SYM_WIDTH = 34,
	                          HEALTH_SYM_HEIGHT = 36,
	                          HEALTH_SYM_POS_X = 30,
	                          HEALTH_SYM_POS_Y_1 = 182,
	                          HEALTH_SYM_POS_Y_2 = 598,
	                          GAME_OVER_POS_X = 32,
	                          EXTRA_LETTERS_WIDTH = 28,
	                          EXTRA_LETTERS_HEIGHT = 30,
	                          EXTRA_LETTERS_POS_X = 24,
	                          EXTRA_LETTERS_POS_Y_1 = 262,
	                          EXTRA_LETTERS_POS_Y_2 = 678,
	                          BONUS_ICON_WIDTH = 28,
	                          BONUS_ICON_HEIGHT = 26,
	                          BONUS_ICON_POS_X = 24,
	                          BONUS_ICON_POS_Y_1 = 300,
	                          BONUS_ICON_POS_Y_2 = 716,
	                          SCORE_POS_X = 24,
	                          SCORE_POS_Y_1 = 390,
	                          SCORE_POS_Y_2 = 806;
	const sf::Color DISABLED_COLOR = sf::Color(100, 100, 100, 255);
	const sf::Vector2f TIME_POS = sf::Vector2f(42, 464);

	/** The LevelManager this panel is connected with */
	const lif::LevelManager& lm;

	/** The background image */
	sf::Sprite backgroundSprite;
	sf::Texture *bgTexture;

	/** The players' heads sprites */
	std::array<sf::Sprite, 2> playerHeadsSprite;
	sf::Texture *playerHeadsTexture;

	/** The health symbols' sprites: empty, half, full */
	enum {
		HEALTH_EMPTY = 0,
		HEALTH_HALF  = 1,
		HEALTH_FULL  = 2
	};
	std::array<sf::Sprite, 3> healthSprite;
	sf::Texture *healthTexture;

	/** The EXTRA letters icons: (empty, E, X, T, R, A) */
	std::array<sf::Sprite, lif::conf::player::N_EXTRA_LETTERS + 1> extraLettersSprite;
	sf::Texture *extraLettersTexture;

	/** The Bonus icons */
	Matrix<sf::Sprite, lif::MAX_PLAYERS, lif::conf::bonus::N_PERMANENT_BONUS_TYPES> bonusesSprite;

	void _drawHealthSprites(sf::RenderTarget& window, sf::RenderStates states,
			const lif::Player& player) const;
	void _drawExtraLetters(sf::RenderTarget& window, sf::RenderStates states,
			const lif::Player& player) const;
	/** Draws the time remaining in format MM:SS */
	void _drawTime(sf::RenderTarget& window, sf::RenderStates states) const;
public:
	explicit SidePanel(const lif::LevelManager& lm);

	void update();
	void draw(sf::RenderTarget& window, sf::RenderStates states) const override;
};

}
