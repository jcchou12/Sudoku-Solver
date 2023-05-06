#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include "timing.h"
using namespace std;

// UNASSIGNED is used for empty
// cells in sudoku grid
#define UNASSIGNED 0

#define cellMin 1
#define cellMax 9

// N is used for the size of Sudoku grid.
// Size will be NxN
#define N 9
#define b 3

void gridCopy(int grid[N][N], int copy[N][N]) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        #pragma omp parallel for
        for (int j = 0; j < N; j++) {
            copy[i][j] = grid[i][j];
        }
    }
}

void candCopy(unordered_set<int> candidates[N][N],
              unordered_set<int> copy[N][N]) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        #pragma omp parallel for
        for (int j = 0; j < N; j++) {
            copy[i][j] = candidates[i][j];
        }
    }
}

int boxFromCell(int i, int j) {
    int boxi = i/b;
    int boxj = j/b;
    return boxi*N*b + boxj*b;
}

void updateRow(int grid[N][N],
    unordered_set<int> candidates[N][N],
    int i, int c) {
    #pragma omp parallel for
    for (int j = 0; j < N; j++) {
        if (grid[i][j] != UNASSIGNED) continue;
        candidates[i][j].erase(c);
    }
}

void updateCol(int grid[N][N],
    unordered_set<int> candidates[N][N],
    int i, int c) {
    #pragma omp parallel for
    for (int j = 0; j < N; j++) {
        if (grid[j][i] != UNASSIGNED) continue;
        candidates[j][i].erase(c);
    }
}

void updateBox(int grid[N][N],
    unordered_set<int> candidates[N][N],
    int box, int c) {
    int i0 = box/N;
    int i1 = i0+b;
    int j0 = box%N;
    int j1 = j0+b;
    #pragma omp parallel for
    for (int i = i0; i < i1; i++) {
        #pragma omp parallel for
        for (int j = j0; j < j1; j++) {
            if (grid[i][j] != UNASSIGNED) continue;
            candidates[i][j].erase(c);
        }
    }
}

void updateGrid(int grid[N][N],
    unordered_set<int> candidates[N][N],
    int i, int j, int c) {
    grid[i][j] = c;
    int box = boxFromCell(i,j);
    updateRow(grid, candidates, i, c);
    updateCol(grid, candidates, j, c);
    updateBox(grid, candidates, box, c);
}

bool patternOne(int grid[N][N],
    unordered_set<int> candidates[N][N]) {
    bool progress = false;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] != UNASSIGNED) continue;
            if (candidates[i][j].size() == 1) {
                progress = true;
                int c = *(candidates[i][j].begin());
                updateGrid(grid, candidates, i, j, c);
            }
        }
    }
    return progress;
}

bool patternTwoRowI(int grid[N][N],
    unordered_set<int> candidates[N][N],
    int i) {
    bool progress = false;
    int counts[N] {0};
    int cols[N] {0};
    #pragma omp parallel for
    for (int j = 0; j < N; j++) {
        if (grid[i][j] != UNASSIGNED) continue;
        unordered_set<int>::iterator iter;
        for (iter = candidates[i][j].begin(); iter != candidates[i][j].end(); iter++) {
            int c = (*iter)-1;
            #pragma omp atomic
            counts[c]++;
            #pragma omp atomic write
            cols[c] = j;
        }
    }
    for (int c = 0; c < N; c++) {
        if (counts[c] == 1) {
            progress = true;
            int col = cols[c];
            int cell = i*N+col;
            updateGrid(grid, candidates, i, col, c+1);
        }
    }
    return progress;
}

bool patternTwoColI(int grid[N][N],
    
    unordered_set<int> candidates[N][N],
    int i) {
    bool progress = false;
    int counts[N] {0};
    int rows[N] {0};
    #pragma omp parallel for 
    for (int j = 0; j < N; j++) {
        if (grid[j][i] != UNASSIGNED) continue;
        unordered_set<int>::iterator iter;
        for (iter = candidates[j][i].begin(); iter != candidates[j][i].end(); iter++) {
            int c = (*iter)-1;
            #pragma omp atomic
            counts[c]++;
            #pragma omp atomic write
            rows[c] = j;
        }
    }
    for (int c = 0; c < N; c++) {
        if (counts[c] == 1) {
            progress = true;
            int row = rows[c];
            int cell = rows[c]*N+i;
            updateGrid(grid, candidates, row, i, c+1);
        }
    }
    return progress;
}

