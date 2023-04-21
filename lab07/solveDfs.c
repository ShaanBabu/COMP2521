// DFS maze solver

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cell.h"
#include "matrix.h"
#include "Maze.h"
#include "Stack.h"
#define MAX_NEIGHBOURS 4 

static void fillPath(Maze m, Cell start, Cell exit, Cell **prev); 
static bool validCell(Maze m, Cell c); 

bool solve(Maze m) {
    int height = MazeHeight(m);
    int width = MazeWidth (m);

    bool **current = createBoolMatrix (height, width);
    Cell **prev = createCellMatrix(height, width); 
    
    Stack q = StackNew();

    Cell start = MazeGetStart(m);
    StackPush(q, start);

    bool exists = false;
    while (!exists && !StackIsEmpty(q)) {
        Cell n = StackPop(q);
        if (current[n.row][n.col]) {
            continue;
        }
        current[n.row][n.col] = true; 
        if (MazeVisit(m, n)) {
            fillPath(m, start, n, prev);
            exists = true;
            break;
        }
        Cell table[MAX_NEIGHBOURS] = {
             { .row = n.row + 1, .col = n.col},
             { .row = n.row, .col = n.col + 1 },
             { .row = n.row, .col = n.col - 1 },
             { .row = n.row - 1, .col = n.col},
             
        };
        for (int i = 0; i<MAX_NEIGHBOURS; i++) {
            Cell a = table[i];
            if (validCell(m, a) && !MazeIsWall(m, a) && !current[a.row][a.col]) {
                StackPush(q, a);
                prev[a.row][a.col] = n;
            }
        }
    }

     StackFree(q);
     freeBoolMatrix(current);
     freeCellMatrix(prev);
     return exists;
    
}

static void fillPath(Maze m, Cell start, Cell exit, Cell **prev) {    
    Cell curr = exit;
    while (!(curr.col == start.col && curr.row == start.row)) {
        MazeMarkPath(m, curr);
        curr = prev[curr.row][curr.col];
    }    
    MazeMarkPath(m, start);
}
// use same naming scheme to keep track
static bool validCell(Maze m, Cell c) {     
    return (
        c.row >= 0 && c.row < MazeHeight(m) &&
        c.col >= 0 && c.col < MazeWidth(m)
    );
}

