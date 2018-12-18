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
    uint8_t current_frame;
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

disp_node_t* game_displayables;
disp_node_t* ui_displayables;

int insertDispNode(disp_node_t* node, disp_node_t* list);
disp_node_t* removeDispNode(uint64_t id, disp_node_t* list);

SDL_Window *makeWindow(uint16_t x, uint16_t y, char* name);

int blitSprite(sprite_t* src, uint16_t x, uint16_t y, uint8_t frame);

int fillRect(uint32_t color);

int updateWindow(void);
int updateViewport(SDL_Rect* p1, SDL_Rect* p2);
void drawGame(void);
void drawUI(void);

void destroyWindow(void);

#endif

