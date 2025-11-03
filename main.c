#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>

#include "game.h"

int main(void) {
    srand((unsigned int)time(NULL));

    const int screenWidth = 1000;
    const int screenHeight = 700;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "ImobiTech");
    SetTargetFPS(60);

    GameState game = {0};
    initGame(&game, 4);

    while (!WindowShouldClose()) {
        if (!game.gameOver) {
            if (IsKeyPressed(KEY_SPACE)) {
                nextTurn(&game);
            }
        } else {
            if (IsKeyPressed(KEY_ENTER)) {
                initGame(&game, game.numPlayers);
            }
        }

        BeginDrawing();
        ClearBackground((Color){18, 18, 24, 255});
        drawGame(&game);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}


