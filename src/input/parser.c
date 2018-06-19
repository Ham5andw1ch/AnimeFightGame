#include <stdio.h>
#include <stdint.h>
	int bufferLength;
    
		uint8_t** p1Buffer;

    int head, tail;
	int main(){
		p1Buffer = malloc(30 * sizeof(*p1Buffer));
		for(int i = 0; i < 10; ++i)
		{
    		p1Buffer[i] = malloc(8 * sizeof(**p1Buffer));
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


