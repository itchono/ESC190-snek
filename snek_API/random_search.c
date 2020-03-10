//#include "snek_api.h"
//#include "generic_stack.h"
#include "random_search.h"
#include <time.h>
//Outputs
/*struct stack* random_search(GameBoard* board) {
    int debug = 0;
    //srand(time(0));
    //struct stack* layer = create_stack();
    int highest_score = -1;
    struct stack *highest_steps = NULL;
    for (int trials = 0; trials < 1000000; trials++) {//50000
        if (debug) printf("Trial %d\n", trials);
        int alive = 1;
        struct stack *steps = create_stack();
        if (debug) printf("stack created\n");
        struct GameBoard *clone = clone_board(board);
        if (debug) printf("board cloned\n");
        int delta_score = 0;//((clone->timeOut) - (clone->currFrame))
        for (int s = 0; s < 100 && alive; s++) {//100
            if (debug) printf("Step %d\n",s);
            struct step *new_step = (struct step *) malloc(sizeof(step));
            if (debug) printf("Created step\n");
            int axis = (rand() % 2) * 2 - 1;
            int direction = (rand() % 2) * 2 - 1;
            new_step->axis = axis;
            new_step->direction = direction;
            push(steps, new_step);
            if (debug) printf("Added step to stack\n");
            int result = contained_advance_frame(axis,direction,clone);
            delta_score += result;
            if (debug) printf("Frame advanced\n");
            if (result == 0) {
                //printf("%d",trials);
                alive = 0;
                if (s < 1 && trials!=-1) {
                    trials--;
                }
            }
        }
        //printf("final score: %d || ",clone->score);
        if (delta_score > highest_score) {
            if (debug) printf("New highest\n");
            highest_score = delta_score;
            if (debug) printf("Highest score replaced\n");
            if (highest_steps != NULL) {
                if (debug) printf("Not null\n");
                //Free highest steps
                if (highest_steps->size == 0) {
                    if (debug) printf("0\n");
                    free(highest_steps);
                } else {
                    if (debug) printf("Non 0\n");
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
                }
                if (debug) printf("Freed highest steps\n");
            }
            //
            highest_steps = steps;
            if (debug) printf("Highest steps replaced\n");
        } else {
            if (debug) printf("Not highest\n");
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
            if (debug) printf("Freed steps");
        }
        if (debug) printf("Freeing board...\n");
        delete_board(&clone);
        if (debug) printf("Freed board");
    }
    if (debug) printf("Highest Score: %d, currFrame: %d/%d", highest_score,board->currFrame,board->timeOut);
    return highest_steps;
}*/




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
            /*
            if (clone->snek->length == 1) {
                axis = (rand() % 2) * 2 - 1;
                direction = (rand() % 2) * 2 - 1;
            } else {
                int random = rand() % 3;
                if (random == 0) {
                    axis = -(clone->snek->axis);
                    direction = -1;
                } else if (random == 1) {
                    axis = -(clone->snek->axis);
                    direction = 1;
                } else {
                    axis = clone->snek->axis;
                    direction = clone->snek->direction;
                }
            }*/

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
    //printf("counter: %d\n", counter);
    return steps;
}