// A Backtracking program in
// C++ to solve Sudoku problem
#include <iostream>
#include "timing.h"
using namespace std;
  
// UNASSIGNED is used for empty
// cells in sudoku grid
#define UNASSIGNED 0
  
#define M 16
// N is used for the size of Sudoku grid.
// Size will be NxN
// #define N 9
#define N M

#define BLOCKSIZE 1024
#define THREADS 32
  
// This function finds an entry in grid
// that is still unassigned
bool FindUnassignedLocation(int grid[N][N],
                            int& row, int& col);
  
// Checks whether it will be legal
// to assign num to the given row, col
bool isSafe(int grid[N][N], int row,
            int col, int num);
  

void possibleBoards(int* newBoards, int* old_boards, )

/* Takes a partially filled-in grid and attempts 
to assign values to all unassigned locations in 
such a way to meet the requirements for
Sudoku solution (non-duplication across rows,
columns, and boxes) */
__global__ bool SolveSudoku(int grid[N][N])
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int row, col;
  
    // If there is no unassigned location,
    // we are done
    if (!FindUnassignedLocation(grid, row, col))
        // success!
        return true;
  
    // Consider digits 1 to 9
    for (int num = 1; num <= N; num++) 
    {
          
        // Check if looks promising
        if (isSafe(grid, row, col, num)) 
        {
              
            // Make tentative assignment
            grid[row][col] = num;
  
            // Return, if success
            if (SolveSudoku(grid))
                return true;
  
            // Failure, unmake & try again
            grid[row][col] = UNASSIGNED;
        }
    }
     
    // This triggers backtracking
    return false;
}
  
/* Searches the grid to find an entry that is 
still unassigned. If found, the reference 
parameters row, col will be set the location 
that is unassigned, and true is returned. 
If no unassigned entries remain, false is returned. */
bool FindUnassignedLocation(int grid[N][N],
                            int& row, int& col)
{
    for (row = 0; row < N; row++)
        for (col = 0; col < N; col++)
            if (grid[row][col] == UNASSIGNED)
                return true;
    return false;
}
  
/* Returns a boolean which indicates whether 
an assigned entry in the specified row matches
the given number. */
bool UsedInRow(int grid[N][N], int row, int num)
{
    for (int col = 0; col < N; col++)
        if (grid[row][col] == num)
            return true;
    return false;
}
  
/* Returns a boolean which indicates whether 
an assigned entry in the specified column
matches the given number. */
bool UsedInCol(int grid[N][N], int col, int num)
{
    for (int row = 0; row < N; row++)
        if (grid[row][col] == num)
            return true;
    return false;
}
  
/* Returns a boolean which indicates whether 
an assigned entry within the specified sqrt(N)xsqrt(N) box 
matches the given number. */
bool UsedInBox(int grid[N][N], int boxStartRow,
               int boxStartCol, int num)
{
    for (int row = 0; row < sqrt(N); row++)
        for (int col = 0; col < sqrt(N); col++)
            if (grid[row + boxStartRow]
                    [col + boxStartCol] ==
                                       num)
                return true;
    return false;
}
  
/* Returns a boolean which indicates whether 
it will be legal to assign num to the given 
row, col location. */
bool isSafe(int grid[N][N], int row,
            int col, int num)
{
    /* Check if 'num' is not already placed in 
    current row, current column 
    and current sqrt(N)xsqrt(N) box */
    return !UsedInRow(grid, row, num)
           && !UsedInCol(grid, col, num)
           && !UsedInBox(grid, row - row % int(sqrt(N)),
                         col - col % int(sqrt(N)), num)
           && grid[row][col] == UNASSIGNED;
}
  
/* A utility function to print grid */
void printGrid(int grid[N][N])
{
    for (int row = 0; row < N; row++) 
    {
        for (int col = 0; col < N; col++)
            std::cout << grid[row][col] << " ";
        std::cout << endl;
    }
}
  
