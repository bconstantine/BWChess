#include"BoardManager.h"

//currentBoard is the general board that the game uses to render
string BoardManager::currentBoard[8] = {
    "rnbqkbnr",
    "pppppppp",
    "00000000",
    "00000000",
    "00000000",
    "00000000",
    "PPPPPPPP",
    "RNBQKBNR",
};

//imaginaryBoard is to store te imaginary board that the AI uses to predict the game several steps ahead
string BoardManager::imaginaryBoard[8] = {
    "rnbqkbnr",
    "pppppppp",
    "00000000",
    "00000000",
    "00000000",
    "00000000",
    "PPPPPPPP",
    "RNBQKBNR",
};

//allObjectOnBoard stores the current pieces on the currentBoard whether it is white or black
//The content will always be the same even when a piece is eaten because if a piece is eaten, then only its position will be moved 
//to Position(8,8), no content will be deleted
vector<GameObject> BoardManager::allObjectOnBoard = {
    King(Position(4,7),"white"),King(Position(4,0),"black"),
    Pawn(Position(0,6),"white"),Pawn(Position(0,1),"black"),
    Pawn(Position(1,6),"white"),Pawn(Position(1,1),"black"),
    Pawn(Position(2,6),"white"),Pawn(Position(2,1),"black"),
    Pawn(Position(3,6),"white"),Pawn(Position(3,1),"black"),
    Pawn(Position(4,6),"white"),Pawn(Position(4,1),"black"),
    Pawn(Position(5,6),"white"),Pawn(Position(5,1),"black"),
    Pawn(Position(6,6),"white"),Pawn(Position(6,1),"black"),
    Pawn(Position(7,6),"white"),Pawn(Position(7,1),"black"),
    Rook(Position(0,7),"white"),Rook(Position(0,0),"black"),
    Rook(Position(7,7),"white"),Rook(Position(7,0),"black"),
    Knight(Position(1,7),"white"),Knight(Position(1,0),"black"),
    Knight(Position(6,7),"white"),Knight(Position(6,0),"black"),
    Bishop(Position(2,7),"white"),Bishop(Position(2,0),"black"),
    Bishop(Position(5,7),"white"),Bishop(Position(5,0),"black"),
    Queen(Position(3,7),"white"),Queen(Position(3,0),"black")
};

//imaginaryBoard will be used to handle imaginary positions to see whether the king will got checked or not
vector<GameObject> BoardManager::imaginaryObject = allObjectOnBoard;

//Declare history list with the first index being the init of allObjectOnBoard
vector<vector<GameObject>> BoardManager::historyList = { allObjectOnBoard };

//Declare enPassent position of each color, default value is (-1,-1)
Position BoardManager::enPassentW;
Position BoardManager::enPassentB;

//Declare log replay with no content
vector<pair<Position, Position>> BoardManager::logReplay;

//Updating board functions
//-------------------------------------------------------------
//Update the real board that is used for the render of the game
void BoardManager::updateBoard()
{
    for (int i = 0; i < 8; i++)
    {
        currentBoard[i] = "00000000";
    }

    for (int i = 0; i < allObjectOnBoard.size(); i++) {
        if (allObjectOnBoard[i].pos.y == 8 || allObjectOnBoard[i].pos.x == 8)
        {
            continue;
        }

        currentBoard[allObjectOnBoard[i].pos.y][allObjectOnBoard[i].pos.x] = allObjectOnBoard[i].symbol;
    }

    //Call this function to update the AI board so that it's up to date
    resetImaginaryBoard();
}

//Update the AI board from the current friendObject and enemyObject based on the AI's prediction
void BoardManager::updateImaginaryBoard(vector<GameObject> friendObject, vector<GameObject> enemyObject)
{
    for (int i = 0; i < 8; i++)
    {
        imaginaryBoard[i] = "00000000";
    }

    for (int i = 0; i < friendObject.size(); i++)
    {
        if (friendObject[i].pos.y == 8 || friendObject[i].pos.x == 8)
        {
            continue;
        }

        imaginaryBoard[friendObject[i].pos.y][friendObject[i].pos.x] = friendObject[i].symbol;
    }

    for (int i = 0; i < enemyObject.size(); i++)
    {
        if (enemyObject[i].pos.y == 8 || enemyObject[i].pos.x == 8)
        {
            continue;
        }

        imaginaryBoard[enemyObject[i].pos.y][enemyObject[i].pos.x] = enemyObject[i].symbol;
    }
}

