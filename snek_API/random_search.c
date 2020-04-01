//#include "snek_api.h"
//#include "tree_search.h"
#include "random_search.h"
#include <unistd.h>
#include <time.h>
//Outputs

struct stack* random_search_cant_die(GameBoard* board) {
    int debug_start = 0;
    int debug = 0;
    //srand(time(0));
    //struct stack* layer = create_stack();
    int dead = 1;
    int sMax = BOARD_SIZE*BOARD_SIZE; // EDITED to work with more boards
    struct stack *steps;
    int counter = 0;
    while (dead){
        counter++;
        dead = 0;
        steps = create_stack();
        struct GameBoard *clone = clone_board(board);
        for (int s = 0; (s < sMax && !(dead)); s++) {//100
            struct step *new_step = (struct step *) malloc(sizeof(step));
            //int axis = (rand() % 2) * 2 - 1;
            //int direction = (rand() % 2) * 2 - 1;
            int options[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
            int num_options = 4;
            for (int i = 0; i < 4; i++){
                if (contained_is_failure_state(options[i][0],options[i][1],clone)) {
                    options[i][0] = 0;
                    options[i][1] = 0;
                    num_options--;
                }
                if (debug) printf("Options: %d %d ",options[i][0],options[i][1]);
            }
            if (debug) printf("Num Options: %d\n",num_options);
            int axis = -1;
            int direction = -1;
            if (num_options > 0) {
                int random = rand() % num_options;
                int count = 0;
                for (int i = 0; i < 4; i++) {
                    if (!options[i][0] == 0) {
                        if (count == random) {
                            axis = options[i][0];
                            direction = options[i][1];
                            i = 4;
                        } else {
                            count++;
                        }
                    }
                }
            }

            new_step->axis = axis;
            new_step->direction = direction;
            push(steps, new_step);
            int result = contained_advance_frame(axis,direction,clone);
            if (result>1 && debug){
                debug_start=1;
                printf("\n\n\n\n\n\n");
            }
            if (debug && debug_start) printf("Frame advanced: \ndeltascore:%d\nmoogleFlag:%d\ntimeOut:%d\ncurrFrame:%d\n",
                    result,clone->moogleFlag,clone->timeOut,clone->currFrame);
            if (result == 0) {
                if (debug && debug_start) printf("Dead\n");
                //printf("%d",trials);
                dead = 1;
                //Free steps
                if (steps->size == 0){
                    free(steps);
                } else {
                    int last_entry = 0;
                    struct stack_entry* top = steps->top;
                    while (!last_entry){
                        struct stack_entry* next = top->next;
                        free(top->value);
                        free(top);
                        if (next != NULL){
                            top = next;
                        } else {
                            last_entry = 1;
                        }
                    }
                    free(steps);
                }
                //
                //printf("DEAD");
            }
        }
        delete_board(&clone);
        if (counter%100000==0){// no result after prev 100000 calculations
            sMax-=BOARD_SIZE*BOARD_SIZE/2; // make it harsher
        }
        if (sMax <= 0){
            struct stack* bad_steps = create_stack();
            struct step* bad_step = (struct step *) malloc(sizeof(step));
            bad_step->axis = -1;
            bad_step->direction = -1;
            push(bad_steps,bad_step);//Need to change this system eventually to live as long as possibles
            return bad_steps;
        }
    }
    //printf("counter: %d\n", counter);
    return steps;
}