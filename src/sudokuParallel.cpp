
// A Backtracking program in
// C++ to solve Sudoku problem
#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <unordered_set>
#include "timing.h"
#include "tests.cpp"
#include "pattern-parallel.cpp"
using namespace std;

// unassIGNED is used for empty
// cells in sudoku grid
#define UNASSIGNED 0

#define cellMin 1
#define cellMax 9

// N is used for the size of Sudoku grid.
// Size will be NxN
#define N 9
#define b 3
// #define N M

/* A utility function to print grid */
void printGrid(int grid[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++)
            printf(" %d ", grid[row][col]);
        printf("\n");
    }
}

bool inRow(int grid[N][N], int i, int c) {
    bool in = false;
    #pragma omp parallel for reduction(||:in)
    for (int j = 0; j < N; j++) {
        if (grid[i][j] == c) in = true;
    }
    return in;
}

bool inCol(int grid[N][N], int i, int c) {
    bool in = false;
    #pragma omp parallel for reduction(||:in)
    for (int j = 0; j < N; j++) {
        if (grid[j][i] == c) in = true;
    }
    return in;
}

bool inBox(int grid[N][N], int box, int c) {
    int i0 = box/N;
    int i1 = i0+b;
    int j0 = box%N;
    int j1 = j0+b;
    bool in = false;
    #pragma omp parallel for reduction(||:in)
    for (int i = i0; i < i1; i++) {
        #pragma omp parallel for reduction(||:in)
        for (int j = j0; j < j1; j++) {
            if (grid[i][j] == c) in = true;
        }
    }
    return in;
}

bool isCandidate(int grid[N][N], int i, int j, int c) {
    if (inRow(grid, i, c)) return false;
    if (inCol(grid, j, c)) return false;
    int box = boxFromCell(i, j);
    if (inBox(grid, box, c)) return false;
    return true;
}

bool validRow(int grid[N][N], int i) {
    int counts[N] {0};
    for (int j = 0; j < N; j++) counts[grid[i][j]-1]++;
    for (int j = 0; j < N; j++) {
        if (counts[grid[i][j]-1] != 1) return false;
    }
    return true;
}

bool validCol(int grid[N][N], int i) {
    int counts[N] {0};
    for (int j = 0; j < N; j++) counts[grid[j][i]-1]++;
    for (int j = 0; j < N; j++) {
        if (counts[grid[j][i]-1] != 1) return false;
    }
    return true;
}

bool validBox(int grid[N][N], int box) {
    int counts[N] {0};
    int i0 = box/N;
    int i1 = i0+b;
    int j0 = box%N;
    int j1 = j0+b;
    for (int i = i0; i < i1; i++) {
        for (int j = j0; j < j1; j++) {
            counts[grid[i][j]-1]++;
        }
    }
    for (int i = i0; i < i1; i++) {
        for (int j = j0; j < j1; j++) {
            if (counts[grid[i][j]-1] != 1) return false;
        }
    }
    return true;
}

bool validSolution(int grid[N][N]) {
    for (int i = 0; i < N; i++) {
        if (!validRow(grid, i)) return false;
    }
    for (int i = 0; i < N; i++) {
        if (!validCol(grid, i)) return false;
    }
    for (int i = 0; i < N; i+=3) {
        for (int j = 0; j < N; j+=3) {
            if (!validBox(grid, i*N+j)) return false;
        }
    }
    return true;
}

void initCandidateGrid(int grid[N][N],
    unordered_set<int> candidates[N][N]) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++){
        #pragma omp parallel for
        for (int j = 0; j < N; j++)
            candidates[i][j] = unordered_set<int>();
    }
}

void initCandidates(int grid[N][N],
    unordered_set<int> candidates[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] != UNASSIGNED) continue;
            for (int c = cellMin; c <= cellMax; c++)
                if (isCandidate(grid, i, j, c)) candidates[i][j].insert(c);
        }
    }
}


float CandidateGrid(int grid[N][N]) {
    unordered_set<int> candidates[N][N];
    unordered_set<int> twins[b];
    bool solved = false;
    Timer t;
    t.reset();
    initCandidateGrid(grid, candidates);
    initCandidates(grid, candidates);
    while (!solved) {
        solved = simpleSolve(grid, candidates, twins);
        if (!solved) solved = patternFive(grid, candidates, twins);
    }
    float time = t.elapsed();
    if (validSolution(grid)) {
        printf("SOLUTION:\n");
        printGrid(grid);
    }
    return time;
}

int main() {
    printf("Testing Easy Boards...\n");
    for (int i = 0; i < E; i++) {
        float time = CandidateGrid(easys[i]);
        printf("Easy board %d solved in %f.\n", i, time);
    }
    printf("...Done!\n");

    printf("Testing Medium Boards...\n");
    for (int i = 0; i < M; i++) {
        float time = CandidateGrid(medis[i]);
        printf("Medium board %d solved in %f.\n", i, time);
    }
    printf("...Done!\n");

    printf("Testing Hard Boards...\n");
    for (int i = 0; i < H; i++) {
        float time = CandidateGrid(hards[i]);
        printf("Hard board %d solved in %f.\n", i, time);
    }
    printf("...Done!\n");

    printf("Testing Expert Boards...\n");
    for (int i = 0; i < X; i++) {
        float time = CandidateGrid(xprts[i]);
        printf("Expert board %d solved in %f.\n", i, time);
    }
    printf("...Done!\n");

    return 0;
}