//Update the imaginary board that is used to get all the moves that can be played without having your king getting checked
void BoardManager::updateImaginaryObjectBoard()
{

    //If position is on (8,8), that means that the piece is eaten
    for (int i = 0; i < 8; i++)
    {
        currentBoard[i] = "00000000";
    }

    for (int i = 0; i < imaginaryObject.size(); i++)
    {
        if (imaginaryObject[i].pos.y == 8 || imaginaryObject[i].pos.x == 8)
        {
            continue;
        }

        currentBoard[imaginaryObject[i].pos.y][imaginaryObject[i].pos.x] = imaginaryObject[i].symbol;
    }
}

//Update the AI board from the latest board;
void BoardManager::resetImaginaryBoard()
{
    for (int i = 0; i < 8; i++)
    {
        imaginaryBoard[i] = currentBoard[i];
    }
}
//-------------------------------------------------------------

void BoardManager::promotePiece(Position specifiedPosition, char to)
{
    //Find the index of the piece from the specified position
    int index = 0;
    while (specifiedPosition != allObjectOnBoard[index].pos)
    {
        index++;
    }

    //Promote the found piece to specified choice ( Queen, Knight, Rook and Bishop )
    if (allObjectOnBoard[index].isWhite)
    {
        allObjectOnBoard[index].symbol = toupper(to);
    }
    else
    {
        allObjectOnBoard[index].symbol = to;
    }

    //Update the board to change the state of the board
    updateBoard();
}

//Local functions
//-------------------------------------------------------------
//isFriend is used to check whether the piece from either currentBoard or imaginaryBoard in a specified position is a friend or enemy
bool isFriend(string currentColor, int x, int y, string tempBoard[]) 
{

   //charOfEnemy stores the possible chars of the opposite side
    string charOfEnemy;

    if (currentColor == "white")
    {
        charOfEnemy = "prnbqk";
    }
    else
    {
        charOfEnemy = "PRNBQK";
    }

    //If found enemy character, then return false
    //Else it's either a friend or '0'
    for (int i = 0; i < charOfEnemy.length(); i++) 
    {
        if (tempBoard[y][x] == charOfEnemy[i])
        {
            return false;
        }
    }

    return true;
}

//isEmpty is used to check if a specified row from a specified beginnning and ending has no pieces
bool isEmpty(int y, int xBegin, int xEnd) 
{
    for (int i = xBegin; i <= xEnd; i++) 
    {
        if (BoardManager::currentBoard[y][i] != '0')
        {
            return false;
        }
    }
        
    return true;
}

//castleChecker is used to store if the specified color king can castle and check which possible side to castle
//0 means cannot castle
//1 means can castle to the left
//2 means can castle to the right
//3 means can castle to both sides
int castleChecker(bool isWhite) 
{
    int castleDirection = 0;

    //Try to find if there is a rook on the side of the specified king that hasn't moved
    char rookSymbol = isWhite == true ? 'R' : 'r';
    int y = isWhite == true ? 7 : 0;

    for (int i = 0; i < BoardManager::imaginaryObject.size(); i++) 
    {
        if (BoardManager::imaginaryObject[i].symbol == rookSymbol && !BoardManager::imaginaryObject[i].hasMoved) 
        {
            //Check left side
            if (BoardManager::imaginaryObject[i].pos.x == 0 && BoardManager::imaginaryObject[i].pos.y == y) {
                if (isEmpty(y, 1, 3))
                {
                    castleDirection += 1;
                }
            }
            //Check right side
            else if (BoardManager::imaginaryObject[i].pos.x == 7 && BoardManager::imaginaryObject[i].pos.y == y) 
            {
                if (isEmpty(y, 5, 6))
                {
                    castleDirection += 2;
                }
            }
        }
    }
    
    return castleDirection;
}
//-------------------------------------------------------------

