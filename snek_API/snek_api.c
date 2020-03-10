/** snake API for C **/
// need to add the elongating of snek after nomming
#include "snek_api.h"
#include <string.h>
#include <time.h>

int CURR_FRAME = 0;
int SCORE = 0;
int MOOGLE_FLAG = 0;
int MOOGLES_EATEN = 0;
int TIME_OUT = ((BOARD_SIZE * 4) - 4) * CYCLE_ALLOWANCE;
//int TIME_OUT = 100;

GameBoard* init_board(){
	//srand(time(0)); moved to another function
	GameBoard* gameBoard = (GameBoard*)(malloc(sizeof(GameBoard)));

	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			gameBoard->cell_value[i][j] = 0;
			gameBoard->occupancy[i][j] = 0;
		}
	}
	gameBoard->occupancy[0][0] = 1; //snake initialized
	gameBoard->snek = init_snek(0, 0);

	//My own stuff KZ
	gameBoard->moogleFlag = 0;
	gameBoard->currFrame = 0;
	gameBoard->score = 0;
	gameBoard->timeOut = TIME_OUT;
	gameBoard->mooglesEaten = 0;
	return gameBoard;
}

Snek* init_snek(int a, int b){
	Snek* snek = (Snek *)(malloc(sizeof(Snek)));

	snek->head = (SnekBlock *)malloc(sizeof(SnekBlock));
	snek->head->coord[x] = a;
	snek->head->coord[y] = b;

	snek->tail = (SnekBlock *)malloc(sizeof(SnekBlock));
	snek->tail->coord[x] = a;
	snek->tail->coord[y] = b;

	snek->tail->next = NULL;
	snek->head->next = snek->tail;
	
	snek->length = 1;

	snek->direction = DIR_INIT;
	snek->axis = AXIS_INIT;

	return snek;
}

void randSeeding(int seed) {
	srand(seed);
}

int hits_edge(int axis, int direction, GameBoard* gameBoard){
	// Replaced gameBoard->snek->head[y] with gameBoard->snek->head->coord[y]
	if (((axis == AXIS_Y) && ((direction == UP && gameBoard->snek->head->coord[y] + UP < 0) || (direction == DOWN && gameBoard->snek->head->coord[y] + DOWN > BOARD_SIZE - 1)))
	   || (axis == AXIS_X && ((direction == LEFT && gameBoard->snek->head->coord[x] + LEFT < 0) || (direction == RIGHT && gameBoard->snek->head->coord[x] + RIGHT > BOARD_SIZE-1))))
	{
		return 1;
	} else {
		return 0;
	}

}


int hits_self(int axis, int direction, GameBoard *gameBoard){
	int new_x, new_y;
	if (axis == AXIS_X){
		new_x = gameBoard->snek->head->coord[x] + direction;
		new_y = gameBoard->snek->head->coord[y];
	} else if (axis == AXIS_Y){
		new_x = gameBoard->snek->head->coord[x];
		new_y = gameBoard->snek->head->coord[y] + direction;
	}
	return gameBoard->occupancy[new_y][new_x]; //1 if occupied
}

int time_out(){
	return (MOOGLE_FLAG == 1 && CURR_FRAME > TIME_OUT);

}

int is_failure_state(int axis, int direction, GameBoard *gameBoard){
	return (hits_self(axis, direction, gameBoard) || hits_edge(axis, direction, gameBoard) || time_out());
}

void populate_moogles(GameBoard *gameBoard){
	if (MOOGLE_FLAG == 0){

		int r1 = rand() % BOARD_SIZE;
		int r2 = rand() % BOARD_SIZE;
		
		int r3 = rand() % (BOARD_SIZE * 10);
		if (r3 == 0){
			gameBoard->cell_value[r1][r2] = MOOGLE_POINT * HARRY_MULTIPLIER;
			MOOGLE_FLAG = 1;
			gameBoard->moogleFlag = 1;
		} else if (r3 < BOARD_SIZE){
			gameBoard->cell_value[r1][r2] = MOOGLE_POINT;
			MOOGLE_FLAG = 1;
            gameBoard->moogleFlag = 1;
        }
	    //gameBoard->cell_value[2][2] = MOOGLE_POINT;
	    //MOOGLE_FLAG = 1;
	}
}

