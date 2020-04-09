from snek import *
from time import *
import random
import pygame
import pickle
from ctypes import *

'''
RUDA - Randomized Uninformed Depth First Algorithm

Mingde Yin and Kamron Zaidi

Developed for ESC190

Version Beta 1.2

CHANGELOG:
Beta 1.2
- Navigation Algorithm V3

Beta 1.1
- Cleanup and test-ready

Beta 1.0
- Pygame

Alpha 0.4
- Navigation Algorithm V2

Alpha 0.3
- Navigation Algorithm V1

Alpha 0.2
- UI Wrapping and Replay functionality

Alpha 0.1
- Basic algorithm

'''
# drawing constants
OFFSET = 50
SIZE = (500 - 2*OFFSET) / BOARD_SIZE

def draw_board(SNAKE_STATES, TGT_STATES, f, MOVE, NEXT, SCORE, MOOGLES, screen, CURR_FRAME, TIME_OUT):
	print("Frame number {}\n".format(f+1))
	pygame.display.set_caption('KM Snake - Frame Number {}'.format(f+1)) 

	# Fill the background with white
	screen.fill((20, 20, 20))

	for event in pygame.event.get(): 
		if event.type == pygame.QUIT: pygame.quit()

	for i in range(BOARD_SIZE):
		for j in range(BOARD_SIZE):
			if TGT_STATES and (j, i) == TGT_STATES and SNAKE_STATES[i][j]:
				pygame.draw.rect(screen, (0, 150, 255), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))

			elif TGT_STATES and (j, i) == TGT_STATES:
				# food
				pygame.draw.rect(screen, (0, 0, 255), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))
			elif SNAKE_STATES[i][j] == 'H':
				pygame.draw.rect(screen, (0, 200, 0), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))

				d = {"LEFT":(OFFSET+SIZE*j, OFFSET + SIZE*i + SIZE/8, SIZE/4, SIZE/4), 
				"RIGHT":(OFFSET+SIZE*j + 3*SIZE/4, OFFSET + SIZE*i + 5*SIZE/8, SIZE/4, SIZE/4),
				"UP":(OFFSET+SIZE*j + 5*SIZE/8, OFFSET + SIZE*i , SIZE/4, SIZE/4),
				"DOWN":(OFFSET+SIZE*j + SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4),
				"START":(OFFSET+SIZE*j + SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4)}[MOVE]
				# seeker head

				z = {"LEFT":(OFFSET+SIZE*j, OFFSET + SIZE*i + 5*SIZE/8, SIZE/4, SIZE/4), 
				"RIGHT":(OFFSET+SIZE*j + 3*SIZE/4, OFFSET + SIZE*i + SIZE/8, SIZE/4, SIZE/4),
				"UP":(OFFSET+SIZE*j + SIZE/8, OFFSET + SIZE*i , SIZE/4, SIZE/4),
				"DOWN":(OFFSET+SIZE*j + 5*SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4),
				"START":(OFFSET+SIZE*j + 5*SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4)}[MOVE]
				# seeker head

				e = {"LEFT":(OFFSET+SIZE*j-SIZE/2, OFFSET + SIZE*i + 3*SIZE/8, SIZE/2, SIZE/4), 
				"RIGHT":(OFFSET+SIZE*j + SIZE, OFFSET + SIZE*i + 3*SIZE/8, SIZE/2, SIZE/4),
				"UP":(OFFSET+SIZE*j + 3*SIZE/8, OFFSET + SIZE*i - SIZE/2, SIZE/4, SIZE/2),
				"DOWN":(OFFSET+SIZE*j + 3*SIZE/8, OFFSET + SIZE*i + SIZE, SIZE/4, SIZE/2),
				"START":(OFFSET+SIZE*j + 3*SIZE/8, OFFSET + SIZE*i + SIZE, SIZE/4, SIZE/2)}[MOVE]
				# current move

				tgt = {"LEFT":(OFFSET+SIZE*j-int(SIZE/2), OFFSET + SIZE*i + int(SIZE/2)), 
				"RIGHT":(OFFSET+SIZE*j + SIZE + int(SIZE/2), OFFSET + SIZE*i + int(SIZE/2)),
				"UP":(OFFSET+SIZE*j + int(SIZE/2), OFFSET + SIZE*i - int(SIZE/2)),
				"DOWN":(OFFSET+SIZE*j + int(SIZE/2), OFFSET + SIZE*i + SIZE + int(SIZE/2))}[NEXT]

				pygame.draw.circle(screen, (150, 50, 0, 100), [int(x) for x in tgt], int(SIZE/2)) # looking square
				pygame.draw.rect(screen, (150, 0, 0, 100), e) # current move
				pygame.draw.rect(screen, (0, 0, 0, 100), d) # next move eyes 1
				pygame.draw.rect(screen, (0, 0, 0, 100), z) # next move eyes 2


			elif SNAKE_STATES[i][j] == 'T':
				pygame.draw.rect(screen, (0, 100, 0), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))
			elif SNAKE_STATES[i][j] == 1:
				pygame.draw.rect(screen, (0, 150, 0), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))
			
			pygame.draw.rect(screen, (120, 120, 120), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE), 1) # outline

			font = pygame.font.Font('freesansbold.ttf', 22)
			text = font.render("CURRENT SCORE: {}".format(SCORE), True, (200, 200, 200))
			
			try:
				text2 = font.render("MOOGLES EATEN: {}".format(MOOGLES), True, (200, 200, 200))
				text4 = font.render("TIME: {}/{}".format(CURR_FRAME, TIME_OUT), True, (200, 200, 200))
			except Exception:
				pass
			text3 = font.render("NEXT MOVE: {}".format(NEXT), True, (200, 200, 200))
			

			r1 = text.get_rect()
			try: 
				r2 = text2.get_rect()
				r4 = text4.get_rect()
			except Exception:
				pass
			r3 = text3.get_rect()

			r1.topleft = (OFFSET, 500)
			try: 
				r2.topleft = (OFFSET, 525)
				r4.topleft = (OFFSET, 575)
			except Exception:
				pass
			r3.topleft = (OFFSET, 550)

			screen.blit(text, r1)
			try: 
				screen.blit(text2, r2)
				screen.blit(text4, r4)
			except Exception:
				pass
			screen.blit(text3, r3)

	# Flip the display
	pygame.display.flip()


	print("CURRENT SCORE: {}".format(SCORE))
	if MOOGLES: print("MOOGLES: {}".format(MOOGLES))
	print("CURRENT MOVE: {}\nNEXT MOVE: {}".format(MOVE, NEXT))


