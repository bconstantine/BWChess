#include "GameManager.h"

GameManager::GameManager() {
	// init collection of ai for white and black

	for (int i = 0; i < 3; i++) {
		AIPlayer aiObj1(true, i);
		this->whiteAIs.push_back(aiObj1);
		AIPlayer aiObj2(false, i);
		this->blackAIs.push_back(aiObj2);
	}

	for (int i = 0; i < 2; i++) {
		HumanPlayer humanObj(!i);
		this->humans.push_back(humanObj);
	}

	for (int i = 0; i < PLAYER_NUMBER; i++) {
		this->undoChances.push_back(10);
	}

}

void GameManager::initMultiplayerGame() {
	for (int i = 0; i < PLAYER_NUMBER; i++) {
		players[i] = &humans[i];
	}

}

void GameManager::setStatus(bool firstIsHuman, bool secondIsHuman) {
	if (firstIsHuman) {
		this->status[0] = Status::HUMAN;
	}
	else {
		this->status[0] = Status::AI;
	}

	if (secondIsHuman) {
		this->status[1] = Status::HUMAN;
	}
	else {
		this->status[1] = Status::AI;
	}
}

void GameManager::changeTurn() {
	this->currentPlayer = (this->currentPlayer + 1) % 2;
}

void GameManager::minusTime(int amtSec) {
	if (this->status[this->currentPlayer] == Status::HUMAN) {
		players[this->currentPlayer]->pTime--; // reduce players time by 1 sec
	}
	else {
		players[this->currentPlayer]->pTime -= amtSec;
	}
	if (players[this->currentPlayer]->pTime == 0) {
		if (this->currentPlayer == 0) {
			gameStatus = BLACK_WINS;
		}
		else {
			gameStatus = WHITE_WINS;
		}
	}

}

void GameManager::wins() {

	players[this->currentPlayer]->myScore++;

}

void GameManager::newGame() {

	for (int i = 0; i < PLAYER_NUMBER; i++) {
		players[i]->resetAll();
		players[i]->myScore = 0;
	}
}

void GameManager::resetGame() {

	for (int i = 0; i < PLAYER_NUMBER; i++) {
		players[i]->resetAll();
	}

	gameStatus = CONTINUE;
	this->currentPlayer = 0;
	this->chosenDifficulty = Difficulty::EASY;
	this->status = { Status::HUMAN, Status::HUMAN };
	this->historyIndexPointer = 0;
	BoardManager::historyList.clear();
	BoardManager::historyList.push_back(BoardManager::allObjectOnBoard);

}

void GameManager::setAIDifficulty(int recDifficulty) {

	switch (recDifficulty)
	{
	case 0:
		chosenDifficulty = Difficulty::EASY;
		break;
	case 1:
		chosenDifficulty = Difficulty::MEDIUM;
		break;
	case 2:
		chosenDifficulty = Difficulty::HARD;
		break;
	default:
		break;
	}

	for (int i = 0; i < PLAYER_NUMBER; i++) {
		bool isWhite;
		if (i == 0) {
			isWhite = true;
		}
		else {
			isWhite = false;
		}
		
		if (isWhite) {
			if (status[i] == Status::AI) {
				whiteAIs[chosenDifficulty].myObjects = players[i]->myObjects;
				whiteAIs[chosenDifficulty].isCheck = players[i]->isCheck;
				whiteAIs[chosenDifficulty].myScore = players[i]->myScore;
				whiteAIs[chosenDifficulty].white = players[i]->white;
				whiteAIs[chosenDifficulty].pTime = players[i]->pTime;
				whiteAIs[chosenDifficulty].castling = players[i]->castling;

				players[i] = &whiteAIs[chosenDifficulty];
			}
		}
		else {
			if (status[i] == Status::AI) {
				blackAIs[chosenDifficulty].myObjects = players[i]->myObjects;
				blackAIs[chosenDifficulty].isCheck = players[i]->isCheck;
				blackAIs[chosenDifficulty].myScore = players[i]->myScore;
				blackAIs[chosenDifficulty].white = players[i]->white;
				blackAIs[chosenDifficulty].pTime = players[i]->pTime;
				blackAIs[chosenDifficulty].castling = players[i]->castling;

				players[i] = &blackAIs[chosenDifficulty];
			}
		}

	}

}

