#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <global.h>
#include "joyState.h"
#include "settings.h"
	
	
typedef struct
{
	int head;
	int tail;
	uint8_t *inputs[bufferLength];
} queue_t;

uint8_t updateFlag = 0;
queue_t Buffers[2];
int mod(int x, int m)
{
	int r = x%m;
	if(r<0)
	{
		return r+m;
	}else
	{
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
void add(uint8_t inputs[], queue_t* player)
{	
	if(player->inputs[player->head] != NULL)
	{	
		if(mod((player->head-1), bufferLength) == player->tail)
		{			
			player->inputs[player->tail] = inputs;
			player->tail = mod((player->tail-1),bufferLength);
			player->head = mod((player->head-1),bufferLength);
		}else
		{
			player->inputs[mod(player->head-1, bufferLength)] = inputs;
			player->head = mod((player->head-1),bufferLength);
		}
		
	}else
	{
		player->inputs[player->head] = inputs;
		player->tail = player->head;
	}	
}
uint8_t* pop(queue_t* player)
{
	uint8_t* temp = player->inputs[player->head];
	free(player->inputs[player->head]);
	player->inputs[player->head] = NULL;
	player->head = mod((player->head+1), bufferLength);
	return temp;
}

uint8_t* peek(queue_t* player)
{
	uint8_t* temp = player->inputs[player->head];
	return temp;
}

void removeTail(queue_t* player)
{
	if(player->inputs[player->tail] != NULL)
	{
		free(player->inputs[player->tail]);
		player->inputs[player->tail] = NULL;
		player->tail = mod((player->tail-1),bufferLength);
	}
}
//Input: uint8_t array of an input in anime notation with the first element being the length of the input, player input buffer
uint8_t searchInput(uint8_t* input, queue_t buffer, uint8_t flag)
{
	int length = input[0];
	int currentIndex = length;
	int yPos;
	int xPos;
	int fiveCount;
	int savedI;
	//Every possible joystick position in the game defined by a 2D array.
	int possibleInputs[3][3] = 
	{  
   		{7,8,9} ,   
		{4,5,6} ,   
		{1,2,3}   
	};
	
	//Scan the entire buffer
	for (int i = buffer.head; i != buffer.tail; i= mod((i+1), bufferLength))
	{
		if(buffer.inputs[i][buttonCount+1] == 1 && !flag)
		{
			return 0;
		}else
		{
			//Use inputs to calculate what direction is being held
			yPos = 1;
			xPos = 1;
			if(buffer.inputs[i][0]==1||buffer.inputs[i][0]==2)
			{
				yPos--;
			}
			if(buffer.inputs[i][1]==1||buffer.inputs[i][1]==2)
			{
				yPos++;
			}
			if(buffer.inputs[i][2]==1||buffer.inputs[i][2]==2)
			{
				xPos--;
			}
			if(buffer.inputs[i][3]==1||buffer.inputs[i][3]==2)
			{
				xPos++;
			}
			//Give up on the input if more than fiveLimit fives are consecutive *unless* that five is required by the input. 
			dbgprint("[%d %d %d]", possibleInputs[yPos][xPos], input[currentIndex], currentIndex); 
			if(possibleInputs[yPos][xPos]==5 && input[currentIndex] != possibleInputs[yPos][xPos])
			{
				fiveCount++;
				if(fiveCount >= fiveLimit){
				return 0;
			}
			//Wasn't a five or five was needed.
			}else
			{
				fiveCount = 0;
				if (input[currentIndex] == possibleInputs[yPos][xPos])
				{
					
					currentIndex--;
					if (currentIndex == length-1)
					{
						savedI = i;
					}else if(currentIndex == 0)
					{
						buffer.inputs[savedI][buttonCount+1] = 1;
						return 1;
					}	
				}
			}
		}
	}
	//Input wasn't found until now. Just return 0.
	return 0;
}
//My update function.
void parserUpdate()
{

	add(joyStatep1(), &Buffers[0]);
	add(joyStatep2(), &Buffers[1]);
	uint8_t testInput[] = {3,2,3,6};
	dbgprint("[%d %d %d %d %d %d %d %d]\t", peek(Buffers)[0], peek(Buffers)[1], peek(Buffers)[2],
            peek(Buffers)[3], peek(Buffers)[4], peek(Buffers)[5], peek(Buffers)[6], peek(Buffers)[7]); 
    dbgprint("[%d %d %d %d %d %d %d %d]\n", peek(Buffers + 1)[0], peek(Buffers + 1)[1], peek(Buffers + 1)[2],
            peek(Buffers + 1)[3], peek(Buffers + 1)[4], peek(Buffers + 1)[5], peek(Buffers + 1)[6], peek(Buffers + 1)[7]); 
	
	if(searchInput(testInput, Buffers[0], 0)){
		printf("%s","true\n");
}
	else
		printf("%s","false\n");
}

