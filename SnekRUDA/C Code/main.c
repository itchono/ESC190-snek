#include "random_search.h"
#include <unistd.h>
#include <time.h>
#include "controller.h"

// COMPATIBILITY C CODE FOR OTHER PLATFORMS
// Mingde Yin and Kamron Zaidi

void play_game() {
    randSeeding(time(0));
    printf("KM SNAKE, C Compatibility Version");
	GameBoard* board = init_board();

	int axis = AXIS_INIT;
	int direction = DIR_INIT;
	
	int play_on = 1;
	
	while (play_on){
		gameStep(axis,direction, board);
		show_board(board);
	}
	end_game(&board);

}

int main(){
    play_game();

	return 0;
}
