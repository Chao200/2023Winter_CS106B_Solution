#include <iostream>
#include "TowersOfHanoi.h"
using namespace std;

/* Constant: kPauseTime
 *
 * Amount of time to pause for when beginning the Towers Of Hanoi animation.
 */
const double kPauseTime = 500;

/* (This function is adapted from the one in Chapter 8.1 of the textbook. If you
 * haven't yet read that chapter, I recommend that you do so before starting
 * this problem.)
 *
 * Solves the Towers of Hanoi for numDisks disks, assuming they start on spindle
 * start, end on spindle finish, and that the spindle temp is for temporary
 * storage. Then returns the number of disks that needed to be moved.
 */
int moveTower(int numDisks, char start, char finish, char temp) {
    if (numDisks == 0) {
        return 0;
    } else {
        int movesOne = moveTower(numDisks - 1, start, temp, finish);
        moveSingleDisk(start, finish);
        int movesTwo = moveTower(numDisks - 1, temp, finish, start);

        return 1 + movesOne + movesTwo;
    }
}

int solveTowersOfHanoi(int numDisks, char start, char finish, char temp) {
    /* Want to slow this down? Try these other options for the final parameter:
     *
     *   AnimationSpeed::MOLASSES         (extremely slow)
     *   AnimationSpeed::CHEETAH          (regular)
     *   AnimationSpeed::PEREGRINE_FALCON (extremely fast)
     */
    initHanoiDisplay(numDisks, AnimationSpeed::CHEETAH);
    pause(kPauseTime);

    int totalMoves = moveTower(numDisks, start, finish, temp);
    return totalMoves;
}
