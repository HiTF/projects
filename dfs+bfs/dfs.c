#include <stdio.h>
#include <stdlib.h>
const int n=6;
#define size 255

int visit[size];
int grph[6][6] = 
{
	{0, 1, 0, 0, 1, 1},
	{1, 0, 1, 1, 0, 0},
	{0, 1, 0, 0, 1, 1},
	{0, 1, 0, 0, 1, 0},
	{1, 0, 1, 1, 0, 1},
	{0, 1, 0, 1, 1, 0}
};

void DFS(int s) { // s – начальный узел
	int r;
	printf("%d\n", s+1);
	visit[s] = 1;
	for (r = 0; r < n; r++)
		if ((visit[r]!=1) && (grph[s][r]!=0))
		DFS(r);
}

int main() {
	for(int i = 0; i < size; i++) 
		visit[i] = 0;
	DFS(0);
	return 0;
}

