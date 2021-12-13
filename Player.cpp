#include "Player.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "BoardManager.h"
#include "GameObject.h"
#define TIME_LIMIT 600 //in second
using namespace std;

float kingMoveMat[8][8] = { 2.0, 3.0, 1.0, 0, 0, 1.0, 3.0, 2.0, 2.0, 2.0, 0, 0, 0, 0, 2, 2, -1, -2, -2, -2, -2, -2, -1, -2, -3, -3, -4, -4, -4, -3, -3, -3, -2, -3, -4, -4, -5, -5, -4, -4, -3, -3, -4, -4, -5, -5. - 4, -4, -3, -3, -4, -4, -5, -5, -4,  -4, -3, -3, -4, -4, -5, -5, -4, -4, -3 };
float queenMoveMat[8][8] = { -2, -1, -1, -0.5, -0.5, -1, -1, -2, -1, 0, 0.5, 0,0,0,0,-1,-1,0.5,0.5,0.5,0.5,0.5,0,-1,0,0,0.5,0.5,0.5,0.5,0,-0.5,-0.5,0,0.5,0.5,0.5,0.5,0,-0.5,-1,0,0.5,0.5,0.5,0.5,0,-1,-1,0,0,0,0,0,0,-1,-2,-1,-1,-0.5,-0.5,-1,-1,-2 };
float rookMoveMat[8][8] = {
	0,0,0,0.5,0.5,0,0,0,
	-0.5,0,0,0,0,0,0,-0.5,
	-0.5,0,0,0,0,0,0,-0.5,
	-0.5,0,0,0,0,0,0,-0.5,
	-0.5,0,0,0,0,0,0,-0.5,
	-0.5,0,0,0,0,0,0,-0.5,
	0.5,1,1,1,1,1,1,0.5,
	0,0,0,0,0,0,0,0
};
float bishopMoveMat[8][8] =
{
	-2,-1,-1,-1,-1,-1,-1,-2,
	-1,-0.5,0,0,0,0,-0.5,-1,
	-1,1,1,1,1,1,1,-1,
	-1,0,1,1,1,1,0,-1,
	-1,0.5,0.5,1,1,0.5,0.5,-1,
	-1,0,0.5,1,1,0.5,0,-1,
	-1,0,0,0,0,0,0,-1,
	-2,-1,-1,-1,-1,-1,-1,-2
};
float knightMoveMat[8][8] =
{
	-5,-4,-3,-3,-3,-3,-4,-5,
	-4,-2,0,0.5,0.5,0,-2,-4,
	-3,0.5,1,1.5,1.5,1,0.5,-3,
	-3,0,1,1.5,1.5,1,0,-3
	- 3,0,1,1.5,1.5,1,0,-3
	- 3,0.5,1,1.5,1.5,1,0.5,-3,
	-4,-2,0,0.5,0.5,0,-2,-4,
	-5,-4,-3,-3,-3,-3,-4,-5
};
float pawnMoveMat[8][8] =
{
	0,0,0,0,0,0,0,0,
	0.5,1,1,-2,-2,1,1,0.5,
	0.5,0,-1,0,0,-1,0,0.5,
	0,0,0,2,2,0,0,0,
	0.5,0.5,1,2.5,2.5,1,0.5,0.5,
	1,1,2,3,3,2,1,1,
	5,5,5,5,5,5,5,5,
	0,0,0,0,0,0,0,0
};

