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


		/*RANDOM SEARCH
		struct stack* steps = random_search_cant_die(board);
		while (steps->size>0){
		    struct step* best_step = pop(steps);
		    axis = best_step->axis;
		    direction = best_step->direction;
		    //printf("Axis: %d, Direction: %d  ||  ",axis,direction);
		    free(best_step);
		}
		//printf("\n");
		delete_stack(steps);*/

		///*Random Search Future
        printf("Starting algorithm");
		int* move = random_search_future(board);
        printf("Finished algorithm");

        axis = move[0];
		direction = move[1];//*/


		show_board(board);
		//printf("Cloning board");
		//board = clone_board(board);
		//board->snek = clone_snek(board->snek);
		//printf("Done cloning");
		int old_flag = board->moogleFlag;
		play_on = advance_frame(axis, direction, board);
		int new_flag = board->moogleFlag;
		if (old_flag == 0 && new_flag == 1){
		    //printf("FOOD SPAWNED\n");
		}

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
	show_board(board);
	end_game(&board);

}

int main(){
    //while(1){
        play_game();
    //}
	return 0;
}
