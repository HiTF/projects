#include <stdio.h>
#include <stdlib.h>

#define SIZE 255

typedef struct {
  int first; // позиция первого элемента
  int last; // последнего элемента
  int data[SIZE + 1]; // тело очереди
  int count; // колво элементов в очереди
} queue_t;

// для проверки 
int grph[6][6] = 
{
	{0, 0, 1, 1, 0, 0},
	{0, 0, 0, 0, 1, 1},
	{0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0}
};

void init_queue(queue_t *que) {
  que->first = 0;
  que->last = SIZE - 1;
  que->count = 0;
return;
}

void push(queue_t *que, int x) { // добавляет элемент x в хвост очереди que
	if (que->count >= SIZE)
		printf("Warning: queue overflow push x = %d\n", x);
	else {
		que->last = (que->last + 1 ) % SIZE; 
		que->data[ que->last ] = x;
		que->count = que->count + 1;
	}
}

int front(queue_t *que) { // возвращает элемент из головы очереди
	int x;
	if (que->count <= 0) 
		printf ("Warning: empty queue pop\n");
	else {
		x = que->data[ que->first ];
	}
	return x;
}

int back(queue_t *que) { // -//- хвоста очереди
	int x;
	if (que->count >= SIZE)
		printf("Warning: queue overflow pop \n");
	else {
		x = que->data[ que->last ];
	}
	return x;
}

void pop(queue_t *que) { // удаляет элемент из головы очереди que;
    que->first = (que->first + 1) % SIZE;
    que->count = que->count - 1;
	return;
}

int is_empty(queue_t *que) { // возвращает 1, если очередь пуста, и 0 - в противном случае
	if (que->count <= 0) 
		return 1;
	else 
		return 0;
}

int main() {
	int i;
	int dir;
	queue_t s; 
	init_queue(&s);
	push(&s, 0);
	while((is_empty(&s)) == 0) {
		dir = front(&s);
		printf("%d %d %d\n", dir + 1, s.first, s.last);
		pop(&s); //удаляем первый элемент из головы
		for(i = 5; i >= 0; i--)
			if(grph[dir][i] == 1)
				push(&s, i);
	}
	return 0;
}
