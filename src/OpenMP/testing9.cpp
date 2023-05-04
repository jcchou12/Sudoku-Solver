#include <iostream>
#include "timing.h"
#include "openmp-parallel.cpp"
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


void checkCorrect9()
{
    // std::cout << "9x9 Tests: \n";
    // std::cout << "Test 1 - solvable-random: \n";
    // int grid1[N][N] = { { 1, 2, 3, 4, 5, 6, 7, 8, 9 },
    //                    { 4, 5, 6, 7, 8, 9, 1, 2, 3 },
    //                    { 7, 8, 9, 1, 2, 3, 4, 5, 6 },
    //                    { 2, 3, 4, 5, 6, 7, 8, 9, 1 },
    //                    { 5, 6, 7, 8, 9, 1, 2, 3, 4 },
    //                    { 8, 9, 1, 2, 3, 4, 5, 6, 7 },
    //                    { 3, 4, 5, 6, 7, 8, 9, 1, 2 },
    //                    { 6, 7, 8, 9, 1, 2, 3, 4, 5 },
    //                    { 9, 1, 2, 3, 4, 5, 6, 7, 0 } };
    // Timer t;
    // Board b1;
    // b1.copyBoard(grid1);
    // t.reset();
    // if (SolveSudoku(b1) == true)
    //     printGrid(b1.board);
    // else
    //     std::cout << "No solution exists\n";
    // std::cout << "Time Taken: " << t.elapsed() << "\n";

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
    Board b1;
    b1.copyBoard(grid1);
    t.reset();
    if (SolveSudoku(b1) == true)
        printGrid(b1.board);
    else
        std::cout << "No solution exists\n";
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
    Board b2;
    b2.copyBoard(grid2);
    t.reset();
    if (SolveSudoku(b2) == true)
        printGrid(b2.board);
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
    Board b3;
    b3.copyBoard(grid3);
    t.reset();
    if (SolveSudoku(b3) == true)
        printGrid(b3.board);
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
    Board b4;
    b4.copyBoard(grid4);
    if (SolveSudoku(b4) == true)
        printGrid(b4.board);
    else
        std::cout << "No solution exists \n";
    std::cout << "Time Taken: " << t.elapsed() << "\n";

    std::cout << "\nTest 5 - solvable-random-one: \n";
    int grid5[N][N] = { { 0, 8, 0, 0, 0, 5, 0, 4, 0 },
                       { 0, 4, 0, 0, 9, 0, 3, 7, 0 },
                       { 9, 3, 0, 0, 8, 4, 0, 6, 0 },
                       { 6, 1, 3, 0, 0, 0, 4, 0, 2 },
                       { 4, 5, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 2, 0, 0, 6, 0, 0, 1, 0 },
                       { 0, 9, 0, 0, 4, 0, 7, 0, 0 },
                       { 0, 0, 2, 5, 0, 6, 0, 0, 4 },
                       { 0, 6, 4, 3, 0, 0, 0, 0, 0} };
    t.reset();
    Board b5;
    b5.copyBoard(grid5);
    if (SolveSudoku(b5) == true)
        printGrid(b5.board);
    else
        std::cout << "No solution exists\n";

    std::cout << "Time Taken: " << t.elapsed() << "\n";

    std::cout << "Test 6 - solvable-sparse-one: \n";
    int grid6[N][N] = { { 0, 0, 0, 3, 2, 0, 0, 6, 0 },
                       { 0, 4, 5, 0, 0, 9, 0, 1, 0 },
                       { 7, 0, 0, 0, 1, 0, 0, 0, 0 },
                       { 1, 0, 4, 0, 9, 0, 3, 0, 0 },
                       { 0, 0, 0, 0, 6, 0, 0, 8, 5 },
                       { 0, 0, 3, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 2, 0, 0, 0, 9, 0, 0 },
                       { 5, 0, 0, 0, 0, 0, 0, 0, 6 },
                       { 0, 0, 0, 0, 0, 8, 0, 7, 0 } };
    t.reset();
    Board b6;
    b6.copyBoard(grid6);
    if (SolveSudoku(b6) == true)
        printGrid(b6.board);
    else
        std::cout << "No solution exists";
    std::cout << "Time Taken: " << t.elapsed() << "\n";

}

int main()
{
    checkCorrect9();
    return 0;
}
