#include <stdio.h>
#include <stdlib.h>
#include "events.h"
#include "ai.h"

void applyHackathon(GameState* state, int playerIndex) {
    int bonus = 150 + (rand() % 251); // 150..400
    state->players[playerIndex].balance += bonus;
    char aiText[256];
    aiGenerateText("Você brilhou no hackathon! Descreva o momento:", aiText, sizeof(aiText));
    snprintf(state->lastEventText, sizeof(state->lastEventText),
             "Hackathon! %s ganhou R$%d. %s", state->players[playerIndex].name, bonus, aiText);
}

void applyBug(GameState* state, int playerIndex) {
    int loss = 100 + (rand() % 201); // 100..300
    state->players[playerIndex].balance -= loss;
    char aiText[256];
    aiGenerateText("Um bug crítico ocorreu! Descreva o impacto:", aiText, sizeof(aiText));
    snprintf(state->lastEventText, sizeof(state->lastEventText),
             "Bug no sistema! %s perdeu R$%d. %s", state->players[playerIndex].name, loss, aiText);
}

void applyTax(GameState* state, int playerIndex) {
    int tax = state->players[playerIndex].balance / 10; // 10%
    if (tax < 100) tax = 100;
    state->players[playerIndex].balance -= tax;
    snprintf(state->lastEventText, sizeof(state->lastEventText),
             "Imposto de Dados! %s pagou R$%d em taxas.", state->players[playerIndex].name, tax);
}


