#ifndef DISPLAY_H_
#define DISPLAY_H_

uint64_t accumulator;


typedef struct{
    uint8_t frames;
    SDL_Surface* surface;
}sprite_t;

typedef struct drawable_t{
    uint16_t x;
    uint16_t y;
    uint16_t cached_x;
    uint16_t cached_y;
    uint_fast8_t calculated;    
    sprite_t* sprite;
    struct drawable_t* parent;
}drawable_t;

typedef struct disp_node_t{
    uint8_t z_index;
    uint64_t id;
    drawable_t* drawable;
    struct disp_node_t* next;
}disp_node_t;

int insert(disp_node_t from, disp_node_t to);
int remove(uint64_t id);

SDL_Window *makeWindow(uint16_t x, uint16_t y, char* name);

int blitSprite(sprite_t* src, uint16_t x, uint16_t y, uint8_t frame);

int fillRect(uint32_t color);

disp_node_t* head;

int updateWindow();
int updateViewport();

void destroyWindow();

#endif