def replay_game(file_name):

	delay = eval(input("Delay between moves? (ms)\n"))

	pygame.init()

	# Set up the drawing window
	screen = pygame.display.set_mode([100 + 40*BOARD_SIZE, 200 + 40*BOARD_SIZE])
	pygame.display.set_caption('KM Snek') 

	with open(file_name, 'rb') as f:
		dat = pickle.load(f)
		TOTAL_SEQ = dat["Sequence"] if "Sequence" in dat else None
		SNAKE_STATES = dat["Snakes"] if "Snakes" in dat else None
		TGT_STATES = dat["Targets"] if "Targets" in dat else None
		SCORES = dat["Scores"] if "Scores" in dat  else None
		MOOGLES = dat["Moogles"] if "Moogles" in dat else None
		CURR_FRAME = dat["Frames"] if "Frames" in dat else None
		TIME_OUT = dat["Timeout"] if "Timeout" in dat else None

		print("Replaying Game of Size {}".format(len(SNAKE_STATES)))

		for frame in range(len(SNAKE_STATES)):

			#draw_board(SNAKE_STATES[i], TGT_STATES[i], i, TOTAL_SEQ[i], TOTAL_SEQ[i+1], SCORES[i], MOOGLES[i], screen, CURR_FRAME[i], TIME_OUT[i])	

			# Fill the background with white
			screen.fill((20, 20, 20))

			pygame.display.set_caption('KM Snake [REPLAY] - Frame Number {}/{}'.format(frame+1, len(SNAKE_STATES))) 
			
			for event in pygame.event.get(): 
				if event.type == pygame.QUIT: pygame.quit()

			for i in range(BOARD_SIZE):
				for j in range(BOARD_SIZE):
					if TGT_STATES and TGT_STATES[frame] and (j, i) == TGT_STATES[frame] and SNAKE_STATES[frame][i][j]:
						pygame.draw.rect(screen, (0, 150, 255), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))

					elif TGT_STATES and TGT_STATES[frame] and (j, i) == TGT_STATES[frame]:
						# food
						pygame.draw.rect(screen, (0, 0, 255), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))
					elif SNAKE_STATES[frame][i][j] == 'H':
						pygame.draw.rect(screen, (0, 200, 0), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))

						d = {"LEFT":(OFFSET+SIZE*j, OFFSET + SIZE*i + SIZE/8, SIZE/4, SIZE/4), 
						"RIGHT":(OFFSET+SIZE*j + 3*SIZE/4, OFFSET + SIZE*i + 5*SIZE/8, SIZE/4, SIZE/4),
						"UP":(OFFSET+SIZE*j + 5*SIZE/8, OFFSET + SIZE*i , SIZE/4, SIZE/4),
						"DOWN":(OFFSET+SIZE*j + SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4),
						"START":(OFFSET+SIZE*j + SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4)}[TOTAL_SEQ[frame]]
						# seeker head

						f = {"LEFT":(OFFSET+SIZE*j, OFFSET + SIZE*i + 5*SIZE/8, SIZE/4, SIZE/4), 
						"RIGHT":(OFFSET+SIZE*j + 3*SIZE/4, OFFSET + SIZE*i + SIZE/8, SIZE/4, SIZE/4),
						"UP":(OFFSET+SIZE*j + SIZE/8, OFFSET + SIZE*i , SIZE/4, SIZE/4),
						"DOWN":(OFFSET+SIZE*j + 5*SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4),
						"START":(OFFSET+SIZE*j + 5*SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4)}[TOTAL_SEQ[frame]]
						# seeker head

						e = {"LEFT":(OFFSET+SIZE*j-SIZE/2, OFFSET + SIZE*i + 3*SIZE/8, SIZE/2, SIZE/4), 
						"RIGHT":(OFFSET+SIZE*j + SIZE, OFFSET + SIZE*i + 3*SIZE/8, SIZE/2, SIZE/4),
						"UP":(OFFSET+SIZE*j + 3*SIZE/8, OFFSET + SIZE*i - SIZE/2, SIZE/4, SIZE/2),
						"DOWN":(OFFSET+SIZE*j + 3*SIZE/8, OFFSET + SIZE*i + SIZE, SIZE/4, SIZE/2),
						"START":(OFFSET+SIZE*j + 3*SIZE/8, OFFSET + SIZE*i + SIZE, SIZE/4, SIZE/2)}[TOTAL_SEQ[frame]]
						# current move

						tgt = {"LEFT":(OFFSET+SIZE*j-int(SIZE/2), OFFSET + SIZE*i + int(SIZE/2)), 
						"RIGHT":(OFFSET+SIZE*j + SIZE + int(SIZE/2), OFFSET + SIZE*i + int(SIZE/2)),
						"UP":(OFFSET+SIZE*j + int(SIZE/2), OFFSET + SIZE*i - int(SIZE/2)),
						"DOWN":(OFFSET+SIZE*j + int(SIZE/2), OFFSET + SIZE*i + SIZE + int(SIZE/2))}[TOTAL_SEQ[frame+1]]

						pygame.draw.circle(screen, (150, 50, 0, 100), [int(x) for x in tgt], int(SIZE/2)) # looking square
						pygame.draw.rect(screen, (150, 0, 0, 100), e) # current move
						pygame.draw.rect(screen, (0, 0, 0, 100), d) # next move eyes 1
						pygame.draw.rect(screen, (0, 0, 0, 100), f) # next move eyes 2


					elif SNAKE_STATES[frame][i][j] == 'T':
						pygame.draw.rect(screen, (0, 100, 0), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))
					elif SNAKE_STATES[frame][i][j] == 1:
						pygame.draw.rect(screen, (0, 150, 0), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))
					
					pygame.draw.rect(screen, (120, 120, 120), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE), 1) # outline

			font = pygame.font.Font('freesansbold.ttf', 22)
			text = font.render("CURRENT SCORE: {}".format(SCORES[frame]), True, (200, 200, 200))
			try:
				text2 = font.render("MOOGLES EATEN: {}".format(MOOGLES[frame]), True, (200, 200, 200))
				text4 = font.render("TIME: {}/{}".format(CURR_FRAME[frame], TIME_OUT[frame]), True, (200, 200, 200))
			except Exception:
				pass
			text3 = font.render("NEXT MOVE: {}".format(TOTAL_SEQ[frame+1]), True, (200, 200, 200))
			

			r1 = text.get_rect()
			try: 
				r2 = text2.get_rect()
				r4 = text4.get_rect()
			except Exception:
				pass
			r3 = text3.get_rect()

			r1.topleft = (OFFSET, 500)
			try: 
				r2.topleft = (OFFSET, 525)
				r4.topleft = (OFFSET, 575)
			except Exception:
				pass
			r3.topleft = (OFFSET, 550)

			screen.blit(text, r1)
			try: 
				screen.blit(text2, r2)
				screen.blit(text4, r4)
			except Exception:
				pass
			screen.blit(text3, r3)
			

			# Flip the display
			pygame.display.flip()
			sleep(delay/1000)
	input()

