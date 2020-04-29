
# The following class describes goals set on the opposite sides of the pitch.
# Each goal contains two posts ( the upper and the lower one ).

import pygame
from Post import Post
from Collision import Collision

class Goal( object ):
    def __init__(self, game, color, p_post_x, p_post_y_up, p_post_y_down, width, direction):
        self.game = game
        self.color = color
        self.x = p_post_x
        self.y_up = p_post_y_up
        self.y_down = p_post_y_down
        self.width = width
        self.direction = direction

        # initialize Posts
        self.post_up = Post(self.game, self.x, self.y_up)
        self.post_down = Post(self.game, self.x, self.y_down)

        # load goal image
        if(direction == -1):
            self.goal = pygame.image.load("../goal_left.png").convert_alpha()
        elif(direction == 0):
            self.goal = pygame.image.load("../goal_right.png").convert_alpha()

     
    def goal_collide(self):
        Collision.collide(self.post_up)
        Collision.collide(self.post_down)
        
    def get_px(self):
        return self.x + self.direction * self.width

    def get_py(self):
        return self.y_up

    def get_width(self):
        return self.width

    def get_height(self):
        return self.y_down - self.y_up