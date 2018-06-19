#include <stdio.h>
#include <stdint.h>
#include <settings.h>
	
	
    uint8_t** p1Buffer;
    uint8_t** p2Buffer;

    int head, tail;
	
	//Initialize Buffers
	void initBuffers(){
		//Player 1 2D array of Byte Strings
		p1Buffer = malloc(bufferLength * sizeof(*p1Buffer));
		for(int i = 0; i < 10; ++i)
		{
    		p1Buffer[i] = malloc(bufferTolerance * sizeof(**p1Buffer));
		}
		//Player 2 2D array of Byte Strings
		p2Buffer = malloc(bufferLength * sizeof(*p2Buffer));
		for(int i = 0; i < 10; ++i)
		{
    		p2Buffer[i] = malloc(bufferTolerance * sizeof(**p2Buffer));
		}
	}

	void add(uint8_t inputs[]){
		if(p1Buffer[head] != NULL){
			if(mod((head-1),bufferLength) ==tail){
				
				p1Buffer[tail] = inputs;
				tail = mod((tail-1),bufferLength);
				head = mod((head-1),bufferLength);
			}else{
				p1Buffer[head-1] = inputs;
				head = mod((head-1),bufferLength);
			}
			
		}else{
			
			p1Buffer[head] = inputs;
			tail = head;
		}	
	}

	uint8_t[] remove(){
		uint8_t[] temp = p1Buffer[head];
		p1Buffer[head] = null;
		head = mod((head+1),bufferLength);
		return temp;
	}
	uint8_t[] peek(){
		uint8_t[] temp = p1Buffer[head];
		return temp;
	}
	void removeTail(){
		if(p1Buffer[tail] != null){
		p1Buffer[tail] = null;
		tail = mod((tail-1),bufferLength);
		}
	}
	int mod(int x, int m) {
    	int r = x%m;
		if(r<0){
			return r+m;
		}else{
			return r;
		}
	}


