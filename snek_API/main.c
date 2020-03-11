//#include "snek_api.h"
//#include "tree_search.h"
#include "random_search.h"
#include <unistd.h>
#include <time.h>

void play_game() {
    randSeeding(time(0));
    //printf("starting\n");
	//board initialized, struct has pointer to snek
	GameBoard* board = init_board();
	//show_board(board);

	int axis = AXIS_INIT;
	int direction = DIR_INIT;
	
	int play_on = 1;
	int coord[2];
	
	while (play_on){
		game_step(coord, &axis, &direction, board);
	}
	end_game(&board);

}

int game_step(int* axis, int* direction, GameBoard* board) {
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

int main(){
    //while(1){
        play_game();
    //}
	return 0;
}