//obtainPosition is to find which position the specified piece can move without seeing whether the friend king can be checked or not
//All of the variables are stored before the switch because you can't store variable in a switch
vector<Position> BoardManager::obtainPosition(GameObject specifiedObject, string user) 
{

    //currentBoard is to know which board to use based on the user
    string tempBoard[8]; 
    if (user == "player") 
    {
        for (int i = 0; i < 8; i++)
        {
            tempBoard[i] = currentBoard[i];
        }
    }
    else 
    {
        for (int i = 0; i < 8; i++)
        {
            tempBoard[i] = imaginaryBoard[i];
        }
    }

    //finalPossibleMove is used to store the possible position
    vector<Position> finalPossibleMove;

    //store basic information of the specified piece
    string color = specifiedObject.isWhite ? "white" : "black";
    int xPos = specifiedObject.pos.x, yPos = specifiedObject.pos.y;

    //pawnDirection is only used for pawn
    int pawnDirection;

    //store the moves of knight and king
    const int knightDirection[][2] = { {2,1}, {2,-1}, {1,2}, {1,-2}, {-2,1}, {-2,-1}, {-1,2}, {-1,-2} };
    const int kingDirection[][2] = { {1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {-1,1}, {1,-1}, {-1,-1} };

    //castle is only used for king
    int castle = castleChecker(specifiedObject.isWhite);

    //If the current piece's position is (8,8) then it is eaten and should be skipped
    if (specifiedObject.pos.x == 8 || specifiedObject.pos.y == 8)
    {
        return finalPossibleMove;
    }

    switch (specifiedObject.symbol) 
    {
    case 'p':
    case 'P':

        if (color == "white")
        {
            pawnDirection = -1; //white pawn's direction
        }
        else
        {
            pawnDirection = 1; //black pawn's direction
        }

        //General move of the pawn
        if (xPos < 8 && xPos > -1 && yPos + pawnDirection < 8 && yPos + pawnDirection > -1) 
        {
            if (tempBoard[yPos + pawnDirection][xPos] == '0')
            {
                finalPossibleMove.push_back(Position(xPos, yPos + pawnDirection));
            }
        }

        //First move of the pawn that can move two space
        if (!specifiedObject.hasMoved) 
        {
            //If there is nothing on the way, then it is possible
            if (tempBoard[yPos + pawnDirection * 2][xPos] == '0' && tempBoard[yPos + pawnDirection][xPos] == '0')
            {
                finalPossibleMove.push_back(Position(xPos, yPos + pawnDirection * 2));
            }
        }
        
        //Eat enemy move
        if (yPos + pawnDirection < 8 && yPos + pawnDirection > -1) 
        {
            if (xPos + 1 < 8)
            {
                //If the diagonal right is enemy, then it's possible to eat
                if (!isFriend(color, xPos + 1, yPos + pawnDirection, tempBoard))
                {
                    finalPossibleMove.push_back(Position(xPos + 1, yPos + pawnDirection));
                }
            }
                
            if(xPos - 1 > -1)
            {
                //If the diagonal left is enemy, then it's possible to eat
                if (!isFriend(color, xPos - 1, yPos + pawnDirection, tempBoard))
                {
                    finalPossibleMove.push_back(Position(xPos - 1, yPos + pawnDirection));
                }
            }
        }
        
        //En passent move
        //If there is enPassent enemy position = diagonal of the pawn's position, then it's possible to eat the enemy pawn
        if (color == "white") 
        {
            if (enPassentB == Position(xPos + 1, yPos + pawnDirection) || enPassentB == Position(xPos - 1, yPos + pawnDirection))
            {
                finalPossibleMove.push_back(enPassentB);
            }
        }
        else 
        {
            if (enPassentW == Position(xPos + 1, yPos + pawnDirection) || enPassentW == Position(xPos - 1, yPos + pawnDirection))
            {
                finalPossibleMove.push_back(enPassentW);
            }
        }

        break;

    case 'q':
    case 'Q':
        //Eligible moves for queen is rook + bishop

    case 'r':
    case 'R':
        //Eligible moves for rook
        //Vertical Down
        for (int i = 1; i < 8 - yPos; i++) 
        {
            if (yPos + i > 7)
            {
                break;
            }

            if (tempBoard[yPos + i][xPos] == '0')
            {
                finalPossibleMove.push_back(Position(xPos, yPos + i));
            }

            else 
            {
                if (!isFriend(color, xPos, yPos + i, tempBoard))
                {
                    finalPossibleMove.push_back(Position(xPos, yPos + i));
                }
                break;
            }
        }

        //Vertical Up
        for (int i = 1; i <= yPos; i++) 
        {
            if (yPos - i > 7)
            {
                break;
            }

            if (tempBoard[yPos - i][xPos] == '0')
            {
                finalPossibleMove.push_back(Position(xPos, yPos - i));
            }

            else 
            {
                if (!isFriend(color, xPos, yPos - i, tempBoard))
                {
                    finalPossibleMove.push_back(Position(xPos, yPos - i));
                }
                break;
            }
        }

        //Horizontal Right
        for (int i = 1; i < 8 - xPos; i++) {
            if (xPos + i > 7)
            {
                break;
            }

            if (tempBoard[yPos][xPos + i] == '0')
            {
                finalPossibleMove.push_back(Position(xPos + i, yPos));
            }

            else {
                if (!isFriend(color, xPos + i, yPos, currentBoard))
                {
                    finalPossibleMove.push_back(Position(xPos + i, yPos));
                }
                break;
            }
        }

        //Horizontal Left
        for (int i = 1; i <= xPos; i++) {
            if (xPos - i < 0)
            {
                break;
            }

            if (tempBoard[yPos][xPos - i] == '0')
            {
                finalPossibleMove.push_back(Position(xPos - i, yPos));
            }

            else {
                if (!isFriend(color, xPos - i, yPos, tempBoard))
                {
                    finalPossibleMove.push_back(Position(xPos - i, yPos));
                }
                break;
            }
        }

        //If rook just break, if queen continue to bishop part
        if(specifiedObject.symbol == 'r' || specifiedObject.symbol == 'R')
        break;

    case 'b':
    case 'B':
        //Eligible moves for bishop
        //Diagonal Right Down
        for (int i = 1; i < 8; i++) 
        {
            if (xPos + i > 7 || yPos + i > 7)
            {
                break;
            }

            if (tempBoard[yPos + i][xPos + i] == '0')
            {
                finalPossibleMove.push_back(Position(xPos + i, yPos + i));
            }

            else 
            {
                if (!isFriend(color, xPos + i, yPos + i, tempBoard))
                {
                    finalPossibleMove.push_back(Position(xPos + i, yPos + i));
                }
                break;
            }
        }

        //Diagonal Left Down
        for (int i = 1; i < 8; i++) 
        {
            if (xPos - i < 0 || yPos + i > 7)
            {
                break;
            }

            if (tempBoard[yPos + i][xPos - i] == '0')
            {
                finalPossibleMove.push_back(Position(xPos - i, yPos + i));
            }

            else 
            {
                if (!isFriend(color, xPos - i, yPos + i, tempBoard))
                {
                    finalPossibleMove.push_back(Position(xPos - i, yPos + i));
                }
                break;
            }
        }

        //Diagonal Right Up
        for (int i = 1; i < 8; i++) 
        {
            if (xPos + i > 7 || yPos - i < 0)
            {
                break;
            }

            if (tempBoard[yPos - i][xPos + i] == '0')
            {
                finalPossibleMove.push_back(Position(xPos + i, yPos - i));
            }

            else 
            {
                if (!isFriend(color, xPos + i, yPos - i, tempBoard))
                {
                    finalPossibleMove.push_back(Position(xPos + i, yPos - i));
                }
                break;
            }
        }

        //Diagonal Left up
        for (int i = 1; i < 8; i++) 
        {
            if (xPos - i < 0 || yPos - i < 0)
            {
                break;
            }

            if (tempBoard[yPos - i][xPos - i] == '0')
            {
                finalPossibleMove.push_back(Position(xPos - i, yPos - i));
            }

            else 
            {
                if (!isFriend(color, xPos - i, yPos - i, tempBoard))
                {
                    finalPossibleMove.push_back(Position(xPos - i, yPos - i));
                }
                break;
            }
        }

        break;
       
    case 'n':
    case 'N':
        //Loop all the general possible moves of a knight
        for (int i = 0; i < 8; i++) 
        {
            //Check out of bound
            if (xPos + knightDirection[i][0] > -1 && xPos + knightDirection[i][0] < 8 && yPos + knightDirection[i][1] > -1 && yPos + knightDirection[i][1] < 8) 
            {
                //If the possible position has no piece, then it's a possible move
                if (tempBoard[yPos + knightDirection[i][1]][xPos + knightDirection[i][0]] == '0')
                {
                    finalPossibleMove.push_back(Position(xPos + knightDirection[i][0], yPos + knightDirection[i][1]));
                }
                //If the possible position has a piece, then check if it's an enemy
                //If it's an enemy, then it's a possible move
                else 
                {
                    if (!isFriend(color, xPos + knightDirection[i][0], yPos + knightDirection[i][1], tempBoard))
                    {
                        finalPossibleMove.push_back(Position(xPos + knightDirection[i][0], yPos + knightDirection[i][1]));
                    }
                }
            }
        }

        break;

    case 'k':
    case 'K':
        //Loop all the general possible moves of a king
        for (int i = 0; i < 8; i++) 
        {
            //Check out of bound
            if (xPos + kingDirection[i][0] > -1 && xPos + kingDirection[i][0] < 8 && yPos + kingDirection[i][1] > -1 && yPos + kingDirection[i][1] < 8) 
            {
                //If the possible position has no piece, then it's a possible move
                if (tempBoard[yPos + kingDirection[i][1]][xPos + kingDirection[i][0]] == '0')
                {
                    finalPossibleMove.push_back(Position(xPos + kingDirection[i][0], yPos + kingDirection[i][1]));
                }
                //If the possible position has a piece, then check if it's an enemy
                //If it's an enemy, then it's a possible move
                else 
                {
                    if (!isFriend(color, xPos + kingDirection[i][0], yPos + kingDirection[i][1], tempBoard))
                    {
                        finalPossibleMove.push_back(Position(xPos + kingDirection[i][0], yPos + kingDirection[i][1]));
                    }
                }
            }
            
        }

        //If the king hasn't moved, then it's possible to castle
        if (!specifiedObject.hasMoved) 
        {
            if (castle > 0)
            {
                if (castle == 1 || castle == 3)
                {
                    finalPossibleMove.push_back(Position(xPos - 2, yPos));
                }
                if (castle == 2 || castle == 3)
                {
                    finalPossibleMove.push_back(Position(xPos + 2, yPos));
                }
            }
        }

        break;

    default:
        break;
    }

    return finalPossibleMove;
}

//For checking
bool localCheck(vector<Position> possibleMove, int x, int y)
{
    //If a possible position from enemy piece is the same with the king then check is possible
    for (int j = 0; j < possibleMove.size(); j++)
    {
        if (possibleMove[j].x == x && possibleMove[j].y == y)
        {
            return true;
        }
    }
    return false;
}

//Player's functions
//-----------------------
//isCheck used for players
bool BoardManager::isCheck(bool isWhite)
{ 
    int kingX, kingY;
    bool enemyColor = !isWhite;

    //Store the king's position
    if (isWhite) 
    {
        kingX = imaginaryObject[0].pos.x;
        kingY = imaginaryObject[0].pos.y;
    }
    else
    {
        kingX = imaginaryObject[1].pos.x;
        kingY = imaginaryObject[1].pos.y;
    }

    for (int i = 0; i < imaginaryObject.size(); i++) 
    {
        //If the current piece's position is (8,8) then it is eaten and should be skipped
        if (imaginaryObject[i].pos.x == 8 || imaginaryObject[i].pos.y == 8)
        {
            continue;
        }

        //Loop all the enemy piece's possible moves and check if it's the same as the king's position
        if (imaginaryObject[i].isWhite == enemyColor) 
        {
            vector<Position> possibleMove = obtainPosition(imaginaryObject[i], "player");

            if (localCheck(possibleMove, kingX, kingY))
            {
                return true;
            }
        }
    }

    return false;
}

//getAllPossibleMove used for players
vector<Position> BoardManager::getAllPossibleMove(Position currentPos)
{
    //Store all the valid moves
    vector<Position> finalPossibleMove;

    //Store a temporary currentBoard
    string tempBoard[8];
    for (int i = 0; i < 8; i++)
    {
        tempBoard[i] = currentBoard[i];
    }

    //Ensure imaginaryBoard is up to date
    imaginaryObject = allObjectOnBoard;

    //Find the index of the piece from the current position
    int index = 0;
    while (currentPos != imaginaryObject[index].pos)
    {
        index++;
    }

    //Get all the possible moves of the specified piece
    vector<Position> possibleMove = obtainPosition(imaginaryObject[index], "player");

    for (int i = 0; i < possibleMove.size(); i++)
    {
        /*if (imaginaryObject[index].symbol == 'k')
        {
            if (possibleMove[i] == Position(2, 0) || possibleMove[i] == Position(6, 0))
            {
                if (isCheck(imaginaryObject[index].isWhite))
                {
                    continue;
                }
            }
        }
        if (imaginaryObject[index].symbol == 'K')
        {
            if (possibleMove[i] == Position(2, 7) || possibleMove[i] == Position(6, 7))
            {
                if (isCheck(imaginaryObject[index].isWhite))
                {
                    continue;
                }
            }
        }*/

        //Update the specified piece position based on the possible moves of the piece
        imaginaryObject[index].pos = possibleMove[i];

        //If there is an enemy piece, then enemy position is placed on position(8,8) 
        for (int i = 0; i < imaginaryObject.size(); i++)
        {
            if (imaginaryObject[i].pos == imaginaryObject[index].pos && imaginaryObject[i].isWhite != imaginaryObject[index].isWhite)
            {
                imaginaryObject[i].pos = Position(8, 8);
            }
        }

        //Update the currentBoard
        updateImaginaryObjectBoard();

        //If check then the move isn't valid
        if (!isCheck(imaginaryObject[index].isWhite))
        {
            finalPossibleMove.push_back(possibleMove[i]);
        }

        //reset imaginaryObject
        imaginaryObject = allObjectOnBoard;

        //Reset currentBoard
        for (int i = 0; i < 8; i++)
        {
            currentBoard[i] = tempBoard[i];
        }
    }

    return finalPossibleMove;
};

//isCheckmate used for players
bool BoardManager::isCheckmate(bool isWhite)
{

    //Ensure imaginaryBoard is finalPossibleMoveet and currentBoard is up to date
    imaginaryObject = allObjectOnBoard;
    updateBoard();

    //Check if friendly pieces have any valid moves left and king is in check
    //If no more valid moves are available then checkmate
    if (isCheck(isWhite))
    {
        for (int i = 0; i < allObjectOnBoard.size(); i++)
        {
            if (allObjectOnBoard[i].isWhite == isWhite)
            {
                if (getAllPossibleMove(allObjectOnBoard[i].pos).size() != 0)
                {
                    return false;
                }
            }
        }

        return true;
    }

    return false;
}

//isStalemate used for players
bool BoardManager::isStalemate(bool isWhite)
{

    //Ensure imaginaryBoard is finalPossibleMoveet and currentBoard is up to date
    imaginaryObject = allObjectOnBoard;
    updateBoard();

    //If there is any specific combinations then stalemate
    vector<GameObject> stalemateObject;
    for (int i = 0; i < allObjectOnBoard.size(); i++)
    {
        if (allObjectOnBoard[i].pos.x != 8 && allObjectOnBoard[i].pos.y != 8)
        {
            stalemateObject.push_back(allObjectOnBoard[i]);
        }
    }

    if (stalemateObject.size() == 2)
    {
        return true;
    }

    if (stalemateObject.size() == 3) {
        if (stalemateObject[2].symbol == 'n' || stalemateObject[2].symbol == 'N' || stalemateObject[2].symbol == 'B' || stalemateObject[2].symbol == 'b')
        {
            return true;
        }
    }

    //Check if friendly pieces have any valid moves left and king isn't in check
    //If no more valid moves are available then stalemate
    if (!isCheck(isWhite))
    {
        for (int i = 0; i < allObjectOnBoard.size(); i++)
        {
            if (allObjectOnBoard[i].isWhite == isWhite)
            {
                if (getAllPossibleMove(allObjectOnBoard[i].pos).size() != 0)
                {
                    return false;
                }
            }
        }
        return true;
    }

    return false;
}
//-----------------------

//AI's functions
//-----------------------
//isCheck used for AI's prediction
bool BoardManager::isCheck(vector<GameObject> friendObject, vector<GameObject> enemyObject)
{

    //Update the AI currentBoard to ensure it's up to date
    updateImaginaryBoard(friendObject, enemyObject);

    //Store king's position
    int kingX = friendObject[0].pos.x, kingY = friendObject[0].pos.y;

    //Loop all the enemy piece's possible moves and check if it's the same as the king's position
    for (int i = 0; i < enemyObject.size(); i++) 
    {
        //If the current piece's position is (8,8) then it is eaten and should be skipped
        if (enemyObject[i].pos.x == 8 || enemyObject[i].pos.y == 8)
        {
            continue;
        }

        vector<Position> possibleMove = obtainPosition(enemyObject[i], "AI");

        if (localCheck(possibleMove, kingX, kingY)) 
        {
            return true;
        }
    }

    return false;
}

//getAllPossibleMove used for AI's prediction
vector<Position> BoardManager::getAllPossibleMove(GameObject specifiedObject, vector<GameObject> friendObject, vector<GameObject> enemyObject)
{
    //Store all the valid moves
    vector<Position> finalPossibleMove;

    //Store a temporary AI currentBoard
    string tempAIboard[8];
    for (int i = 0; i < 8; i++)
    {
        tempAIboard[i] = imaginaryBoard[i];
    }

    //Find the index of the piece from the current position
    int index = 0;
    while (specifiedObject.pos != friendObject[index].pos)
    {
        index++;
    }

    //Get all the possible moves of the specified piece
    vector<Position> possibleMove = obtainPosition(friendObject[index], "AI");

    //Store temporary vector of both friend and enemy ( used to reset )
    vector<GameObject> tempFriendObject = friendObject;
    vector<GameObject> tempEnemyObject = enemyObject;

    for (int i = 0; i < possibleMove.size(); i++)
    {
        /*if (tempFriendObject[index].symbol == 'k')
        {
            if (possibleMove[i] == Position(2, 0) || possibleMove[i] == Position(6, 0))
            {
                updateImaginaryBoard(tempFriendObject, enemyObject);
                if (!isCheck(tempFriendObject, enemyObject))
                {
                    continue;
                }
            }
        }
        if (tempFriendObject[index].symbol == 'K')
        {
            if (possibleMove[i] == Position(2, 7) || possibleMove[i] == Position(6, 7))
            {
                updateImaginaryBoard(tempFriendObject, enemyObject);
                if (!isCheck(tempFriendObject, enemyObject))
                {
                    continue;
                }
            }
        }*/

        //Update the specified piece position based on the possible moves of the piece
        tempFriendObject[index].pos = possibleMove[i];

        //If there is an enemy piece, then enemy position is placed on position(8,8) 
        for (int i = 0; i < enemyObject.size(); i++)
        {
            if (enemyObject[i].pos == tempFriendObject[index].pos)
            {
                enemyObject[i].pos = Position(8, 8);
            }
        }

        //Update the AI currentBoard
        updateImaginaryBoard(tempFriendObject, enemyObject);

        //If check then the move isn't valid
        if (!isCheck(tempFriendObject, enemyObject))
        {
            finalPossibleMove.push_back(possibleMove[i]);
        }

        //Reset both vector
        tempFriendObject = friendObject;
        enemyObject = tempEnemyObject;

        //Reset AI currentBoard
        for (int i = 0; i < 8; i++)
        {
            imaginaryBoard[i] = tempAIboard[i];
        }
    }

    return finalPossibleMove;
};

//isCheckmate used for AI's prediction
bool BoardManager::isCheckmate(vector<GameObject> friendObject, vector<GameObject> enemyObject) 
{

    //Ensure AI currentBoard is up to date
    updateImaginaryBoard(friendObject, enemyObject);

    //Check if friendly pieces have any valid moves left and king is in check
    //If no more valid moves are available then checkmate
    if (isCheck(friendObject, enemyObject)) 
    {
        for (int i = 0; i < friendObject.size(); i++) 
        {
            if (getAllPossibleMove(friendObject[i], friendObject, enemyObject).size() != 0)
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

//isStalemate used for AI's prediction
bool BoardManager::isStalemate(vector<GameObject> friendObject, vector<GameObject> enemyObject) 
{

    //Ensure AI currentBoard is up to date
    updateImaginaryBoard(friendObject, enemyObject);

    //Check if friendly pieces have any valid moves left and king isn't in check
    //If no more valid moves are available then stalemate
    if (!isCheck(friendObject, enemyObject)) 
    {
        for (int i = 0; i < friendObject.size(); i++) 
        {
            if (getAllPossibleMove(friendObject[i], friendObject, enemyObject).size() != 0)
            {
                return false;
            }
        }

        return true;
    }

    return false;
}
//-----------------------