void eat_moogle(GameBoard* gameBoard, int head_x, int head_y) {
	SCORE = SCORE + gameBoard->cell_value[head_y][head_x];
	gameBoard->score = SCORE;
	gameBoard->cell_value[head_y][head_x] = 0;

	gameBoard->snek->length ++;
	MOOGLES_EATEN ++;
	gameBoard->mooglesEaten = MOOGLES_EATEN;
	MOOGLE_FLAG = 0;
    gameBoard->moogleFlag = 0;
    CURR_FRAME = 0;
	gameBoard->currFrame = 0;
}

int advance_frame(int axis, int direction, GameBoard *gameBoard){
    gameBoard->snek->direction = direction;
    gameBoard->snek->axis = axis;
    /*
    SnekBlock* pointer = gameBoard->snek->head;
    printf("length: %d",gameBoard->snek->length);
    while (pointer!=gameBoard->snek->tail->next){
        printf("x: %d     y:%d\n",pointer->coord[x],pointer->coord[y]);
        pointer = pointer->next;
    }*/
	if (is_failure_state(axis, direction, gameBoard)){
		return 0;
	} else {
		// update the occupancy grid and the snake coordinates
		int head_x, head_y;
		// figure out where the head should now be
		if (axis == AXIS_X) {
			head_x = gameBoard->snek->head->coord[x] + direction;
			head_y = gameBoard->snek->head->coord[y];
		} else if (axis == AXIS_Y){
			head_x = gameBoard->snek->head->coord[x];
			head_y = gameBoard->snek->head->coord[y] + direction;
		}
		
		int tail_x = gameBoard->snek->tail->coord[x];
		int tail_y = gameBoard->snek->tail->coord[y];

		// update the occupancy grid for the head
		gameBoard->occupancy[head_y][head_x] = 1;

		if (gameBoard->snek->length > 1) { //make new head
			SnekBlock *newBlock = (SnekBlock *)malloc(sizeof(SnekBlock));
			newBlock->coord[x] = gameBoard->snek->head->coord[x];
			newBlock->coord[y] = gameBoard->snek->head->coord[y];
			newBlock->next = gameBoard->snek->head->next;

			gameBoard->snek->head->coord[x] = head_x;
			gameBoard->snek->head->coord[y] = head_y;
			gameBoard->snek->head->next = newBlock;
	
			if (gameBoard->cell_value[head_y][head_x] > 0){  //eat something
				eat_moogle(gameBoard, head_x, head_y);
			} else { //did not eat
				//delete the tail
				gameBoard->occupancy[tail_y][tail_x] = 0;
				SnekBlock *currBlock = gameBoard->snek->head;
				while (currBlock->next != gameBoard->snek->tail){
					currBlock = currBlock->next;
				} //currBlock->next points to tail

				currBlock->next = NULL;
				free(gameBoard->snek->tail);
				gameBoard->snek->tail = currBlock;
			}

		} else if ((gameBoard->snek->length == 1) && gameBoard->cell_value[head_y][head_x] == 0){ // change both head and tail coords, head is tail
			gameBoard->occupancy[tail_y][tail_x] = 0;
			gameBoard->snek->head->coord[x] = head_x;
			gameBoard->snek->head->coord[y] = head_y;
			gameBoard->snek->tail->coord[x] = head_x;
			gameBoard->snek->tail->coord[y] = head_y;
			
		} else { //snake is length 1 and eats something
			eat_moogle(gameBoard, head_x, head_y);
			gameBoard->snek->head->coord[x] = head_x;
			gameBoard->snek->head->coord[y] = head_y;
		}

		// update the score and board
		SCORE = SCORE + LIFE_SCORE;
		gameBoard->score = SCORE;
		if (MOOGLE_FLAG == 1){
			CURR_FRAME ++;
			gameBoard->currFrame++;
		}

		// populate moogles
		populate_moogles(gameBoard);
		return 1;
	} 
}

