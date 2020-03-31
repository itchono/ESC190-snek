#include "random_search.h"
#include <time.h>


int gameStep(int* axis, int* direction, GameBoard* board) {
	// needs python input at byref of a c_int
	int coord[2];

	coord[x] = board->snek->head->coord[x];
	coord[y] = board->snek->head->coord[y];


	struct stack* steps = random_search_cant_die(board);

	while (steps->size>0){
		struct step* best_step = pop(steps);
		*axis = best_step->axis;
		*direction = best_step->direction;
		free(best_step);
	}

	delete_stack(steps);

	return(advance_frame(*axis, *direction, board));
}
