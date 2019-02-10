#pragma once
#define SIZE 1000
#define SIZE_DESC 6

struct descriptor {
	int size; // -size занят, +size свободен
	int next : 16;
};



static void ArrayInitialization(void);
static void AddFreeList(struct descriptor *p, struct descriptor *q);
static void* MemorySplit(struct descriptor *q, struct descriptor *p, int size);
void* MyMalloc(int size);
void MyFree(void *p, int size);