bool patternTwoBoxI(int grid[N][N],
    
    unordered_set<int> candidates[N][N],
    int box) {
    bool progress = false;
    int counts[N] {0};
    int cells[N] {0};
    int i0 = box/N;
    int i1 = i0+b;
    int j0 = box%N;
    int j1 = j0+b;
    for (int i = i0; i < i1; i++) {
        #pragma omp parallel for
        for (int j = j0; j < j1; j++) {
            if (grid[i][j] != UNASSIGNED) continue;
            unordered_set<int>::iterator iter;
            for (iter = candidates[i][j].begin(); iter != candidates[i][j].end(); iter++) {
                int c = (*iter)-1;
                #pragma omp atomic
                counts[c]++;
                #pragma omp atomic write
                cells[c] = i*N + j;
            }
        }
    }
    for (int c = 0; c < N; c++) {
        if (counts[c] == 1) {
            progress = true;
            int cell = cells[c];
            int i = cells[c]/N;
            int j = cells[c]%N;
            updateGrid(grid, candidates, i, j, c+1);
        }
    }
    return progress;
}

bool patternTwoRow(int grid[N][N],
    
    unordered_set<int> candidates[N][N]) {
    bool progress = false;
    #pragma omp parallel for reduction (||:progress)
    for (int i = 0; i < N; i++) {
        progress |= patternTwoRowI(grid, candidates, i);
    }
    return progress;
}

bool patternTwoCol(int grid[N][N],
    
    unordered_set<int> candidates[N][N]) {
    bool progress = false;
    #pragma omp parallel for reduction (||:progress)
    for (int i = 0; i < N; i++) {
        progress |= patternTwoColI(grid, candidates, i);
    }
    return progress;
}

bool patternTwoBox(int grid[N][N],
    
    unordered_set<int> candidates[N][N]) {
    bool progress = false;
    #pragma omp parallel for reduction (||:progress)
    for (int i = 0; i < N; i+=3) {
        #pragma omp parallel for reduction (||:progress)
        for (int j = 0; j < N; j+=3) {
            progress = patternTwoBoxI(grid, candidates, i*N+j);
        }
    }
    return progress;
}

bool patternTwo(int grid[N][N],
    
    unordered_set<int> candidates[N][N]) {
    bool r, c, x;
    r = patternTwoRow(grid, candidates);
    c = patternTwoCol(grid, candidates);
    x = patternTwoBox(grid, candidates);
    return r || c || x;
}

bool patternThreeRowI(int grid[N][N],
    unordered_set<int> candidates[N][N],
    unordered_set<int> &twins,
    int i) {
    bool progress = false;
    unordered_map<int,int> pairs;
    vector<pair<int,int>> new_twins;
    unordered_set<int>::iterator iter;
    for (int j = 0; j < N; j++) {
        if (grid[i][j] != UNASSIGNED) continue;
        if (candidates[i][j].size() == 2) {
            int cell = i*N+j;
            if (twins.find(cell) != twins.end()) continue;
            iter = candidates[i][j].begin();
            int c0 = (*iter)-1;
            iter++;
            int c1 = (*iter)-1;
            int key = c0*N+c1;
            if (pairs.find(key) == pairs.end()) pairs[key] = j;
            else new_twins.push_back(pair(key, pairs[key]*N+j));
        }
    }
    for (auto &twin : new_twins) {
        progress = true;
        int vals = twin.first;
        int clms = twin.second;
        int col0 = clms/N;
        int col1 = clms%N;
        int val0 = vals/N;
        int val1 = vals%N;
        //printf("twin found! cells: (%d, %d), (%d, %d). vals: (%d, %d).\n", i, col0, i, col1, val0, val1);
        #pragma omp parallel for
        for (int j = 0; j < N; j++) {
            if (j == col0 || j == col1) continue;
            candidates[i][j].erase(val0+1);
            candidates[i][j].erase(val1+1);
            //if (i == 1 && j ==2 && c == 4) printf("p3R WRONG\n");
        }
        int cell0 = i*N + col0;
        int cell1 = i*N + col1;
        twins.insert(cell0);
        twins.insert(cell1);
    }
    return progress;
}

