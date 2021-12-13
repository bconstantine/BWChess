#include "Viewer.h"

GameManager gameManager;

void Viewer::initialise()
{
    icon.loadFromFile("Visuals/icon.png");
    setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    font.loadFromFile("Visuals/www.1001freefonts.com.ttf");
    texButtons.loadFromFile("Visuals/buttons.png");
    texPieces.loadFromFile("Visuals/commons.wikimedia.org.png");
}

bool Viewer::animate(Sprite& sprite, Vector2f from, Vector2f to, float duration, float elapsed)
{
    if (duration == 0.f || elapsed > duration)
    {
        sprite.setPosition(to + Vector2f(BOARD_OFFSET));
        return true;
    }

    Vector2f m((to - from) / sqrt(duration));
    sprite.setPosition(sqrt(elapsed) * m + from + Vector2f(BOARD_OFFSET));

    return false;
}

bool Viewer::isPosInsideBoard(const Vector2i vec) const
{
    return vec.x >= BOARD_OFFSET.x && vec.x <= BOARD_OFFSET.x + BOARD_TILE * 8 && vec.y >= BOARD_OFFSET.y && vec.y <= BOARD_OFFSET.y + BOARD_TILE * 8;
}

string Viewer::getDisplayTime(int secs) const
{
    string min = to_string(secs / 60);
    string sec = to_string(secs % 60);
    if (min.length() == 1) min = "0" + min;
    if (sec.length() == 1) sec = "0" + sec;
    return (min + ":" + sec);
}

void Viewer::loop()
{
    switch (curScene)
    {
    case Scene::MAIN_MENU:
        loopMenu();
        break;
    case Scene::GAME:
        loopGame();
        break;
    }
}

