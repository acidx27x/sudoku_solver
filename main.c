#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 

#define SIZE 9 //SIZE x SIZE sudoku field



void print_board(int[SIZE][SIZE]);

bool check_free_cell(int[SIZE][SIZE], int, int, int); //check if we can put a number in a cell

int* find_suitable_numbers(int[SIZE][SIZE], int, int, int*); //find every possible number we can put in a cell

void next_free_cell(int[SIZE][SIZE], int, int, int*, int*); //finding first free cell

void copy_board(int[SIZE][SIZE], int[SIZE][SIZE]); //make a copy of a board

bool solve_sudoku(int[SIZE][SIZE], int, int);



int main()
{//guaranteed that we have a valid input
	int sudoku_field1[SIZE][SIZE] = {	5, 3, 0,	0, 7, 0,	0, 0, 0,
								6, 0, 0,	1, 9, 5,	0, 0, 0,
								0, 9, 8,	0, 0, 0,	0, 6, 0,

								8, 0, 0,	0, 6, 0,	0, 0, 3,
								4, 0, 0,	8, 0, 3,	0, 0, 1,
								7, 0, 0,	0, 2, 0,	0, 0, 6,
			
								0, 6, 0,	0, 0, 0,	2, 8, 0,
								0, 0, 0,	4, 1, 9,	0, 0, 5,
								0, 0, 0,	0, 8, 0,	0, 7, 9 };

	int sudoku_field2[SIZE][SIZE] = { 0, 0, 4,	0, 5, 0,	0, 0, 0,
								0, 0, 8,	0, 0, 7,	2, 3, 0,
								0, 0, 7,	0, 0, 8,	0, 0, 0,

								6, 0, 0,	9, 4, 0,	0, 0, 0,
								0, 8, 0,	0, 0, 0,	0, 9, 0,
								0, 0, 0,	0, 8, 3,	0, 0, 1,

								0, 0, 0,	2, 0, 0,	5, 0, 0,
								0, 4, 1,	5, 0, 0,	3, 0, 0,
								0, 0, 0,	0, 1, 0,	6, 0, 0 };

	int sudoku_field3[SIZE][SIZE] = { 0 };

	printf("\nUnsolved sudoku:\n");
	print_board(sudoku_field2);
	if (solve_sudoku(sudoku_field2, 0, 0))
	{
		printf("\nSuccessfully solved sudoku:\n");
		print_board(sudoku_field2);
	}
	else
		printf("smth went wrong, cant solve sudoku!");
}


void print_board(int board[SIZE][SIZE]) 
{
	printf(" ---------------------\n");
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			if (j % 3 == 0 && j != 0)
				printf(" |");
			printf(" %d", *(*(board + i) + j));
		}
		if ((i + 1) % 3 == 0)
			printf("\n ---------------------\n");
		else
			printf("\n");
	}
}

bool check_free_cell(int board[SIZE][SIZE], int row, int col, int number) 
{
	if (*(*(board + row) + col) != 0) //cell is occupied
		return false;

	int grid_row = row / 3 * 3; //returns beginning coords
	int grid_col = col / 3 * 3; //of minor square

	for (int i = 0; i < SIZE; ++i)
		if ((*(*(board + row) + i) == number) || (*(*(board + i) + col) == number))
			return false; //check if number already exist in row and col

	for (int i = 0; i < SIZE; i += 3)
		for (int j = 0; j < SIZE; j += 3)
			if ((*(*(board + grid_row + i / 3) + grid_col + j / 3) == number))
				return false; //check if number already exist in minor square

	return true;
}

int* find_suitable_numbers(int board[SIZE][SIZE], int row, int col, int* size) 
{ //returning an array of suitable numbers
	*size = 0;
	int* suitable_numbers = NULL;
	for (int i = 1; i <= 9; ++i)
	{
		if (check_free_cell(board, row, col, i)) //check if we can use this number
		{
			int* tmp_board = (int*)realloc(suitable_numbers, sizeof(int) * (*size + 1));
			if (tmp_board == NULL)
			{
				printf("bad alloc at 'find_suitable_numbers' func\n");
				exit(-1);
			}
			else
				suitable_numbers = tmp_board;

			*(suitable_numbers + (*size)++) = i;
		}
	}
	return suitable_numbers;
}

void next_free_cell(int board[SIZE][SIZE], int row, int col, int* nrow, int* ncol)
{
	while (row < SIZE)
	{
		if (col > SIZE - 1)
		{
			col = 0; //next row
			++row;
		}
		if (*(*(board + row) + col) == 0)
			break;
		++col;
	}

	*nrow = row;
	*ncol = col;
}

void copy_board(int SRC[SIZE][SIZE], int DST[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; ++i)
		for (int j = 0; j < SIZE; ++j)
			DST[i][j] = SRC[i][j];
}

bool solve_sudoku(int board[SIZE][SIZE], int row, int col)
{
	int nrow, ncol; //next free cell
	next_free_cell(board, row, col, &nrow, &ncol);
	if (row > SIZE - 1) //end of board
		return true; 

	int size_of_found;
	int* found = find_suitable_numbers(board, nrow, ncol, &size_of_found);
	if (size_of_found == 0)
		return false;

	for (int i = 0; i < size_of_found; ++i)
	{
		int board_copy[SIZE][SIZE] = { 0 };
		copy_board(board, board_copy);
		board_copy[nrow][ncol] = found[i];
		int vnrow = nrow; //very next cell
		int vncol = ncol;
		next_free_cell(board_copy, nrow, ncol, &vnrow, &vncol);
		if (solve_sudoku(board_copy, vnrow, vncol)) //backtracking
		{
			copy_board(board_copy, board);
			return true;
		}
	}

	return false;
}