bool patternThreeColI(int grid[N][N],
    
    unordered_set<int> candidates[N][N],
    unordered_set<int> &twins,
    int i) {
    bool progress = false;
    unordered_map<int,int> pairs;
    vector<pair<int,int>> new_twins;
    unordered_set<int>::iterator iter;
    for (int j = 0; j < N; j++) {
        if (grid[j][i] != UNASSIGNED) continue;
        if (candidates[j][i].size() == 2) {
            int cell = j*N+i;
            if (twins.find(cell) != twins.end()) continue;
            iter = candidates[j][i].begin();
            int c0 = (*iter)-1;
            iter++;
            int c1 = (*iter)-1;
            int key = c0*N+c1;
            if (pairs.find(key) == pairs.end()) pairs[key] = j;
            else new_twins.push_back(pair(key, pairs[key]*N+j));
        }
    }
    for (auto &twin : new_twins) {
        progress = true;
        int vals = twin.first;
        int rows = twin.second;
        int row0 = rows/N;
        int row1 = rows%N;
        int val0 = vals/N;
        int val1 = vals%N;
        //printf("twin found! cells: (%d, %d), (%d, %d). vals: (%d, %d).\n", row0, i, row1, i, val0, val1);
        #pragma omp parallel for
        for (int j = 0; j < N; j++) {
            if (j == row0 || j == row1) continue;
            candidates[j][i].erase(val0+1);
            candidates[j][i].erase(val1+1);
        }
        int cell0 = row0*N+i;
        int cell1 = row1*N+i;
        twins.insert(cell0);
        twins.insert(cell1);
    }
    return progress;
}

bool patternThreeBoxI(int grid[N][N],
    
    unordered_set<int> candidates[N][N],
    unordered_set<int> &twins,
    int box) {
    bool progress = false;
    unordered_map<int,int> pairs;
    vector<pair<int,int>> new_twins;
    int i0 = box/N;
    int i1 = i0+b;
    int j0 = box%N;
    int j1 = j0+b;
    unordered_set<int>::iterator iter;
    for (int i = i0; i < i1; i++) {
        for (int j = j0; j < j1; j++) {
            if (grid[i][j] != UNASSIGNED) continue;
            if (candidates[i][j].size() == 2) {
                int cell = (i-i0)*b+j-j0;
                if (twins.find(i*N+j) != twins.end()) continue;
                iter = candidates[i][j].begin();
                int c0 = (*iter)-1;
                iter++;
                int c1 = (*iter)-1;
                int key = c0*N+c1;
                if (pairs.find(key) == pairs.end()) pairs[key] = cell;
                else new_twins.push_back(pair(key, pairs[key]*N+cell));
            }
        }
    }
    for (auto &twin : new_twins) {
        progress = true;
        int vals = twin.first;
        int clls = twin.second;
        int cell0 = clls/N;
        int cell1 = clls%N;
        int row0 = cell0/b+i0;
        int col0 = cell0%b+j0;
        int row1 = cell1/b+i0;
        int col1 = cell1%b+j0;
        cell0 = row0*N+col0;
        cell1 = row1*N+col1;
        int val0 = vals/N;
        int val1 = vals%N;
        #pragma omp parallel for
        for (int i = i0; i < i1; i++) {
            #pragma omp parallel for
            for (int j = j0; j < j1; j++) {
                int cell = i*N+j;
                if (cell == cell0 || cell == cell1) continue;
                candidates[i][j].erase(val0+1);
                candidates[i][j].erase(val1+1);
            }
        }
        twins.insert(cell0);
        twins.insert(cell1);
    }
    return progress;
}

bool patternThreeRow(int grid[N][N],
    
    unordered_set<int> candidates[N][N],
    unordered_set<int> &twins) {
    bool progress = false;
    #pragma omp parallel for reduction (||:progress)
    for (int i = 0; i < N; i++) {
        progress |= patternThreeRowI(grid, candidates, twins, i);
    }
    return progress;
}

bool patternThreeCol(int grid[N][N],
    
    unordered_set<int> candidates[N][N],
    unordered_set<int> &twins) {
    bool progress = false;
    #pragma omp parallel for reduction (||:progress)
    for (int i = 0; i < N; i++) {
        progress |= patternThreeColI(grid, candidates, twins, i);
    }
    return progress;
}