void Viewer::loopMenu()
{
    Text title0("B & W", font, 42);
    Text title1("Chess", font, 42);
    Button buttonSinglePlayer("New Solo (V.S. AI)", &font);
    Button buttonMultiPlayer("New Multi (V.S. Human)", &font);
    Button buttonLoad("Load Game", &font);
    Button buttonQuit("Quit Game", &font);
    RectangleShape transparentBackground(Vector2f(512, 512));
    Text text0("Choose difficulty", font, 32);
    Button buttonEasy("Easy", &font);
    Button buttonMedium("Medium", &font);
    Button buttonHard("Hard", &font);
    Button buttonBack("Back", &font);

    title0.setPosition(Vector2f((WINDOW_SIZE[0] - title0.getGlobalBounds().width) * 0.5f, 72.f));    
    title1.setPosition(Vector2f((WINDOW_SIZE[0] - title1.getGlobalBounds().width) * 0.5f, 80.f + title0.getGlobalBounds().height));    
    buttonSinglePlayer.setPosition(Vector2f(160.f, 212.f));
    buttonSinglePlayer.setSize(Vector2f(320.f, 48.f));   
    buttonMultiPlayer.setPosition(Vector2f(160.f, 292.f));
    buttonMultiPlayer.setSize(Vector2f(320.f, 48.f));   
    buttonLoad.setPosition(Vector2f(160.f, 372.f));
    buttonLoad.setSize(Vector2f(320.f, 48.f));    
    buttonQuit.setPosition(Vector2f(160.f, 452.f));
    buttonQuit.setSize(Vector2f(320.f, 48.f));    
    transparentBackground.setFillColor(Color(0, 0, 0, 128));
    transparentBackground.setPosition(Vector2f(BOARD_OFFSET));    
    text0.setPosition(Vector2f(BOARD_OFFSET.x + (BOARD_TILE * 8 - text0.getGlobalBounds().width) * 0.5f, 76.f));   
    buttonEasy.setPosition(Vector2f(210.f, 212.f));
    buttonEasy.setSize(Vector2f(220.f, 48.f));    
    buttonMedium.setPosition(Vector2f(210.f, 292.f));
    buttonMedium.setSize(Vector2f(220.f, 48.f));    
    buttonHard.setPosition(Vector2f(210.f, 372.f));
    buttonHard.setSize(Vector2f(220.f, 48.f));
    buttonBack.setPosition(Vector2f(210.f, 452.f));
    buttonBack.setSize(Vector2f(220.f, 48.f));

    bool isChoosingDifficulty = false;

    gameManager.initMultiplayerGame();
    gameManager.resetGame();
    gameManager.logIndex = -1;
    BoardManager::logReplay.clear();

    while (isOpen())
    {
        // EVENT HANDLING
        Event event;
        while (pollEvent(event))
        { 
            if (isChoosingDifficulty)
            {
                buttonEasy.update(event, *this);
                buttonMedium.update(event, *this);
                buttonHard.update(event, *this);
                buttonBack.update(event, *this);

                if (buttonEasy.getOnClick())
                {
                    gameManager.setAIDifficulty(Difficulty::EASY);
                    curScene = Scene::GAME;
                }
                else if (buttonMedium.getOnClick())
                {
                    gameManager.setAIDifficulty(Difficulty::MEDIUM);
                    curScene = Scene::GAME;
                }
                else if (buttonHard.getOnClick())
                {
                    gameManager.setAIDifficulty(Difficulty::HARD);
                    curScene = Scene::GAME;
                }
                else if (buttonBack.getOnClick())
                {
                    isChoosingDifficulty = false;
                }
            }
            else
            {
                buttonSinglePlayer.update(event, *this);
                buttonMultiPlayer.update(event, *this);
                buttonLoad.update(event, *this);
                buttonQuit.update(event, *this);

                if (buttonSinglePlayer.getOnClick())
                {
                    gameManager.setStatus(true, false);
                    isChoosingDifficulty = true;
                }
                else if (buttonMultiPlayer.getOnClick())
                {
                    gameManager.setStatus(true, true);
                    gameManager.initMultiplayerGame();
                    curScene = Scene::GAME;
                }
                else if (buttonLoad.getOnClick())
                {
                    if (gameManager.loadGame()) {
                        curScene = Scene::GAME;
                    }
                }
                else if (buttonQuit.getOnClick())
                {
                    close();
                }
            }

            if (event.type == Event::Closed)
            {
                close();
            }
        }

        // CHANGE SCENE
        if (curScene == Scene::GAME)
        {
            break;
        }

        // DRAWING
        clear(COLOR_BACKGROUND);

        const Color BOARD_COLORS[] = { BOARD_WHITE, BOARD_BLACK };
        Sprite piece(texPieces);
        RectangleShape rect(Vector2f(Vector2i(BOARD_TILE, BOARD_TILE)));
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                rect.setPosition(j * BOARD_TILE + BOARD_OFFSET.x, i * BOARD_TILE + BOARD_OFFSET.y);
                rect.setFillColor(BOARD_COLORS[(i + j) % 2]);
                draw(rect);

                if (BoardManager::currentBoard[i][j] != '0')
                {
                    piece.setPosition(rect.getPosition());
                    piece.setTextureRect(pieceTex.at(BoardManager::currentBoard[i][j]));
                    draw(piece);
                }
            }
        }

        draw(transparentBackground);

        if (isChoosingDifficulty)
        {
            draw(text0);
            draw(buttonEasy);
            draw(buttonMedium);
            draw(buttonHard);
            draw(buttonBack);
        }
        else
        {
            draw(title0);
            draw(title1);
            draw(buttonSinglePlayer);
            draw(buttonMultiPlayer);
            draw(buttonLoad);
            draw(buttonQuit);
        }

        display();
    }
}

