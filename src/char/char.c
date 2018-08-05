#include "SDL_thread.h"
#include "joyState.h"
#include "settings.h"
#include <global.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	uint8_t player;		//Player controlling the character
	int hp;			//Health Points
	int ac;			//Armor Class
	int x,y;		//X and Y positions for the character
	int state;		//Character's state(with universal states defined in the enum in char.h)
	int stateFrames;	//How long the character is in this state (-1 if in state indefinitely).
	int nextState;		//State to jump to when stateFrames = 0;
	int numStates;		//How many states the player has total;
	void (*updates)();	//Update functions for each state
	int* hitboxes[4]	//Currently active hitboxes
	int* hurtboxes[4]	//Currently active hurtboxes
} char_t;

void changeState(char_t* character, int state){
	character->state = state;
}
void changeHealth(char_t* character, int health){
	character->hp = health;
}
void damage(char_t* character, int health){
	changeHealth(character, character->hp - health);
}
void Updates(char_t* character){
	character->updates[character->states]();
} 

