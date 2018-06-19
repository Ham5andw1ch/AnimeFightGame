#include <stdio.h>
#include <stdint.h>

    uint8_t p1Buffer[30][8];
    int head, tail;
	int main(){

	}
	void add(uint8_t inputs[]){
		if(p1Buffer[head] != null){
			if(mod((head-1),p1Buffer.Length) ==tail){
				
				p1Buffer[tail] = inputs;
				tail = mod((tail-1),queue.Length);
				head = mod((head-1),queue.Length);
			}else{
				p1Buffer[head-1] = inputs;
				head = mod((head-1),p1Buffer.Length);
			}
			
		}else{
			
			p1Buffer[head] = inputs;
			tail = head;
		}	
	}

	uint8_t[] remove(){
		uint8_t[] temp = p1Buffer[head];
		p1Buffer[head] = null;
		head = mod((head+1),p1Buffer.Length);
		return temp;
	}
	uint8_t[] peek(){
		uint8_t[] temp = p1Buffer[head];
		return temp;
	}
	void removeTail(){
		if(p1Buffer[tail] != null){
		p1Buffer[tail] = null;
		tail = mod((tail-1),p1Buffer.Length);
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


