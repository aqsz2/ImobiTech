#include <string.h>
#include <stdio.h>
#include "player.h"

void initPlayers(GameState* state, int numPlayers) {
    if (numPlayers < 2) numPlayers = 2;
    if (numPlayers > MAX_PLAYERS) numPlayers = MAX_PLAYERS;
    state->numPlayers = numPlayers;

    Color colors[MAX_PLAYERS] = {
        (Color){0, 180, 255, 255},
        (Color){0, 200, 120, 255},
        (Color){200, 220, 0, 255},
        (Color){220, 60, 90, 255}
    };

    for (int i = 0; i < numPlayers; i++) {
        snprintf(state->players[i].name, sizeof(state->players[i].name), "P%d", i + 1);
        state->players[i].position = 0;
        state->players[i].balance = START_MONEY;
        state->players[i].eliminated = false;
        state->players[i].color = colors[i];
    }
}