void show_board(GameBoard* gameBoard) {
	fprintf(stdout, "\033[2J"); // clear terminal ANSI code
	fprintf(stdout, "\033[0;0H\n"); // reset cursor position NOTE: added a \n
	
	char blank = 	43;
	char snek = 	83;
	char moogle = 	88;
	char snekMoogle = 'M';

	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			if (gameBoard->occupancy[i][j] == 1){
			    if (gameBoard->cell_value[i][j] > 0){
			        fprintf(stdout,"%c",snekMoogle);
			    } else if (gameBoard->snek->head->coord[0] == j && gameBoard->snek->head->coord[1] == i){
			        fprintf(stdout,"H");
			    } else if (gameBoard->snek->tail->coord[0] == j && gameBoard->snek->tail->coord[1] == i) {
                    fprintf(stdout,"T");
                } else {
                    fprintf(stdout, "%c", snek);
                }
				//snake is here
			} else if (gameBoard->cell_value[i][j] > 0) {
				//there be a moogle
				fprintf(stdout, "%c", moogle);
			} else {
				//nothing to see here
				fprintf(stdout, "%c", blank);
			}
		} //new line
		fprintf(stdout, "\n");
		
	}

	fprintf(stdout, "\n\n");
	
	if (MOOGLE_FLAG == 1){
		fprintf(stdout, "!..ALERT, MOOGLE IN VICINITY..!\n\n");
	}
	fprintf(stdout, "SCORE: %d\n", SCORE);
	fprintf(stdout, "YOU HAVE EATEN %d MOOGLES\n\n", MOOGLES_EATEN);

	fprintf(stdout, "SNEK HEAD\t(%d, %d)\n", gameBoard->snek->head->coord[x], gameBoard->snek->head->coord[y]);
	fprintf(stdout, "SNEK TAIL\t(%d, %d)\n", gameBoard->snek->tail->coord[x], gameBoard->snek->tail->coord[y]);
	fprintf(stdout, "LENGTH \t%d\n", gameBoard->snek->length);
	fprintf(stdout, "CURR FRAME %d vs TIME OUT %d || MOOGLE FLAG %d\n", CURR_FRAME, TIME_OUT,MOOGLE_FLAG);


	fflush(stdout);
}

int get_score() {
	return SCORE;
}

void end_game(GameBoard **board){
	//fprintf(stdout, "\033[2J");
	//fprintf(stdout, "\033[0;0H"); 
	fprintf(stdout, "\n--!!---GAME OVER---!!--\n\nYour score: %d", SCORE);
	fflush(stdout);
	// need to free all allocated memory
	// first snek
	SnekBlock **snekHead = &((*board)->snek->head);
	SnekBlock *curr;
	SnekBlock *prev;
	while ((*snekHead)->next != NULL) {
		curr = *snekHead;
		while (curr->next != NULL){
			prev = curr;
			curr = curr->next;
		}
		prev->next = NULL;
		free(curr);
	}
	free(*snekHead);
	free((*board)->snek);
	free(*board);

	// RESET VARS

	CURR_FRAME = 0;
	SCORE = 0;
	MOOGLE_FLAG = 0;
	MOOGLES_EATEN = 0;
}

void delete_board(GameBoard** board){
    SnekBlock* pointer = (*board)->snek->head;
    while (pointer != (*board)->snek->tail){
        SnekBlock* next = pointer->next;
        free(pointer);
        pointer = next;
    }
    free(pointer);
    free((*board)->snek);
    free(*board);

    /*
    SnekBlock **snekHead = &((*board)->snek->head);
    SnekBlock *curr;
    SnekBlock *prev;
    while ((*snekHead)->next != NULL) {
        curr = *snekHead;
        while (curr->next != NULL){
            prev = curr;
            curr = curr->next;
        }
        prev->next = NULL;
        free(curr);
    }
    free(*snekHead);
    free((*board)->snek);
    free(*board);*/
}





