﻿#include <stdio.h>
#include <stdlib.h>

#define MAZE_SIZE 10
#define EMTPY '0'
#define FILLED '1'
#define VISITED '.'

typedef struct {
	short r;
	short c;
} element;

typedef struct stack {
	element* data;
	int capacity;
	int top;
} Stack;

const int DIR[4][2] = { {-1,0},{0,-1},{1,0},{0,1} };

element entry = { 1,0 };
char maze[MAZE_SIZE][MAZE_SIZE] = {
	{'1','1','1','1','1','1','1','1','1','1'},
	{'e','1','0','1','0','0','0','1','0','1'},
	{'0','0','0','1','0','0','0','1','0','1'},
	{'0','1','0','0','0','1','1','0','0','1'},
	{'1','0','0','0','1','0','0','0','0','1'},
	{'1','0','0','0','1','0','0','0','0','1'},
	{'1','0','0','0','0','0','1','0','1','1'},
	{'1','0','1','1','1','0','1','1','0','1'},
	{'1','1','0','0','0','0','0','0','0','x'},
	{'1','1','1','1','1','1','1','1','1','1'}
};

void initStack(Stack* st) {
	st->capacity = 1;
	st->top = -1;
	st->data = (element*)malloc(st->capacity * sizeof(element));
}

int is_empty(Stack* st) {
	return st->top == -1;
}

int is_full(Stack* st) {
	return st->top == st->capacity - 1;
}

void push(Stack* st, element value) {
	if (is_full(st)) {
		st->capacity *= 2;
		void* tmp = realloc(st->data, st->capacity * sizeof(element));
		if (tmp != NULL)
			st->data = (element*)tmp;
	}
	if (st->data != NULL)
		st->data[++st->top] = value;
}

void empty_error() {
	printf("스택 공백 에러\n");
	exit(1);
}

element pop(Stack* st) {
	if (is_empty(st))
		empty_error();
	return st->data[st->top--];
}

element peek(Stack* st) {
	if (is_empty(st))
		empty_error();
	return st->data[st->top];
}

void print_maze(const char(*maze)[MAZE_SIZE]) {
	for (int i = 0; i < MAZE_SIZE; ++i) {
		for (int j = 0; j < MAZE_SIZE; ++j)
			printf("%c", maze[i][j]);
		printf("\n");
	}
	printf("\n");
}

int push_loc(Stack* st, int r, int c) {
	if (r < 0 || c < 0 || maze[r][c] == VISITED || maze[r][c] == FILLED)
		return 0;

	push(st, (element) { r, c });
	return 1;
}

int is_beside(element here, element top) {
	for (int i = 0; i < 4; ++i)
		if (here.r + DIR[i][0] == top.r && here.c + DIR[i][1] == top.c)
			return 1;
	return 0;
}

void stackClear(Stack* st1, Stack* st2) {
	while (is_empty(st1)) {
		pop(st1);
	}
	while (is_empty(st2)) {
		pop(st2);
	}
}

void escapeMazeRoute() {
	int r, c, i;
	Stack cand_route; // 나아갈 길의 후보를 스택구조로 넣어놓음
	Stack route; // 지금까지 지나온 길을 스택구조로 넣어놓음
	element here = entry;

	initStack(&cand_route);
	initStack(&route);

	while (maze[here.r][here.c] != 'x') {
		r = here.r;
		c = here.c;
		maze[r][c] = VISITED;
		printf("(%d, %d) -> ", r, c);

		int branch = 0;
		for (i = 0; i < 4; ++i)
			if (push_loc(&cand_route, r + DIR[i][0], c + DIR[i][1]))
				branch++;

		if (is_empty(&cand_route)) {
			printf("failed\n");
			return;
		}
		else {
			here = pop(&cand_route);
			push(&route, here);
		}

	}
	printf("success\n");
	stackClear(&cand_route, &route);
}

int main()
{
	print_maze(maze);
	printf("\n");
	escapeMazeRoute();
	return 0;
}