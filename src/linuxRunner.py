from snek import *
from time import *
import random

import pickle

from ctypes import *

'''
SNEK - STRIPPED DOWN LINUX TESTER v1.0

Mingde Yin and Kamron Zaidi

Developed for ESC190
'''

def getDirection(axis, direction):
	if (axis == -1):
		if (direction == -1):
			return "LEFT"
		elif (direction == 1):
			return "RIGHT"

	elif(axis == 1):

		if (direction == -1):
			return "UP"
		elif (direction == 1):
			return "DOWN"
	# converts variables to a word direction

def main():
	SNAKE_STATES = [] # full board display
	TGT_STATES = [] # coordinates of food if applicable
	TOTAL_SEQ = ['START'] # sequence of total moves, set with START as the starting position
	SCORES = []
	MOOGLES = []
	CURR_FRAME = []
	TIME_OUT = []

	# var inits
	board = init_board()
	axis = c_int(0)
	p_axis = cast(addressof(axis),POINTER(c_int))
	direction = c_int(0)
	p_direction = cast(addressof(direction),POINTER(c_int))
	
	play_on = 1

	seedRand(random.randint(0, 1000000))

	mooglex, moogley = -1, -1 # position of target --> SET to -1, -1 as default when there is NO TARGET

	def target_exists():
		'''
		checks if target exists
		'''
		return mooglex != -1 and moogley != -1

	def locate_target():
		'''
		Locates target
		'''
		for j in range(BOARD_SIZE):
			for i in range(BOARD_SIZE):
				if board[0].cell_value[j][i] != 0: # REMEMBER TO DEREFERENCE THE POINTER
					return (i, j)
		return (-1, -1)

	'''
	Game control
	'''

	while (play_on):
		x_coord, y_coord = board[0].snek[0].head[0].coord[x], board[0].snek[0].head[0].coord[y] # get x, y
		tail_x, tail_y = board[0].snek[0].tail[0].coord[x], board[0].snek[0].tail[0].coord[y]

		M = [[0] * BOARD_SIZE for i in range(BOARD_SIZE)]
		for i in range(BOARD_SIZE):
			for j in range(BOARD_SIZE):
				if board[0].occupancy[i][j] == 1:
					M[i][j] = 1
				if j == tail_x and i == tail_y:
					M[i][j] = 'T' # overwrite with tail
				if j == x_coord and i == y_coord:
					M[i][j] = 'H' # overwrite with head
		SNAKE_STATES.append(M)
		# collects snake occupancy info

		mooglex, moogley = locate_target()
		
		TGT_STATES.append((mooglex, moogley) if target_exists() else None)
		SCORES.append(get_score())
		MOOGLES.append(get_moogles_eaten())
		CURR_FRAME.append(get_curr_frame())
		TIME_OUT.append(get_time_out())

		play_on = gameStep(p_axis, p_direction, board) 

		TOTAL_SEQ.append(getDirection(axis.value, direction.value)) # append to current total moves

	score = get_score()
	end_game(byref(board))
	return (score, {"Sequence":TOTAL_SEQ, "Targets":TGT_STATES, "Snakes":SNAKE_STATES, "Scores":SCORES, "Moogles":MOOGLES, "Frames":CURR_FRAME, "Timeout":TIME_OUT})

if __name__ == "__main__":

	print("KM SNEK - DATA COLLECTOR MODE")

	TRIALS = 0
	NAME_EXT = ''

	# main function --> this allows some funky scoping stuff to work

	random.seed()

	response = ''

	while(not response.isnumeric()):
		response = input("Number of Trials?\n")
		TRIALS = int(response)

	NAME_EXT = input("File Prefix Name?\n") # common file prefix

	# data collection mode
	with open(NAME_EXT+"_output.tsv", 'a') as f:
		f.write("Trial Number	Score	Moogles Eaten	Time Taken	Data Location\n")

	for i in range(TRIALS):
		with open(NAME_EXT+"_output.tsv", 'a') as f:

			t_start = time()

			(score, dat) = main()

			t_end = time()

			print("\nTrial {}/{} Completed in {} seconds. Progress {}%".format(i+1, TRIALS, t_end-t_start, 100*(i+1)/TRIALS))

			f.write(str(i) + '\t' + str(score) + '\t' + str(dat["Moogles"][-1]) + '\t' + str(t_end-t_start) + '\t' + NAME_EXT + 'data'+str(i)  +'\n')
			
			# I'M PICKLE RICK WUBBA LUBBA DUB DUB
			

			with open('data/'+NAME_EXT +'data'+str(i)+'.dat', 'wb') as datout:
				pickle.dump(dat, datout)

