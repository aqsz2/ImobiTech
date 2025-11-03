#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

#include "game.h"
#include "board.h"
#include "player.h"
#include "property.h"
#include "events.h"
#include "ui.h"

int rollDice(void) {
    int d1 = (rand() % 6) + 1;
    int d2 = (rand() % 6) + 1;
    return d1 + d2;
}

static void movePlayer(GameState* state, int playerIndex, int steps) {
    Player* p = &state->players[playerIndex];
    int old = p->position;
    int pos = (old + steps) % BOARD_SIZE;
    // bônus ao passar pelo START
    if ((old + steps) >= BOARD_SIZE) {
        p->balance += 200;
        snprintf(state->lastEventText, sizeof(state->lastEventText), "%s passou pelo START e ganhou R$200.", p->name);
    }
    p->position = pos;
}

void resolveLanding(GameState* state, int playerIndex, int tileIndex) {
    Tile* tile = &state->board.tiles[tileIndex];
    switch (tile->type) {
        case TILE_START:
            snprintf(state->lastEventText, sizeof(state->lastEventText), "%s chegou no START.", state->players[playerIndex].name);
            break;
        case TILE_PROPERTY:
            if (tile->property.ownerIndex == -1) {
                tryBuyProperty(state, playerIndex, tileIndex);
            } else if (tile->property.ownerIndex != playerIndex) {
                payRent(state, playerIndex, tileIndex);
            }
            break;
        case TILE_HACKATHON:
            applyHackathon(state, playerIndex);
            break;
        case TILE_BUG:
            applyBug(state, playerIndex);
            break;
        case TILE_TAX:
            applyTax(state, playerIndex);
            break;
    }
}

void eliminatePlayer(GameState* state, int playerIndex) {
    state->players[playerIndex].eliminated = true;
    // Reverter propriedades para sem dono
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (state->board.tiles[i].type == TILE_PROPERTY && state->board.tiles[i].property.ownerIndex == playerIndex) {
            state->board.tiles[i].property.ownerIndex = -1;
        }
    }
}

bool isOnlyOneAlive(const GameState* state, int* outWinnerIndex) {
    int alive = 0;
    int lastIdx = -1;
    for (int i = 0; i < state->numPlayers; i++) {
        if (!state->players[i].eliminated && state->players[i].balance > 0) {
            alive++;
            lastIdx = i;
        }
    }
    if (alive == 1) {
        if (outWinnerIndex) *outWinnerIndex = lastIdx;
        return true;
    }
    return false;
}

void initGame(GameState* state, int numPlayers) {
    memset(state, 0, sizeof(*state));
    initBoard(&state->board);
    initPlayers(state, numPlayers);
    state->currentPlayerIndex = 0;
    state->gameOver = false;
    state->winnerIndex = -1;
    strcpy(state->lastEventText, "Bem-vindo ao ImobiTech! Pressione ESPACO para jogar.");
}

void nextTurn(GameState* state) {
    if (state->gameOver) return;

    int pidx = state->currentPlayerIndex;
    if (state->players[pidx].eliminated) {
        // pular eliminados
        state->currentPlayerIndex = (state->currentPlayerIndex + 1) % state->numPlayers;
        return;
    }

    int dice = rollDice();
    state->lastDice = dice;
    movePlayer(state, pidx, dice);
    resolveLanding(state, pidx, state->players[pidx].position);

    // Eliminação se saldo <= 0
    if (state->players[pidx].balance <= 0) {
        eliminatePlayer(state, pidx);
        snprintf(state->lastEventText, sizeof(state->lastEventText), "%s faliu e foi eliminado!", state->players[pidx].name);
    }

    // Checar fim de jogo
    int winner;
    if (isOnlyOneAlive(state, &winner)) {
        state->gameOver = true;
        state->winnerIndex = winner;
    }

    // Próximo jogador
    state->currentPlayerIndex = (state->currentPlayerIndex + 1) % state->numPlayers;
}

void drawGame(const GameState* state) {
    drawBoard(state);
    drawSidebar(state);
}


