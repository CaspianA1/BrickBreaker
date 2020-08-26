#include <curses.h>
// #include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define NUMBER_OF_BLOCKS 30
#define SENTENCE_LENGTH 8
#define LETTER_WIDTH 14
#define LETTER_HEIGHT 8


enum ballAngle {
	STEEP_LEFT,
	FORTY_FIVE_LEFT,
	SHALLOW_LEFT,
	SHALLOW_RIGHT,
	FORTY_FIVE_RIGHT,
	STEEP_RIGHT
};

typedef struct ball {
	unsigned int x;
	unsigned int y;
	bool goingDown;
	enum ballAngle angle;
	const char shape;
} Ball;

typedef struct block {
	unsigned int x;
	unsigned int y;
	unsigned int color;
	bool destroyed;
	char shape;
} Block;

const char DEATH_MESSAGE[SENTENCE_LENGTH][LETTER_HEIGHT][LETTER_WIDTH] = {
{"___     ___\n", 
"\\  \\___/  /\n",
" \\  \\ /  /\n",
"  |  |  |\n",
"  |  |  |\n",
"  |__|__|\n"},

{"  _________ \n",
" /   __    \\\n",
"|   |  |    |\n",
"|   |  |    |\n",
"|   |__|    |\n",
" \\_________/\n"},

{"   __   __\n",
"  / |   | \\\n",
" |  |   |  |\n",
" |  \\___/  |\n",
" \\_________/\n"},

{" __\n",
"|  |\n",  
"|  |\n",
"|  |__\n",
"|_____/\n"},

{"  _________ \n",
" /   __    \\\n",
"|   |  |    |\n",
"|   |  |    |\n",
"|   |__|    |\n",
" \\_________/\n"},

{" _________\n",
"/   _____/\n",
"\\_____   \\\n",
" /        \\\n",
"/_______  /\n",
"        \\/\n"},

{"___________\n",
"\\__    ___/\n",
"   |   |\n",
"   |   |\n",
"   |   |\n",
"   |___|\n"},

{"\\\\|||||//\n",
" \\\\\\|///\n",
" \\\\\\|///\n",
"  \\\\|//\n",
"   \\|/\n",
"   ***\n",
"   ***\n",
"    *\n"}
};

void drawScreen(const int maxX, const int maxY, 
				const int LEFT_BOUNDARY, const int RIGHT_BOUNDARY,
				char paddle[], Ball playerBall, int paddlePosition,
				Block levelBlocks[], int numLives, int currLevel, int currScore);

void wallCollision(Ball *ball, const int maxY,
				   const int LEFT_BOUNDARY, const int RIGHT_BOUNDARY, int *numLives);

void paddleCollision(Ball *ball, char paddle[], int paddlePosition, const int maxY);

void initializeBlocks(Block *blocks, int maxY,
						const int LEFT_BOUNDARY, const int RIGHT_BOUNDARY);

int randNum(int lower, int upper) {return (rand() % (upper - lower + 1)) + lower;}

int inRange(int item, int lower, int upper) {return lower <= item <= upper;}

void detectBlockCollision(Block *blocks, Ball *ball, int *currentScore);

void moveBall(Ball *ball);

int detectDeath(int numLives, int maxX, int maxY);
