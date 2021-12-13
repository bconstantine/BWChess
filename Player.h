#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>
#include "BoardManager.h"
#include "GameObject.h"
using namespace std;

class Player
{
public:
	Player()
	{

	}

	int pTime = 0;
	int myScore;
	vector<GameObject*> myObjects;
	bool castling;
	bool white;  
	bool isCheck = false;

	bool resetAll();
	bool undoPromote(GameObject* demote);
	bool doPromote(Position promotePos, char promoteChoice);
	bool eaten(GameObject* victim);
	bool changePos(Position from, Position to); 
	bool doCastling(GameObject* kingPos, GameObject* rookPos);
	bool doRespawn(Position respawnPos, char symbol); 
};

class HumanPlayer : public Player
{
public:
	//human version of player
	HumanPlayer(bool white);
	HumanPlayer()
	{

	}
};

class AIPlayer : public Player
{
public:

	AIPlayer()
	{

	};

	AIPlayer(bool white, int difficulty); //difficulty = 0 - 2

	int scoreRatio[12];
	//0 enemy Pawn
	//1 enemy Knight
	//2 enemy Bishop
	//3 enemy Rook
	//4 enemy Queen
	//5 enemy King to check
	//6 self Pawn
	//7 self Knight
	//8 self Bishop
	//9 self Rook
	//10 self Queen
	//11 self King

	int depthSearch; //depth search limit 
	float weightForEndgame;


	vector<GameObject> allPieceCopy;
	vector<GameObject> fieldObject;
	vector<GameObject> enemyFieldObject;


	void updateObjectVector();
	char erasedEat(Position enemy, vector<GameObject>& checkVector);
	char noEraseEat(Position enemy, vector<GameObject> checkVector);

	void manipulateEndWeight()
	{
		weightForEndgame = ((fieldObject.size() + enemyFieldObject.size()) / 16.0) * 2;
	}


	float kingDistanceEvaluator(Position myKingPos, Position enemyKingPos)
	{
		float returnVal = 0;

		returnVal += (14 - abs(enemyKingPos.x - myKingPos.x) - abs(myKingPos.y - enemyKingPos.y));

		//add points from the distance of the enemy king to the center
		returnVal += max(3 - enemyKingPos.x, enemyKingPos.x - 4);
		returnVal += max(3 - enemyKingPos.y, enemyKingPos.y - 4);

		return returnVal * weightForEndgame; //return multiply with weight so it won't affect much early game
	}

	
	float moveIterator(int depthSearch, int currentDepth, float bScore, vector<GameObject> fieldObject, vector<GameObject> enemyFieldObject, Position &from, Position &to, vector<int> &lowest, bool &checkSet); 
	pair<Position, Position> generateTurn(); 
};