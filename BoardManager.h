#pragma once
#include"GameObject.h"

class BoardManager {
public:
	//Set all the function as static so that it's up to date
	//P: Pawn
	//R: Rook
	//N: Knight
	//B: Bishop
	//K: King
	//Symbol of white piece: "PRNBQK"
	//Symbol of black piece: "prnbqk"

	static string currentBoard[8];
	static string imaginaryBoard[8];

	//Contains all of the pieces in board
	static vector<GameObject> allObjectOnBoard;
	static vector<GameObject> imaginaryObject;

	//Update board functions
	static void updateBoard();
	static void updateImaginaryObjectBoard();
	static void updateImaginaryBoard(vector<GameObject> friendObject, vector<GameObject> enemyObject);
	static void resetImaginaryBoard();

	//Contains history
	static vector<vector<GameObject>> historyList;

	//Contains all of the history
	static vector<pair<Position, Position>> logReplay;

	//Return all possible position of the piece without seeing king getting checked
	static vector<Position> obtainPosition(GameObject specifiedObject, string user);

	//Human player functions
	static bool isCheck(bool isWhite);
	static vector<Position> getAllPossibleMove(Position currentPos);
	static bool isCheckmate(bool isWhite);
	static bool isStalemate(bool isWhite);

	//AI player functions
	static bool isCheck(vector<GameObject> friendObject, vector<GameObject> enemyObject);
	static vector<Position> getAllPossibleMove(GameObject specifiedObject, vector<GameObject> friendObject, vector<GameObject> enemyObject);
	static bool isCheckmate(vector<GameObject> friendObject, vector<GameObject> enemyObject);
	static bool isStalemate(vector<GameObject> friendObject, vector<GameObject> enemyObject);
	
	static void promotePiece(Position specifiedPosition, char to);
	
	//enPassent position
	static Position enPassentW, enPassentB;
};