bool GameManager::saveGame() {

	ofstream of;

	of.open("save_file.txt");

	if (!of.is_open()) {
		cout << "Failed when opening file! Please check your file name again." << endl;
		return false;
	}

	// write piece on board
	for (int i = 0; i < 32; i++) {
		of << BoardManager::allObjectOnBoard[i].symbol << " " << BoardManager::allObjectOnBoard[i].isWhite << " " << BoardManager::allObjectOnBoard[i].hasMoved << " " << BoardManager::allObjectOnBoard[i].pos.x << " " << BoardManager::allObjectOnBoard[i].pos.y << endl;
	}

	// write history list
	for (int c = 0; c < BoardManager::historyList.size(); c++) {
		of << "*" << " ";
		for (int i = 0; i < 32; i++) {
			of << BoardManager::historyList[c][i].symbol << " " << BoardManager::historyList[c][i].isWhite << " " << BoardManager::historyList[c][i].hasMoved << " " << BoardManager::historyList[c][i].pos.x << " " << BoardManager::historyList[c][i].pos.y << endl;
		}
	}
	of << "*****" << endl;

	// write gamemanager content
	// difficulty gamestate currentplayer historyindex
	of << chosenDifficulty << " " << gameStatus << " " << currentPlayer << " " << historyIndexPointer << endl;

	for (int i = 0; i < 2; i++) {
		of << humans[i].myScore << " " << humans[i].white << " " << humans[i].pTime << " " << humans[i].castling << " " << humans[i].isCheck << endl;
	}

	for (int i = 0; i < 3; i++) {
		of << whiteAIs[i].myScore << " " << whiteAIs[i].white << " " << whiteAIs[i].pTime << " " << whiteAIs[i].castling << " " << whiteAIs[i].isCheck << " " << whiteAIs[i].depthSearch << endl;
	}

	for (int i = 0; i < 3; i++) {
		of << blackAIs[i].myScore << " " << blackAIs[i].white << " " << blackAIs[i].pTime << " " << blackAIs[i].castling << " " << blackAIs[i].isCheck << " " << blackAIs[i].depthSearch << endl;
	}

	of << status[0] << " " << status[1] << endl;

	for (int i = 0; i < BoardManager::logReplay.size(); i++) {
		cout << "* " << BoardManager::logReplay[i].first.x << " " << BoardManager::logReplay[i].first.y << " " << BoardManager::logReplay[i].second.x << " " << BoardManager::logReplay[i].second.y << endl;
		of << "* " << BoardManager::logReplay[i].first.x << " " << BoardManager::logReplay[i].first.y << " " << BoardManager::logReplay[i].second.x << " " << BoardManager::logReplay[i].second.y << endl;
	}
	of << "*****" << endl;

	of << logIndex << endl;


	

	of.close();



	return true;
}

