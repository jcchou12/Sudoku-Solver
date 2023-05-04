// A Backtracking program in
// C++ to solve Sudoku problem
#include <iostream>
#include <stack>
#include <cmath>
#include "timing.h"
using namespace std;
  
// UNASSIGNED is used for empty
// cells in sudoku grid
#define UNASSIGNED 0
  
// N is used for the size of Sudoku grid.
// Size will be NxN
#define N 9
// #define N 16

#define EMP 2
#define POSSIBLE (N**EMP)

class Board
{
    public:
        int board[N][N];
        Board() {};

    void copyBoard(int A[N][N])
    {
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                board[r][c] = A[r][c];
            }
        }
    }
};
  
/* Searches the grid to find an entry that is 
still unassigned. If found, the reference 
parameters row, col will be set the location 
that is unassigned, and true is returned. 
If no unassigned entries remain, false is returned. */
bool FindUnassignedLocation(Board grid,
                            int& row, int& col)
{
    for (row = 0; row < N; row++)
        for (col = 0; col < N; col++)
            if (grid.board[row][col] == UNASSIGNED)
                return true;
    return false;
}
  
/* Returns a boolean which indicates whether 
an assigned entry in the specified row matches
the given number. */
bool UsedInRow(Board grid, int row, int num)
{
    for (int col = 0; col < N; col++)
        if (grid.board[row][col] == num)
            return true;
    return false;
}
  
/* Returns a boolean which indicates whether 
an assigned entry in the specified column
matches the given number. */
bool UsedInCol(Board grid, int col, int num)
{
    for (int row = 0; row < N; row++)
        if (grid.board[row][col] == num)
            return true;
    return false;
}
  
/* Returns a boolean which indicates whether 
an assigned entry within the specified sqrt(N)xsqrt(N) box 
matches the given number. */
bool UsedInBox(Board grid, int boxStartRow,
               int boxStartCol, int num)
{
    for (int row = 0; row < sqrt(N); row++)
        for (int col = 0; col < sqrt(N); col++)
            if (grid.board[row + boxStartRow]
                    [col + boxStartCol] ==
                                       num)
                return true;
    return false;
}
  
/* Returns a boolean which indicates whether 
it will be legal to assign num to the given 
row, col location. */
bool isSafe(Board grid, int row,
            int col, int num)
{
    return !UsedInRow(grid, row, num)
           && !UsedInCol(grid, col, num)
           && !UsedInBox(grid, row - row % int(sqrt(N)),
                         col - col % int(sqrt(N)), num)
           && grid.board[row][col] == UNASSIGNED;
}


void generateBoards(Board grid, stack<Board> &board)
{
    int row, col;
    if (FindUnassignedLocation(grid, row, col)) {
        for (int j = 1; j <= N; j++) {
            if (isSafe(grid, row, col, j)) {
                // Copying grid
                Board B;
                B.copyBoard(grid.board);
                B.board[row][col] = j;
                board.push(B);
            }
        }
    }
    
}

bool parallelSolve(Board &grid, stack<Board> &boards) 
{
    
    if (boards.empty()) return false;
    Board currBoard;
    currBoard.copyBoard((boards.top()).board);
    boards.pop();
    int row, col;
    if (!FindUnassignedLocation(currBoard, row, col)) {
        grid.copyBoard(currBoard.board);
        return true;
    } else {
        generateBoards(currBoard, boards);
        return parallelSolve(grid, boards);
    }

}

/* Takes a partially filled-in grid and attempts 
to assign values to all unassigned locations */
bool SolveSudoku(Board &grid)
{
    stack<Board> boards;
    boards.push(grid);
    generateBoards(grid, boards);
    return parallelSolve(grid, boards);
}
