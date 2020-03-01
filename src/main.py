from snek import *
from time import sleep

if __name__ == "__main__":
	# ========== EXISTING CODE ======
	#ptr to board
	board = init_board()
	
	play_on = 1
	show_board(board)
	axis = AXIS_INIT
	direction = DIR_INIT

	# ========== /EXISTING CODE ======

	
			
	while (play_on):

		# ========== EXISTING CODE ======
		#indexing at 0 dereferences the pointer
		x_coord, y_coord = board[0].snek[0].head[0].coord[x], board[0].snek[0].head[0].coord[y]
		# ========== EXISTING CODE ======

		mooglex, moogley = -1, -1 # position of target
		
		for j in range(BOARD_SIZE):
			for i in range(BOARD_SIZE):
				if board[0].cell_value[j][i] != 0: # REMEMBER TO DEREFERENCE THE POINTER
					mooglex, moogley = i, j
		
		
		
		
		if mooglex != -1:
			print("{}, {}".format(mooglex, moogley))

			if y_coord - moogley > 0:
				print("UP")
				axis = AXIS_Y
				direction = UP
			elif y_coord - moogley < 0:
				print("DOWN")
				axis = AXIS_Y
				direction = DOWN
			elif x_coord - mooglex < 0:
				print("RIGHT")
				axis = AXIS_X
				direction = RIGHT
			elif x_coord - mooglex > 0:
				print("LEFT")
				axis = AXIS_X
				direction = LEFT
			elif y_coord - moogley < 0:
				print("UP")
				axis = AXIS_Y
				direction = UP
		else:
			# idle hold pattern
			print("IDLING")
				
			if axis == AXIS_Y and ((direction == UP and y_coord == 0) or (direction == DOWN and y_coord == BOARD_SIZE-1)):
				axis = AXIS_X
				direction = RIGHT if x_coord < BOARD_SIZE // 2 else LEFT
			elif axis == AXIS_X and ((direction == LEFT and x_coord == 0) or (direction == RIGHT and x_coord == BOARD_SIZE-1)):
				axis = AXIS_Y
				direction = DOWN if y_coord < BOARD_SIZE // 2 else UP
		
		# ==== EXISTING CODE
		play_on = advance_frame(axis, direction, board)
		show_board(board)
		sleep(0.65)

		# ========== EXISTING CODE ======
	
	#pass by reference to clean memory	
	end_game(byref(board))