bool GameManager::loadGame() {

	ifstream inf;

	inf.open("save_file.txt");

	if (!inf.is_open()) {
		cout << "Failed when opening file! Please check your file name again." << endl;
		return false;
	}

	// read piece on board
	for (int i = 0; i < 32; i++) {
		inf >> BoardManager::allObjectOnBoard[i].symbol >> BoardManager::allObjectOnBoard[i].isWhite >> BoardManager::allObjectOnBoard[i].hasMoved >> BoardManager::allObjectOnBoard[i].pos.x >> BoardManager::allObjectOnBoard[i].pos.y;
	}
	
	BoardManager::updateBoard();

	string tmp;
	int historyCounter = 0;
	BoardManager::historyList.clear();
	while (inf >> tmp) {
		if (tmp == "*****") break;
		cout << tmp << endl;
		vector<GameObject> tmpColObj;
		GameObject tmpObj;

		for (int i = 0; i < 32; i++) {
			inf >> tmpObj.symbol >> tmpObj.isWhite >> tmpObj.hasMoved >> tmpObj.pos.x >> tmpObj.pos.y;
			tmpColObj.push_back(tmpObj);
		}

		BoardManager::historyList.push_back(tmpColObj);

	}

	cout << BoardManager::historyList.size() << endl;

	// difficulty gamestate currentplayer historyindex
	int tmpInt;
	// difficulty
	inf >> tmpInt;
	switch (tmpInt)
	{
	case 0:
		this->chosenDifficulty = Difficulty::EASY;
		break;
	case 1:
		this->chosenDifficulty = Difficulty::MEDIUM;
		break;
	case 2:
		this->chosenDifficulty = Difficulty::HARD;
		break;
	}
	
	// gamestate
	inf >> tmpInt;
	switch (tmpInt)
	{
	case 0:
		this->gameStatus = GameStatus::CONTINUE;
		break;
	case 1:
		this->gameStatus = GameStatus::WHITE_WINS;
		break;
	case 2:
		this->gameStatus = GameStatus::BLACK_WINS;
		break;
	case 3:
		this->gameStatus = GameStatus::DRAW;
		break;
	case 4:
		this->gameStatus = GameStatus::PROMOTE;
		break;
	}

	// current player
	inf >> this->currentPlayer;

	// history index
	inf >> this->historyIndexPointer;

	humans.clear();
	for (int i = 0; i < 2; i++) {
		Player tmpPlayer;
		inf >> tmpPlayer.myScore;
		inf >> tmpInt;
		if (tmpInt == 0) {
			tmpPlayer.white = false;
		}
		else {
			tmpPlayer.white = true;
		}
		inf >> tmpPlayer.pTime;
		inf >> tmpInt;
		if (tmpInt == 0) {
			tmpPlayer.castling = false;
		}
		else {
			tmpPlayer.castling = true;
		}
		inf >> tmpInt;
		if (tmpInt == 0) {
			tmpPlayer.isCheck = false;
		}
		else {
			tmpPlayer.isCheck = true;
		}

		// pointer to piece on board
		if (i == 0) { // white
			for (int j = 0, x = 0; j < BoardManager::allObjectOnBoard.size(); j += 2, x++) {
				tmpPlayer.myObjects.push_back(&BoardManager::allObjectOnBoard[j]);
			}
		}
		else {
			for (int j = 1, x = 0; j < BoardManager::allObjectOnBoard.size(); j += 2, x++) {
				tmpPlayer.myObjects.push_back(&BoardManager::allObjectOnBoard[j]);
			}
		}
		humans.push_back(tmpPlayer);
	}

	whiteAIs.clear();
	for (int i = 0; i < 3; i++) {
		AIPlayer aiPlayer;
		inf >> aiPlayer.myScore;
		inf >> tmpInt;
		if (tmpInt == 0) {
			aiPlayer.white = false;
		}
		else {
			aiPlayer.white = true;
		}
		inf >> aiPlayer.pTime;
		inf >> tmpInt;
		if (tmpInt == 0) {
			aiPlayer.castling = false;
		}
		else {
			aiPlayer.castling = true;
		}
		inf >> tmpInt;
		if (tmpInt == 0) {
			aiPlayer.isCheck = false;
		}
		else {
			aiPlayer.isCheck = true;
		}
		inf >> aiPlayer.depthSearch;

		if (i == 0)
		{
			aiPlayer.scoreRatio[0] = 5;
			aiPlayer.scoreRatio[1] = 20;
			aiPlayer.scoreRatio[2] = 30;
			aiPlayer.scoreRatio[3] = 40;
			aiPlayer.scoreRatio[4] = 45;
			aiPlayer.scoreRatio[5] = 900;
			aiPlayer.scoreRatio[6] = -5;
			aiPlayer.scoreRatio[7] = -20;
			aiPlayer.scoreRatio[8] = -30;
			aiPlayer.scoreRatio[9] = -40;
			aiPlayer.scoreRatio[10] = -45;
			aiPlayer.scoreRatio[11] = -900;
			aiPlayer.depthSearch = 2;
		}
		else if (i == 1)
		{
			aiPlayer.scoreRatio[0] = 10;
			aiPlayer.scoreRatio[1] = 30;
			aiPlayer.scoreRatio[2] = 30;
			aiPlayer.scoreRatio[3] = 50;
			aiPlayer.scoreRatio[4] = 90;
			aiPlayer.scoreRatio[5] = 900;
			aiPlayer.scoreRatio[6] = -10;
			aiPlayer.scoreRatio[7] = -30;
			aiPlayer.scoreRatio[8] = -30;
			aiPlayer.scoreRatio[9] = -50;
			aiPlayer.scoreRatio[10] = -90;
			aiPlayer.scoreRatio[11] = -900;
			aiPlayer.depthSearch = 3;
		}
		else if (i == 2)
		{
			aiPlayer.scoreRatio[0] = 10;
			aiPlayer.scoreRatio[1] = 30;
			aiPlayer.scoreRatio[2] = 30;
			aiPlayer.scoreRatio[3] = 50;
			aiPlayer.scoreRatio[4] = 90;
			aiPlayer.scoreRatio[5] = 900;
			aiPlayer.scoreRatio[6] = -10;
			aiPlayer.scoreRatio[7] = -30;
			aiPlayer.scoreRatio[8] = -30;
			aiPlayer.scoreRatio[9] = -50;
			aiPlayer.scoreRatio[10] = -90;
			aiPlayer.scoreRatio[11] = -900;
			aiPlayer.depthSearch = 4;
		}

		for (int j = 0, x = 0; j < BoardManager::allObjectOnBoard.size(); j += 2, x++) {
			aiPlayer.myObjects.push_back(&BoardManager::allObjectOnBoard[j]);
		}

		whiteAIs.push_back(aiPlayer);
	}

	blackAIs.clear();
	for (int i = 0; i < 3; i++) {
		AIPlayer aiPlayer;
		inf >> aiPlayer.myScore;
		inf >> tmpInt;
		if (tmpInt == 0) {
			aiPlayer.white = false;
		}
		else {
			aiPlayer.white = true;
		}
		inf >> aiPlayer.pTime;
		inf >> tmpInt;
		if (tmpInt == 0) {
			aiPlayer.castling = false;
		}
		else {
			aiPlayer.castling = true;
		}
		inf >> tmpInt;
		if (tmpInt == 0) {
			aiPlayer.isCheck = false;
		}
		else {
			aiPlayer.isCheck = true;
		}
		inf >> aiPlayer.depthSearch;

		if (i == 0)
		{
			aiPlayer.scoreRatio[0] = 5;
			aiPlayer.scoreRatio[1] = 20;
			aiPlayer.scoreRatio[2] = 30;
			aiPlayer.scoreRatio[3] = 40;
			aiPlayer.scoreRatio[4] = 45;
			aiPlayer.scoreRatio[5] = 900;
			aiPlayer.scoreRatio[6] = -5;
			aiPlayer.scoreRatio[7] = -20;
			aiPlayer.scoreRatio[8] = -30;
			aiPlayer.scoreRatio[9] = -40;
			aiPlayer.scoreRatio[10] = -45;
			aiPlayer.scoreRatio[11] = -900;
			aiPlayer.depthSearch = 2;
		}
		else if (i == 1)
		{
			aiPlayer.scoreRatio[0] = 10;
			aiPlayer.scoreRatio[1] = 30;
			aiPlayer.scoreRatio[2] = 30;
			aiPlayer.scoreRatio[3] = 50;
			aiPlayer.scoreRatio[4] = 90;
			aiPlayer.scoreRatio[5] = 900;
			aiPlayer.scoreRatio[6] = -10;
			aiPlayer.scoreRatio[7] = -30;
			aiPlayer.scoreRatio[8] = -30;
			aiPlayer.scoreRatio[9] = -50;
			aiPlayer.scoreRatio[10] = -90;
			aiPlayer.scoreRatio[11] = -900;
			aiPlayer.depthSearch = 3;
		}
		else if (i == 2)
		{
			aiPlayer.scoreRatio[0] = 10;
			aiPlayer.scoreRatio[1] = 30;
			aiPlayer.scoreRatio[2] = 30;
			aiPlayer.scoreRatio[3] = 50;
			aiPlayer.scoreRatio[4] = 90;
			aiPlayer.scoreRatio[5] = 900;
			aiPlayer.scoreRatio[6] = -10;
			aiPlayer.scoreRatio[7] = -30;
			aiPlayer.scoreRatio[8] = -30;
			aiPlayer.scoreRatio[9] = -50;
			aiPlayer.scoreRatio[10] = -90;
			aiPlayer.scoreRatio[11] = -900;
			aiPlayer.depthSearch = 4;
		}

		for (int j = 1, x = 0; j < BoardManager::allObjectOnBoard.size(); j += 2, x++) {
			aiPlayer.myObjects.push_back(&BoardManager::allObjectOnBoard[j]);
		}

		blackAIs.push_back(aiPlayer);
	}

	for (int i = 0; i < PLAYER_NUMBER; i++) {
		inf >> tmpInt;
		if (tmpInt == 0) {
			status[i] = Status::HUMAN;
		}
		else {
			status[i] = Status::AI;
		}
	}

	// CHECK AND ASSIGN PLAYER DATA
	for (int i = 0; i < PLAYER_NUMBER; i++) {
		switch (status[i])
		{
		case Status::HUMAN:
			players[i] = &humans[i];
			break;
		case Status::AI:
			if (i == 0) {
				players[i] = &whiteAIs[chosenDifficulty];
			}
			else {
				players[i] = &blackAIs[chosenDifficulty];
			}
		default:
			break;
		}
	}

	BoardManager::logReplay.clear();

	while (inf >> tmp) {
		if (tmp == "*****") break;
		cout << tmp << " ";
		pair<Position, Position> logRead;
		inf >> logRead.first.x >> logRead.first.y >> logRead.second.x >> logRead.second.y;
		cout << logRead.first.x << " " << logRead.first.y << " " << logRead.second.x << " " << logRead.second.y << endl;
		BoardManager::logReplay.push_back(logRead);
	}

	inf >> this->logIndex;

	inf.close();
	return true;
}