bool Player::resetAll()
{
	this->castling = false;
	this->pTime = TIME_LIMIT;
	this->isCheck = false;
	int pawnX = 0;
	int rookX = 0;
	int knightX = 1;
	int bishopX = 2;
	int remainingAmount[4] = { 2, 2, 2, 1 };
	//reset every position everything
	for (auto iterator = myObjects.begin(); iterator != myObjects.end(); iterator++)
	{
		if (white)
		{
			if ((*iterator)->symbol == 'P')
			{
				(*iterator)->pos.x = pawnX;
				pawnX++;
				(*iterator)->pos.y = 6;
				(*iterator)->hasMoved = false;
			}
			else if ((*iterator)->symbol == 'N')
			{
				if (remainingAmount[1] > 0)
				{
					(*iterator)->pos.x = knightX;
					knightX += 5;
					(*iterator)->pos.y = 7;
					(*iterator)->hasMoved = false;
					remainingAmount[1]--;
				}
				else
				{
					(*iterator)->symbol = 'P';
					(*iterator)->pos.x = pawnX;
					pawnX++;
					(*iterator)->pos.y = 6;
					(*iterator)->hasMoved = false;
				}

			}
			else if ((*iterator)->symbol == 'R')
			{
				if (remainingAmount[0] > 0)
				{
					(*iterator)->pos.x = rookX;
					rookX += 7;
					(*iterator)->pos.y = 7;
					(*iterator)->hasMoved = false;
					remainingAmount[0]--;
				}
				else
				{
					(*iterator)->symbol = 'P';
					(*iterator)->pos.x = pawnX;
					pawnX++;
					(*iterator)->pos.y = 6;
					(*iterator)->hasMoved = false;
				}
			}
			else if ((*iterator)->symbol == 'Q')
			{
				if (remainingAmount[3] > 0)
				{
					(*iterator)->pos.x = 3;
					(*iterator)->pos.y = 7;
					(*iterator)->hasMoved = false;
					remainingAmount[3]--;
				}
				else
				{
					(*iterator)->symbol = 'P';
					(*iterator)->pos.x = pawnX;
					pawnX++;
					(*iterator)->pos.y = 6;
					(*iterator)->hasMoved = false;
				}
			}
			else if ((*iterator)->symbol == 'B')
			{
				if (remainingAmount[2] > 0)
				{
					(*iterator)->pos.x = bishopX;
					bishopX += 3;
					(*iterator)->pos.y = 7;
					(*iterator)->hasMoved = false;
					remainingAmount[2]--;
				}
				else
				{
					(*iterator)->symbol = 'P';
					(*iterator)->pos.x = pawnX;
					pawnX++;
					(*iterator)->pos.y = 6;
					(*iterator)->hasMoved = false;
				}
			}
			else if ((*iterator)->symbol == 'K')
			{
				(*iterator)->pos.x = 4;
				(*iterator)->pos.y = 7;
				(*iterator)->hasMoved = false;
			}
		}
		else
		{
			if ((*iterator)->symbol == 'p')
			{
				(*iterator)->pos.x = pawnX;
				pawnX++;
				(*iterator)->pos.y = 1;
				(*iterator)->hasMoved = false;
			}
			else if ((*iterator)->symbol == 'n')
			{
				if (remainingAmount[1] > 0)
				{
					(*iterator)->pos.x = knightX;
					knightX += 5;
					(*iterator)->pos.y = 0;
					(*iterator)->hasMoved = false;
					remainingAmount[1]--;
				}
				else
				{
					(*iterator)->symbol = 'p';
					(*iterator)->pos.x = pawnX;
					pawnX++;
					(*iterator)->pos.y = 1;
					(*iterator)->hasMoved = false;
				}
				
			}
			else if ((*iterator)->symbol == 'r')
			{
				if (remainingAmount[0] > 0)
				{
					(*iterator)->pos.x = rookX;
					rookX += 7;
					(*iterator)->pos.y = 0;
					(*iterator)->hasMoved = false;
					remainingAmount[0]--;
				}
				else
				{
					(*iterator)->symbol = 'p';
					(*iterator)->pos.x = pawnX;
					pawnX++;
					(*iterator)->pos.y = 1;
					(*iterator)->hasMoved = false;
				}
			}
			else if ((*iterator)->symbol == 'b')
			{
				if (remainingAmount[2] > 0)
				{
					(*iterator)->pos.x = bishopX;
					bishopX += 3;
					(*iterator)->pos.y = 0;
					(*iterator)->hasMoved = false;
					remainingAmount[2]--;
				}
				else
				{
					(*iterator)->symbol = 'p';
					(*iterator)->pos.x = pawnX;
					pawnX++;
					(*iterator)->pos.y = 1;
					(*iterator)->hasMoved = false;
				}
				
			}
			else if ((*iterator)->symbol == 'q')
			{
				if (remainingAmount[3] > 0)
				{
					(*iterator)->pos.x = 3;
					(*iterator)->pos.y = 0;
					(*iterator)->hasMoved = false;
					remainingAmount[3]--;
				}
				else
				{
					(*iterator)->symbol = 'p';
					(*iterator)->pos.x = pawnX;
					pawnX++;
					(*iterator)->pos.y = 1;
					(*iterator)->hasMoved = false;
				}
				
			}
			else if ((*iterator)->symbol == 'k')
			{
				(*iterator)->pos.x = 4;
				(*iterator)->pos.y = 0;
				(*iterator)->hasMoved = false;
			}
		}
	}       
	BoardManager::updateBoard();
	return true;
}

HumanPlayer::HumanPlayer(bool white)
{
	this->white = white;
	this->myScore = 0;
	for (int index = 0; index < BoardManager::allObjectOnBoard.size(); index++)
	{
		if (BoardManager::allObjectOnBoard[index].isWhite == this->white)
		{
			myObjects.push_back(&BoardManager::allObjectOnBoard[index]);
		}
	}
	resetAll();
}

