#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<ctype.h>

using namespace std;

class Position {
public:
	int x = -1, y = -1;
	Position() 
	{
	};

	Position(int xPos, int yPos) 
	{
		x = xPos;
		y = yPos;
	}

	void updatePosition(int xPos, int yPos) 
	{
		x = xPos;
		y = yPos;
	}

	bool operator==(Position pos) 
	{
		return x == pos.x && y == pos.y;
	}

	bool operator!=(Position pos) 
	{
		return !(*this == pos);
	}

	void operator=(Position pos) 
	{
		x = pos.x;
		y = pos.y;
	}
};

class GameObject
{
public:
	//Symbol of each pawn
	char symbol = '0';

	//for current position of game object, where we set it as (x,y)
	Position pos;

	//needed for the getAllValidMove(), whether we can eat an object or not
	bool isWhite = true;

	//this one is needed for en passant and castling, 0 means has not move, 1 means has moved and 2 for already move more
	bool hasMoved = false;

	GameObject()
	{
	};

	GameObject(Position incomingPosition, string color) 
	{
		pos.updatePosition(incomingPosition.x, incomingPosition.y);
		isWhite = color == "white" ? true : false;
	}

	void updateMove() 
	{
		hasMoved = true;
	}
	
	void operator=(GameObject* myObject) 
	{
		symbol = myObject->symbol;
		pos = myObject->pos;
		isWhite = myObject->isWhite;
		hasMoved = myObject->hasMoved;
	}
	
};

class Pawn :public GameObject 
{
public:
	Pawn(Position incomingPosition, string color) : GameObject(incomingPosition, color) 
	{
		symbol = color == "white" ? 'P' : 'p';
	}
};

class Rook :public GameObject 
{
public:
	Rook(Position incomingPosition, string color) : GameObject(incomingPosition, color) 
	{
		symbol = color == "white" ? 'R' : 'r';
	}
};

class Knight :public GameObject 
{
public:
	Knight(Position incomingPosition, string color) : GameObject(incomingPosition, color) 
	{
		symbol = color == "white" ? 'N' : 'n';
	}
};

class Bishop :public GameObject 
{
public:
	Bishop(Position incomingPosition, string color) : GameObject(incomingPosition, color) 
	{
		symbol = color == "white" ? 'B' : 'b';
	}
};

class Queen :public GameObject 
{
public:
	Queen(Position incomingPosition, string color) : GameObject(incomingPosition, color) 
	{
		symbol = color == "white" ? 'Q' : 'q';
	}
};

class King :public GameObject 
{
public:
	King(Position incomingPosition, string color) : GameObject(incomingPosition, color) 
	{
		symbol = color == "white" ? 'K' : 'k';
	}
};