def replay_200(file_name):

	delay = eval(input("Delay between moves? (ms)\n"))

	pygame.init()

	# Set up the drawing window
	screen = pygame.display.set_mode([100 + 40*BOARD_SIZE, 200 + 40*BOARD_SIZE])
	pygame.display.set_caption('KM Snek') 

	with open(file_name, 'rb') as f:
		dat = pickle.load(f)
		TOTAL_SEQ = dat["Sequence"] if "Sequence" in dat else None
		SNAKE_STATES = dat["Snakes"] if "Snakes" in dat else None
		TGT_STATES = dat["Targets"] if "Targets" in dat else None
		SCORES = dat["Scores"] if "Scores" in dat  else None
		MOOGLES = dat["Moogles"] if "Moogles" in dat else None
		CURR_FRAME = dat["Frames"] if "Frames" in dat else None
		TIME_OUT = dat["Timeout"] if "Timeout" in dat else None

		print("Replaying Game of Size {}".format(len(SNAKE_STATES)))

		for frame in range(len(SNAKE_STATES)-200,len(SNAKE_STATES)):

			#draw_board(SNAKE_STATES[i], TGT_STATES[i], i, TOTAL_SEQ[i], TOTAL_SEQ[i+1], SCORES[i], MOOGLES[i], screen, CURR_FRAME[i], TIME_OUT[i])	

			# Fill the background with white
			screen.fill((20, 20, 20))

			pygame.display.set_caption('KM Snake [REPLAY] - Frame Number {}/{}'.format(frame+1, len(SNAKE_STATES))) 
			
			for event in pygame.event.get(): 
				if event.type == pygame.QUIT: pygame.quit()

			for i in range(BOARD_SIZE):
				for j in range(BOARD_SIZE):
					if TGT_STATES and TGT_STATES[frame] and (j, i) == TGT_STATES[frame] and SNAKE_STATES[frame][i][j]:
						pygame.draw.rect(screen, (0, 150, 255), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))

					elif TGT_STATES and TGT_STATES[frame] and (j, i) == TGT_STATES[frame]:
						# food
						pygame.draw.rect(screen, (0, 0, 255), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))
					elif SNAKE_STATES[frame][i][j] == 'H':
						pygame.draw.rect(screen, (0, 200, 0), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))

						d = {"LEFT":(OFFSET+SIZE*j, OFFSET + SIZE*i + SIZE/8, SIZE/4, SIZE/4), 
						"RIGHT":(OFFSET+SIZE*j + 3*SIZE/4, OFFSET + SIZE*i + 5*SIZE/8, SIZE/4, SIZE/4),
						"UP":(OFFSET+SIZE*j + 5*SIZE/8, OFFSET + SIZE*i , SIZE/4, SIZE/4),
						"DOWN":(OFFSET+SIZE*j + SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4),
						"START":(OFFSET+SIZE*j + SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4)}[TOTAL_SEQ[frame]]
						# seeker head

						f = {"LEFT":(OFFSET+SIZE*j, OFFSET + SIZE*i + 5*SIZE/8, SIZE/4, SIZE/4), 
						"RIGHT":(OFFSET+SIZE*j + 3*SIZE/4, OFFSET + SIZE*i + SIZE/8, SIZE/4, SIZE/4),
						"UP":(OFFSET+SIZE*j + SIZE/8, OFFSET + SIZE*i , SIZE/4, SIZE/4),
						"DOWN":(OFFSET+SIZE*j + 5*SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4),
						"START":(OFFSET+SIZE*j + 5*SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4)}[TOTAL_SEQ[frame]]
						# seeker head

						e = {"LEFT":(OFFSET+SIZE*j-SIZE/2, OFFSET + SIZE*i + 3*SIZE/8, SIZE/2, SIZE/4), 
						"RIGHT":(OFFSET+SIZE*j + SIZE, OFFSET + SIZE*i + 3*SIZE/8, SIZE/2, SIZE/4),
						"UP":(OFFSET+SIZE*j + 3*SIZE/8, OFFSET + SIZE*i - SIZE/2, SIZE/4, SIZE/2),
						"DOWN":(OFFSET+SIZE*j + 3*SIZE/8, OFFSET + SIZE*i + SIZE, SIZE/4, SIZE/2),
						"START":(OFFSET+SIZE*j + 3*SIZE/8, OFFSET + SIZE*i + SIZE, SIZE/4, SIZE/2)}[TOTAL_SEQ[frame]]
						# current move

						tgt = {"LEFT":(OFFSET+SIZE*j-int(SIZE/2), OFFSET + SIZE*i + int(SIZE/2)), 
						"RIGHT":(OFFSET+SIZE*j + SIZE + int(SIZE/2), OFFSET + SIZE*i + int(SIZE/2)),
						"UP":(OFFSET+SIZE*j + int(SIZE/2), OFFSET + SIZE*i - int(SIZE/2)),
						"DOWN":(OFFSET+SIZE*j + int(SIZE/2), OFFSET + SIZE*i + SIZE + int(SIZE/2))}[TOTAL_SEQ[frame+1]]

						pygame.draw.circle(screen, (150, 50, 0, 100), [int(x) for x in tgt], int(SIZE/2)) # looking square
						pygame.draw.rect(screen, (150, 0, 0, 100), e) # current move
						pygame.draw.rect(screen, (0, 0, 0, 100), d) # next move eyes 1
						pygame.draw.rect(screen, (0, 0, 0, 100), f) # next move eyes 2


					elif SNAKE_STATES[frame][i][j] == 'T':
						pygame.draw.rect(screen, (0, 100, 0), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))
					elif SNAKE_STATES[frame][i][j] == 1:
						pygame.draw.rect(screen, (0, 150, 0), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))
					
					pygame.draw.rect(screen, (120, 120, 120), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE), 1) # outline

			font = pygame.font.Font('freesansbold.ttf', 22)
			text = font.render("CURRENT SCORE: {}".format(SCORES[frame]), True, (200, 200, 200))
			try:
				text2 = font.render("MOOGLES EATEN: {}".format(MOOGLES[frame]), True, (200, 200, 200))
				text4 = font.render("TIME: {}/{}".format(CURR_FRAME[frame], TIME_OUT[frame]), True, (200, 200, 200))
			except Exception:
				pass
			text3 = font.render("NEXT MOVE: {}".format(TOTAL_SEQ[frame+1]), True, (200, 200, 200))
			

			r1 = text.get_rect()
			try: 
				r2 = text2.get_rect()
				r4 = text4.get_rect()
			except Exception:
				pass
			r3 = text3.get_rect()

			r1.topleft = (OFFSET, 500)
			try: 
				r2.topleft = (OFFSET, 525)
				r4.topleft = (OFFSET, 575)
			except Exception:
				pass
			r3.topleft = (OFFSET, 550)

			screen.blit(text, r1)
			try: 
				screen.blit(text2, r2)
				screen.blit(text4, r4)
			except Exception:
				pass
			screen.blit(text3, r3)
			

			# Flip the display
			pygame.display.flip()
			sleep(delay/1000)
	input()