void checkCorrect9()
{
    // 0 means unassigned cells
    std::cout << "9x9 Tests: \n";
    std::cout << "Test 1 - solvable-random: \n";
    int grid1[N][N] = { { 3, 0, 6, 5, 0, 8, 4, 0, 0 },
                       { 5, 2, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 8, 7, 0, 0, 0, 0, 3, 1 },
                       { 0, 0, 3, 0, 1, 0, 0, 8, 0 },
                       { 9, 0, 0, 8, 6, 3, 0, 0, 5 },
                       { 0, 5, 0, 0, 9, 0, 6, 0, 0 },
                       { 1, 3, 0, 0, 0, 0, 2, 5, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 7, 4 },
                       { 0, 0, 5, 2, 0, 6, 3, 0, 0 } };
    Timer t;
    t.reset();
    if (SolveSudoku(grid1) == true)
        printGrid(grid1);
    else
        std::cout << "No solution exists";
    std::cout << "Time Taken: " << t.elapsed() << "\n";

    std::cout << "\nTest 2 - solvable-sparse: \n";
    int grid2[N][N] = { { 4, 0, 0, 0, 0, 6, 0, 0, 0 },
                       { 0, 7, 1, 0, 5, 0, 4, 0, 0 },
                       { 0, 0, 3, 0, 0, 9, 0, 0, 0 },
                       { 0, 0, 0, 4, 0, 0, 6, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 2, 0 },
                       { 7, 0, 0, 0, 0, 0, 0, 1, 0 },
                       { 0, 0, 0, 0, 0, 4, 0, 0, 0 },
                       { 0, 6, 0, 0, 0, 0, 0, 8, 3 },
                       { 0, 5, 0, 1, 0, 0, 0, 0, 0} };
    t.reset();
    if (SolveSudoku(grid2) == true)
        printGrid(grid2);
    else
        std::cout << "No solution exists\n";
    std::cout << "Time Taken: " << t.elapsed() << "\n";

    std::cout << "\nTest 3 - unsolvable-random: \n";
    int grid3[N][N] = { { 1, 2, 3, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 4, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 4 },
                       { 0, 0, 0, 4, 0, 0, 6, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 2, 0 },
                       { 7, 0, 0, 0, 0, 0, 0, 1, 0 },
                       { 0, 0, 0, 0, 0, 4, 0, 0, 0 },
                       { 0, 6, 0, 0, 0, 0, 0, 8, 3 },
                       { 0, 5, 0, 1, 0, 0, 2, 0, 0} };
    t.reset();
    if (SolveSudoku(grid3) == true)
        printGrid(grid3);
    else
        std::cout << "No solution exists\n";
    std::cout << "Time Taken: " << t.elapsed() << "\n";

    std::cout << "\nTest 4 - unsolvable-sparse: \n";
    int grid4[N][N] = { { 1, 2, 3, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 4, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 4 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
    t.reset();
    if (SolveSudoku(grid4) == true)
        printGrid(grid4);
    else
        std::cout << "No solution exists \n";
    std::cout << "Time Taken: " << t.elapsed() << "\n";
}

void checkCorrect16()
{
    std::cout << "16x16 Tests: \n";
    std::cout << "Test 5 - solvable-random: \n";
    Timer t;
    int grid5[M][M] = { { 0, 0, 12, 8, 11, 0, 13, 10, 0, 2, 9, 1, 6, 15, 0, 0 },
                       { 0, 0, 0, 11, 8, 0, 0, 0, 5, 12, 13, 10, 4, 0, 0, 2 },
                       { 13, 2, 10, 0, 3, 7, 5, 0, 8, 6, 14, 15, 11, 0, 12, 16 },
                       { 16, 6, 9, 15, 2, 0, 0, 14, 11, 4, 7, 3, 5, 10, 0, 13 },
                       { 15, 12, 7, 0, 0, 0, 0, 11, 2, 0, 16, 13, 0, 4, 1, 0 },
                       { 11, 8, 6, 0, 4, 13, 3, 7, 0, 9, 0, 14, 16, 0, 0, 12 },
                       { 2, 4, 5, 9, 1, 16, 14, 6, 10, 3, 12, 11, 15, 7, 0, 0 },
                       { 14, 13, 1, 16, 15, 12, 2, 5, 0, 7, 8, 0, 9, 0, 11, 10 },
                       { 9, 1, 13, 0, 14, 11, 15, 4, 12, 16, 0, 0, 10, 6, 0, 0 },
                       { 0, 0, 4, 6, 0, 9, 0, 0, 13, 14, 0, 7, 12, 11, 0, 1 },
                       { 0, 11, 16, 12, 0, 0, 1, 0, 3, 15, 4, 6, 13, 14, 0, 7 },
                       { 0, 0, 2, 14, 0, 0, 0, 13, 9, 1, 11, 5, 8, 0, 4, 15 },
                       { 0, 7, 11, 0, 9, 2, 10, 8, 0, 0, 0, 16, 1, 12, 15, 0 },
                       { 4, 0, 15, 2, 13, 0, 11, 0, 1, 10, 6, 0, 0, 8, 16, 14 },
                       { 12, 0, 0, 1, 6, 14, 0, 15, 7, 11, 5, 2, 3, 13, 10, 9 },
                       { 10, 9, 14, 13, 7, 0, 16, 1, 4, 8, 0, 0, 0, 5, 6, 11 } };
    t.reset();
    if (SolveSudoku(grid5) == true)
        printGrid(grid5);
    else
        std::cout << "No solution exists";
    std::cout << "Time Taken: " << t.elapsed() << "\n";

    std::cout << "\nTest 2 - solvable-sparse: \n";
    int grid2[M][M] = { { 14, 0, 16, 0, 15, 0, 5, 4, 0, 0, 0, 2, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 2, 12, 7, 0, 0, 0, 0, 16, 0, 5, 0, 8 },
                       { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 10, 5, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 8, 16, 9, 7, 0, 4, 3, 0, 0, 0, 11 },
                       { 0, 0, 14, 0, 0, 15, 0, 0, 0, 0, 12, 4, 0, 0, 9, 0 },
                       { 9, 0, 0, 0, 0, 0, 8, 5, 0, 2, 0, 0, 11, 0, 0, 7 },
                       { 0, 0, 0, 4, 13, 0, 0, 6, 0, 0, 0, 0, 16, 0, 0, 0 },
                       { 0, 0, 6, 7, 0, 16, 0, 0, 0, 5, 9, 0, 0, 0, 10, 13 },
                       { 5, 0, 0, 0, 0, 0, 0, 3, 9, 0, 14, 0, 0, 0, 6, 1 },
                       { 0, 1, 0, 0, 0, 13, 0, 0, 0, 10, 0, 8, 0, 2, 0, 4 },
                       { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15 },
                       { 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 } };
    t.reset();
    if (SolveSudoku(grid2) == true)
        printGrid(grid2);
    else
        std::cout << "No solution exists";
    std::cout << "Time Taken: " << t.elapsed() << "\n";
  
    std::cout << "\nTest 3 - unsolvable-random: \n";
    int grid3[M][M] = { { 0, 0, 12, 8, 11, 0, 13, 10, 0, 2, 9, 1, 6, 15, 0, 0 },
                       { 0, 0, 0, 11, 8, 0, 0, 0, 5, 12, 13, 10, 4, 0, 0, 2 },
                       { 13, 2, 10, 0, 3, 7, 5, 0, 8, 6, 14, 15, 11, 0, 12, 16 },
                       { 16, 6, 9, 15, 2, 0, 0, 14, 11, 4, 7, 3, 5, 10, 0, 13 },
                       { 15, 12, 7, 0, 0, 0, 0, 11, 2, 0, 16, 13, 0, 4, 1, 0 },
                       { 11, 8, 6, 0, 4, 13, 3, 7, 0, 9, 0, 14, 16, 0, 0, 12 },
                       { 2, 4, 5, 9, 1, 16, 14, 6, 10, 3, 12, 11, 15, 7, 0, 0 },
                       { 14, 13, 1, 16, 15, 12, 2, 5, 0, 7, 8, 0, 9, 0, 11, 10 },
                       { 9, 1, 13, 0, 14, 11, 15, 4, 12, 16, 0, 0, 10, 6, 0, 0 },
                       { 0, 0, 4, 6, 0, 9, 0, 0, 13, 14, 0, 7, 12, 11, 0, 1 },
                       { 0, 11, 16, 12, 0, 0, 1, 0, 3, 15, 4, 6, 13, 14, 0, 7 },
                       { 0, 0, 2, 14, 0, 0, 0, 13, 9, 1, 11, 5, 8, 0, 4, 15 },
                       { 0, 7, 11, 0, 9, 2, 10, 8, 0, 0, 0, 16, 1, 12, 15, 0 },
                       { 4, 0, 15, 2, 13, 0, 11, 0, 1, 10, 6, 0, 0, 8, 16, 14 },
                       { 12, 0, 0, 1, 6, 14, 0, 15, 7, 11, 5, 2, 3, 13, 10, 9 },
                       { 10, 9, 14, 13, 7, 0, 16, 1, 4, 8, 3, 0, 0, 5, 6, 11 } };
    t.reset();
    if (SolveSudoku(grid3) == true)
        printGrid(grid3);
    else
        std::cout << "No solution exists\n";
    std::cout << "Time Taken: " << t.elapsed() << "\n";

    std::cout << "\nTest 4 - unsolvable-sparse: \n";
    int grid4[M][M] = { { 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 } };
    t.reset();
    if (SolveSudoku(grid4) == true)
        printGrid(grid4);
    else
        std::cout << "No solution exists\n";
    std::cout << "Time Taken: " << t.elapsed() << "\n";
}

// Driver Code
int main()
{
    // checkCorrect9();
    checkCorrect16();
    return 0;
}
