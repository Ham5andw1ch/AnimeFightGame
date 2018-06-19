#include <stdio.h>
#include <stdint.h>
#include <settings.h>
	
	
    typedef struct
	{
    	int head;
    	int tail;
    	uint8_t *inputs[bufferLength];
	} buffer_t;
	buffer_t Buffers[2];
	
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
		free(Buffers[0]);
		free(Buffers[1]);
	}

	void add(uint8_t inputs[], uint8_t player){
		

		if(currentBuffer[head] != NULL){
			if(mod((head-1),bufferLength) ==tail){
				
				currentBuffer[tail] = inputs;
				tail = mod((tail-1),bufferLength);
				head = mod((head-1),bufferLength);

			}else{
				currentBuffer[head-1] = inputs;
				head = mod((head-1),bufferLength);
			}
			
		}else{
			currentBuffer[head] = inputs;
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


