#pragma once

/* Tells a story that can only be read from the debugger. */
void tellStory();

/* Launches a stack overflow. */
void initiateStackOverflow();

/* Used internally by the storytelling algorithm. */
int scramble(int value);

/* Used internally by the Stack Overflow exercise. */
int shuffleValues(int seed, int values[], int numValues, int target);
