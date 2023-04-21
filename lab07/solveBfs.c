// BFS maze solver

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cell.h"
#include "matrix.h"
#include "Maze.h"
#include "Queue.h"

#define MAX_NEIGHBOURS 4 
static void fillPath(Maze m, Cell start, Cell exit, Cell **pred); 
static bool validCell(Maze m, Cell c); 

bool solve(Maze m) {

    // TODO: Complete this function
    //       Feel free to add helper functions

    int height = MazeHeight(m);
    int width = MazeWidth (m);

    bool **current = createBoolMatrix (height, width);
    Cell **prev = createCellMatrix(height, width); 
    
    Queue q = QueueNew();

    Cell begin = MazeGetStart(m);
    QueueEnqueue(q, begin);

    bool exists = false;
    while (!exists && !QueueIsEmpty(q)) {
        Cell n = QueueDequeue(q);
        if (current[n.row][n.col]) {
            continue;
        }
        current[n.row][n.col] = true; 
        if (MazeVisit(m, n)) {
            fillPath(m, begin, n, prev);
            exists = true;
            break;
        }
        Cell table[MAX_NEIGHBOURS] = {
             { .row = n.row - 1, .col = n.col}, // moving up action
             { .row = n.row, .col = n.col + 1 }, // move to the right
             { .row = n.row + 1, .col = n.col}, // moding down action
             { .row = n.row, .col = n.col - 1 }, // move to the left
        };
        for (int i = 0; i<MAX_NEIGHBOURS; i++) {
            Cell a = table[i];
            if (validCell(m, a) && !MazeIsWall(m, a) && !current[a.row][a.col]) {
                QueueEnqueue(q, a);
                prev[a.row][a.col] = n;
            }
        }
    }

     QueueFree(q);

     freeBoolMatrix(current);
     freeCellMatrix(prev);
     return exists;
    
}

static void fillPath(Maze m, Cell begin, Cell exit, Cell **prev) {    
    Cell curr = exit;
    while (!(curr.col == begin.col && curr.row == begin.row)) {
        MazeMarkPath(m, curr);
        curr = prev[curr.row][curr.col];
    }    
    MazeMarkPath(m, begin);
}
 // use same naming scheme to keep track
static bool validCell(Maze m, Cell c) {     
    return (
        c.row >= 0 && c.row < MazeHeight(m) &&
        c.col >= 0 && c.col < MazeWidth(m)
    );
}