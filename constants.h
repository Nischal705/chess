#pragma once

#define cellSize 90
#define SCREEN_WIDTH (10*cellSize)
#define SCREEN_HEIGHT (10*cellSize)
constexpr float padding = 0.074f * cellSize;
#define FPS 60
#define boardX (SCREEN_WIDTH/2-4*cellSize)
#define boardY (SCREEN_HEIGHT/2-4*cellSize)
#define boardSize (8*cellSize)

enum Pieces{
    W_PAWN, 
    W_KNIGHT, 
    W_BISHOP, 
    W_ROOK, 
    W_KING,
    W_QUEEN,
    B_PAWN,
    B_KNIGHT,
    B_BISHOP,
    B_ROOK,
    B_KING,
    B_QUEEN,
    EMPTY
};

enum Occupied{
    white,
    black,
    either
};

enum Squares{
    A1, A2, A3, A4, A5, A6, A7, A8,
    B1, B2, B3, B4, B5, B6, B7, B8,
    C1, C2, C3, C4, C5, C6, C7, C8,
    D1, D2, D3, D4, D5, D6, D7, D8,
    E1, E2, E3, E4, E5, E6, E7, E8,
    F1, F2, F3, F4, F5, F6, F7, F8,
    G1, G2, G3, G4, G5, G6, G7, G8,
    H1, H2, H3, H4, H5, H6, H7, H8
};

enum Castles{
    NO,
    SHORT,
    LONG,
    BOTH
};

inline constexpr int value[] = {1, 3, 3, 5, 0, 9, 1, 3, 3, 5, 0, 9};
