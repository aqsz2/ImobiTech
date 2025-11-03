#include <stdio.h>
#include <string.h>
#include <raylib.h>
#include "ui.h"

static Rectangle getBoardRect(void) {
    return (Rectangle){20, 20, 640, 640};
}

static Vector2 tileCenter(int idx) {
    Rectangle r = getBoardRect();
    int perSide = BOARD_SIZE / 4; // 6 por lado (com 24 casas)
    float stepX = r.width / (float)(perSide - 1);
    float stepY = r.height / (float)(perSide - 1);

    int side = idx / perSide;
    int pos = idx % perSide;
    switch (side) {
        case 0: return (Vector2){r.x + pos * stepX, r.y};
        case 1: return (Vector2){r.x + r.width, r.y + pos * stepY};
        case 2: return (Vector2){r.x + r.width - pos * stepX, r.y + r.height};
        default: return (Vector2){r.x, r.y + r.height - pos * stepY};
    }
}

void drawBoard(const GameState* state) {
    Rectangle r = getBoardRect();
    DrawRectangleLinesEx(r, 2, (Color){60, 80, 110, 255});

    // Desenha as casas
    for (int i = 0; i < BOARD_SIZE; i++) {
        Vector2 c = tileCenter(i);
        Color tileColor = (Color){40, 50, 70, 255};
        switch (state->board.tiles[i].type) {
            case TILE_START: tileColor = (Color){0, 120, 200, 255}; break;
            case TILE_PROPERTY: tileColor = (Color){20, 140, 120, 255}; break;
            case TILE_HACKATHON: tileColor = (Color){0, 160, 220, 255}; break;
            case TILE_BUG: tileColor = (Color){180, 60, 80, 255}; break;
            case TILE_TAX: tileColor = (Color){200, 160, 40, 255}; break;
        }
        DrawCircleV(c, 16, tileColor);
        DrawCircleLines((int)c.x, (int)c.y, 16, (Color){20, 26, 34, 255});
    }

    // Jogadores
    for (int p = 0; p < state->numPlayers; p++) {
        if (state->players[p].eliminated) continue;
        Vector2 c = tileCenter(state->players[p].position);
        Vector2 offset = { (float)((p%2)*8 - 4), (float)((p/2)*8 - 4) };
        DrawCircleV((Vector2){c.x + offset.x, c.y + offset.y}, 6, state->players[p].color);
    }
}

void drawSidebar(const GameState* state) {
    Rectangle panel = {680, 20, 300, 640};
    DrawRectangleRec(panel, (Color){28, 34, 44, 255});
    DrawRectangleLinesEx(panel, 2, (Color){60, 80, 110, 255});

    DrawText("ImobiTech", (int)panel.x + 16, (int)panel.y + 12, 28, (Color){150, 200, 255, 255});
    DrawText("[ESPACO] Avancar turno", (int)panel.x + 16, (int)panel.y + 48, 16, (Color){200, 220, 235, 255});

    char turn[64];
    snprintf(turn, sizeof(turn), "Vez: %s (dado: %d)",
             state->players[state->currentPlayerIndex].name, state->lastDice);
    DrawText(turn, (int)panel.x + 16, (int)panel.y + 80, 18, (Color){220, 240, 255, 255});

    // Saldos
    int y = (int)panel.y + 120;
    for (int p = 0; p < state->numPlayers; p++) {
        Color col = state->players[p].color;
        DrawRectangle((int)panel.x + 16, y, 12, 12, col);
        char info[128];
        snprintf(info, sizeof(info), "%s: R$%d%s", state->players[p].name, state->players[p].balance,
                 state->players[p].eliminated ? " (OUT)" : "");
        DrawText(info, (int)panel.x + 36, y - 2, 18, state->players[p].eliminated ? (Color){180, 100, 120, 255} : (Color){210, 230, 255, 255});
        y += 26;
    }

    // Último evento (render simples, 3 linhas máx)
    DrawText("Evento:", (int)panel.x + 16, y + 8, 18, (Color){200, 220, 235, 255});
    int textY = y + 30;
    const char* text = state->lastEventText;
    int maxLines = 8;
    int lineLen = 36;
    int len = (int)strlen(text);
    for (int i = 0; i < maxLines && len > 0; i++) {
        int take = len < lineLen ? len : lineLen;
        char line[64];
        memcpy(line, text, (size_t)take);
        line[take] = '\0';
        DrawText(line, (int)panel.x + 16, textY, 18, (Color){220, 240, 255, 255});
        text += take;
        len -= take;
        textY += 20;
    }

    if (state->gameOver) {
        char over[128];
        snprintf(over, sizeof(over), "Vencedor: %s (ENTER para reiniciar)", state->players[state->winnerIndex].name);
        DrawText(over, (int)panel.x + 16, (int)panel.y + (int)panel.height - 36, 18, (Color){255, 240, 200, 255});
    }
}


