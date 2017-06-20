#include<stdio.h>
#include <stdlib.h>

#define BitsForPageNo 6
#define BitsForPageOffset 10

#define ProcessSize 24800

/*
1. What is the maximum number of page table entries?
2 bytes(16 bits)이다. 16-bit computer이기 때문이다.

2. What is size of a page(in bytes)?
2^10 bytes이다. page offset이 10 bits 이기 때문에 0000000000 부터 1111111111를 구분하기 위해서는 2^10 bytes가 필요하다.

3. How many pages are necessary to store a process whose size is 24800 bytes?
25개이다. 24800 / 2^10(1024) 의 올림은 25이기 때문이다. 프로세스의 전체 사이즈인 24800 bytes를 한 페이지의 사이즈인 1024 bytes로
나눈 후 올림하면 필요한 페이지의 갯수가 나온다. 
*/

void init_page_table(int table[], int no_page);
char* to_binary(unsigned short addr);
unsigned short logical2physical(unsigned short logical_addr, int page_table[]);

int main(){
	int no_page = 0;
	int *page_table = NULL;
	unsigned short logical_addr[] = { 0x21dd, 0x0829, 0x08f0, 0x0ad1, 0x071e, 0x0431, 0x0fb8, 0x0b18, 0x38ab, 0x07a0 };	
	int i = 0;
	
	no_page = 25; 
	page_table = (int*)malloc(no_page * sizeof(int));
	
	if(page_table == NULL){
		printf("Failed to allocate memory!\n");
		exit(-1);	
	}
		
	init_page_table(page_table, no_page);
	
	for(i = 0; i < 10; i++){
		unsigned short physical_addr = logical2physical(logical_addr[i], page_table);
		printf("0x%04x (%s) ",logical_addr[i], to_binary(logical_addr[i]));
		printf("--> 0x%04x (%s)\n", physical_addr, to_binary(physical_addr));	
	}
	free(page_table);
		
	return 0;

}

void init_page_table(int table[], int no_page){
	for(int i = 0; i < no_page; i++){
		table[i] = 60 - i * 2;
	}
}

char* to_binary(unsigned short addr){
	static char bits[17];
	int i = 0;

	for(i = 0; i < 16; i++){
		bits[15 - i] = '0' + (addr & 1);
		addr >>= 1;
	}
	bits[16] = 0;

	return bits;
}

unsigned short logical2physical(unsigned short logical_addr, int page_table[]){
	unsigned short temp_addr = logical_addr;
	unsigned short physical_addr;
	unsigned short frame;	//frame 번호를 저장할 변수 

	logical_addr >>= BitsForPageOffset;
	temp_addr &= 1023; // logical address에 이진수인 1111111111를 & 해서page offset을 구함. 
	frame = page_table[logical_addr]; 
	frame <<= BitsForPageOffset;
	physical_addr = frame | temp_addr;
	
	return physical_addr;
}





















