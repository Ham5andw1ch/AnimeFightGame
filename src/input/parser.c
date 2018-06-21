#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
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
		player.inputs[player.tail] = NULL;
		player.tail = mod((player.tail-1),bufferLength);
		}
	}
	



