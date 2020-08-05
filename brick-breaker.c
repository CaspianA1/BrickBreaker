#include "brick-breaker.h"

// a guide: https://www.viget.com/articles/game-programming-in-c-with-the-ncurses-library/

void drawScreen(int maxX, int maxY, 
				int LEFT_BOUNDARY, int RIGHT_BOUNDARY,
				char paddle[], Ball playerBall, int paddlePosition,
				Block levelBlocks[], int numLives, int currLevel, int currScore) {

	// draw left side of screen
	attron(COLOR_PAIR(1));
	for (int i = 0; i < maxY; i++) {
	for (int j = 0; j < LEFT_BOUNDARY; j++)
		mvprintw(i, j, " ", 1);
	}

	// draw right side of screen
	for (int i = 0; i < maxY; i++) {
		for (int j = RIGHT_BOUNDARY; j < maxX; j++)
			mvprintw(i, j, " ", 1);
	}
	attroff(COLOR_PAIR(1));

	// draw boundaries
	for (int boundY = 0; boundY < maxY; boundY++) {
		mvprintw(boundY, LEFT_BOUNDARY, "|");
		mvprintw(boundY, RIGHT_BOUNDARY, "|");
	}

	// draw paddle
	attron(COLOR_PAIR(2));
	for (int x = 0; x < strlen(paddle); x++)
		mvprintw(maxY - round(maxY / 20), paddlePosition + x, &paddle[x]);


	// draw current level
	mvprintw(maxY - round(maxY * 5 / 6), round(maxX / 10), "Current level: %d", currLevel);
	// draw number of lives 
	mvprintw(maxY - round(maxY * 4 / 6), round(maxX / 10), "Number of lives: %d", numLives);
	// draw current score
	mvprintw(maxY - round(maxY / 2), round(maxX / 10), "Current score: %d", currScore);
	

	attroff(COLOR_PAIR(2));

	// draw ball
	attron(COLOR_PAIR(3));
	mvprintw(playerBall.y, playerBall.x, &playerBall.shape);
	attroff(COLOR_PAIR(3));

	// draw blocks
	for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
		Block block = levelBlocks[i];
		if (!block.destroyed) {
			attron(COLOR_PAIR(block.color));
			mvprintw(block.y, block.x, &block.shape);
			attroff(COLOR_PAIR(block.color));
		}
	}
	// refresh screen
	refresh();

}

void wallCollision(Ball *ball, int maxY,
	int LEFT_BOUNDARY, int RIGHT_BOUNDARY, int *numLives) {
		if (ball -> y <= 0) {
			ball -> goingDown = true;
		}

		else if (ball -> y == maxY) {
			ball -> goingDown = false;
			(*numLives)--;
		}

		if (ball -> x <= LEFT_BOUNDARY || ball -> x == LEFT_BOUNDARY + 1) {
			ball -> angle = SHALLOW_RIGHT;
		}
		else if (ball -> x >= RIGHT_BOUNDARY) ball -> angle = SHALLOW_LEFT;
}

void paddleCollision(Ball *ball, char paddle[], int paddlePosition, int maxY) {
	const int END_OF_PADDLE = paddlePosition + strlen(paddle);
	for (int paddleX = paddlePosition; paddleX < END_OF_PADDLE; paddleX++) {
		if (ball -> x == paddleX && ball -> y == maxY - round(maxY / 20)) {
			ball -> goingDown = !ball -> goingDown;

			ball -> angle = (paddleX - paddlePosition + 1) / 2;
		}
	}
}

void initializeBlocks(Block *blocks, int maxY,
						int LEFT_BOUNDARY, int RIGHT_BOUNDARY) {
	for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
		Block b = {randNum(LEFT_BOUNDARY + 1, RIGHT_BOUNDARY - 1),
		randNum(1, round(maxY / 2)), randNum(4, 8), false, '-'};
		blocks[i] = b;
	}
}

void detectBlockCollision(Block *blocks, Ball *ball, int *currentScore) {
	for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
		Block* b = &blocks[i];

			if (!b -> destroyed) {
			 if (b -> x == ball -> x - 1 || b -> x == ball -> x || b -> x == ball -> x + 1) {
			 	if (b -> y == ball -> y - 1 || b -> y == ball -> y || b -> y == ball -> y + 1) {
			 		beep();
					b -> destroyed = true;
					ball -> goingDown = !ball -> goingDown;
					(*currentScore)++;
				}
			}
		}
	}
}

void moveBall(Ball *playerBall) {
	switch (playerBall -> angle) {
		case STEEP_LEFT: playerBall -> x -= 3; break;
		case FORTY_FIVE_LEFT: playerBall -> x -= 2; break;
		case SHALLOW_LEFT: playerBall -> x --; break;
		case SHALLOW_RIGHT: playerBall -> x ++; break;
		case FORTY_FIVE_RIGHT: playerBall -> x += 2; break;
		case STEEP_RIGHT: playerBall -> x += 3; break;
	}

	if (playerBall -> goingDown) playerBall -> y++;
	else playerBall -> y--;
}