bool patternThreeBox(int grid[N][N],
    
    unordered_set<int> candidates[N][N],
    unordered_set<int> &twins) {
    bool progress = false;
    #pragma omp parallel for reduction (||:progress)
    for (int i = 0; i < N; i+=3) {
        #pragma omp parallel for reduction (||:progress)
        for (int j = 0; j < N; j+=3) {
            progress |= patternThreeBoxI(grid, candidates, twins, i*N+j);
        }
    }
    return progress;
}

bool patternThree(int grid[N][N],
    
    unordered_set<int> candidates[N][N],
    unordered_set<int> twins[b]) {
    bool r = patternThreeRow(grid, candidates, twins[0]);
    bool c = patternThreeCol(grid, candidates, twins[1]);
    bool x = patternThreeBox(grid, candidates, twins[2]);
    return x || c || r;
}

bool patternFourRowI(int grid[N][N],
    
    unordered_set<int> candidates[N][N],
    unordered_set<int> &twins,
    int i) {
    bool progress = false;
    int counts[N] {0};
    int cols[N][2] {0};
    unordered_set<int>::iterator iter;
    for (int j = 0; j < N; j++) {
        if (grid[i][j] != UNASSIGNED) continue;
        for (iter = candidates[i][j].begin(); iter != candidates[i][j].end(); iter++) {
            int c = (*iter)-1;
            counts[c]++;
            if (counts[c]==1) cols[c][0] = j;
            else cols[c][1] = j;
        }
    }
    unordered_map<int,int> pairs;
    vector<pair<int,int>> new_twins;
    for (int c = 0; c < N; c++) {
        if (counts[c] == 2) {
            int col0 = min(cols[c][0], cols[c][1]);
            int col1 = max(cols[c][0], cols[c][1]);
            int key = col0*N+col1;
            int cell0 = i*N + col0;
            int cell1 = i*N + col1;
            if (twins.find(cell0) != twins.end() && twins.find(cell1) != twins.end()) continue;
            if (pairs.find(key) == pairs.end()) pairs[key] = c;
            else new_twins.push_back(pair(key, pairs[key]*N+c));
        }
    }
    for (auto &twin : new_twins) {
        progress = true;
        int clms = twin.first;
        int vals = twin.second;
        int col0 = clms/N;
        int col1 = clms%N;
        int val0 = vals/N;
        int val1 = vals%N;
        for (int c = 0; c < N; c++) {
            if (c == val0 || c == val1) continue;
            candidates[i][col0].erase(c+1);
            candidates[i][col1].erase(c+1);
        }
        int cell0 = i*N + col0;
        int cell1 = i*N + col1;
        twins.insert(cell0);
        twins.insert(cell1);
    }
    return progress;
}

bool patternFourColI(int grid[N][N],
    
    unordered_set<int> candidates[N][N],
    unordered_set<int> &twins,
    int i) {
    bool progress = false;
    int counts[N] {0};
    int rows[N][2] {0};
    unordered_set<int>::iterator iter;
    for (int j = 0; j < N; j++) {
        if (grid[j][i] != UNASSIGNED) continue;
        for (iter = candidates[j][i].begin(); iter != candidates[j][i].end(); iter++) {
            int c = (*iter)-1;
            if (!counts[c]) {
                counts[c] = 1;
                rows[c][0] = j;
            } else {
                counts[c]++;
                rows[c][1] = j;
            }
        }
    }
    unordered_map<int,int> pairs;
    vector<pair<int,int>> new_twins;
    for (int c = 0; c < N; c++) {
        if (counts[c] == 2) {
            int row0 = min(rows[c][0], rows[c][1]);
            int row1 = max(rows[c][0], rows[c][1]);
            int cell0 = row0*N+i;
            int cell1 = row1*N+i;
            if (twins.find(cell0) != twins.end() && twins.find(cell1) != twins.end()) continue;
            int key = row0*N+row1;
            if (pairs.find(key) == pairs.end()) pairs[key] = c;
            else new_twins.push_back(pair(key, pairs[key]*N+c));
        }
    }
    for (auto &twin : new_twins) {
        progress = true;
        int rws = twin.first;
        int vals = twin.second;
        int row0 = rws/N;
        int row1 = rws%N;
        int val0 = vals/N;
        int val1 = vals%N;
        for (int c = 0; c < N; c++) {
            if (c == val0 || c == val1) continue;
            candidates[row0][i].erase(c+1);
            candidates[row1][i].erase(c+1);
        }
        int cell0 = row0*N+i;
        int cell1 = row1*N+i;
        twins.insert(cell0);
        twins.insert(cell1);
    }
    return progress;
}

