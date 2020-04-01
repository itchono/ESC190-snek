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
    int sMax = 100;
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
        //printf("%d",dead);
        delete_board(&clone);
        if (counter%100000==0){//1000000
            //printf("Mill Checkpoint\n");
            sMax-=50;
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
    printf("counter: %d\n", counter);
    return steps;
}

int* random_search_future(GameBoard* board) {
    int debug_start = 0;
    int debug = 0;
    int highestAliveCount = -1;
    int* bestMove = (int*)(malloc(sizeof(int)*2));

    int main_options[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
    int main_num_options = 4;
    for (int i = 0; i < 4; i++) {
        if (contained_is_failure_state(main_options[i][0], main_options[i][1], board)) {
            main_options[i][0] = 0;
            main_options[i][1] = 0;
            main_num_options--;
        }
    }
    if (main_num_options > 0) {
        for (int m = 0; m < 4; m++) {
            if (!(main_options[m][0] == 0)) {
                if (debug) printf("Head currently at %d,%d. Curr Frame at %d\n",board->snek->head->coord[0],board->snek->head->coord[1],board->currFrame);
                if (debug) printf("analysing option %d:%d. Will fail: %d\n",main_options[m][0],main_options[m][1],contained_is_failure_state(main_options[m][0], main_options[m][1], board));
                GameBoard* primary_clone = clone_board(board);
                contained_advance_frame(main_options[m][0], main_options[m][1], primary_clone);//might want to use populate

                int aliveCount = 0;
                int numTrials = 25;
                int max_counter = 100000;
                for (int trials = 0; trials < numTrials; trials++){
                    //if (debug) printf("starting trial %d\n",trials);
                    aliveCount++;

                    int dead = 1;
                    int sMax = 150;
                    int counter = 0;

                    while (dead){
                        //if (debug) printf("trying not to die\n");
                        counter++;
                        dead = 0;
                        struct GameBoard *clone = clone_board(primary_clone);
                        for (int s = 0; (s < sMax && !(dead)); s++) {//100
                            int options[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
                            int num_options = 4;
                            for (int i = 0; i < 4; i++){
                                if (contained_is_failure_state(options[i][0],options[i][1],clone)) {
                                    options[i][0] = 0;
                                    options[i][1] = 0;
                                    num_options--;
                                }
                            }
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

                            int result = populate_advance_frame(axis,direction,clone);
                            if (result == 0) {
                                //if (debug) printf("died\n");
                                dead = 1;
                            }
                        }
                        delete_board(&clone);
                        if (counter>max_counter){//1000000
                            if (debug) printf("exceeded 100000 without surviving, failed\n");
                            aliveCount--;
                            break;
                        }
                    }
                }
                if (debug) printf("Option had a score of %d/%d\n",aliveCount,numTrials);
                if (aliveCount > highestAliveCount){
                    highestAliveCount = aliveCount;
                    bestMove[0] = main_options[m][0];
                    bestMove[1] = main_options[m][1];
                    if (aliveCount==numTrials){
                        m=4;
                    }
                }
                delete_board(&primary_clone);
            }
        }
    }
    return bestMove;
}