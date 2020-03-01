from snek import *
from time import sleep
import os
clear = lambda: os.system('cls')



'''
SNEK

Mingde Yin and Kamron Zaidi

Developed for ESC190

Current Version: 0.2 Alpha

'''

DATA_COLLECT = True
CONTROL_DISABLE = False
TEST_SEQ = ['RIGHT', 'WAIT', 'WAIT', 'WAIT', 'WAIT', 'DOWN', 'WAIT', 'WAIT', 'WAIT', "LEFT", 'WAIT', 'WAIT', 'WAIT', 'WAIT', 'DOWN', 'WAIT', 'WAIT']

def main(dataCollectMode=False):
	# ========== EXISTING CODE ======
	#ptr to board
	board = init_board()
	
	play_on = 1
	show_board(board)
	axis = AXIS_INIT
	direction = DIR_INIT

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

	
	inputSequence = [] if not (CONTROL_DISABLE) else TEST_SEQ # sequence of inputs (queue)

	'''
	Allowable values:
	LEFT
	RIGHT
	UP
	DOWN
	WAIT
	'''

	'''
	Direction indicators and givers
	'''
	# direction checkers
	def going_left():
		return (direction == LEFT and axis == AXIS_X)
	def going_right():
		return (direction == RIGHT and axis == AXIS_X)
	def going_up():
		return (direction == UP and axis == AXIS_Y)
	def going_down():
		return (direction == DOWN and axis == AXIS_Y)

	# directional commandments
	
	def go_right():
		nonlocal axis
		nonlocal direction
		axis = AXIS_X
		direction = RIGHT

	def go_left():
		nonlocal axis
		nonlocal direction
		axis = AXIS_X
		direction = LEFT

	def go_down():
		nonlocal axis
		nonlocal direction
		axis = AXIS_Y
		direction = DOWN

	def go_up():
		nonlocal axis
		nonlocal direction
		axis = AXIS_Y
		direction = UP

	def wait():
		pass


	def addToSequence(direction=None, delay=0):
		'''
		Adds a direction input to the queue after n-inputs
		--> Accepts shorthand or long notation (L = Left, R = Right, U = Up, D = Down)

		Can insert delay or not, as well as specify direction or not

		'''

		if not CONTROL_DISABLE:
			# allows custom move input
			if delay:
				inputSequence.extend(['WAIT'] * delay) # add delay steps to thingy

			if direction and (direction in {'LEFT', 'RIGHT', 'UP', 'DOWN'} or direction in {'L', 'R', 'U', 'D'}):
				
				direction = {'L':'LEFT', 'R':'RIGHT', 'U':'UP', 'D':'DOWN'}[direction] if (not (direction in {'LEFT', 'RIGHT', 'UP', 'DOWN'}) and direction in {'L', 'R', 'U', 'D'}) else direction # convert shorthand to longhand notation
				
				inputSequence.append(direction)

			elif direction:
				# this should NOT happen
				print("Unexpected input! {}".format(direction))

	def inputSequencer(inputSequence):
		'''
		Apply next action of input sequence. SHOULD only be called once per frame at most.
		'''
		# seems illegal but it works so well (Python is amazing)
		{'LEFT':go_left, 'RIGHT':go_right, 'UP':go_up, 'DOWN':go_down, 'WAIT':wait}[inputSequence.pop(0)]()
		
	
	def atBoundary(x_coord, y_coord):
		'''
		Determines if snake is at boundary, and thus would need to save itself before targeting
		'''
		return (going_up() and y_coord == 0) or (going_down() and y_coord == BOARD_SIZE-1) or (going_left() and x_coord == 0) or (going_right() and x_coord == BOARD_SIZE-1)

	'''
	Game control

	'''
	while (play_on):
		#clear()

		# ========== EXISTING CODE ======
		#indexing at 0 dereferences the pointer
		x_coord, y_coord = board[0].snek[0].head[0].coord[x], board[0].snek[0].head[0].coord[y]
		# ========== EXISTING CODE ======

		if not dataCollectMode: print("{},{}".format(x_coord, y_coord))

		mooglex, moogley = locate_target()
		
		if target_exists() and not(atBoundary(x_coord, y_coord)) and not ((x_coord, y_coord) == (mooglex, moogley)):
			# Also: check that we're not on top of the target
			# Else: it will add one extra move for when we are on top of the target, causing death

			if not dataCollectMode: print("STATE: Targeting\nTarget Position: {}, {}".format(mooglex, moogley))

			d = None # default value
			
			'''
			Preliminary targeting algorithms
			'''
			if moogley < y_coord and not(going_down()) and not(going_up()):
				d = 'UP'

			elif moogley > y_coord and not(going_down()) and not(going_up()):
				d = 'DOWN'

			elif mooglex > x_coord and not(going_left()) and not(going_right()):
				d = 'RIGHT'

			elif mooglex < x_coord and not(going_left()) and not(going_right()):
				d = 'LEFT'

			if not dataCollectMode: print("Choice: {}".format(d))

			addToSequence(direction=d, delay=(0 if d else 1))

		else:
			# Obstacle avoidance (default pattern state)

			if (going_up() and y_coord == 0) or (going_down() and y_coord == BOARD_SIZE-1):
				if not dataCollectMode: print("HAZARD AVOIDANCE: CHANGE DIRECTION ({})".format('R' if x_coord < BOARD_SIZE // 2 else 'L'))
				addToSequence('R' if x_coord < BOARD_SIZE // 2 else 'L')

			elif (going_left() and x_coord == 0) or (going_right() and x_coord == BOARD_SIZE-1):
				if not dataCollectMode: print("HAZARD AVOIDANCE: CHANGE DIRECTION ({})".format('D' if y_coord < BOARD_SIZE // 2 else 'U'))
				addToSequence('D' if y_coord < BOARD_SIZE // 2 else 'U')
			else:
				if not dataCollectMode: print("HAZARD AVOIDANCE: Nothing to change")
				addToSequence(delay=1) # empty move

		# INPUT SEQUENCER
		inputSequencer(inputSequence)

		# ==== EXISTING CODE
		play_on = advance_frame(axis, direction, board)

		if not dataCollectMode: print("\n\nNEXT FRAME:")

		if not dataCollectMode: show_board(board)
		if not dataCollectMode: sleep(0.4)

		# ========== EXISTING CODE ======
	
	#pass by reference to clean memory	
	return get_score()
	end_game(byref(board))


if __name__ == "__main__":

	# main function --> this allows some funky scoping stuff to work

	if DATA_COLLECT:

		with open("test_output.csv", 'w') as f:

			f.write("Trial Number,Final Score\n")

			for i in range(20):
				f.write(str(i) + ",")

				score = main(dataCollectMode=True)
				f.write(str(score) + "\n")

	else:
		main() 