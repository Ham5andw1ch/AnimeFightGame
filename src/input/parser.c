#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "joyState.h"
#include "settings.h"
	
	
typedef struct
{
	int head;
	int tail;
	uint8_t *inputs[bufferLength];
} buffer_t;

buffer_t Buffers[2];
int mod(int x, int m) {
	int r = x%m;
	if(r<0){
		return r+m;
	}else{
		return r;
	}
}
//Initialize Buffers
void initBuffers()
{
    Buffers[0].head = 0;
    Buffers[1].head = 0;
    Buffers[0].tail = 0;
    Buffers[1].tail = 0;
}
void destroyBuffers()
{
}
void add(uint8_t inputs[], buffer_t player){
	
	if(player.inputs[player.head] != NULL){
		
		if(mod((player.head-1),bufferLength) ==player.tail){
			
			player.inputs[player.tail] = inputs;
			player.tail = mod((player.tail-1),bufferLength);
			player.head = mod((player.head-1),bufferLength);
			}else{
			player.inputs[player.head-1] = inputs;
			player.head = mod((player.head-1),bufferLength);
		}
		
	}else{
		player.inputs[player.head] = inputs;
		player.tail = player.head;
	}	
}
uint8_t* pop(buffer_t player){
	uint8_t* temp = player.inputs[player.head];
	free(player.inputs[player.head]);
	player.inputs[player.head] = NULL;
	player.head = mod((player.head+1),bufferLength);
	return temp;
}

uint8_t* peek(buffer_t player){
	uint8_t* temp = player.inputs[player.head];
	return temp;
}

void removeTail(buffer_t player){
	if(player.inputs[player.tail] != NULL){
		free(player.inputs[player.tail]);
		player.inputs[player.tail] = NULL;
		player.tail = mod((player.tail-1),bufferLength);
	}
}
//Input: uint8_t array with the first element being the length of the array, player input buffer
uint8_t searchInput(uint8_t* input, buffer_t buffer){
	int length = input[0]
	int currentIndex = 1;
	//Every possible joystick position in the game defined by a 2D array.
	int Inputs[3][3] = {  
   		{7,8,9} ,   
		{4,5,6} ,   
		{1,2,3}   
	};
	int yPos;
	int xPos;

	for (int i = buffer.head; i != buffer.tail; i= mod((i+1), bufferLength)){
		yPos = 1;
		xPos = 1;
		if(inputs[i][0]==1||inputs[i][0]==2){
			yPos++;
		}
		if(inputs[i][1]==1||inputs[i][1]==2){
			yPos--;
		}
		if(inputs[i][2]==1||inputs[i][2]==2){
			xPos--;
		}
		if(inputs[i][3]==1||inputs[i][3]==2){
			xPos++;
		}

	}
}
void parserUpdate(){
	add(joyStatep1(), Buffers[0]);
	add(joyStatep2(), Buffers[1]);
}

