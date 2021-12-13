#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "Player.h"
#include "BoardManager.h"

const int PLAYER_NUMBER = 2; // number of players
const int MAX_HISTORY = 11; // constant for max history 10 + 1

using namespace std;

enum Difficulty { EASY, MEDIUM, HARD };
enum Status { HUMAN, AI };
enum GameStatus { CONTINUE, WHITE_WINS, BLACK_WINS, DRAW, PROMOTE };

// class GameManager
class GameManager 
{
public:
	Player* players[2] = {NULL, NULL};
	vector<AIPlayer> whiteAIs;
	vector<AIPlayer> blackAIs;
	vector<Player> humans;
	vector<int> status = { Status::HUMAN, Status::HUMAN };
	Difficulty chosenDifficulty = Difficulty::EASY;
	GameStatus gameStatus = GameStatus::CONTINUE;
	int currentPlayer = 0; // 0 for WHITE, 1 for BLACK

	int historyIndexPointer = 0;
	int logIndex = -1;
	vector<int> undoChances;
	void updateHistory(int i);

	GameManager(); // default constructor
	void initMultiplayerGame();
	void setStatus(bool firstIsHuman, bool secondIsHuman);
	void changeTurn();
	void minusTime(int amtSec);
	void wins();
	void newGame(); // reset game and score
	void resetGame(); // reset game only
	void setAIDifficulty(int recDifficulty);
	bool saveGame();
	bool loadGame();
	void getScore();
	void surrender(); 
	static pair<Position, Position> moveAI(AIPlayer& aiObj);
	int checkSituation(); // false: game ends; true: game continue
	int movePiece(Position from, Position to, bool isReplaying = false);
	void promotePawn(Position pawnPos, char newPawn);
	void undoMovement();
	void redoMovement();
	bool undoChecker();
	bool redoChecker();
};