void GameManager::getScore() {
	// print both myScore in console

	for (int i = 0; i < PLAYER_NUMBER; i++) {
		cout << "Player" << i + 1 << " myScore: " << players[i]->myScore << endl;
	}
	
}

void GameManager::surrender() {

	if (this->currentPlayer == 0) {
		gameStatus = BLACK_WINS;
	}
	else if (this->currentPlayer == 1) {
		gameStatus = WHITE_WINS;
	}

}

void GameManager::updateHistory(int index) {

	BoardManager::historyList.resize(index + 1);
	BoardManager::historyList.push_back(BoardManager::allObjectOnBoard);

	this->historyIndexPointer++;

}

pair<Position, Position> GameManager::moveAI(AIPlayer& aiObj) {

	vector<int> lowestmyScore;
	Position resultForm, resultTo;
	bool checkIsSet = false;
	aiObj.updateObjectVector();
	int points = aiObj.moveIterator(aiObj.depthSearch, 0, 0, aiObj.fieldObject, aiObj.enemyFieldObject, resultForm, resultTo, lowestmyScore, checkIsSet);
	return make_pair(resultForm, resultTo);
	
}

int GameManager::checkSituation() {

	bool isWhite;
	if (this->currentPlayer == 0) {
		isWhite = true;
	}
	else {
		isWhite = false;
	}

	if (gameStatus != WHITE_WINS && gameStatus != BLACK_WINS) {
		if (BoardManager::isCheckmate(isWhite)) {
			if (isWhite) {
				gameStatus = GameStatus::BLACK_WINS;
				return GameStatus::BLACK_WINS;
			}
			else {
				gameStatus = GameStatus::WHITE_WINS;
				return GameStatus::WHITE_WINS;
			}
		}
		else if (BoardManager::isStalemate(isWhite)) {
			gameStatus = GameStatus::DRAW;
			return GameStatus::DRAW;
		}
		else {
			// reset variables checkedPlayer
			players[0]->isCheck = false;
			players[1]->isCheck = false;
			if (BoardManager::isCheck(isWhite)) {
				if (isWhite) {
					players[0]->isCheck = true;
				}
				else {
					players[1]->isCheck = true;
				}
			}
			gameStatus = GameStatus::CONTINUE;
			return GameStatus::CONTINUE;
		}
	}
	else {
		return gameStatus;
	}
}

