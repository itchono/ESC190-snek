from snek import *
from time import sleep
import os
import random

import pickle

from ctypes import *


clear = lambda: os.system('cls')

'''
SNEK

Mingde Yin and Kamron Zaidi

Developed for ESC190

Current Version: 0.3 Alpha

'''

def draw_board(snake_state, tgt_state, f, seq, scores):
	print("Frame number {}\n".format(f+1))
	for i in range(BOARD_SIZE):
		for j in range(BOARD_SIZE):
			
			if tgt_state[f] and (j, i) == tgt_state[f]:
				print("X",end='')
			elif snake_state[f][i][j] == 1:
				print("S",end='')
			else:
				print("·",end='')
		print()
	print("CURRENT SCORE: {}".format_map(scores[i]))
	print("CURRENT MOVE: {}\nNEXT MOVE: {}".format(seq[f], seq[f+1]))


def replay_game(file_name):

	seq = None
	snakes = None
	tgts = None

	with open(file_name, 'rb') as f:
		dat = pickle.load(f)
		seq = dat["Sequence"]
		snakes = dat["Snakes"]
		tgts = dat["Targets"]
		scores = dat["Scores"]


	print("Replaying Game of Size {}\nSequence Buffer Size: {}".format(len(snakes), len(seq)))

	for i in range(len(snakes)):
		sleep(0.5)
		draw_board(snakes, tgts, i, seq, scores)

def kill_screen(file_name):
	'''
	Where were u when snek was kil

	I was at computer, coding algorithm

	When program show

	"Gam ovr"

	"No"

	--> Shows final frame before death
	'''
	seq = None
	snakes = None
	tgts = None
	scores = None

	with open(file_name, 'rb') as f:
		dat = pickle.load(f)
		seq = dat["Sequence"]
		snakes = dat["Snakes"]
		tgts = dat["Targets"]
		scores = dat["Scores"]


	print("Replaying Game of Size {}\nSequence Buffer Size: {}".format(len(snakes), len(seq)))
	draw_board(snakes, tgts, len(snakes)-1, seq, scores)


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

def main(dataCollectMode=False):
	SNAKE_STATES = [] # full board display
	TGT_STATES = [] # coordinates of food if applicable
	TOTAL_SEQ = ['START'] # sequence of total moves, set with START as the starting position
	SCORES = []

	# ========== EXISTING CODE ======
	#ptr to board
	board = init_board()

	#ptr to axes and direction
	axis = c_int(0)
	p_axis = cast(addressof(axis),POINTER(c_int))
	direction = c_int(0)
	p_direction = cast(addressof(direction),POINTER(c_int))
	
	play_on = 1
	
	if not dataCollectMode: show_board(board)

	# ========== /EXISTING CODE ======

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
		#clear()

		x_coord, y_coord = board[0].snek[0].head[0].coord[x], board[0].snek[0].head[0].coord[y] # get x, y

		if LOG_GAMES:
			M = [[0] * BOARD_SIZE for i in range(BOARD_SIZE)]
			for i in range(BOARD_SIZE):
				for j in range(BOARD_SIZE):
					if board[0].occupancy[i][j] == 1:
						M[i][j] = 1
					if i == x_coord and j == y_coord:
						M[i][j] = 'H' # overwrite with head
			SNAKE_STATES.append(M)
		# collects snake occupancy info
		
		if not dataCollectMode: print("{},{}".format(x_coord, y_coord))

		mooglex, moogley = locate_target()

		if LOG_GAMES and target_exists():
			TGT_STATES.append((mooglex, moogley))
		elif LOG_GAMES:
			TGT_STATES.append(None)

		SCORES.append(get_score())
		
		play_on = gameStep(p_axis, p_direction, board) # execute next move
		# also determine current input move

		TOTAL_SEQ.append(getDirection(axis, direction)) # append to current total moves

		if not dataCollectMode: print("\n\nNEXT FRAME:")
		if not dataCollectMode: show_board(board)
		#if not dataCollectMode: sleep(0.4)

	#pass by reference to clean memory
	score = get_score()
	end_game(byref(board))
	return (score, {"Sequence":TOTAL_SEQ, "Targets":TGT_STATES, "Snakes":SNAKE_STATES})

if __name__ == "__main__":

	TRIALS = 0
	NAME_EXT = ''

	# main function --> this allows some funky scoping stuff to work

	random.seed()

	response = ''

	while(response.lower() not in {'collect', 'collectlog', 'replay', 'play', 'lastframe'}):
		response = input("Mode Selection?\n[COLLECT] - data collection mode without game logs\n[COLLECTLOG] - log moves\n[REPLAY] - load data file\n[LASTFRAME] = print last frame\n[PLAY] - play normally\n")

	response = response.upper()

	DATA_COLLECT = False
	LOG_GAMES = False
	REPLAY = False
	LAST_FRAME = False

	if response == 'COLLECT':
		DATA_COLLECT = True

		while(not response.isnumeric()):
			response = input("Number of Trials?\n")
			TRIALS = int(response)

		NAME_EXT = input("File Prefix Name?\n") # common file prefix


	elif response == 'COLLECTLOG':
		DATA_COLLECT = True
		LOG_GAMES = True

		while(not response.isnumeric()):
			response = input("Number of Trials?\n")
			TRIALS = int(response)

		NAME_EXT = input("File Prefix Name?\n") # common file prefix

	elif response == 'REPLAY':
		REPLAY = True
		NAME_EXT = input("File Prefix Name?\n") # common file prefix

	elif response == 'LASTFRAME':
		REPLAY = True
		NAME_EXT = input("File Prefix Name?\n") # common file prefix
		LAST_FRAME = True

	elif response == 'PLAY':
		pass


	if DATA_COLLECT:
		# data collection mode
		with open(NAME_EXT+"_output.tsv", 'w') as f:
			f.write("Trial Number	Score\n" if not LOG_GAMES else "Trial Number	Score	Data Location\n")

			for i in range(TRIALS):
				seedRand(random.randint(0, 1000000))

				print("\nTrial {}/{} Completed ({}%)".format(i+1, TRIALS, 100*(i+1)/TRIALS))

				(score, dat) = main(dataCollectMode=True)

				if LOG_GAMES:
					f.write(str(i) + '\t' + str(score) + '\t' + NAME_EXT + 'data'+str(i)  +'\n')
					
					# I'M PICKLE RICK WUBBA LUBBA DUB DUB
					
					with open('data/'+NAME_EXT +'data'+str(i)+'.dat', 'wb') as datout:
						pickle.dump(dat, datout)
				else:
					f.write(str(i) + '\t' + str(score) + '\n')
	elif REPLAY:
		i = eval(input("Trial Number?\n")) # trial number

		if LAST_FRAME:
			kill_screen('data/'+NAME_EXT +'data'+str(i)+'.dat')
		else:
			replay_game('data/'+NAME_EXT +'data'+str(i)+'.dat')

	else:
		main() 
