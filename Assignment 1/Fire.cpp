#include "Fire.h"
using namespace std;

void updateFire(Grid<int>& fire) {
    /* TODO: The next line just exists to make sure you don't get compiler warning messages
     * when this function isn't implemented. Delete this comment and the next line, then
     * implement this function.
     */
//    (void) fire;
    int Rows = fire.numRows();   // 行数
    int Cols = fire.numCols();   // 列数
    for (int i = 1; i < Rows; i++) {  // 从第二行开始向下开始循环
        for (int j = 0; j < Cols; j++) {  // 从左到右对列循环
            int randomnum;
            // 如果有三个单元可以传播
            if (fire.inBounds(i-1, j) && fire.inBounds(i-1, j-1) && fire.inBounds(i-1, j+1)) {
                randomnum = randomInteger(-1, 1);
            } else if (fire.inBounds(i-1, j) && fire.inBounds(i-1, j-1)) {  // 上、上左两个
                randomnum = randomInteger(-1, 0);
            } else if (fire.inBounds(i-1, j) && fire.inBounds(i-1, j+1)) {  // 上、上右边两个
                randomnum = randomInteger(0, 1);
            } else {   // 上面一个
                randomnum = 0;
            }
            // 传播
            fire[i-1][j+randomnum] = fire[i][j];
            if (fire[i][j] != 0) {
                // 降温
                if (randomChance(2.0/3)){
                    fire[i-1][j+randomnum] -= 1;
                }
            }
        }
    }
}


/* * * * * * Provided Test Cases * * * * * */
#include "GUI/SimpleTest.h"

/*** Tests for startFire ***/

PROVIDED_TEST("updateFire does not change dimensions of world.") {
    const int numRows = 5;
    const int numCols = 7;

    /* Make sure the fire has the right size to begin with. */
    Grid<int> fire(numRows, numCols);
    EXPECT_EQUAL(fire.numRows(), numRows);
    EXPECT_EQUAL(fire.numCols(), numCols);

    /* Update the fire; make sure everything is still there. */
    updateFire(fire);
    EXPECT_EQUAL(fire.numRows(), numRows);
    EXPECT_EQUAL(fire.numCols(), numCols);
}


PROVIDED_TEST("updateFire does not change bottom row.") {
    Grid<int> fire = {
        { 3, 3, 3 },
        { 5, 5, 5 },
        { 7, 7, 7 }
    };

    /* Update the fire; the bottom row should be unchanged. */
    updateFire(fire);
    for (int col = 0; col < 3; col++) {
        EXPECT_EQUAL(fire[2][col], 7);
    }

}

namespace {
    /* Helper function to test if the specified value matches one of the items
     * from the given list.
     */
    bool isOneOf(int) {
        return false;
    }
    template <typename First, typename... Rest> bool isOneOf(int value, First option1, Rest... remaining) {
        return value == option1 || isOneOf(value, remaining...);
    }
}

PROVIDED_TEST("updateFire copies heat values upward.") {
    Grid<int> fire = {
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 4, 4, 4, 4, 4, 4, 4, 4 },
        { 9, 9, 9, 9, 9, 9, 9, 9 }
    };

    updateFire(fire);

    /* In the top row, every value should be either 0 (nothing copied
     * onto this point), 4 (copied from the row below), or 3 (copied,
     * then things cooled down).
     */
    for (int col = 0; col < fire.numCols(); col++) {
        EXPECT(isOneOf(fire[0][col], 0, 3, 4));
    }

    /* The top row should include at least one 3 or 4. If not, nothing was propagated
     * upward from the row below.
     */
    bool found = false;
    for (int col = 0; col < fire.numCols(); col++) {
        if (isOneOf(fire[0][col], 3, 4)) {
            found = true;
            break;
        }
    }
    if (!found) {
        SHOW_ERROR("Top row did not contain any 3 or 4 values.");
    }

    /* The middle row should be 4s, 8s, or 9s. */
    for (int col = 0; col < fire.numCols(); col++) {
        EXPECT(isOneOf(fire[1][col], 4, 8, 9));
    }

    /* The middle row should include at least one 8 or one 9. If it doesn't, nothing
     * was copied upward.
     */
    found = false;
    for (int col = 0; col < fire.numCols(); col++) {
        if (isOneOf(fire[1][col], 8, 9)) {
            found = true;
            break;
        }
    }
    if (!found) {
        SHOW_ERROR("Middle row did not contain any 8 or 9 values.");
    }

    /* The bottom row should be all 9s. */
    for (int col = 0; col < fire.numCols(); col++) {
        EXPECT_EQUAL(fire[2][col], 9);
    }
}