//My own stuff KZ
int contained_advance_frame(int axis, int direction, GameBoard *gameBoard){//Note can remove delta_score
    gameBoard->snek->direction = direction;
    gameBoard->snek->axis = axis;
    int debug = 0;
    if (debug) {
        SnekBlock* pointer = gameBoard->snek->head;
        printf("length: %d",gameBoard->snek->length);
        while (pointer!=gameBoard->snek->tail->next){
            printf("x: %d     y:%d\n",pointer->coord[x],pointer->coord[y]);
            pointer = pointer->next;
        }
    }
    int delta_score = 0;
    if (debug) printf("check if alive\n");
    if (contained_is_failure_state(axis, direction, gameBoard)){
        if (debug) printf("dead\n");
        //gameBoard->score = 0;
        return 0;
    } else {
        // update the occupancy grid and the snake coordinates
        int head_x, head_y;
        // figure out where the head should now be
        if (axis == -1) {//AXIS_X
            head_x = gameBoard->snek->head->coord[x] + direction;
            head_y = gameBoard->snek->head->coord[y];
        } else if (axis == 1){//AXIS_Y
            head_x = gameBoard->snek->head->coord[x];
            head_y = gameBoard->snek->head->coord[y] + direction;
        }
        if (debug) printf("calculated the head coordinates after moving\n");

        int tail_x = gameBoard->snek->tail->coord[x];
        int tail_y = gameBoard->snek->tail->coord[y];
        if (debug) printf("found tailx and taily\n");

        // update the occupancy grid for the head
        gameBoard->occupancy[head_y][head_x] = 1;
        if (debug) printf("updated occupancy for the head\n");

        if (gameBoard->snek->length > 1) { //make new head
            //if (debug) printf("snek is longer than 1\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n######################################################################### ");
            if (debug) printf("snek is longer than 1\n");
            SnekBlock *newBlock = (SnekBlock *)malloc(sizeof(SnekBlock));
            newBlock->coord[x] = gameBoard->snek->head->coord[x];
            newBlock->coord[y] = gameBoard->snek->head->coord[y];
            newBlock->next = gameBoard->snek->head->next;

            gameBoard->snek->head->coord[x] = head_x;
            gameBoard->snek->head->coord[y] = head_y;
            gameBoard->snek->head->next = newBlock;
            if (debug) printf("moved head forwards, turned old head into another block\n");

            if (gameBoard->cell_value[head_y][head_x] > 0){  //eat something
                if (debug) printf("Eating moogle\n");
                //Eat Moogle
                gameBoard->score = gameBoard->score + gameBoard->cell_value[head_y][head_x];
                delta_score = delta_score + gameBoard->cell_value[head_y][head_x];
                gameBoard->cell_value[head_y][head_x] = 0;

                gameBoard->snek->length ++;
                gameBoard->mooglesEaten ++;
                gameBoard->moogleFlag = 0;
                gameBoard->currFrame = 0;

                if (debug) printf("Consumed\n");
                if (debug) printf("Ate moogle, extended snake\n");
            } else { //did not eat
                //delete the tail
                if (debug) printf("Not eating moogle\n");
                gameBoard->occupancy[tail_y][tail_x] = 0;
                if (debug) printf("set occupancy to 0\n");
                SnekBlock *currBlock = gameBoard->snek->head;
                if (debug) printf("set currblock to head\n");

                if (debug) printf("\n\ntailx: %d, taily: %d\n",gameBoard->snek->tail->coord[x],gameBoard->snek->tail->coord[y]);
                while (currBlock->next != gameBoard->snek->tail){
                    if (debug) printf("was x:%d, y:%d",currBlock->coord[x],currBlock->coord[y]);
                    currBlock = currBlock->next;
                    if (debug) printf("now x:%d, y:%d",currBlock->coord[x],currBlock->coord[y]);
                } //currBlock->next points to tail


                if (debug) printf("currblock now points to block before tail\n");

                currBlock->next = NULL;
                free(gameBoard->snek->tail);
                if (debug) printf("free tail\n");
                gameBoard->snek->tail = currBlock;
                if (debug) printf("set tail to the block before the tail\n");
                if (debug) printf("Did not eat, deleted the tail\n");
            }

        } else if ((gameBoard->snek->length == 1) && gameBoard->cell_value[head_y][head_x] == 0){ // change both head and tail coords, head is tail
            if (debug) printf("snake is of length one and did not eat\n");
            gameBoard->occupancy[tail_y][tail_x] = 0;
            gameBoard->snek->head->coord[x] = head_x;
            gameBoard->snek->head->coord[y] = head_y;
            gameBoard->snek->tail->coord[x] = head_x;
            gameBoard->snek->tail->coord[y] = head_y;
            if (debug) printf("moved single block\n");

        } else { //snake is length 1 and eats something
            if (debug) printf("snake is of length one and ate\n");
            //eat moogle
            gameBoard->score = gameBoard->score + gameBoard->cell_value[head_y][head_x];
            if (debug) printf("consumed, cell value: %d, total score: %d\n",gameBoard->cell_value[head_y][head_x],gameBoard->score);
            delta_score = delta_score + gameBoard->cell_value[head_y][head_x];
            gameBoard->cell_value[head_y][head_x] = 0;

            gameBoard->snek->length ++;
            gameBoard->mooglesEaten ++;
            gameBoard->moogleFlag = 0;
            gameBoard->currFrame = 0;

            gameBoard->snek->head->coord[x] = head_x;
            gameBoard->snek->head->coord[y] = head_y;
            if (debug) printf("extended snake\n");
        }

        // update the score and board
        gameBoard->score = gameBoard->score + LIFE_SCORE;
        delta_score = delta_score + LIFE_SCORE;
        if (gameBoard->moogleFlag == 1){
            gameBoard->currFrame ++;
        }
        if (debug) printf("updated board variables\n");

        // populate moogles
        //populate_moogles(gameBoard);
        return delta_score;
    }
}