AIPlayer::AIPlayer(bool white, int difficulty)
{
	srand(time(NULL));
	this->white = white;
	this->myScore = 0;
	for (int index = 0; index < BoardManager::allObjectOnBoard.size(); index++)
	{
		if (BoardManager::allObjectOnBoard[index].isWhite == this->white)
		{
			myObjects.push_back(&BoardManager::allObjectOnBoard[index]);
		}
	}
	resetAll();
	if (difficulty == 0)
	{
		scoreRatio[0] = 5;
		scoreRatio[1] = 20;
		scoreRatio[2] = 30;
		scoreRatio[3] = 40;
		scoreRatio[4] = 45;
		scoreRatio[5] = 20;
		scoreRatio[6] = -5;
		scoreRatio[7] = -20;
		scoreRatio[8] = -30;
		scoreRatio[9] = -40;
		scoreRatio[10] = -45;
		scoreRatio[11] = -900;
		depthSearch = 2;
	}
	else if (difficulty == 1)
	{
		scoreRatio[0] = 10;
		scoreRatio[1] = 30;
		scoreRatio[2] = 30;
		scoreRatio[3] = 50;
		scoreRatio[4] = 90;
		scoreRatio[5] = 35;
		scoreRatio[6] = -10;
		scoreRatio[7] = -30;
		scoreRatio[8] = -30;
		scoreRatio[9] = -50;
		scoreRatio[10] = -90;
		scoreRatio[11] = -900;
		depthSearch = 2;
	}
	else if (difficulty == 2)
	{
		scoreRatio[0] = 10;
		scoreRatio[1] = 30;
		scoreRatio[2] = 30;
		scoreRatio[3] = 50;
		scoreRatio[4] = 90;
		scoreRatio[5] = 35;
		scoreRatio[6] = -10;
		scoreRatio[7] = -30;
		scoreRatio[8] = -30;
		scoreRatio[9] = -50;
		scoreRatio[10] = -90;
		scoreRatio[11] = -900;
		depthSearch = 2;
	}
	else
	{
		cout << "difficulty invalid, set medium\n";
		scoreRatio[0] = 10;
		scoreRatio[1] = 30;
		scoreRatio[2] = 30;
		scoreRatio[3] = 50;
		scoreRatio[4] = 90;
		scoreRatio[5] = 35;
		scoreRatio[6] = -10;
		scoreRatio[7] = -30;
		scoreRatio[8] = -30;
		scoreRatio[9] = -50;
		scoreRatio[10] = -90;
		scoreRatio[11] = -900;
		depthSearch = 2;
	}

	//initialize the matrix
}