bool patternFourBoxI(int grid[N][N],
    
    unordered_set<int> candidates[N][N],
    unordered_set<int> &twins,
    int box) {
    bool progress = false;
    int counts[N] {0};
    int cells[N][2] {0};
    int i0 = box/N;
    int i1 = i0+b;
    int j0 = box%N;
    int j1 = j0+b;
    for (int i = i0; i < i1; i++) {
        for (int j = j0; j < j1; j++) {
            if (grid[i][j] != UNASSIGNED) continue;
            unordered_set<int>::iterator iter;
            for (iter = candidates[i][j].begin(); iter != candidates[i][j].end(); iter++) {
                int c = (*iter)-1;
                counts[c]++;
                if (counts[c] == 1) cells[c][0] = i*N+j;
                else cells[c][1] = i*N+j;
            }
        }
    }
    unordered_map<int,int> pairs;
    vector<pair<int,int>> new_twins;
    for (int c = 0; c < N; c++) {
        if (counts[c] == 2) {
            int cell0 = min(cells[c][0], cells[c][1]);
            int cell1 = max(cells[c][0], cells[c][1]);
            if (twins.find(cell0) != twins.end() && twins.find(cell1) != twins.end()) continue;
            int row0 = cell0/N;
            int col0 = cell0%N;
            int row1 = cell1/N;
            int col1 = cell1%N;
            cell0 = (row0-i0)*b+col0-j0;
            cell1 = (row1-i0)*b+col1-j0;
            int key = cell0*N+cell1;
            if (pairs.find(key) == pairs.end()) pairs[key] = c;
            else new_twins.push_back(pair(key, pairs[key]*N+c));
        }
    }
    for (auto &twin : new_twins) {
        progress = true;
        int clls = twin.first;
        int vals = twin.second;
        int cell0 = clls/N;
        int cell1 = clls%N;
        int row0 = cell0/b+i0;
        int col0 = cell0%b+j0;
        int row1 = cell1/b+i0;
        int col1 = cell1%b+j0;
        cell0 = row0*N+col0;
        cell1 = row1*N+col1;
        int val0 = vals/N;
        int val1 = vals%N;
        for (int c = 0; c < N; c++) {
            if (c == val0 || c == val1) continue;
            candidates[row0][col0].erase(c+1);
            candidates[row1][col1].erase(c+1);
        }
        twins.insert(cell0);
        twins.insert(cell1);
    }
    return progress;
}

bool patternFourRow(int grid[N][N],
    
    unordered_set<int> candidates[N][N],
    unordered_set<int> &twins) {
    bool progress = false;
    #pragma omp parallel for reduction (||:progress)
    for (int i = 0; i < N; i++) {
        progress |= patternFourRowI(grid, candidates, twins, i);
    }
    return progress;
}

bool patternFourCol(int grid[N][N],
    
    unordered_set<int> candidates[N][N],
    unordered_set<int> &twins) {
    bool progress = false;
    #pragma omp parallel for reduction (||:progress)
    for (int i = 0; i < N; i++) {
        progress |= patternFourColI(grid, candidates, twins, i);
    }
    return progress;
}

bool patternFourBox(int grid[N][N],
    unordered_set<int> candidates[N][N],
    unordered_set<int> &twins) {
    bool progress = false;
    #pragma omp parallel for reduction (||:progress)
    for (int i = 0; i < N; i+=3) {
        #pragma omp parallel for reduction (||:progress)
        for (int j = 0; j < N; j+=3) {
            progress |= patternFourBoxI(grid, candidates, twins, i*N+j);
        }
    }
    return progress;
}

bool patternFour(int grid[N][N],
    unordered_set<int> candidates[N][N],
    unordered_set<int> twins[b]) {
    bool r, c, x;
    r = patternFourRow(grid, candidates, twins[0]);
    c = patternFourCol(grid, candidates, twins[1]);
    x = patternFourBox(grid, candidates, twins[2]);
    return x || c || r;
}

