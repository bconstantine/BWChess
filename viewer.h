#pragma once
#include <future>
#include <chrono>
#include <map>
#include <algorithm>
#include "Button.h"
#include "GameManager.h"

using namespace sf;
using namespace std;

const int WINDOW_SIZE[2] = { 640, 640 };

class Viewer : public RenderWindow
{
public:
	using RenderWindow::RenderWindow;
	const int BOARD_TILE = 64;
	const Vector2i BOARD_OFFSET = Vector2i(64, 64);
	const Color BOARD_WHITE = Color(214, 214, 214);
	const Color BOARD_BLACK = Color(113, 113, 113);
	const Color BOARD_RED = Color(255, 0, 0, 160);
	const Color BOARD_ORANGE = Color(255, 165, 0 , 160);
	const Color BOARD_BLUE = Color(166, 216, 248, 160);
	const Color COLOR_BACKGROUND = Color(49, 46, 43);
	enum Buttons
	{
		UNDO, REDO, SAVE, MENU, SURRENDER
	};
	const map<int, IntRect> buttonTex
	{
		{ UNDO, IntRect(0, 0, 48, 48) },
		{ REDO, IntRect(48, 0, 48, 48) },
		{ SAVE, IntRect(48 * 2, 0, 48, 48) },
		{ MENU, IntRect(48 * 3, 0, 48, 48) },
		{ SURRENDER, IntRect(48 * 4, 0, 48, 48) }
	};
	const map<char, IntRect> pieceTex
	{
		{ 'K', IntRect(0, 0, BOARD_TILE, BOARD_TILE) },
		{ 'Q', IntRect(BOARD_TILE, 0, BOARD_TILE, BOARD_TILE) },
		{ 'B', IntRect(BOARD_TILE * 2, 0, BOARD_TILE, BOARD_TILE) },
		{ 'N', IntRect(BOARD_TILE * 3, 0, BOARD_TILE, BOARD_TILE) },
		{ 'R', IntRect(BOARD_TILE * 4, 0, BOARD_TILE, BOARD_TILE) },
		{ 'P', IntRect(BOARD_TILE * 5, 0, BOARD_TILE, BOARD_TILE) },
		{ 'k', IntRect(0, BOARD_TILE, BOARD_TILE, BOARD_TILE) },
		{ 'q', IntRect(BOARD_TILE, BOARD_TILE, BOARD_TILE, BOARD_TILE) },
		{ 'b', IntRect(BOARD_TILE * 2, BOARD_TILE, BOARD_TILE, BOARD_TILE) },
		{ 'n', IntRect(BOARD_TILE * 3, BOARD_TILE, BOARD_TILE, BOARD_TILE) },
		{ 'r', IntRect(BOARD_TILE * 4, BOARD_TILE, BOARD_TILE, BOARD_TILE) },
		{ 'p', IntRect(BOARD_TILE * 5, BOARD_TILE, BOARD_TILE, BOARD_TILE) }
	};
	enum Scene
	{
		MAIN_MENU, GAME
	};
	Image icon;
	Texture texButtons;
	Texture texPieces;
	Font font;
	int curScene = Scene::MAIN_MENU;
	bool animate(Sprite&, Vector2f, Vector2f, float, float);
	bool isPosInsideBoard(const Vector2i) const;
	string getDisplayTime(int) const;
	void loopMenu();
	void loopGame();
	void initialise();
	void loop();
};