bool Player::changePos(Position from, Position to)
{
	vector<Position> getAllPossibleMoveResult;
	bool fromObjectExist = false;
	bool toPositionValid = false;
	bool toObjectExist = false;
	GameObject* fromObjectPointer = NULL;
	GameObject* toObjectPointer = NULL;

	if (from.x == to.x && from.y == to.y)
	{
		cout << "cant move to the same position!\n";
		return false;
	}

	for (auto iterator = myObjects.begin(); iterator != myObjects.end(); iterator++)
	{
		if ((*iterator)->pos.x == from.x && (*iterator)->pos.y == from.y)
		{
			fromObjectExist = true;
			fromObjectPointer = *iterator;
			break;
		}
	}

	if (fromObjectExist == false)
	{
		cout << "no object exists\n";
		return false;
	}
	//clicking enemy piece
	else if (fromObjectPointer->isWhite != this->white)
	{
		cout << "Clicking enemy piece\n";
		return false;
	}
	getAllPossibleMoveResult = BoardManager::getAllPossibleMove(fromObjectPointer->pos);
	for (auto iterator = getAllPossibleMoveResult.begin(); iterator != getAllPossibleMoveResult.end(); iterator++)
	{
		if ((iterator)->x == to.x && (iterator)->y == to.y)
		{
			toPositionValid = true;
			break;
		}
	}

	if (toPositionValid == false)
	{
		cout << "Invalid because not included inside the getAllValidMoveResult!\n";
		return false;
	}

	for (auto iterator = BoardManager::allObjectOnBoard.begin(); iterator != BoardManager::allObjectOnBoard.end(); iterator++)
	{
		if (iterator->pos.x == to.x && iterator->pos.y == to.y)
		{
			toObjectExist = true;
			toObjectPointer = &(*iterator);
			break;
		}
	}

	if (toObjectExist)
	{
		//make sure that the eaten object is not a king
		if (toObjectPointer->symbol == 'k' || toObjectPointer->symbol == 'K')
		{
			cout << "Error! Can't eat a king!\n";
			return false;
		}
		//eating same color or castling
		else if (toObjectPointer->isWhite == fromObjectPointer->isWhite)
		{
			cout << "Error! Can't eat mutual color object\n";
			return false;
		}

		//not deciding about enPassant yet

		if (!eaten(toObjectPointer))
		{
			cout << "false eating!";
			return false;
		}
	}
	else
	{
		if (white)
		{
			//try to see castling
			if (fromObjectPointer->symbol == 'K' && abs(to.x - from.x) > 1)
			{
				//try to see castling
				//find first the object
				Position rookPos;
				rookPos.y = fromObjectPointer->pos.y;
				if (to.x > from.x)
				{
					//right castle
					rookPos.x = 7;
				}
				else
				{
					rookPos.x = 0;
				}
				bool objectFound = false;
				for (auto iterator = myObjects.begin(); iterator != myObjects.end(); iterator++)
				{
					if ((*iterator)->pos.x == rookPos.x && (*iterator)->pos.y == rookPos.y)
					{
						objectFound = true;
						toObjectPointer = (*iterator);
						break;
					}
				}
				if (!objectFound)
				{
					cout << "castling error! Rook not found!\n";
					return false;
				}
				doCastling(fromObjectPointer, toObjectPointer);
				fromObjectPointer->hasMoved = true;
				toObjectPointer->hasMoved = true;
				BoardManager::updateBoard();
				return true;
			}
			if (BoardManager::enPassentB.x != -1 && BoardManager::enPassentB.y != -1 && fromObjectPointer->symbol == 'P')
			{
				//check the position to make sure that it eats only at the right position
				if (to.x == BoardManager::enPassentB.x && to.y == BoardManager::enPassentB.y)
				{
					for (auto iterator = BoardManager::allObjectOnBoard.begin(); iterator != BoardManager::allObjectOnBoard.end(); iterator++)
					{
						if (iterator->pos.x == BoardManager::enPassentB.x && iterator->pos.y == BoardManager::enPassentB.y + 1)
						{
							toObjectPointer = &(*iterator);
							break;
						}
					}
					if (!eaten(toObjectPointer))
					{
						cout << "white en Passant fail\n";
					}
				}
			}
		}
		else
		{
			if (fromObjectPointer->symbol == 'k' && abs(to.x - from.x) > 1)
			{
				Position rookPos;
				rookPos.y = fromObjectPointer->pos.y;
				if (to.x > from.x)
				{
					//right castle
					rookPos.x = 7;
				}
				else
				{
					rookPos.x = 0;
				}

				bool objectFound = false;

				for (auto iterator = myObjects.begin(); iterator != myObjects.end(); iterator++)
				{
					if ((*iterator)->pos.x == rookPos.x && (*iterator)->pos.y == rookPos.y)
					{
						objectFound = true;
						toObjectPointer = (*iterator);
						break;
					}
				}

				if (!objectFound)
				{
					cout << "Error, Rook not found!\n";
					return false;
				}

				doCastling(fromObjectPointer, toObjectPointer);
				fromObjectPointer->hasMoved = true;
				toObjectPointer->hasMoved = true;
				BoardManager::updateBoard();
				return true;
			}
			//check en passant for white
			if (BoardManager::enPassentW.x != -1 && BoardManager::enPassentW.y != -1 && fromObjectPointer->symbol == 'p')
			{
				if (to.x == BoardManager::enPassentW.x && to.y == BoardManager::enPassentW.y)
				{
					for (auto iterator = BoardManager::allObjectOnBoard.begin(); iterator != BoardManager::allObjectOnBoard.end(); iterator++)
					{
						if (iterator->pos.x == BoardManager::enPassentW.x && iterator->pos.y == BoardManager::enPassentW.y - 1)
						{
							toObjectPointer = &(*iterator);
							break;
						}
					}
					if (!eaten(toObjectPointer))
					{
						cout << "en Passant Fail! - Black - \n";
					}
				}
			}
		}
	}
	if (fromObjectPointer->symbol == 'P' || fromObjectPointer->symbol == 'p')
	{
		if (this->white && fromObjectPointer->hasMoved == false)
		{
			if (abs(to.y - from.y) == 2)
			{
				BoardManager::enPassentW.x = to.x;
				BoardManager::enPassentW.y = to.y + 1;
				BoardManager::enPassentB.x = -1;
				BoardManager::enPassentB.y = -1;
			}
		}
		else if (fromObjectPointer->hasMoved == false)
		{
			if (abs(to.y - from.y) == 2)
			{
				BoardManager::enPassentB.x = to.x;
				BoardManager::enPassentB.y = to.y - 1;
				BoardManager::enPassentW.x = -1;
				BoardManager::enPassentW.y = -1;
			}
		}
		else
		{
			BoardManager::enPassentB.x = -1;
			BoardManager::enPassentB.y = -1;
			BoardManager::enPassentW.x = -1;
			BoardManager::enPassentW.y = -1;
		}
	}
	else
	{
		BoardManager::enPassentB.x = -1;
		BoardManager::enPassentB.y = -1;
		BoardManager::enPassentW.x = -1;
		BoardManager::enPassentW.y = -1;
	}
	fromObjectPointer->pos.x = to.x;
	fromObjectPointer->pos.y = to.y;
	fromObjectPointer->hasMoved = true;
	BoardManager::updateBoard();
	return true;
}

bool Player::doCastling(GameObject* kingPos, GameObject* rookPos)
{
	if (castling == true)
	{
		cout << "Error because castling has been done!\n";
		return false;
	}

	if (kingPos->hasMoved != false || rookPos->hasMoved != false)
	{
		cout << "Castling error! Object has already moved before\n";
		return false;
	}

	if (rookPos->pos.x < kingPos->pos.x)
	{
		rookPos->pos.x += 3;
		kingPos->pos.x -= 2;
	}
	else
	{
		rookPos->pos.x -= 2;
		kingPos->pos.x += 2;
	}
	return true;
}

