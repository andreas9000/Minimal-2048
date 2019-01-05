// Written by Andreas Hansson

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// A move is considered possible if the board contains at least one empty square,
//  or if two adjacent tiles contain the same value
int move_possible(int* board)
{
	int i,j;
	for (i = 0; i < 16; i++)
		if (!*(board + i))
			return 1;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
			if (*(board + i * 4 + j) == *(board + i * 4 + j + 1) ||
				*(board + i * 4 + j) == *(board + i * 4 + j + 4))
					return 1;
		if (*(board + 3 + i * 4) == *(board + 7 + i * 4) || *(board + 12 + i) == *(board + 13 + i))
			return 1;
	}
	return 0;
}

// Rotates the board 90 degrees
void rotate_right(int* board)
{
	int *board2 = calloc(16, sizeof(int));
	
	int i,j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			*(board2 + (3 - i) + j * 4) = *(board + i * 4 + j);
	
	for (i = 0; i < 16; i++)
		*(board + i) = *(board2 + i);
	
	free(board2);
}

// Direction is 0 for up, 1 for left, 2 for down, 3 for right
// If will make that many rotations before moving up, and then rotating
// to the original position
int make_move(int* board, int direction)
{
	int i,j,k, move_was_done = 0;
	
	for (i = 0; i < direction; i++)
		rotate_right(board);
	
	for (i = 0; i < 4; i++)
	{
		int last_nonzero_val = 0, last_nonzero_index = 0;
		
		for (j = 0; j < 4; j++)
		{
			if (*(board + i + j * 4))
			{
				if (last_nonzero_val == *(board + i + j * 4))
				{
					*(board + last_nonzero_index) *= 2;
					*(board + i + j * 4) = 0;
					last_nonzero_val = 0;
					move_was_done = 1;
				}
				else
				{
					last_nonzero_val = *(board + i + j * 4);
					last_nonzero_index = i + j * 4;
				}
			}
		}
		for (j = 0; j < 4; j++)
			if (*(board + i + j * 4))
				for (k = 0; k < j; k++)
					if (!*(board + i + k * 4))
					{
						*(board + i + k * 4) = *(board + i + j * 4);
						*(board + i + j * 4) = 0;
						move_was_done = 1;
					}
	}
	
	for (i = 0; i < (4 - direction) % 4; i++)
		rotate_right(board);
	
	return move_was_done;
}

void place_tile(int* board)
{
	int i,c = 0, *possible = calloc(16, sizeof(int));
	
	for(i = 0; i < 16; i++)
		if (!*(board + i))
			*(possible + c++) = i;
	
	*(board + *(possible + rand() % c)) = rand() % 10 ? 2 : 4;
}

void draw_board(int* board)
{
	printf("\n\n\n\n\n\n\n\n\n    ");
	int i,j,l;
	for(i = 0; i < 16; i++)
	{
		if (*(board + i))
		{
			printf("%d", *(board + i));
			l = 1;
			if (*(board + i) >= 10000)
				l = 5;
			else if (*(board + i)  >= 1000)
				l = 4;
			else if (*(board + i) >= 100)
				l = 3;
			else if (*(board + i) >= 10)
				l = 2;
			
			for (j = 0; j < 7-l; j++)
				printf(" ");
		}
		else
			printf(".      ");
		
		if ((i %  4) == 3)
			printf("\n    ");
	}
	printf("\n\n");
}

int main()
{
	int * board = calloc(16, sizeof(int));
	
	place_tile(board);
	draw_board(board);
	
	while (move_possible(board))
	{
			getch();
			int dir = getch();
			
			if (dir == 72)
				dir = 0;
			else if (dir == 75)
				dir = 1;
			else if (dir == 80)
				dir = 2;
			else if (dir == 77)
				dir = 3;
			
			if (make_move(board, dir))
				place_tile(board);
			draw_board(board);
	}
	
	printf("\n\nGame over\n");
	free(board);
	getch();
}
