#ifndef IMOBITECH_GAME_H
#define IMOBITECH_GAME_H

#include <stdbool.h>
#include <raylib.h>

#define MAX_PLAYERS 4
#define BOARD_SIZE 24
#define START_MONEY 1500

typedef enum {
    TILE_START,
    TILE_PROPERTY,
    TILE_HACKATHON,
    TILE_BUG,
    TILE_TAX
} TileType;

typedef struct {
    char name[32];
    int price;
    int rent;
    int ownerIndex; // -1 = sem dono
} Property;

typedef struct {
    TileType type;
    Property property; // válido quando type == TILE_PROPERTY
    char label[32];
} Tile;

typedef struct {
    char name[32];
    int position; // índice no tabuleiro
    int balance;
    bool eliminated;
    Color color;
} Player;

typedef struct {
    Tile tiles[BOARD_SIZE];
} Board;

typedef struct {
    Board board;
    Player players[MAX_PLAYERS];
    int numPlayers;
    int currentPlayerIndex;
    int lastDice;
    char lastEventText[256];
    bool gameOver;
    int winnerIndex;
} GameState;

void initGame(GameState* state, int numPlayers);
void nextTurn(GameState* state);
void drawGame(const GameState* state);

// util
int rollDice(void);
void eliminatePlayer(GameState* state, int playerIndex);
bool isOnlyOneAlive(const GameState* state, int* outWinnerIndex);

// tile resolvers
void resolveLanding(GameState* state, int playerIndex, int tileIndex);

#endif // IMOBITECH_GAME_H