bool Player::eaten(GameObject* victim)
{
	if (victim->symbol == 'k' || victim->symbol == 'K')
	{
		return false;
	}
	victim->pos.y = 8;
	victim->pos.x = 8;
	return true;
}

bool Player::doPromote(Position promotePos, char promoteChoice)
{
	for (auto iteratorVec = BoardManager::allObjectOnBoard.begin(); iteratorVec != BoardManager::allObjectOnBoard.end(); iteratorVec++)
	{
		if (iteratorVec->pos.x == promotePos.x && iteratorVec->pos.y == promotePos.y)
		{
			if (iteratorVec->symbol == 'p' || iteratorVec->symbol == 'P')
			{
				iteratorVec->symbol = promoteChoice;
				return true;
			}
		}
	}

	cout << "no correct object, fail promote\n";
	return false;
}

bool Player::doRespawn(Position respawnPos, char symbol)
{
	for (auto iterator = myObjects.begin(); iterator != myObjects.end(); iterator++)
	{
		if ((*iterator)->pos.x == 8 && (*iterator)->pos.y == 8)
		{
			if ((*iterator)->symbol == symbol)
			{
				(*iterator)->pos.x = respawnPos.x;
				(*iterator)->pos.y = respawnPos.y;
				BoardManager::updateBoard();
				return true;
			}
		}
	}
	cout << "Fail respawn! no object is found\n";
	return false;
}



bool Player::undoPromote(GameObject* demote)
{
	if (demote->symbol == 'K' || demote->symbol == 'k')
	{
		cout << "Error! King can't be demoted\n";
		return false;
	}
	else
	{
		if (white)
		{
			demote->symbol = 'P';
		}
		else
		{
			demote->symbol = 'p';
		}
		return true;
	}
}

void AIPlayer::updateObjectVector()
{
	allPieceCopy.clear();
	fieldObject.clear();
	enemyFieldObject.clear();

	allPieceCopy = BoardManager::allObjectOnBoard;
	for (auto iterator = allPieceCopy.begin(); iterator != allPieceCopy.end(); iterator++)
	{
		if (iterator->pos.x == 8 && iterator->pos.y == 8)
		{
			continue;
		}
		else
		{
			if (iterator->isWhite == this->white)
			{
				fieldObject.push_back(*iterator);
			}
			else
			{
				enemyFieldObject.push_back(*iterator);
			}
		}
	}
}

char AIPlayer::noEraseEat(Position enemy, vector<GameObject> checkVector)
{
	for (int index = 0; index < checkVector.size(); index++)
	{
		if (checkVector[index].pos.x == enemy.x && checkVector[index].pos.y == enemy.y)
		{
			if (checkVector[index].symbol == 'p' || checkVector[index].symbol == 'P')
			{
				return 'p';
			}
			else if (checkVector[index].symbol == 'n' || checkVector[index].symbol == 'N')
			{
				return 'n';
			}
			else if (checkVector[index].symbol == 'q' || checkVector[index].symbol == 'Q')
			{
				return 'q';
			}
			else if (checkVector[index].symbol == 'b' || checkVector[index].symbol == 'B')
			{
				return 'b';
			}
			else if (checkVector[index].symbol == 'r' || checkVector[index].symbol == 'R')
			{
				return 'r';
			}

		}
	}
	return '0';
}

char AIPlayer::erasedEat(Position enemy, vector<GameObject> &checkVector)
{
	for (int index = 0; index < checkVector.size(); index++)
	{
		if (checkVector[index].pos.x == enemy.x && checkVector[index].pos.y == enemy.y)
		{
			if (checkVector[index].symbol == 'p' || checkVector[index].symbol == 'P')
			{
				checkVector.erase(checkVector.begin() + index);
				return 'p';
			}
			else if (checkVector[index].symbol == 'r' || checkVector[index].symbol == 'R')
			{
				checkVector.erase(checkVector.begin() + index);
				return 'r';
			}
			else if (checkVector[index].symbol == 'n' || checkVector[index].symbol == 'N')
			{
				checkVector.erase(checkVector.begin() + index);
				return 'n';
			}
			else if (checkVector[index].symbol == 'q' || checkVector[index].symbol == 'Q')
			{
				checkVector.erase(checkVector.begin() + index);
				return 'q';
			}
			else if (checkVector[index].symbol == 'b' || checkVector[index].symbol == 'B')
			{
				checkVector.erase(checkVector.begin() + index);
				return 'b';
			}
			
		}
	}
	return '0';
}

