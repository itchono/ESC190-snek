//#include "snek_api.h"
//#include "tree_search.h"
#include "random_search.h"
#include <unistd.h>

void play_game() {
	printf("starting\n");
	//board initialized, struct has pointer to snek
	GameBoard* board = init_board();
	show_board(board);

	int axis = AXIS_INIT;
	int direction = DIR_INIT;
	
	int play_on = 1;
	int coord[2];
	
	while (play_on){
		coord[x] = board->snek->head->coord[x];
		coord[y] = board->snek->head->coord[y];
		/*
		unsigned short go_x = (axis == AXIS_Y && direction == 1 && coord[y] == (BOARD_SIZE - 1)) || (axis == AXIS_Y && direction == -1 && coord[y] == 0);
		unsigned short go_y = (axis == AXIS_X && direction == 1 && coord[x] == (BOARD_SIZE - 1)) || (axis == AXIS_X && direction == -1 && coord[x] == 0);
		if (go_x) {
			// going to hit the bottom or top
			// go right or left
			axis = AXIS_X;
			if (coord[x] < (int)(BOARD_SIZE / 2)){ 
				direction = RIGHT;
			} else {
				direction = LEFT;
			}
		} else if (go_y) {
			axis = 	AXIS_Y;
			if (coord[y] < (int)(BOARD_SIZE / 2)){ 
				direction = DOWN;
			} else {
				direction = UP;
			}
		}*/
		//struct stack* steps = tree_search(board, get_score());
		//printf("REEEE");
		///*
		//printf("Start random search");
		struct stack* steps = random_search(board);
		//printf("Finished random search");
		while (steps->size>0){
		    struct step* best_step = pop(steps);
		    axis = best_step->axis;
		    direction = best_step->direction;
		    //printf("Axis: %d, Direction: %d  ||  ",axis,direction);
		    free(best_step);
		}
		//printf("\n");
		delete_stack(steps);//*/
		show_board(board);
		//printf("Cloning board");
		//board = clone_board(board);
		//board->snek = clone_snek(board->snek);
		//printf("Done cloning");
		play_on = advance_frame(axis, direction, board);

		/*
		printf("Going ");

		if (axis == AXIS_X){
			if (direction == RIGHT){
				printf("RIGHT");
			} else {
				printf("LEFT");
			}
		} else {
			if (direction == UP){
				printf("UP");
			} else {
				printf("DOWN");
			}
		} printf("\n");*/

		//usleep(1000);
	}
	printf("REE");
	show_board(board);
	end_game(&board);

}

int main(){
	play_game();
	return 0;
}
