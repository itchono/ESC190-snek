// Randomized Uninformed Depth First Search 
// Kamron Zaidi and Mingde Yin

#include "random_search.h"
#include <unistd.h>
#include <time.h>
//#include <math.h>
//Outputs
//#define CALC_MAX (((BOARD_SIZE * 4) - 4) * CYCLE_ALLOWANCE + 2) > BOARD_SIZE*BOARD_SIZE ? (((BOARD_SIZE * 4) - 4) * CYCLE_ALLOWANCE + 2) : BOARD_SIZE*BOARD_SIZE
#define CALC_MAX ((BOARD_SIZE * 4) - 4) * CYCLE_ALLOWANCE + BOARD_SIZE*BOARD_SIZE/4
// choose the larger variant of the two
//#define MAX_POPULATE 50
//#define SEARCH_LIMIT (CALC_MAX) * 1000;
//#define SEARCH_LIMIT (int)(pow(3,CALC_MAX/104.0)*100000)//int division is debatable
#define SEARCH_LIMIT CALC_MAX*1000//530710

//int CYCLE_ALLOWANCE;
//int BOARD_SIZE;

int dead_stack;//Try this to make extern work
int populate_around_count;

struct stack* random_search_cant_die(GameBoard* board) {
    dead_stack = 0;
    int debug_start = 0;
    int debug = 0;
    //srand(time(0));
    //struct stack* layer = create_stack();
    int dead = 1;//Is the snake dead?
    int sMax = CALC_MAX; // EDITED to work with more boards
    struct stack* steps;//Store the moves while randomly exploring
    struct stack* highest_steps = create_stack();//Might need this because it will be freed in the first instance
    int highest_score = -1;
    int counter = 0;//Number of times the algorithm has failed to find a living path
    int MAX_POPULATE = (BOARD_SIZE*BOARD_SIZE-board->mooglesEaten)/2;
    if (MAX_POPULATE == 0){
        MAX_POPULATE = 1;
    }
    int decrease_mp = SEARCH_LIMIT/MAX_POPULATE;
    while (dead){//The algorithm will keep looking until it survives for sMax moves
        counter++;
        dead = 0;
        steps = create_stack();
        struct GameBoard *clone = clone_board(board);
        populate_around_count = 0;
        for (int s = 0; (s < sMax && !(dead)); s++) {//Plan a path of length sMax
            struct step *new_step = (struct step *) malloc(sizeof(step));
            //int axis = (rand() % 2) * 2 - 1;
            //int direction = (rand() % 2) * 2 - 1;
            int options[4][2] = {{-1, -1},
                                 {-1, 1},
                                 {1,  -1},
                                 {1,  1}};
            int num_options = 4;
            //Remove all options that lead to death
            for (int i = 0; i < 4; i++) {
                if (contained_is_failure_state(options[i][0], options[i][1], clone)) {
                    options[i][0] = 0;
                    options[i][1] = 0;
                    num_options--;
                }
                if (debug) printf("Options: %d %d ", options[i][0], options[i][1]);
            }
            if (debug) printf("Num Options: %d\n", num_options);
            int axis = -1;
            int direction = -1;
            //If there are no options, the snake will move towards -1, -1
            if (num_options > 0) {
                //If there are options available, the algorithm will randomly select one
                int random = rand() % num_options;
                int count = 0;
                for (int i = 0; i < 4; i++) {
                    if (!(options[i][0] == 0)) {
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
            //Add the randomly selected move to the stack
            new_step->axis = axis;
            new_step->direction = direction;
            push(steps, new_step);
            int result;
            //if (clone->moogleFlag == 1 && populate_around_count < 3) {
            //printf("Popcount: %d || Currframe: %d\n", populate_around_count,clone->currFrame);
            if (populate_around_count < MAX_POPULATE) {
                result = populate_around_advance_frame(axis, direction, clone);
                /*
                if (start_counting == 1){
                    populate_around_count++;
                }
                if ((result > LIFE_SCORE)||(result == LIFE_SCORE + MOOGLE_POINT)||(result == LIFE_SCORE + MOOGLE_POINT*HARRY_MULTIPLIER)){
                    start_counting = 1;
                }*/
                //if (clone->score > 5000)
                    //printf("Pop Around || count: %d || currFrame: %d || moogle_flag: %d\n\n\n\n\n\n\n\n\n",populate_around_count,clone->currFrame,clone->moogleFlag);
            } else if (clone->moogleFlag==1){
                reset_populate_around(clone);//ree2
                result = contained_advance_frame(axis, direction, clone);//ree2
                //result = populate_around_advance_frame(axis,direction,clone);//ree2
                //if (clone->score > 5000)
                    //printf("Resetting || count: %d || currFrame: %d || moogle_flag: %d\n\n\n\n\n\n\n\n\n",populate_around_count,clone->currFrame,clone->moogleFlag);
            } else {
                result = contained_advance_frame(axis, direction, clone);//ree2
                //result = populate_around_advance_frame(axis,direction,clone);//ree2
                //if (clone->score > 5000)
                    //printf("Contained Advance Frame || count: %d || currFrame: %d || moogle_flag: %d\n",populate_around_count,clone->currFrame,clone->moogleFlag);
            }
            if (result>1 && debug){
                debug_start=1;
                printf("\n\n\n\n\n\n");
            }
            if (debug && debug_start) printf("Frame advanced: \ndeltascore:%d\nmoogleFlag:%d\ntimeOut:%d\ncurrFrame:%d\n",
                    result,clone->moogleFlag,clone->timeOut,clone->currFrame);

            //If the path leads to death
            if (result == 0) {
                if (debug && debug_start) printf("Dead\n");
                dead = 1;
                if (clone->score > highest_score){
                    highest_score = clone->score;
                    //Free previous highest_steps;
                    /*
                    if (highest_steps->size == 0) {
                        free(highest_steps);
                    } else {
                        int last_entry = 0;
                        struct stack_entry *top = highest_steps->top;
                        while (!last_entry) {
                            struct stack_entry *next = top->next;
                            free(top->value);
                            free(top);
                            if (next != NULL) {
                                top = next;
                            } else {
                                last_entry = 1;
                            }
                        }
                        free(highest_steps);
                    }*/
                    delete_step_stack(highest_steps);
                    highest_steps = steps;
                } else {
                    //Free steps if its not the highest
                    /*
                    if (steps->size == 0) {
                        free(steps);
                    } else {
                        int last_entry = 0;
                        struct stack_entry *top = steps->top;
                        while (!last_entry) {
                            struct stack_entry *next = top->next;
                            free(top->value);
                            free(top);
                            if (next != NULL) {
                                top = next;
                            } else {
                                last_entry = 1;
                            }
                        }
                        free(steps);
                    }*/
                    delete_step_stack(steps);
                }
            }
        }
        delete_board(&clone);

        //int SEARCH_LIMIT = (CALC_MAX) * 1000;//Note: might need to increase this to get better results

        /*
        if (counter%SEARCH_LIMIT==0){// no result after prev 100000 calculations
            sMax -= CALC_MAX/2; // decrease depth
        }
        if (sMax <= 0){
            struct stack* bad_steps = create_stack();
            struct step* bad_step = (struct step *) malloc(sizeof(step));
            bad_step->axis = -1;
            bad_step->direction = -1;
            push(bad_steps,bad_step);//Need to change this system eventually to live as long as possibles
            return bad_steps;
        }*/
        if (counter%decrease_mp == 0){
            MAX_POPULATE--;
            //printf("%d",MAX_POPULATE);
        }
        if (counter > SEARCH_LIMIT){
            if (dead == 1){
                dead_stack = 1;
                //printf("Highest score: %d", highest_score);
                //usleep(1000);
                return highest_steps;
            }
        }
    }
    //Path found that survived sMax steps
    //Free highest_steps
    /*
    if (highest_steps->size == 0) {
        free(highest_steps);
    } else {
        int last_entry = 0;
        struct stack_entry *top = highest_steps->top;
        while (!last_entry) {
            struct stack_entry *next = top->next;
            free(top->value);
            free(top);
            if (next != NULL) {
                top = next;
            } else {
                last_entry = 1;
            }
        }
        free(highest_steps);
    }*/
    delete_step_stack(highest_steps);
    dead_stack = 0;
    return steps;
}

void delete_step_stack(struct stack* steps){
    if (steps->size == 0) {
        free(steps);
    } else {
        int last_entry = 0;
        struct stack_entry *top = steps->top;
        while (!last_entry) {
            struct stack_entry *next = top->next;
            free(top->value);
            free(top);
            if (next != NULL) {
                top = next;
            } else {
                last_entry = 1;
            }
        }
        free(steps);
    }
}