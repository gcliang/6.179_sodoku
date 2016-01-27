#include "SDL2/SDL_image.h"
// #include "/Library/Frameworks/SDL2.framework/Headers/SDL_image.h"

typedef SDL_Texture* Texture;

Texture loadImage(char* image);

void displayTexture(Texture t, unsigned x, unsigned y, unsigned width, unsigned height, SDL_RendererFlip flip);

int graphics(int argc, char* argv[]);