bool checkSolved(int grid[N][N]) {
    bool solved = true;
    #pragma omp parallel for reduction (&&: solved)
    for (int i = 0; i < N; i++) {
        #pragma omp parallel for reduction (&&: solved)
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == UNASSIGNED) solved = false;
        }
    }
    return solved;
}

bool simpleSolve(int grid[N][N],
    unordered_set<int> candidates[N][N],
    unordered_set<int> twins[b]) {
    int phases = 0;
    bool progress = true;
    bool solved = false;
    while (progress && !solved) {
        progress = false;
        bool p1, p2, p3, p4;
        phases++;
        #pragma omp task
        p1 = patternOne(grid, candidates);
        #pragma omp task
        p2 = patternTwo(grid, candidates);
        #pragma omp task
        p3 = patternThree(grid, candidates, twins);
        #pragma omp task
        p4 = patternFour(grid, candidates, twins);
        #pragma omp taskwait
        solved = checkSolved(grid);
        progress = p1 || p2 || p3 || p4;
    }
    //printf("Solved in %d phases.\n", phases);
    return solved;
}

bool patternFiveCellVal(int grid[N][N],
    
    unordered_set<int> candidates[N][N],
    unordered_set<int> twins[b],
    int i, int j, int c) {
    updateGrid(grid, candidates, i, j, c);
    return simpleSolve(grid, candidates, twins);
}


bool patternFiveCell(int grid[N][N],
    
    unordered_set<int> candidates[N][N],
    unordered_set<int> twins[b],
    int i, int j) {
    unordered_set<int>::iterator iter0;
    iter0 = candidates[i][j].begin();
    int val0 = *iter0;
    iter0++;
    int val1 = *iter0;
    
    int grid0[N][N] {0};
    int grid1[N][N] {0};
    unordered_set<int> cand0[N][N];
    unordered_set<int> cand1[N][N];
    
    #pragma omp task
    gridCopy(grid, grid0);
    #pragma omp task
    gridCopy(grid, grid1);
    #pragma omp task
    candCopy(candidates, cand0);
    #pragma omp task
    candCopy(candidates, cand1);
    #pragma omp taskwait

    unordered_set<int> t0[b];
    t0[0] = twins[0];
    t0[1] = twins[1];
    t0[2] = twins[2];
    unordered_set<int> t1[b];
    t1[0] = twins[0];
    t1[1] = twins[1];
    t1[2] = twins[2];

    bool soln0, soln1;
    #pragma omp task 
    soln0 = patternFiveCellVal(grid0, cand0, t0, i, j, val0);
    #pragma omp task
    soln1 = patternFiveCellVal(grid1, cand1, t1, i, j, val1);
    #pragma omp taskwait

    if (soln0) {
        gridCopy(grid0, grid);
        candidates = cand0;
        return true;
    } else if (soln1) {
        gridCopy(grid1, grid);
        candidates = cand1;
        return true;
    }
    #pragma omp parallel for
    for (int row = 0; row < N; row++) {
        #pragma omp parallel for
        for (int col = 0; col < N; col++) {
            if (grid[row][col] != UNASSIGNED) continue;
            int val0 = grid0[row][col];
            int val1 = grid1[row][col];
            if (val0 != UNASSIGNED && val1 != UNASSIGNED) {
                if (val0 == val1) {
                    #pragma omp critical
                    {
                        updateGrid(grid, candidates, row, col, val0);
                    }
                } else {
                    for (int c = cellMin; c < cellMax; c++) {
                        if (c == val0 || c == val1) continue;
                        candidates[row][col].erase(c);
                    }
                }
            } else {
                for (int c = cellMin; c < cellMax; c++) {
                    if (cand0[row][col].find(c) == cand0[row][col].end()
                    && cand1[row][col].find(c) == cand1[row][col].end()) {
                        candidates[row][col].erase(c);
                    }
                }
            }
        }
    }
    return false;
}

bool patternFive(int grid[N][N],
    unordered_set<int> candidates[N][N],
    unordered_set<int> twins[b]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] != UNASSIGNED) continue;
            if (candidates[i][j].size() == 2) {
                if (patternFiveCell(grid, candidates, twins, i, j)) return true;
            }
        }
    }
    return false;
}