float AIPlayer::moveIterator(int depthSearch, int currentDepth, float bScore, vector<GameObject> fieldObject, vector<GameObject> enemyFieldObject, Position& from, Position& to, vector<int> &lowest, bool &checkSet)
{
	static float alphaBest = -9999;
	bool firstTime = true; 
	if (depthSearch == currentDepth)
	{
		float returnPoints = bScore;
		if (currentDepth % 2 == 0)
		{
			//my last move
			for (int index = 0; index < fieldObject.size(); index++)
			{
				vector<Position> allAvailableMove;
				BoardManager::updateImaginaryBoard(enemyFieldObject, fieldObject);
				allAvailableMove = BoardManager::getAllPossibleMove(fieldObject[index], fieldObject, enemyFieldObject);
				for (auto moveIterator = allAvailableMove.begin(); moveIterator != allAvailableMove.end(); moveIterator++)
				{
					float totalPoints = bScore;
					vector<GameObject> myFieldCopy = fieldObject;
					myFieldCopy[index].pos.x = moveIterator->x;
					myFieldCopy[index].pos.y = moveIterator->y;
					vector<GameObject> enemyFieldCopy = enemyFieldObject;
					char returnChar = erasedEat(*moveIterator, enemyFieldCopy);
					if (myFieldCopy[index].symbol == 'k')
					{
						totalPoints += kingMoveMat[moveIterator->y][moveIterator->x];
					}
					else if (myFieldCopy[index].symbol == 'q')
					{
						totalPoints += queenMoveMat[moveIterator->y][moveIterator->x];
					}
					else if (myFieldCopy[index].symbol == 'r')
					{
						totalPoints += rookMoveMat[moveIterator->y][moveIterator->x];
					}
					else if (myFieldCopy[index].symbol == 'p')
					{
						totalPoints += pawnMoveMat[moveIterator->y][moveIterator->x];
					}
					else if (myFieldCopy[index].symbol == 'n')
					{
						totalPoints += knightMoveMat[moveIterator->y][moveIterator->x];
					}
					else if (myFieldCopy[index].symbol == 'b')
					{
						totalPoints += bishopMoveMat[moveIterator->y][moveIterator->x];
					}
					BoardManager::updateImaginaryBoard(enemyFieldCopy, myFieldCopy);
					//making enemy checkMate
					if (BoardManager::isCheckmate(enemyFieldObject, fieldObject))
					{
						totalPoints += 2000;
					}
					//making selfCheckmate
					if (BoardManager::isCheckmate(fieldObject, enemyFieldObject))
					{
						totalPoints -= 2000;
					}
					
					if (returnChar == 'p')
					{
						totalPoints += scoreRatio[0];
					}
					else if (returnChar == 'n')
					{
						totalPoints += scoreRatio[1];
					}
					else if (returnChar == 'b')
					{
						totalPoints += scoreRatio[2];
					}
					else if (returnChar == 'r')
					{
						totalPoints += scoreRatio[3];
					}
					else if (returnChar == 'q')
					{
						totalPoints += scoreRatio[4];
					}

					//check if the resulting move can cause enemy to check
					if (BoardManager::isCheck(myFieldCopy, enemyFieldCopy))
					{
						totalPoints += scoreRatio[5];
					}
					if (BoardManager::isCheck(enemyFieldCopy, myFieldCopy))
					{
						totalPoints += scoreRatio[11];
					}
					//do the min max
					if (firstTime)
					{
						firstTime = false;
						returnPoints = totalPoints;
						//re new the Position
						if (depthSearch == 0)
						{
							cout << "faultSetting!\n";
							from.x = fieldObject[index].pos.x;
							from.y = fieldObject[index].pos.y;
							to.x = myFieldCopy[index].pos.x;
							to.y = myFieldCopy[index].pos.y;
						}
					}
					else if (returnPoints == totalPoints)
					{
						int randomPick = rand() % 2;
						if (randomPick == 1)
						{
							//re new the Position
							if (depthSearch == 0)
							{
								cout << "faultSetting!\n";
								from.x = fieldObject[index].pos.x;
								from.y = fieldObject[index].pos.y;
								to.x = myFieldCopy[index].pos.x;
								to.y = myFieldCopy[index].pos.y;
							}
						}
					}
					else if(returnPoints < totalPoints)
					{
						returnPoints = totalPoints;
						//re new the Position
						if (depthSearch == 0)
						{
							cout << "faultSetting!\n";
							from.x = fieldObject[index].pos.x;
							from.y = fieldObject[index].pos.y;
							to.x = myFieldCopy[index].pos.x;
							to.y = myFieldCopy[index].pos.y;
						}
					}
				}
			}

			return returnPoints;
		}
		else
		{
			/*int worstThisRound = 9999;*/
			//enemy last move
			for (int index = 0; index < enemyFieldObject.size(); index++)
			{
				vector<Position> allAvailableMove;
				BoardManager::updateImaginaryBoard(enemyFieldObject, fieldObject);
				allAvailableMove = BoardManager::getAllPossibleMove(enemyFieldObject[index], enemyFieldObject, fieldObject);
				for (auto moveIterator = allAvailableMove.begin(); moveIterator != allAvailableMove.end(); moveIterator++)
				{
					float totalPoints = bScore;
					vector<GameObject> enemyFieldCopy = enemyFieldObject;
					vector<GameObject> myFieldCopy = fieldObject;
					enemyFieldCopy[index].pos.x = moveIterator->x;
					enemyFieldCopy[index].pos.y = moveIterator->y;
					char returnChar = erasedEat(*moveIterator, myFieldCopy);
					BoardManager::updateImaginaryBoard(enemyFieldCopy, myFieldCopy);
					//making enemy checkMate
					if (BoardManager::isCheckmate(enemyFieldObject, fieldObject))
					{
						totalPoints += 2000;
					}
					//making selfCheckmate
					if (BoardManager::isCheckmate(fieldObject, enemyFieldObject))
					{
						totalPoints -= 2000;
					}
					
					if (returnChar == 'p')
					{
						totalPoints += scoreRatio[6];
					}
					else if (returnChar == 'n')
					{
						totalPoints += scoreRatio[7];
					}
					else if (returnChar == 'b')
					{
						totalPoints += scoreRatio[8];
					}
					else if (returnChar == 'r')
					{
						totalPoints += scoreRatio[9];
					}
					else if (returnChar == 'q')
					{
						totalPoints += scoreRatio[10];
					}
					cout << "10!\n";
					//check if the resulting move can cause enemy to check
					if (BoardManager::isCheck(myFieldCopy, enemyFieldCopy))
					{
						totalPoints += scoreRatio[5];
					}
					cout << "11!\n";
					if (BoardManager::isCheck(enemyFieldCopy, myFieldCopy))
					{
						totalPoints += scoreRatio[11];
					}

					if (checkSet)
					{
						//immediately do alpha betaPruning
						cout << "aaa\n";
						if (totalPoints < alphaBest)
						{
							cout << "bbb\n";
							return -9999;
						}
					}

					//do the min max
					if (firstTime)
					{
						firstTime = false;
						returnPoints = totalPoints;
					}
					else if (returnPoints == totalPoints)
					{
						int randomPick = rand() % 2;
						if (randomPick == 1)
						{
						}
					}
					else if (returnPoints > totalPoints)
					{
						returnPoints = totalPoints;
					}
				}
			}
			return returnPoints;
		}
	}
	else
	{
		float returnPoints = bScore;
		int sameScoreRep = 0;
		bool completedCycle = false;
		if (currentDepth % 2 == 0)
		{
			//my move
			for (int index = 0; index < fieldObject.size(); index++)
			{
				vector<Position> allAvailableMove;
				BoardManager::updateImaginaryBoard(enemyFieldObject, fieldObject);
				allAvailableMove = BoardManager::getAllPossibleMove(fieldObject[index], fieldObject, enemyFieldObject);
				for (auto moveIterator = allAvailableMove.begin(); moveIterator != allAvailableMove.end(); moveIterator++)
				{
					float totalPoints = bScore;
					vector<GameObject> myFieldCopy = fieldObject;
					myFieldCopy[index].pos.x = moveIterator->x;
					myFieldCopy[index].pos.y = moveIterator->y;
					vector<GameObject> enemyFieldCopy = enemyFieldObject;
					char returnChar = erasedEat(*moveIterator, enemyFieldCopy);
					if (myFieldCopy[index].symbol == 'k')
					{
						totalPoints += kingMoveMat[moveIterator->y][moveIterator->x];
					}
					else if (myFieldCopy[index].symbol == 'q')
					{
						totalPoints += queenMoveMat[moveIterator->y][moveIterator->x];
					}
					else if (myFieldCopy[index].symbol == 'r')
					{
						totalPoints += rookMoveMat[moveIterator->y][moveIterator->x];
					}
					else if (myFieldCopy[index].symbol == 'p')
					{
						totalPoints += pawnMoveMat[moveIterator->y][moveIterator->x];
					}
					else if (myFieldCopy[index].symbol == 'n')
					{
						totalPoints += knightMoveMat[moveIterator->y][moveIterator->x];
					}
					else if (myFieldCopy[index].symbol == 'b')
					{
						totalPoints += bishopMoveMat[moveIterator->y][moveIterator->x];
					}
					cout << "13!\n";
					BoardManager::updateImaginaryBoard(enemyFieldCopy, myFieldCopy);
					cout << "14!\n";
					//making enemy checkMate
					if (BoardManager::isCheckmate(enemyFieldObject, fieldObject))
					{
						totalPoints += 2000;
					}
					cout << "15!\n";
					//making selfCheckmate
					if (BoardManager::isCheckmate(fieldObject, enemyFieldObject))
					{
						totalPoints -= 2000;
					}
					cout << "16!\n";

					if (returnChar == 'p')
					{
						totalPoints += scoreRatio[0];
					}
					else if (returnChar == 'n')
					{
						totalPoints += scoreRatio[1];
					}
					else if (returnChar == 'b')
					{
						totalPoints += scoreRatio[2];
					}
					else if (returnChar == 'r')
					{
						totalPoints += scoreRatio[3];
					}
					else if (returnChar == 'q')
					{
						totalPoints += scoreRatio[4];
					}
					//check if the resulting move can cause enemy to check
					if (BoardManager::isCheck(myFieldCopy, enemyFieldCopy))
					{
						totalPoints += scoreRatio[5];
					}
					if (BoardManager::isCheck(enemyFieldCopy, myFieldCopy))
					{
						totalPoints += scoreRatio[11];
					}
					//do recursion
					totalPoints = this->moveIterator(depthSearch, currentDepth + 1, totalPoints, myFieldCopy, enemyFieldCopy, from, to, lowest, checkSet);

					if (firstTime)
					{
						firstTime = false;
						returnPoints = totalPoints;
						from.x = fieldObject[index].pos.x;
						from.y = fieldObject[index].pos.y;
						to.x = myFieldCopy[index].pos.x;
						to.y = myFieldCopy[index].pos.y;
					}

					//do the min max
					else if (returnPoints == totalPoints)
					{
						sameScoreRep++;
						int randomPick = rand() % 2;
						if (randomPick == 1)
						{
							//re new the Position
							from.x = fieldObject[index].pos.x;
							from.y = fieldObject[index].pos.y;
							to.x = myFieldCopy[index].pos.x;
							to.y = myFieldCopy[index].pos.y;
						}
					}
					else if (returnPoints < totalPoints)
					{
						sameScoreRep = 0;
						returnPoints = totalPoints;
						alphaBest = returnPoints;
						//re new the Position
						from.x = fieldObject[index].pos.x;
						from.y = fieldObject[index].pos.y;
						to.x = myFieldCopy[index].pos.x;
						to.y = myFieldCopy[index].pos.y;
					}
					else
					{
						sameScoreRep = 0;
					}
					completedCycle = true;
				}
				
			}
			return returnPoints;
		}
		else
		{
			float worstThisRound = 9999;
			int sameScoreRep = 0;
			//enemy move
			for (int index = 0; index < enemyFieldObject.size(); index++)
			{
				vector<Position> allAvailableMove;
				BoardManager::updateImaginaryBoard(enemyFieldObject, fieldObject);
				allAvailableMove = BoardManager::getAllPossibleMove(enemyFieldObject[index], enemyFieldObject, fieldObject);
				for (auto moveIterator = allAvailableMove.begin(); moveIterator != allAvailableMove.end(); moveIterator++)
				{
					float totalPoints = bScore;
					vector<GameObject> enemyFieldCopy = enemyFieldObject;
					vector<GameObject> myFieldCopy = fieldObject;
					enemyFieldCopy[index].pos.x = moveIterator->x;
					enemyFieldCopy[index].pos.y = moveIterator->y;
					char returnChar = erasedEat(*moveIterator, myFieldCopy);
					BoardManager::updateImaginaryBoard(enemyFieldCopy, myFieldCopy);
					//making enemy checkMate
					if (BoardManager::isCheckmate(enemyFieldObject, fieldObject))
					{
						totalPoints += 2000;
					}
					//making selfCheckmate
					if (BoardManager::isCheckmate(fieldObject, enemyFieldObject))
					{
						totalPoints -= 2000;
					}

					if (returnChar == 'p')
					{
						totalPoints += scoreRatio[6];
					}
					else if (returnChar == 'n')
					{
						totalPoints += scoreRatio[7];
					}
					else if (returnChar == 'b')
					{
						totalPoints += scoreRatio[8];
					}
					else if (returnChar == 'r')
					{
						totalPoints += scoreRatio[9];
					}
					else if (returnChar == 'q')
					{
						totalPoints += scoreRatio[10];
					}
					//check if the resulting move can cause enemy to check
					if (BoardManager::isCheck(myFieldCopy, enemyFieldCopy))
					{
						totalPoints += scoreRatio[5];
					}
					if (BoardManager::isCheck(enemyFieldCopy, myFieldCopy))
					{
						totalPoints += scoreRatio[11];
					}

					//do recursion
					totalPoints = this->moveIterator(depthSearch, currentDepth + 1, totalPoints, myFieldCopy, enemyFieldCopy, from, to, lowest, checkSet);

					
					if (checkSet)
					{
						//immediately do alpha betaPruning
						if (totalPoints < alphaBest)
						{

							return -99999;
						}
						
					}
					if (firstTime)
					{
						firstTime = false;
						returnPoints = totalPoints;
					}
					//do the min max
					else if (returnPoints == totalPoints)
					{
						sameScoreRep++;
						int randomPick = rand() % 2;
					}
					else if (returnPoints > totalPoints)
					{
						sameScoreRep = 0;
						returnPoints = totalPoints;
					}
					else
					{
						sameScoreRep = 0;
					}
					worstThisRound = min(worstThisRound, totalPoints);
				}
			}
			return returnPoints;
		}
	}
}

pair<Position, Position> AIPlayer::generateTurn()
{
	Position resultFrom, resultTo;
	updateObjectVector();
	vector<int> lowestScore;
	bool checkSet = false;
	int points = moveIterator(this->depthSearch, 0, 0, this->fieldObject, this->enemyFieldObject, resultFrom, resultTo, lowestScore , checkSet);
	return make_pair(resultFrom, resultTo);
}