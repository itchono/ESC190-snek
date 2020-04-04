#include "random_search.h"
#include <time.h>

// Navigation V4
// for use with python tool

static struct stack* steps = NULL;
static int regenStack = 1; // only generate moves when food spawns or stack goes to zero moves
int dead_stack;//Try this to make extern work
static int old_dead_stack;//For analysis only
static int dead_stack_length;
static int recovery_count;
static int total_dead_count;

int gameStep(int* axis, int* direction, GameBoard* board) {
	// needs python input at byref of a c_int

	// FIRST RUN
	if (!steps || steps->size == 0) {
		// if we run out of moves on the stack, we need to regenerate the list
		steps = create_stack();
		regenStack = 1;
		//printf("Size insufficient. Regenerating stack..\n");
	}

	if (regenStack) {
		delete_stack(steps);
		steps = create_stack();

        old_dead_stack = dead_stack;
		struct stack* backwards = random_search_cant_die(board);
        if(old_dead_stack == 0 && dead_stack == 1){
            //printf("\nDead stack chain begins\n");
            dead_stack_length = 1;
            total_dead_count++;
            //printf("%d / %d", recovery_count / total_dead_count)
        }
        if(old_dead_stack == 1 && dead_stack == 1){dead_stack_length++;}
		else if(old_dead_stack == 1 && dead_stack == 0){
		    //printf("Dead stack chain ends: length %d\n", dead_stack_length);
		    recovery_count++;
		    //printf("%d,",dead_stack_length);
		}
		
		while (backwards->size>0){
			push(steps, pop(backwards));
		} // puts moves from DFS onto stack list (like a queue)
		delete_stack(backwards);
		//regenStack = dead_stack;

	}

	//printf("Stack Health: %d\n", steps->size);

	struct step* best_step = pop(steps);
	*axis = best_step->axis;
	*direction = best_step->direction;
	free(best_step);

	int old_flag = board->moogleFlag;
	int old_score = board->score;

	int play_on = advance_frame(*axis, *direction, board); // call from snek API

	if (old_flag != board->moogleFlag || board->score - old_score > LIFE_SCORE){
		// if NEW food spawns, regenerate moveset
		// OR if we just ate some food
		regenStack = 1;
		//printf("Food Detected\n");
	}

	if (!play_on) delete_stack(steps);

	return play_on;
}