int GameManager::movePiece(Position from, Position to, bool isReplaying) {
	// check piece whether is a pawn
	GameObject chosenPiece;

	Player cPlayer = *this->players[this->currentPlayer];

	// search for the "chosenPiece" symbol
	for (int i = 0; i < cPlayer.myObjects.size(); i++) {
		if (cPlayer.myObjects[i]->pos.x == from.x && cPlayer.myObjects[i]->pos.y == from.y) {
			chosenPiece = cPlayer.myObjects[i];
			break;
		}
	}

	this->players[this->currentPlayer]->changePos(from, to); // move the piece

	if (!isReplaying) {
		if (BoardManager::logReplay.size() > logIndex + 1) {
			BoardManager::logReplay.resize(logIndex + 1);
		}
		BoardManager::logReplay.push_back(make_pair(from, to));
		logIndex++;
	}

	for (auto log : BoardManager::logReplay) printf("%d,%d -> %d,%d\n", log.first.x, log.first.y, log.second.x, log.second.y);

	// return game state
	// check chosen piece is a pawn(?)
	if (chosenPiece.symbol == 'P') { // white pawn

		if (to.y == 0) { // pawn is ready to be promoted
			gameStatus = GameStatus::PROMOTE;
			return GameStatus::PROMOTE;
		}

	}
	else if(chosenPiece.symbol == 'p'){ // black pawn

		if (to.y == 7) { // pawn is ready to be promoted
			gameStatus = GameStatus::PROMOTE;
			return GameStatus::PROMOTE;
		}

	}
	
	this->updateHistory(this->historyIndexPointer);

	gameStatus = GameStatus::CONTINUE;
	return GameStatus::CONTINUE;
}



