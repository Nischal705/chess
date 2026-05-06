#pragma once
#include "raylib.h"

class Images{ //class to load and resize the images
    public:
        inline static Texture2D pieceTextures[12];

        static void loadTexture();
};