def kill_screen(file_name):
	'''
	Where were u when snek was kil

	I was at computer, coding algorithm

	When program show

	"Gam ovr"

	"No"

	--> Shows final frame before death
	'''


	pygame.init()

	# Set up the drawing window
	screen = pygame.display.set_mode([100 + 40*BOARD_SIZE, 200 + 40*BOARD_SIZE])
	pygame.display.set_caption('KM Snek') 

	with open(file_name, 'rb') as f:
		dat = pickle.load(f)
		seq = dat["Sequence"] if "Sequence" in dat else None
		snakes = dat["Snakes"] if "Snakes" in dat else None
		tgts = dat["Targets"] if "Targets" in dat else None
		scores = dat["Scores"] if "Scores" in dat  else None
		moogles = dat["Moogles"] if "Moogles" in dat else None
		frames = dat["Frames"] if "Frames" in dat else None
		timeout = dat["Timeout"] if "Timeout" in dat else None


		print("Replaying Game of Size {}".format(len(snakes)))
		draw_board(snakes[-1], tgts[-1], len(snakes)-1, seq[-2], seq[-1], scores[-1], moogles[-1], screen, frames[-1], timeout[-1])
		input()


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
	MOOGLES = []
	CURR_FRAME = []
	TIME_OUT = []
	frame = 0

	# ========== EXISTING CODE ======
	#ptr to board
	board = init_board()

	#ptr to axes and direction
	axis = c_int(0)
	p_axis = cast(addressof(axis),POINTER(c_int))
	direction = c_int(0)
	p_direction = cast(addressof(direction),POINTER(c_int))
	
	play_on = 1

	# ========== /EXISTING CODE ======

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

	if not dataCollectMode: 
		pygame.init()

		# Set up the drawing window
		screen = pygame.display.set_mode([500, 600])
		pygame.display.set_caption('KM Snek') 

	while (play_on):
		#clear()

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

		if target_exists():
			TGT_STATES.append((mooglex, moogley))
		else:
			TGT_STATES.append(None)

		SCORES.append(get_score())
		MOOGLES.append(get_moogles_eaten())
		CURR_FRAME.append(get_curr_frame())
		TIME_OUT.append(get_time_out())

		length = board[0].snek[0].length
		dead_stack = get_dead_stack()
		curr = board[0].snek[0].head
		snek = {} # squares occupied by snake, along with colour
		for i in range(length):
			c = 75 if i == length-1 else ((200-int(100*i/length)) if (200-int(100*i/length)) > 100 else 100)
			snek[(curr[0].coord[x], curr[0].coord[y])] = (c*dead_stack, c*(1-dead_stack), c/4*(1-dead_stack))
			curr = curr[0].next
		play_on = gameStep(p_axis, p_direction, board) # execute next move
		# also determine current input move

		
		TOTAL_SEQ.append(getDirection(axis.value, direction.value)) # append to current total moves

		if not dataCollectMode: 
			# print("\n\nNEXT FRAME:")
			# show_board(board)
			
			# Fill the background with white
			screen.fill((20, 20, 20))

			pygame.display.set_caption('KM Snake - Frame Number {}'.format(frame+1)) 

			
			
			for event in pygame.event.get(): 
				if event.type == pygame.QUIT: pygame.quit()



			for i in range(BOARD_SIZE):
				for j in range(BOARD_SIZE):
					if (j, i) in snek:
						pygame.draw.rect(screen, snek[(j, i)], (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))

					if TGT_STATES and TGT_STATES[-1] and (j, i) == TGT_STATES[-1] and SNAKE_STATES[-1][i][j]:
						pygame.draw.rect(screen, (0, 150, 255), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))

					elif TGT_STATES and TGT_STATES[-1] and (j, i) == TGT_STATES[-1]:
						# food
						pygame.draw.rect(screen, (0, 0, 255), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE))
					elif SNAKE_STATES[-1][i][j] == 'H':
						d = {"LEFT":(OFFSET+SIZE*j, OFFSET + SIZE*i + SIZE/8, SIZE/4, SIZE/4), 
						"RIGHT":(OFFSET+SIZE*j + 3*SIZE/4, OFFSET + SIZE*i + 5*SIZE/8, SIZE/4, SIZE/4),
						"UP":(OFFSET+SIZE*j + 5*SIZE/8, OFFSET + SIZE*i , SIZE/4, SIZE/4),
						"DOWN":(OFFSET+SIZE*j + SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4),
						"START":(OFFSET+SIZE*j + SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4)}[TOTAL_SEQ[-2]]
						# seeker head

						f = {"LEFT":(OFFSET+SIZE*j, OFFSET + SIZE*i + 5*SIZE/8, SIZE/4, SIZE/4), 
						"RIGHT":(OFFSET+SIZE*j + 3*SIZE/4, OFFSET + SIZE*i + SIZE/8, SIZE/4, SIZE/4),
						"UP":(OFFSET+SIZE*j + SIZE/8, OFFSET + SIZE*i , SIZE/4, SIZE/4),
						"DOWN":(OFFSET+SIZE*j + 5*SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4),
						"START":(OFFSET+SIZE*j + 5*SIZE/8, OFFSET + SIZE*i + 3*SIZE/4, SIZE/4, SIZE/4)}[TOTAL_SEQ[-2]]
						# seeker head

						e = {"LEFT":(OFFSET+SIZE*j-SIZE/2, OFFSET + SIZE*i + 3*SIZE/8, SIZE/2, SIZE/4), 
						"RIGHT":(OFFSET+SIZE*j + SIZE, OFFSET + SIZE*i + 3*SIZE/8, SIZE/2, SIZE/4),
						"UP":(OFFSET+SIZE*j + 3*SIZE/8, OFFSET + SIZE*i - SIZE/2, SIZE/4, SIZE/2),
						"DOWN":(OFFSET+SIZE*j + 3*SIZE/8, OFFSET + SIZE*i + SIZE, SIZE/4, SIZE/2),
						"START":(OFFSET+SIZE*j + 3*SIZE/8, OFFSET + SIZE*i + SIZE, SIZE/4, SIZE/2)}[TOTAL_SEQ[-2]]
						# current move

						tgt = {"LEFT":(OFFSET+SIZE*j-int(SIZE/2), OFFSET + SIZE*i + int(SIZE/2)), 
						"RIGHT":(OFFSET+SIZE*j + SIZE + int(SIZE/2), OFFSET + SIZE*i + int(SIZE/2)),
						"UP":(OFFSET+SIZE*j + int(SIZE/2), OFFSET + SIZE*i - int(SIZE/2)),
						"DOWN":(OFFSET+SIZE*j + int(SIZE/2), OFFSET + SIZE*i + SIZE + int(SIZE/2))}[TOTAL_SEQ[-1]]

						pygame.draw.circle(screen, (150, 50, 0, 100), [int(x) for x in tgt], int(SIZE/2)) # looking square
						pygame.draw.rect(screen, (150, 0, 0, 100), e) # current move
						pygame.draw.rect(screen, (0, 0, 0, 100), d) # next move eyes 1
						pygame.draw.rect(screen, (0, 0, 0, 100), f) # next move eyes 2
					
					pygame.draw.rect(screen, (120, 120, 120), (OFFSET + SIZE*j, OFFSET + SIZE*i, SIZE, SIZE), 1) # outline

			font = pygame.font.Font('freesansbold.ttf', 22)
			text = font.render("CURRENT SCORE: {}".format(SCORES[-1]), True, (200, 200, 200))
			text2 = font.render("MOOGLES EATEN: {}".format(MOOGLES[-1]), True, (200, 200, 200))
			text3 = font.render("NEXT MOVE: {}".format(TOTAL_SEQ[-1]), True, (200, 200, 200))
			text4 = font.render("TIME: {}/{}".format(CURR_FRAME[-1], TIME_OUT[-1]), True, (200, 200, 200))

			r1 = text.get_rect()
			r2 = text2.get_rect()
			r3 = text3.get_rect()
			r4 = text4.get_rect()

			r1.topleft = (OFFSET, 500)
			r2.topleft = (OFFSET, 525)
			r3.topleft = (OFFSET, 550)
			r4.topleft = (OFFSET, 575)

			screen.blit(text, r1)
			screen.blit(text2, r2)
			screen.blit(text3, r3)
			screen.blit(text4, r4)

			# Flip the display
			pygame.display.flip()

			frame += 1
			#print("Frame {}\nScore: {}\nMoogles Eaten:{}\n".format(frame, SCORES[-1], MOOGLES[-1]))
		
		'''
		if not dataCollectMode: 
			sleep(0.5)
		'''

	#pass by reference to clean memory
	score = get_score()
	end_game(byref(board))
	return (score, {"Sequence":TOTAL_SEQ, "Targets":TGT_STATES, "Snakes":SNAKE_STATES, "Scores":SCORES, "Moogles":MOOGLES, "Frames":CURR_FRAME, "Timeout":TIME_OUT})

