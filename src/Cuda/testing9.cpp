#include <iostream>
#include "timing.h"
#include "cuda-parallel.cu"
using namespace std;

#define N 9

/* Prints the grid */
void printGrid(int grid[N][N])
{
    for (int row = 0; row < N; row++) 
    {
        for (int col = 0; col < N; col++)
            std::cout << grid[row][col] << " ";
        std::cout << endl;
    }
}

/* Prints the grid */
void printGrid2(int grid[M][M])
{
    for (int row = 0; row < M; row++) 
    {
        for (int col = 0; col < M; col++)
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
        std::cout << "No solution exists";

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

int main()
{
    checkCorrect9();
    return 0;
}
