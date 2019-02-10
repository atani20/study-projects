#include <stdio.h>
#include "mymalloc.h"


char memory[SIZE];
struct descriptor *freeSpaceList = (void*)(memory + SIZE - SIZE_DESC);
char flagInitialization = 0;

static void ArrayInitialization(void) {
	freeSpaceList->size = SIZE;
	freeSpaceList->next = SIZE - SIZE_DESC; // initially the list of free space refers to itself
	flagInitialization = 1;
}

static void AddFreeList(struct descriptor *p, struct descriptor *q) {
	q->size *= -1;
	if (((char*)p - (char*)memory > 0) && (p - 1)->size > 0) {
		p = (struct descriptor*)((char *)p - SIZE_DESC);
		q->size += p->size;
		struct descriptor* t = p;
		while (t->next != ((char*)p - (char*)memory))
			t = (struct descriptor*)&memory[t->next];
		t->next = (char*)q - (char*)memory;
		q->next = p->next;
	}
	else
	{
		q->next = (char*)freeSpaceList - (char*)memory;
		struct descriptor* t = q;
		while (t->next != q->next)
			t = (struct descriptor*)&memory[t->next];
		t->next = (char*)q - (char*)memory;
	}
}
static void* MemorySplit(struct descriptor *q, struct descriptor *p, int size) {
	struct descriptor *new = (struct descriptor*)((char*)q - size);
	new->size = (q->size) - size;
	if (&memory[q->next] == (char*)q) {
		new->next = q->next - size; //if q refers to itself, then new too
	}
	else {
		new->next = q->next;
		p->next = q->next - size;
	}
	q->size = -size; // q - busy
	freeSpaceList = new;
	q = (void*)((char*)q - size + SIZE_DESC);
	return q;
}


void* MyMalloc(int size) {
	if (!flagInitialization)  // if used for the first time, initialization occurs. Subsequent times not performed
		ArrayInitialization();
	struct descriptor *p = freeSpaceList;
	struct descriptor *q = (struct descriptor *)&memory[freeSpaceList->next];
	size += SIZE_DESC; // need size bigger by descriptor size
	while ((size < q->size) && (q != freeSpaceList) && (q->next >= 0)) { // if the size does not fit And did not return to the beginning
		p = q;
		q = (struct descriptor *)&memory[q->next];
	}
	if (q->size == size) {
		p->next = q->next; // exclude from the list of free space
		q->size *= -1;
		freeSpaceList = (struct descriptor *)&memory[q->next];
		q = (void*)((char*)q - size + SIZE_DESC);
	}
	else if (q->size > size)
		q = MemorySplit(q, p, size);
	else // q->size < size
	{
		q = NULL;
		printf("ERROR: not enough memory");
	}
	return q;
}


void MyFree(void *p, int size) {
	struct descriptor *q = (struct descriptor *)((char*)p + size);
	if (q->size > 0)
		printf("ERROR: re-release of memory!\n");
	else
		AddFreeList(p, q);
}
