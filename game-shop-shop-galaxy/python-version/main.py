import gs
import gs.plus.render as render
import gs.plus.input as input

import math
from utils import *
from basic_vector import *
import ball_physics as ball
import player_racket as player
import board as board
# import intersection as col

def project3DTo2D(x, z, board_width, board_length):
	top_left_x = 120
	top_right_x = 320 - 120
	top_y = 8

	bottom_left_x = 0
	bottom_right_x = 320
	bottom_y = 130

	norm_x = (x / board_width) + 0.5
	norm_y = (z / board_length) + 0.5

	persp_coef = [1, 5, 9, 13, 17, 22, 27, 34, 42, 51, 64, 80, 102, 132]
	persp_coef = list(map(lambda z: z / 132.0, persp_coef))
	norm_y = mapValueToArray(norm_y, 0.0, 1.0, persp_coef)
	
	top_2d_x = (1.0 - norm_x) * top_left_x + norm_x * top_right_x
	bottom_2d_x = (1.0 - norm_x) * bottom_left_x + norm_x * bottom_right_x

	proj_2d_x = (1.0 - norm_y) * top_2d_x + norm_y * bottom_2d_x
	proj_2d_y = (1.0 - norm_y) * top_y + norm_y * bottom_y

	proj_scale = RangeAdjust(norm_y, 0.0, 1.0, 0.285, 1.0) #(norm_y + 1.0) * 0.5

	return proj_2d_x, proj_2d_y, proj_scale

def gameReset():
	ball.reset()
	ball.setImpulse(10.0, 10.0)

def renderBall():
	# Ball
	render.sprite2d(scr_margin_x + ball_2d_x, ball_2d_y - (65 * SCR_SCALE_FACTOR), 24 * SCR_SCALE_FACTOR * ball_2d_scale, "@assets/game_ball.png")

def renderPlayer():
	# Player racket
	render.sprite2d(scr_margin_x + player_2d_x, player_2d_y - (65 * SCR_SCALE_FACTOR), 64 * SCR_SCALE_FACTOR * player_2d_scale, "@assets/game_racket.png")

def ballIsBehindRacket(ball, racket):
	if ball.pos_z < racket.pos_z:
		return True

	return False

def BallIsWithinXReach(ball, racket):
	if ball.pos_x + ball.radius > racket.pos_x - racket.width * 0.5 and ball.pos_x - ball.radius < racket.pos_x + racket.width * 0.5:
		return True

	return False

def ballWasBehindRacket(ball, racket):
	if ball.prev_pos_z < racket.prev_pos_z:
		return True

	return False

def BallWasWithinXReach(ball, racket):
	if ball.prev_pos_x + ball.radius > racket.prev_pos_x - racket.width * 0.5 and ball.prev_pos_x - ball.radius < racket.prev_pos_x + racket.width * 0.5:
		return True

	return False	

SCR_PHYSIC_WIDTH = 320
SCR_PHYSIC_HEIGHT = 200

SCR_DISP_WIDTH = 320 * 2
SCR_DISP_HEIGHT = 200 * 2

SCR_SCALE_FACTOR = min(SCR_DISP_WIDTH / SCR_PHYSIC_WIDTH, SCR_DISP_HEIGHT / SCR_PHYSIC_HEIGHT)

render.init(SCR_DISP_WIDTH, SCR_DISP_HEIGHT, "pkg.core")

# provide access to the data folder
gs.MountFileDriver(gs.StdFileDriver("assets/"), "@assets/")

gameReset()
player.initial_pox_z = (board.board_length * 0.45)
player.reset()

while not input.key_press(gs.InputDevice.KeyEscape):
	dt = 1.0 / 60.0

	scr_margin_x = (SCR_DISP_WIDTH - (SCR_PHYSIC_WIDTH * SCR_SCALE_FACTOR)) / 2.0

	# update mouse
	gs.GetInputSystem().Update()

	# get the mouse device
	mouse_device = gs.GetInputSystem().GetDevice("mouse")

	# Update the ball motion
	ball.update(dt)

	# Update the player motion
	player.setMouse(mouse_device.GetValue(gs.InputDevice.InputAxisX) / SCR_DISP_WIDTH, mouse_device.GetValue(gs.InputDevice.InputAxisY) / SCR_DISP_HEIGHT)
	player.update(dt)

	# Collisions
	if ball.velocity_z > 0.0:
		if (not ballIsBehindRacket(ball, player)) and BallIsWithinXReach(ball, player) and ballWasBehindRacket(ball, player) and BallWasWithinXReach(ball, player):
			ball.setPosition(ball.pos_x, player.pos_z - ball.velocity_z * dt + min(0.0, player.velocity_z) * dt)
			player.setPosition(player.pos_x, ball.pos_z + player.length)
			ball.bounceZ()

	# Compute 3D/2D projections
	ball_2d_x, ball_2d_y, ball_2d_scale = project3DTo2D(ball.pos_x, ball.pos_z, board.board_width, board.board_length)
	ball_2d_x *= SCR_SCALE_FACTOR
	ball_2d_y = SCR_DISP_HEIGHT - (ball_2d_y * SCR_SCALE_FACTOR)

	player_2d_x, player_2d_y, player_2d_scale = project3DTo2D(player.pos_x, player.pos_z, board.board_width, board.board_length)
	player_2d_x *= SCR_SCALE_FACTOR
	player_2d_y = SCR_DISP_HEIGHT - (player_2d_y * SCR_SCALE_FACTOR)

	render.clear()
	render.set_blend_mode2d(render.BlendAlpha)
	# Opponent
	render.sprite2d(scr_margin_x + (320 * 0.5) * SCR_SCALE_FACTOR, (SCR_PHYSIC_HEIGHT - 96 * 0.5) * SCR_SCALE_FACTOR, 106 * SCR_SCALE_FACTOR, "@assets/robot5.png")

	# Game board
	render.image2d(scr_margin_x, 0, SCR_SCALE_FACTOR, "@assets/game_board.png")

	# Score panel
	render.image2d(scr_margin_x, SCR_DISP_HEIGHT - (32 * SCR_SCALE_FACTOR), SCR_SCALE_FACTOR, "@assets/game_score_panel.png")

	# Render moving items according to their Z position
	if ball.pos_z < player.pos_z + player.length:
		renderBall()
		renderPlayer()
	else:
		renderPlayer()
		renderBall()

	render.set_blend_mode2d(render.BlendOpaque)
	render.flip()
