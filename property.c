#include <stdio.h>
#include "property.h"

void tryBuyProperty(GameState* state, int playerIndex, int tileIndex) {
    Tile* tile = &state->board.tiles[tileIndex];
    if (tile->type != TILE_PROPERTY) return;
    if (tile->property.ownerIndex != -1) return;

    int price = tile->property.price;
    if (state->players[playerIndex].balance >= price) {
        state->players[playerIndex].balance -= price;
        tile->property.ownerIndex = playerIndex;
        snprintf(state->lastEventText, sizeof(state->lastEventText), "%s comprou %s por R$%d", state->players[playerIndex].name, tile->property.name, price);
    }
}

void payRent(GameState* state, int payerIndex, int tileIndex) {
    Tile* tile = &state->board.tiles[tileIndex];
    if (tile->type != TILE_PROPERTY) return;
    int owner = tile->property.ownerIndex;
    if (owner == -1 || owner == payerIndex) return;

    int rent = tile->property.rent;
    state->players[payerIndex].balance -= rent;
    state->players[owner].balance += rent;
    snprintf(state->lastEventText, sizeof(state->lastEventText), "%s pagou aluguel de R$%d para %s (%s)",
             state->players[payerIndex].name, rent, state->players[owner].name, tile->property.name);
}


