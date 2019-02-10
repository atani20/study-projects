#include <stdio.h>
#include "mymalloc.h"

int main(void)
{
	/*char *p1 = MyMalloc(20);
	for (int i = 880; i < 1000; i++)
		printf("%i	%i\n", i, memory[i]);
	printf("\n\nadress: %i\n ", (char*)freeSpaceList - (char*)memory);*/
	//printf("\n\nadress: %i\n ", (char*)p1 - (char*)memory);
	/*char *p2 = MyMalloc(8);
	char *p3 = MyMalloc(10);
	char *p4 = MyMalloc(20);
	char *p5 = MyMalloc(8);
	char *p6 = MyMalloc(10);
	for (int i = 880; i < 1000; i++)
		printf("%2i	%i\n", i, memory[i]);
	MyFree(p5, 8);
	MyFree(p3, 10);
	MyFree(p4, 20);
	printf("...\n\n");
	for (int i = 880; i < 1000; i++)
		printf("%2i	%i\n", i, memory[i]);
	printf("\n\nadress: %i\n ", (char*)freeSpaceList - (char*)memory);
	/*printf("...\n\n");
	//p1 = MyMalloc(20);
	for (int i = 0; i < 100; i++)
		printf("%2i	%i\n", i, memory[i]);
	printf("...\n\n");*/
	return 0;
}