// function checkForNextLevel - done
// function checkForDeath - done
// number of lives; hit bottom = lose one - done
// fix weird bump on left side with paddle - done
// make a colored background for the main screen - done
// num balls relative to screen size
// same for paddle size
// ordered balls - like arkanoid, rather than brick-breaker (?)
// in the end, give the executable a brick-breaker name
// extra life when higher level (?)
// adaptive screen size (?)
// score calculator - done
// diff angle of ball depending on where hit - and different speeds (?) - angle ball is in progress, and angle is now 
// make noise for various actions - done (beep for when you hit a ball) - done
// Make sure that the level system actually works! 
// this may be important: https://www.mkssoftware.com/docs/man3/curs_printw.3.asp

// bugs:
// if the ball goes off of the screen vertically, it doesn't bounce back - done
// hitting the ball at certain paddle positions doesn't change its angle

void drawEndMessage(int maxY, int maxX, bool waitBetweenLetter) {
	int letterShiftX = 0;
	for (int idx = 0; idx < SENTENCE_LENGTH; idx++) {
		for (int y = 0; y < LETTER_HEIGHT; y++) {
			for (int x = 0; x < LETTER_WIDTH; x++) {
				mvprintw((maxY / 2) + y, x + letterShiftX, "%c", DEATH_MESSAGE[idx][y][x]);
			}
		}
		letterShiftX += maxX / SENTENCE_LENGTH;
		if (waitBetweenLetter)
			napms(250);
		refresh();
	}
}

int detectDeath(int numLives, int maxY, int maxX) {
	if (numLives == 0) {
		clear();
		bool slowPrint = true;
		bool clearScreen = false;
		while (getch() != 'q') {
			if (clearScreen) {
				clearScreen = false;
				clear();
				refresh();
				napms(500);
				continue;
			}
			drawEndMessage(maxY, maxX, slowPrint);
			napms(250);
			slowPrint = false;
			clearScreen = true;
		}
		return 1;
	}
	return 0;
}

int checkForNextLevel(Block currentBlocks[]) {
		int destroyedTally = 0;
		for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
			Block block = currentBlocks[i];
			if (block.destroyed) destroyedTally++;
		}
		if (destroyedTally == NUMBER_OF_BLOCKS)
			return 1;
		return 0;
}

int main() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, true);
	nodelay(stdscr, true);
	curs_set(false);
	start_color();
	use_default_colors();

	init_pair(1, COLOR_CYAN, COLOR_CYAN); // sides of screen
	init_pair(2, COLOR_RED, COLOR_WHITE); // paddle
	init_pair(3, COLOR_BLUE, COLOR_WHITE); // ball

	init_pair(4, COLOR_MAGENTA, COLOR_YELLOW); // block
	init_pair(5, COLOR_BLACK, COLOR_GREEN); // block
	init_pair(6, COLOR_GREEN, COLOR_BLACK); // block
	init_pair(7, COLOR_WHITE, COLOR_RED); // block
	init_pair(8, COLOR_CYAN, COLOR_WHITE); // block

	srand(time(NULL));
	
	int maxY, maxX;
	getmaxyx(stdscr, maxY, maxX);

	int LEFT_BOUNDARY = round(maxX / 3);
	int RIGHT_BOUNDARY = LEFT_BOUNDARY * 2;

	Ball playerBall = {round(maxX / 2), round(maxY / 2), true, SHALLOW_LEFT, 'O'};
	char paddle[] = "|__________|"; // assigning length of twelve results in crazy alphabet printing
	int paddlePosition = round(maxX / 2) - 12;

	int numLives = 3;
	int currentLevel = 1;
	int currentScore = 0;

	Block currentBlocks[NUMBER_OF_BLOCKS];
	initializeBlocks(currentBlocks, maxY, LEFT_BOUNDARY, RIGHT_BOUNDARY);
	
	while (true) {
		getmaxyx(stdscr, maxY, maxX);
		erase(); // less flickering with erase

		// user input
		switch (getch()) {
		case 'q':
			endwin();
			return 1;
		case KEY_LEFT:
			if (paddlePosition - 1 != LEFT_BOUNDARY)
				paddlePosition--;
			break;

		case KEY_RIGHT:
			if (paddlePosition + strlen(paddle) != RIGHT_BOUNDARY) {
				paddlePosition++;
				break;
			}
		}

		drawScreen(maxX, maxY, LEFT_BOUNDARY, RIGHT_BOUNDARY,
					paddle, playerBall, paddlePosition, currentBlocks,
					numLives, currentLevel, currentScore);


		wallCollision(&playerBall, maxY, LEFT_BOUNDARY, RIGHT_BOUNDARY, &numLives);
		paddleCollision(&playerBall, paddle, paddlePosition, maxY);
		detectBlockCollision(currentBlocks, &playerBall, &currentScore);

		int levelStatus = checkForNextLevel(currentBlocks);
		if (levelStatus == 1) {
			memset(currentBlocks, 0, sizeof currentBlocks);
			initializeBlocks(currentBlocks, maxY, LEFT_BOUNDARY, RIGHT_BOUNDARY);
			currentLevel++;
			currentScore += 10;
			numLives += 1;
		}

		int deathStatus = detectDeath(numLives, maxY, maxX);
		if (deathStatus == 1) {
			endwin();
			return 0;
		}

		moveBall(&playerBall);

		napms(50);
	}
}