int contained_is_failure_state(int axis, int direction, GameBoard* gameBoard) {
    return (hits_self(axis, direction, gameBoard) || hits_edge(axis, direction, gameBoard) ||
            (gameBoard->moogleFlag == 1 && (gameBoard->currFrame > gameBoard->timeOut)));
}

struct GameBoard* clone_board(struct GameBoard* board){
    //printf("starting clone_board");
    int row = BOARD_SIZE;
    int column = BOARD_SIZE;
    GameBoard* clone = (GameBoard*)(malloc(sizeof(GameBoard)));
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            clone->cell_value[i][j] = board->cell_value[i][j];
            clone->occupancy[i][j] = board->occupancy[i][j];
        }
    }
    clone->snek = clone_snek(board->snek);
    clone->moogleFlag = board->moogleFlag;
    clone->currFrame = board->currFrame;
    clone->score = board->score;
    clone->timeOut = board->timeOut;
    clone->mooglesEaten = board->mooglesEaten;
    return clone;
}
/*
int** clone_array(int row, int column, int** array){
    int clone[row][column];
    for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
            clone[i][j] = array[i][j];
        }
    }
    return clone;
}*/

struct Snek* clone_snek(struct Snek* snek){

    int debug = 0;
    if (debug) printf("Starting clone_snek");
    Snek* clone = (Snek *)(malloc(sizeof(Snek)));
    if (debug) printf("snek clone created\n");
    //clone->head = clone_snekblock(snek->head);
    //clone->tail = clone_snekblock(snek->tail);

    clone->head = (SnekBlock*)(malloc(sizeof(SnekBlock)));
    clone->head->coord[x] = snek->head->coord[x];
    clone->head->coord[y] = snek->head->coord[y];
    if (debug) printf("head clone created\n");

    //clone->tail = (SnekBlock*)(malloc(sizeof(SnekBlock)));
    //clone->tail->coord[x] = snek->tail->coord[x];
    //clone->tail->coord[y] = snek->tail->coord[y];
    //printf("tail clone created\n");

    clone->length = snek->length;
    clone->axis = snek->axis;
    clone->direction = snek->direction;
    if (debug) printf("length cloned\n");

    SnekBlock* snekPointer = snek->head;
    if (debug) printf("snekPointer set to next\n");
    SnekBlock* clonePointer = clone->head;
    if (debug) printf("clonePointer set to next\n");

    while (snekPointer != snek->tail){
        clonePointer->next = (SnekBlock*)(malloc(sizeof(SnekBlock)));
        if (debug) printf("clonePointer malloc\n");
        clonePointer = clonePointer->next;
        snekPointer = snekPointer->next;
        if (debug) printf("moved to next pointer\n");
        clonePointer->coord[x] = snekPointer->coord[x];
        clonePointer->coord[y] = snekPointer->coord[y];
        if (debug) printf("copied snekPointer to clonePointer\n");
    }

    clone->tail = clonePointer;
    return clone;
}

struct SnekBlock* clone_snekblock(struct SnekBlock* snekblock){
    SnekBlock* clone = (SnekBlock*)malloc(sizeof(SnekBlock));
    if (snekblock->next != NULL){
        clone->coord[0] = snekblock->coord[0];
        clone->coord[1] = snekblock->coord[1];
        clone->next = clone_snekblock(snekblock->next);
        return clone;
    } else {
        clone->coord[0] = snekblock->coord[0];
        clone->coord[1] = snekblock->coord[1];
        clone->next = NULL;
        return clone;
    }
}