PROVIDED_TEST("updateFire copies zero values upward.") {
    Grid<int> fire = {
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 4, 4, 4, 4, 4, 4, 4, 4 },
        { 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    updateFire(fire);

    /* In the top row, every value should be either 0 (nothing copied
     * onto this point), 4 (copied from the row below), or 3 (copied,
     * then things cooled down).
     */
    for (int col = 0; col < fire.numCols(); col++) {
        EXPECT(isOneOf(fire[0][col], 0, 3, 4));
    }

    /* The top row should include at least one 3 or 4. If not, nothing was propagated
     * upward from the row below.
     */
    bool found = false;
    for (int col = 0; col < fire.numCols(); col++) {
        if (isOneOf(fire[0][col], 3, 4)) {
            found = true;
            break;
        }
    }
    if (!found) {
        SHOW_ERROR("Top row did not contain any 3 or 4 values.");
    }

    /* The middle row should be 4s or 0s. */
    for (int col = 0; col < fire.numCols(); col++) {
        EXPECT(isOneOf(fire[1][col], 4, 0));
    }

    /* The middle row should include at least one 0. If not, nothing was propagated
     * upward from the row below.
     */
    found = false;
    for (int col = 0; col < fire.numCols(); col++) {
        if (fire[1][col] == 0) {
            found = true;
            break;
        }
    }
    if (!found) {
        SHOW_ERROR("Middle row did not contain any 0 values.");
    }

    /* The bottom row should be all 0s. */
    for (int col = 0; col < fire.numCols(); col++) {
        EXPECT_EQUAL(fire[2][col], 0);
    }
}

#include "Demos/ChiSquaredTesting.h"

PROVIDED_TEST("updateFire shifts values horizontally with the correct probabilities.") {
    /* This fire has one heat source at the bottom. We pick a seven-column world
     * so that we can ignore effects on the boundary.
     */
    Grid<int> pattern = {
        { 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 5, 0, 0, 0 }
    };

    /* The heat from the 5 can be copied up to one of the three locations
     * above it, each of which has equal probability. However, there's a small
     * chance that after copying the value upward, it will be overwritten by
     * one of the neighbors.
     *
     * We are going to assume that your implementation works by sweeping from
     * left to right across the columns.
     *
     * If we copy up and to the left (probability 1/3), we will always see the
     * copied value. So
     *
     *   Pr[ 00*0000 ]  = 1/3
     *
     * If we copy straight up (probability 1/3), we will see the copied value
     * if the cell to the right does not overwrite it (probability 2/3). So we
     * have
     *
     *   Pr[ 000*000 ]  = 2/9
     *   Pr[ 0000000 ] += 1/9
     *
     * If we copy up and to the right (probability 1/3), we will see the copied
     * value if the cell to the right doesn't overwrite it (probability 2/3)
     * and the cell to steps to the right doesn't overwrite it (probability 2/3).
     * Collectively, this means
     *
     *   Pr[ 0000*00 ]  = 4/27
     *   Pr[ 0000000 ] += 5/27
     *
     * Overall, we should see the following probability distribution over where
     * the temperatue value was copied:
     *
     *   00*0000  1/3  ~= 0.333333333
     *   000*000  2/9  ~= 0.222222222
     *   0000*00  4/27 ~= 0.148148148
     *   0000000  8/27 ~= 0.296296296
     */
    const double leftProb   = 1.0 / 3.0;
    const double centerProb = 2.0 / 9.0;
    const double rightProb  = 4.0 / 27.0;
    const double noneProb   = 8.0 / 27.0;

    /* Run the experiment and see how it goes. */
    if (!ChiSquaredTesting::isClose({ leftProb, centerProb, rightProb, noneProb }, [&] {
        /* Clone the original fire and update it. */
        auto fire = pattern;
        updateFire(fire);

        /* Confirm the border regions are all zeros. */
        EXPECT_EQUAL(fire[0][0], 0);
        EXPECT_EQUAL(fire[0][1], 0);
        EXPECT_EQUAL(fire[0][5], 0);
        EXPECT_EQUAL(fire[0][6], 0);

        /* At most one of the central values should be nonzero. */
        int nonzero = 0;
        if (fire[0][2] != 0) nonzero++;
        if (fire[0][3] != 0) nonzero++;
        if (fire[0][4] != 0) nonzero++;

        EXPECT(isOneOf(nonzero, 0, 1));

        /* See which type it is. */
        if      (fire[0][2] != 0) return 0;
        else if (fire[0][3] != 0) return 1;
        else if (fire[0][4] != 0) return 2;
        else                      return 3;
    })) {
        SHOW_ERROR("Fire cells were not shifted left/center/right uniformly at random.");
    }
}

PROVIDED_TEST("updateFire cools fire with correct probability.") {
    /* This fire has one heat source at the bottom. We pick a seven-column world
     * so that we can ignore effects on the boundary.
     */
    Grid<int> pattern = {
        { 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 5, 0, 0, 0 }
    };

    /* The heat from the fire has a 19/27 chance of making it up into the next
     * row (see the math in the previous section). Of the 19/27 chance it has
     * to make it up to the row above, 2/3 of the time it should cool down
     * and 1/3 of the time it should stay the same. The net probabilities are
     * therefore
     *
     *   Pr [ nothing ] =  8/27 ~= 0.296296296
     *   Pr [ heat 4  ] = 38/81 ~= 0.469135802
     *   Pr [ heat 5  ] = 19/81 ~= 0.23456790
     */
    const double noneProb =  8.0 / 27.0;
    const double coolProb = 38.0 / 81.0;
    const double sameProb = 19.0 / 81.0;

    if (!ChiSquaredTesting::isClose({ noneProb, coolProb, sameProb }, [&] {
        /* Clone the original fire and update it. */
        auto fire = pattern;
        updateFire(fire);

        /* See which type it is. */
        int found = 0;
        int result = 0;
        for (int col = 2; col <= 4; col++) {
            if (fire[0][col] == 4) {
                found++;
                result = 1;
            } else if (fire[0][col] == 5) {
                found++;
                result = 2;
            }
        }
        if (found > 1) {
            SHOW_ERROR("Heat was copied up to multiple locations.");
        }
        return result;
    })) {
        SHOW_ERROR("Fire cells were not cooled with probability 2/3.");
    }
}

PROVIDED_TEST("Fire propagates upward near world boundaries.") {
    /* This world has a hot source in the far right. */
    Grid<int> pattern = {
        { 0, 0, 0 },
        { 0, 0, 3 }
    };

    Grid<int> option1 = {
        { 0, 3, 0 },
        { 0, 0, 3 }
    };
    Grid<int> option2 = {
        { 0, 2, 0 },
        { 0, 0, 3 }
    };
    Grid<int> option3 = {
        { 0, 0, 3 },
        { 0, 0, 3 }
    };
    Grid<int> option4 = {
        { 0, 0, 2 },
        { 0, 0, 3 }
    };

    /* Each time the fire spreads, the 3 should propagate upward to one of
     * the two cells above it. We'll run 1,000 trials to make sure this is
     * indeed the case.
     */
    for (int trial = 0; trial < 1000; trial++) {
        Grid<int> fire = pattern;
        updateFire(fire);

        if (fire != option1 && fire != option2 && fire != option3 && fire != option4) {
            SHOW_ERROR("Fire did not propagate upward properly at world border.");
        }
    }
}

PROVIDED_TEST("Fire propagates upward in a narrow world.") {
    /* In a 2x1 world with heat at the bottom, the heat should always
     * propagate upward.
     */
    Grid<int> pattern = {
        { 0 },
        { 3 }
    };

    /* If we run 1,000 trials, we should always see the fire propagated upward. */
    for (int trial = 0; trial < 1000; trial++) {
        Grid<int> fire = pattern;
        updateFire(fire);

        EXPECT(isOneOf(fire[0][0], 2, 3));
    }
}
