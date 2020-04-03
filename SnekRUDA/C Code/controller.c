#include "random_search.h"
#include <time.h>

// Navigation V4
// for use with python tool

static struct stack* steps = NULL;
static int regenStack = 1; // only generate moves when food spawns or stack goes to zero moves


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

		struct stack* backwards = random_search_cant_die(board);
		
		while (backwards->size>0){
			push(steps, pop(backwards));
		} // puts moves from DFS onto stack list (like a queue)
		delete_stack(backwards);

		regenStack = 0;
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