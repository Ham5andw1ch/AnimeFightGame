#ifndef DISPLAY_H_
#define DISPLAY_H_

typedef struct sprite_t{
    uint16_t x;
    uint16_t y;
    uint16_t cached_x;
    uint16_t cached_y;
    uint_fast8_t displayed;
    uint8_t frames;
    SDL_Surface* surface;
    struct sprite_t* parent;
};

typedef struct disp_t{
    


SDL_Window *makeWindow(uint16_t x, uint16_t y, char* name);

int blitSprite(sprite_t* src, uint16_t x, uint16_t y, uint8_t frame);

int fillRect(uint32_t color);

int updateWindow();
void destroyWindow();

#endif

