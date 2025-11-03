#include <string.h>
#include "board.h"

static void setProperty(Tile* tile, const char* name, int price, int rent) {
    tile->type = TILE_PROPERTY;
    strncpy(tile->property.name, name, sizeof(tile->property.name) - 1);
    tile->property.price = price;
    tile->property.rent = rent;
    tile->property.ownerIndex = -1;
    strncpy(tile->label, name, sizeof(tile->label) - 1);
}

void initBoard(Board* board) {
    // Layout simples com 24 casas; mistura propriedades e eventos
    // Índices: 0 = START
    memset(board, 0, sizeof(*board));

    board->tiles[0].type = TILE_START;
    strncpy(board->tiles[0].label, "START", sizeof(board->tiles[0].label) - 1);

    setProperty(&board->tiles[1], "StartUp AI", 120, 20);
    board->tiles[2].type = TILE_HACKATHON; strncpy(board->tiles[2].label, "Hackathon", 31);
    setProperty(&board->tiles[3], "CloudHub", 140, 24);
    board->tiles[4].type = TILE_TAX; strncpy(board->tiles[4].label, "Imposto Dados", 31);
    setProperty(&board->tiles[5], "TechTower", 160, 28);
    board->tiles[6].type = TILE_BUG; strncpy(board->tiles[6].label, "Bug Sistema", 31);
    setProperty(&board->tiles[7], "DataCenter X", 180, 30);
    setProperty(&board->tiles[8], "CyberSec Co.", 200, 34);
    board->tiles[9].type = TILE_HACKATHON; strncpy(board->tiles[9].label, "Hackathon", 31);
    setProperty(&board->tiles[10], "Quantum Lab", 220, 38);
    board->tiles[11].type = TILE_BUG; strncpy(board->tiles[11].label, "Bug Sistema", 31);
    setProperty(&board->tiles[12], "DevOps Inc.", 240, 42);
    board->tiles[13].type = TILE_TAX; strncpy(board->tiles[13].label, "Imposto Dados", 31);
    setProperty(&board->tiles[14], "AppWorks", 260, 46);
    board->tiles[15].type = TILE_HACKATHON; strncpy(board->tiles[15].label, "Hackathon", 31);
    setProperty(&board->tiles[16], "VR Studio", 280, 50);
    board->tiles[17].type = TILE_BUG; strncpy(board->tiles[17].label, "Bug Sistema", 31);
    setProperty(&board->tiles[18], "EdgeNet", 300, 55);
    setProperty(&board->tiles[19], "IoT Grid", 320, 60);
    board->tiles[20].type = TILE_TAX; strncpy(board->tiles[20].label, "Imposto Dados", 31);
    setProperty(&board->tiles[21], "BioTech R&D", 340, 65);
    board->tiles[22].type = TILE_BUG; strncpy(board->tiles[22].label, "Bug Sistema", 31);
    setProperty(&board->tiles[23], "NeuroSoft", 360, 70);
}