void Viewer::loopGame()
{
    Button buttonReplay("Log replay", &font);
    Button buttonSurrender(&texButtons, buttonTex.at(Buttons::SURRENDER), Vector2f(48.f, 48.f));
    Button buttonUndo(&texButtons, buttonTex.at(Buttons::UNDO), Vector2f(48.f, 48.f));
    Button buttonRedo(&texButtons, buttonTex.at(Buttons::REDO), Vector2f(48.f, 48.f));
    Button buttonSave(&texButtons, buttonTex.at(Buttons::SAVE), Vector2f(48.f, 48.f));
    Button buttonMenu(&texButtons, buttonTex.at(Buttons::MENU), Vector2f(48.f, 48.f));
    Button buttonQueen(&texPieces, pieceTex.at('Q'));
    Button buttonRook(&texPieces, pieceTex.at('R'));
    Button buttonKnight(&texPieces, pieceTex.at('N'));
    Button buttonBishop(&texPieces, pieceTex.at('B'));
    RectangleShape transparentBackground(Vector2f(512, 512));
    Text text("", font);
    Text textPromote0("Choose your new", font, 32);
    Text textPromote1("promoted piece:", font, 32);
    Clock clock;
    Time time;
    Vector2i mousePressedPos;
    Vector2i mouseReleasedPos;
    Vector2i mouseTilePos(-1, -1);
    Vector2f pieceFrom, pieceTo;
    vector<Position> validTiles;
    bool isMousePressed = false;
    bool isAIThinking = false;
    bool isChoosingMovement = false;
    bool isPieceMoving = false;
    bool isPromoting = false;
    bool isReplaying = false;
    int gameStatus = GameStatus::CONTINUE;
    int gameOverStatus = GameStatus::CONTINUE;
    int replayIndex = 0;
    future<pair<Position, Position>> AInextMove;

    buttonReplay.setPosition(Vector2f(200.f, 296.f));
    buttonReplay.setSize(Vector2f(240.f, 48.f));
    buttonUndo.setDisabled(!gameManager.undoChecker());
    buttonRedo.setDisabled(!gameManager.redoChecker());
    buttonQueen.setPosition(Vector2f(168.f, 285.f));
    buttonRook.setPosition(Vector2f(246.f, 285.f));
    buttonKnight.setPosition(Vector2f(394.f, 285.f));
    buttonBishop.setPosition(Vector2f(472.f, 285.f));
    transparentBackground.setFillColor(Color(0, 0, 0, 100));
    transparentBackground.setPosition(Vector2f(BOARD_OFFSET));
    textPromote0.setPosition(Vector2f(BOARD_OFFSET.x + (BOARD_TILE * 8 - textPromote0.getGlobalBounds().width) * 0.5f, 72.f));
    textPromote1.setPosition(Vector2f(BOARD_OFFSET.x + (BOARD_TILE * 8 - textPromote1.getGlobalBounds().width) * 0.5f, 78.f + textPromote0.getGlobalBounds().height));    
    
    while (isOpen())
    {
        // GAME STATUS
        if (gameStatus == GameStatus::CONTINUE || gameStatus == GameStatus::PROMOTE)
        {
            gameStatus = gameManager.checkSituation();
        }

        // AI
        if (gameStatus == GameStatus::CONTINUE || gameStatus == GameStatus::PROMOTE)
        {
            if (gameManager.status[gameManager.currentPlayer] == Status::AI)
            {
                if (!isAIThinking && !isPieceMoving)
                {
                    isAIThinking = true;
                    AInextMove = async(launch::async, [&]
                        {
                        return gameManager.moveAI(gameManager.blackAIs[gameManager.chosenDifficulty]);
                        });
                }
                else if (AInextMove.valid())
                {
                    if (AInextMove.wait_for(chrono::seconds(0)) == future_status::ready)
                    {
                        pair<Position, Position> pos = AInextMove.get();
                        pieceFrom = Vector2f(pos.first.x, pos.first.y);
                        pieceTo = Vector2f(pos.second.x, pos.second.y);
                        isAIThinking = false;
                        isPieceMoving = true;
                        clock.restart();
                    }
                }
            }
        }
        
        // TIME PROCESS
        time = clock.getElapsedTime();
        if (gameStatus == GameStatus::CONTINUE && !isPieceMoving)
        {
            if (time.asMilliseconds() > 1000)
            {
                gameManager.minusTime(1);
                clock.restart();
            }
        }        
        
        // EVENT PROCESS
        Event event;
        while (pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                close();
            }
           
            buttonReplay.update(event, *this);
            buttonSurrender.update(event, *this);
            buttonUndo.update(event, *this);
            buttonRedo.update(event, *this);
            buttonSave.update(event, *this);
            buttonMenu.update(event, *this);

            if (buttonMenu.getOnClick())
            {
                curScene = Scene::MAIN_MENU;
                break;
            }

            if (isPieceMoving || isAIThinking || isReplaying)
            {
                continue;
            }

            if (isPromoting)
            {
                buttonQueen.update(event, *this);
                buttonRook.update(event, *this);
                buttonKnight.update(event, *this);
                buttonBishop.update(event, *this);

                const char symbols[4] =
                {
                    'q', 'r', 'n', 'b'
                };
                bool onClicks[4] =
                {
                    buttonQueen.getOnClick(), buttonRook.getOnClick(), buttonKnight.getOnClick(), buttonBishop.getOnClick()
                };
                for (int i = 0; i < 4; i++)
                {
                    if (onClicks[i])
                    {
                        gameManager.promotePawn(Position(pieceTo.x, pieceTo.y), symbols[i]);
                        gameManager.changeTurn();
                        isPromoting = false;
                        break;
                    }
                }
                continue;
            }
            
            if (buttonReplay.getOnClick())
            {
                if (!BoardManager::logReplay.empty())
                {
                    gameManager.resetGame();
                    gameStatus = GameStatus::CONTINUE;
                    replayIndex = 0;
                    isReplaying = true;
                    break;
                }
            }
            if (buttonSurrender.getOnClick())
            {
                if (gameManager.gameStatus == GameStatus::CONTINUE)
                {
                    isPromoting = false;
                    gameManager.surrender();
                }
                break;
            }
            if (buttonUndo.getOnClick())
            {
                gameManager.undoMovement();
                buttonUndo.setDisabled(!gameManager.undoChecker());
                buttonRedo.setDisabled(!gameManager.redoChecker());
                break;
            }
            if (buttonRedo.getOnClick())
            {
                gameManager.redoMovement();
                buttonUndo.setDisabled(!gameManager.undoChecker());
                buttonRedo.setDisabled(!gameManager.redoChecker());
                break;
            }
            if (buttonSave.getOnClick())
            {
                if (gameManager.gameStatus == GameStatus::CONTINUE) gameManager.saveGame();
                break;
            }

            if (gameStatus > GameStatus::CONTINUE)
            {
                continue;
            }

            if (event.type == Event::MouseButtonPressed)
            {
                if (!isMousePressed)
                {
                    mousePressedPos = Mouse::getPosition(*this);
                    mouseTilePos = (mousePressedPos - BOARD_OFFSET) / BOARD_TILE;
                    isMousePressed = true;
                }
            }
            else if (event.type == Event::MouseButtonReleased)
            {
                mouseReleasedPos = Mouse::getPosition(*this);
                Vector2i newMouseTilePos = (mouseReleasedPos - BOARD_OFFSET) / BOARD_TILE;
                if (mouseTilePos == newMouseTilePos && isPosInsideBoard(mousePressedPos))
                {
                    if (isChoosingMovement)
                    {
                        Position newTile(newMouseTilePos.x, newMouseTilePos.y);
                        if (find(validTiles.begin(), validTiles.end(), newTile) != validTiles.end())
                        {
                            pieceTo = Vector2f(newMouseTilePos);
                            isPieceMoving = true;
                            clock.restart();
                        }
                        mouseTilePos = Vector2i(-1, -1);
                        validTiles.clear();
                        isChoosingMovement = false;
                    }
                    else
                    {
                        mouseTilePos = newMouseTilePos;
                        pieceFrom = Vector2f(mouseTilePos);

                        bool isWhite = gameManager.players[gameManager.currentPlayer]->white;
                        char p = BoardManager::currentBoard[(int)pieceFrom.y][(int)pieceFrom.x];
                        if ((isWhite && isupper(p)) || (!isWhite && islower(p)))
                        {
                            validTiles = BoardManager::getAllPossibleMove(Position(mouseTilePos.x, mouseTilePos.y));
                            isChoosingMovement = true;
                        }
                    }

                }
                else
                {
                    validTiles.clear();
                    isChoosingMovement = false;
                }
                isMousePressed = false;
            }
        }
        
        // CHANGE SCENE
        if (curScene == Scene::MAIN_MENU)
        {
            break;
        }

        // LOG REPLAY
        if (isReplaying)
        {
            if (!isPieceMoving)
            {
                pieceFrom = Vector2f(BoardManager::logReplay[replayIndex].first.x, BoardManager::logReplay[replayIndex].first.y);
                pieceTo = Vector2f(BoardManager::logReplay[replayIndex].second.x, BoardManager::logReplay[replayIndex].second.y);
                isPieceMoving = true;
                replayIndex++;
                clock.restart();
            }
        }

        // DRAWING
        clear(COLOR_BACKGROUND);

        static const Color BOARD_COLORS[] = { BOARD_WHITE, BOARD_BLACK };
        RectangleShape rect(Vector2f(Vector2i(BOARD_TILE, BOARD_TILE)));
        Sprite piece(texPieces);
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                rect.setPosition(j * BOARD_TILE + BOARD_OFFSET.x, i * BOARD_TILE + BOARD_OFFSET.y);

                rect.setFillColor(BOARD_COLORS[(i + j) % 2]);
                draw(rect);

                if (isChoosingMovement)
                {
                    if (j == mouseTilePos.x && i == mouseTilePos.y)
                    {
                        rect.setFillColor(BOARD_BLUE);
                        draw(rect);
                    }
                }

                bool isWhite = gameManager.players[gameManager.currentPlayer]->white;
                char p = BoardManager::currentBoard[i][j];
                if (tolower(p) == 'k' && gameManager.players[gameManager.currentPlayer]->isCheck)
                {
                    if ((isWhite && isupper(p)) || (!isWhite && islower(p)))
                    {
                        rect.setFillColor(BOARD_RED);
                        draw(rect);
                    }
                }
            }
        }

        for (int i = 0; i < validTiles.size(); i++)
        {
            rect.setFillColor(BOARD_BLUE);
            if (BoardManager::currentBoard[validTiles[i].y][validTiles[i].x] != '0') rect.setFillColor(BOARD_ORANGE);
            rect.setPosition(validTiles[i].x * BOARD_TILE + BOARD_OFFSET.x, validTiles[i].y * BOARD_TILE + BOARD_OFFSET.y);
            draw(rect);
        }

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (BoardManager::currentBoard[i][j] != '0')
                {
                    if (isPieceMoving && pieceFrom == Vector2f(j, i))
                    {
                        continue;
                    }

                    piece.setTextureRect(pieceTex.at(BoardManager::currentBoard[i][j]));
                    piece.setPosition(j * BOARD_TILE + BOARD_OFFSET.x, i * BOARD_TILE + BOARD_OFFSET.y);
                    draw(piece);
                }
            }
        }

        // ANIMATION
        if (isPieceMoving)
        {
            piece.setTextureRect(pieceTex.at(BoardManager::currentBoard[(int)pieceFrom.y][(int)pieceFrom.x]));
            time = clock.getElapsedTime();
            if (animate(piece, pieceFrom * 64.f, pieceTo * 64.f, 0.25f, time.asSeconds()))
            {
                if (isReplaying)
                {
                    gameStatus = gameManager.movePiece(Position(pieceFrom.x, pieceFrom.y), Position(pieceTo.x, pieceTo.y), true);
                    if (gameStatus == GameStatus::PROMOTE) gameStatus = GameStatus::CONTINUE;
                    if (replayIndex > gameManager.logIndex)
                    {
                        gameStatus = gameOverStatus;
                        isReplaying = false;
                    }
                    gameManager.changeTurn();
                }
                else
                {
                    gameStatus = gameManager.movePiece(Position(pieceFrom.x, pieceFrom.y), Position(pieceTo.x, pieceTo.y));
                    if (gameStatus == GameStatus::PROMOTE)
                    {
                        if (gameManager.status[gameManager.currentPlayer] == Status::AI)
                        {
                            gameManager.promotePawn(Position(pieceTo.x, pieceTo.y), 'q');
                            gameManager.changeTurn();
                        }
                        else isPromoting = gameStatus == GameStatus::PROMOTE;
                    }
                    else gameManager.changeTurn();
                    buttonUndo.setDisabled(!gameManager.undoChecker());
                    buttonRedo.setDisabled(!gameManager.redoChecker());
                }
                isPieceMoving = false;
                clock.restart();
            }
            draw(piece);
        }

        if (isPromoting && !isReplaying)
        {
            buttonQueen.setTextureRect(pieceTex.at((gameManager.players[gameManager.currentPlayer]->white) ? 'Q' : 'q'));
            buttonRook.setTextureRect(pieceTex.at((gameManager.players[gameManager.currentPlayer]->white) ? 'R' : 'r'));
            buttonKnight.setTextureRect(pieceTex.at((gameManager.players[gameManager.currentPlayer]->white) ? 'N' : 'n'));
            buttonBishop.setTextureRect(pieceTex.at((gameManager.players[gameManager.currentPlayer]->white) ? 'B' : 'b'));
            draw(transparentBackground);
            draw(textPromote0);
            draw(textPromote1);
            draw(buttonQueen);
            draw(buttonRook);
            draw(buttonKnight);
            draw(buttonBishop);
        }

        string curTurn = ((gameManager.currentPlayer) ? "Black" : "White");
        text.setCharacterSize(32);
        text.setString(curTurn + "\'s turn");
        text.setPosition(Vector2f(BOARD_OFFSET.x + BOARD_TILE * 8 - text.getGlobalBounds().width - 8, 10.f));
        draw(text);

        buttonUndo.setPosition(Vector2f(264.f, 584.f));
        buttonRedo.setPosition(Vector2f(328.f, 584.f));
        buttonSave.setPosition(Vector2f(392.f, 584.f));
        buttonMenu.setPosition(Vector2f(456.f, 584.f));
        buttonSurrender.setPosition(Vector2f(520.f, 584.f));

        text.setString(getDisplayTime(gameManager.players[gameManager.currentPlayer]->pTime));

        if (gameManager.currentPlayer)
        {
            text.setPosition(Vector2f(72.f, 8.f));
        }
        else
        {
            text.setPosition(Vector2f(72.f, 584.f));
        }

        if (gameStatus > GameStatus::CONTINUE && gameStatus < GameStatus::PROMOTE)
        {
            gameOverStatus = gameStatus;
            const string s[3] =
            {
                "White wins!", "Black wins!", "Draw!"
            };
            text.setString(s[gameStatus - 1]);
            text.setCharacterSize(42);
            text.setPosition(Vector2f(BOARD_OFFSET.x + (BOARD_TILE * 8 - text.getGlobalBounds().width) * 0.5f, 96.f));
            buttonReplay.setDisabled(false);
            buttonSurrender.setDisabled(true);
            buttonUndo.setDisabled(true);
            buttonRedo.setDisabled(true);
            buttonSave.setDisabled(true);
            draw(transparentBackground);
            draw(text);
            draw(buttonReplay);
        }
        else if (isReplaying)
        {
            buttonReplay.setDisabled(true);
            buttonSurrender.setDisabled(true);
            buttonSave.setDisabled(true);
        }
        else
        {
            draw(text);
            buttonReplay.setDisabled(true);
            buttonSurrender.setDisabled(false);
            buttonSave.setDisabled(false);
        }

        draw(buttonUndo);
        draw(buttonRedo);
        draw(buttonSave);
        draw(buttonMenu);
        draw(buttonSurrender);

        display();
    }
}