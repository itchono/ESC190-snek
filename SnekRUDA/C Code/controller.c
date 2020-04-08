#include "random_search.h"
#include <time.h>

//int CYCLE_ALLOWANCE;
//int BOARD_SIZE;

#define MAX_DEAD_STACK_LENGTH 5
#define THINK_GAP (BOARD_SIZE*BOARD_SIZE)/8
// Navigation V4
// for use with python tool

static struct stack* steps = NULL;
static int regenStack = 1; // only generate moves when food spawns or stack goes to zero moves
int dead_stack;//Try this to make extern work
static int old_dead_stack;//For analysis only
static int dead_stack_length;
static int counter;

int get_dead_stack() {
	return dead_stack;
}

int gameStep(int* axis, int* direction, GameBoard* board) {
    counter++;
	// needs python input at byref of a c_int
	// FIRST RUN
	if (!steps || steps->size == 0) {
		// if we run out of moves on the stack, we need to regenerate the list
		steps = create_stack();
		regenStack = 1;//Why is regen stack 1?
		//printf("Size insufficient. Regenerating stack..\n");
	}

	//regenStack = 1;
	if (regenStack) {
	    counter = 1;
		delete_step_stack(steps);
		steps = create_stack();

        old_dead_stack = dead_stack;
		struct stack* backwards = random_search_cant_die(board);
        if(old_dead_stack == 0 && dead_stack == 1){
            //printf("\nDead stack chain begins\n");
            dead_stack_length = 1;
        } else if (old_dead_stack == 1 && dead_stack == 1){
            dead_stack_length++;
        } else if (old_dead_stack == 1 && dead_stack == 0){
		    //printf("Dead stack chain ends: length %d\n", dead_stack_length);
		    //printf("%d,",dead_stack_length);
		    dead_stack_length = 0;
		}
		
		while (backwards->size>0){
			push(steps, pop(backwards));
		} // puts moves from DFS onto stack list (like a queue)
		delete_step_stack(backwards);
		regenStack = dead_stack;
        if (dead_stack_length > MAX_DEAD_STACK_LENGTH){//Might need to make this override counter
            regenStack = 0;
			//printf("\nDead stack exceeded %d, retrying...\n",MAX_DEAD_STACK_LENGTH);
        }

		//printf("New Stack Health: %d\n", steps->size);
	}

	

	struct step* best_step = pop(steps);
	*axis = best_step->axis;
	*direction = best_step->direction;
	free(best_step);

	int old_flag = board->moogleFlag;
	int old_score = board->score;

	int play_on = advance_frame(*axis, *direction, board); // call from snek API

	//if (old_flag != board->moogleFlag || board->score - old_score > LIFE_SCORE){
    //if (old_flag != board->moogleFlag || (board->score - old_score > LIFE_SCORE && board->moogleFlag) || board->moogleFlag == 0){
    if (old_flag != board->moogleFlag){
        // if NEW food spawns, regenerate moveset
		// OR if we just ate some food
		regenStack = 1;
        /*
		if (board->moogleFlag) {
			printf("\nDumping stack of size: %d because food detected. Regenerating stack..\n", steps->size);
		}
		else {
			printf("\nDumping stack of size: %d because food eaten. Regenerating stack..\n", steps->size);
		}
		*/
	}
    int temp = ((int)(counter) % (int)(THINK_GAP));
    if (temp == 0) {
        regenStack = 1;
        //printf("Think Gap Reached: %d/%d -> %d",counter,THINK_GAP,temp);
    }

	if (!play_on) delete_step_stack(steps);

	return play_on;
}