if __name__ == "__main__":

	TRIALS = 0
	NAME_EXT = ''

	# main function --> this allows some funky scoping stuff to work

	random.seed()

	response = ''

	while(response.lower() not in {'collect', 'collectlog', 'replay', 'play', 'lastframe','replay200'}):
		response = input("Mode Selection?\n[COLLECT] - data collection mode without game logs\n[COLLECTLOG] - log moves\n[REPLAY] - load data file\n[LASTFRAME] = print last frame\n[PLAY] - play normally\n")

	response = response.upper()

	DATA_COLLECT = False
	LOG_GAMES = False
	REPLAY = False
	LAST_FRAME = False
	REPLAY200 = False
	COLLECT_ALL = False

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
		NAME_EXT = input("File Prefix Name?\n") # common file prefix
	
	elif response == 'REPLAY200':
		REPLAY200 = True
		NAME_EXT = input("File Prefix Name?\n") # common file prefix

	elif response == 'COLLECTALL':
		COLLECTALL == 'TRUE'
		while(not response.isnumeric()):
			response = input("Number of Trials?\n")
			TRIALS = int(response)

		NAME_EXT = input("File Prefix Name?\n") # common file prefix

	if DATA_COLLECT:
		# data collection mode
		with open(NAME_EXT+"_output.tsv", 'a') as f:
			f.write("Trial Number	Score	Moogles Eaten	Time Taken	Data Location\n")

		for i in range(TRIALS):
			with open(NAME_EXT+"_output.tsv", 'a') as f:

				t_start = time()

				(score, dat) = main(dataCollectMode=True)

				t_end = time()

				print("\nTrial {}/{} Completed in {} seconds. Progress {}%".format(i+1, TRIALS, t_end-t_start, 100*(i+1)/TRIALS))

				f.write(str(i) + '\t' + str(score) + '\t' + str(dat["Moogles"][-1]) + '\t' + str(t_end-t_start) + '\t' + NAME_EXT + 'data'+str(i)  +'\n')

				if LOG_GAMES:
					# I'M PICKLE RICK WUBBA LUBBA DUB DUB
					
					with open('data/'+NAME_EXT +'data'+str(i)+'.dat', 'wb') as datout:
						pickle.dump(dat, datout)
	
	elif COLLECT_ALL:
		with open(NAME_EXT+"_output.tsv", 'a') as f:
			f.write("Trial Number	Score	Moogles Eaten	Time Taken	Data Location\n")

		for b in range(2,10):
			for c in [p/2.0 for p in range(1, 7)]:
				print("TEMP")
		for i in range(TRIALS):
			with open(NAME_EXT+"_output.tsv", 'a') as f:

				t_start = time()

				(score, dat) = main(dataCollectMode=True)

				t_end = time()

				print("\nTrial {}/{} Completed in {} seconds. Progress {}%".format(i+1, TRIALS, t_end-t_start, 100*(i+1)/TRIALS))

				f.write(str(i) + '\t' + str(score) + '\t' + str(dat["Moogles"][-1]) + '\t' + str(t_end-t_start) + '\t' + NAME_EXT + 'data'+str(i)  +'\n')

				if LOG_GAMES:
					# I'M PICKLE RICK WUBBA LUBBA DUB DUB
					
					with open('data/'+NAME_EXT +'data'+str(i)+'.dat', 'wb') as datout:
						pickle.dump(dat, datout)
	elif REPLAY:
		i = eval(input("Trial Number?\n")) # trial number

		if LAST_FRAME:
			kill_screen('data/'+NAME_EXT +'data'+str(i)+'.dat')
		else:
			replay_game('data/'+NAME_EXT +'data'+str(i)+'.dat')

	elif REPLAY200:
		i = eval(input("Trial Number?\n")) # trial number
		replay_200('data/'+NAME_EXT +'data'+str(i)+'.dat')

	else:
		cool = ''

		while (cool != "quit"):

			with open(NAME_EXT+"_output.tsv", 'a') as f:
				f.write("Score	Moogles Eaten	Time Taken	Data Location\n")


				t_start = time()

				(score, dat) = main()

				t_end = time()

				f.write(str(score) + '\t' + str(dat["Moogles"][-1]) + '\t' + str(t_end-t_start) + '\t' + NAME_EXT +'\n')
				
				with open('data/'+NAME_EXT +'.dat', 'wb') as datout:
					pickle.dump(dat, datout)

			cool = input()
			# Done! Time to quit.
			pygame.quit()