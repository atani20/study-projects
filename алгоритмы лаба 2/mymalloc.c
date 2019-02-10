#include <stdio.h>
#include "mymalloc.h"


char memory[SIZE];
struct descriptor *freeSpaceList = (void*)(memory + SIZE - SIZE_DESC);
char flagInitialization = 0;

static void ArrayInitialization(void) {
	freeSpaceList->size = SIZE;
	freeSpaceList->next = SIZE - SIZE_DESC; // изначально список сводобного пространства ссылается сам на себя
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
		new->next = q->next - size; // если q ссылался сам на себя, то и new тоже
	}
	else {
		new->next = q->next;
		p->next = q->next - size;
	}
	q->size = -size; // q - занят
	freeSpaceList = new;
	q = (void*)((char*)q - size + SIZE_DESC);
	return q;
}


void* MyMalloc(int size) {
	if (!flagInitialization)  // если используем первый раз, то происходит инициализация. В последующие разы не выполняется
		ArrayInitialization();
	struct descriptor *p = freeSpaceList;
	struct descriptor *q = (struct descriptor *)&memory[freeSpaceList->next];//(struct descriptor *)&memory[p->next];
	size += SIZE_DESC; // нужен размер больше на размер дескриптора
	while ((size < q->size) && (q != freeSpaceList) && (q->next >= 0)) { // если  размер не подходит И не вернулись к началу
		p = q;
		q = (struct descriptor *)&memory[q->next];
	}
	if (q->size == size) {
		p->next = q->next; // исключаем из списка свободного пространства
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