bool GameManager::undoChecker() {
	int minimumTurns = (this->currentPlayer == 0) ? 3 : 4;
	cout << historyIndexPointer + 1 << " " << minimumTurns << endl;
	if (historyIndexPointer + 1 >= minimumTurns && this->undoChances[this->currentPlayer] > 0 && (gameStatus != GameStatus::WHITE_WINS && gameStatus != GameStatus::BLACK_WINS)) {
		return true;
	}
	else {
		return false;
	}
}

bool GameManager::redoChecker() {
	if (historyIndexPointer + 2 <= BoardManager::historyList.size() - 1) {
		return true;
	}
	else {
		return false;
	}
}

void GameManager::undoMovement() {
	players[0]->isCheck = false;
	players[1]->isCheck = false;

	// reduce undo chances
	this->undoChances[this->currentPlayer]--;

	historyIndexPointer -= 2;
	logIndex -= 2;

	gameStatus = CONTINUE;
	
	BoardManager::allObjectOnBoard = BoardManager::historyList[historyIndexPointer];
	BoardManager::imaginaryObject = BoardManager::allObjectOnBoard;
	BoardManager::updateBoard();

}

void GameManager::redoMovement() {
	historyIndexPointer += 2;
	logIndex += 2;
	BoardManager::allObjectOnBoard = BoardManager::historyList[historyIndexPointer];
	BoardManager::imaginaryObject = BoardManager::allObjectOnBoard;
	BoardManager::updateBoard();
}

void GameManager::promotePawn(Position pawnPos, char newPawn) {
	BoardManager::promotePiece(pawnPos, newPawn);

	this->updateHistory(this->historyIndexPointer);

	BoardManager::updateBoard();

	gameStatus = GameStatus::CONTINUE;
}