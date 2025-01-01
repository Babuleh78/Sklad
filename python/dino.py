import pygame
import random
import sys
import math

width = 1280
height = 720
back = (255, 255, 255, 255)

def run_game():

    pygame.init()
    screen = pygame.display.set_mode((width, height))
    clock = pygame.time.Clock()
    road = pygame.image.load('sprites/road.png')
    font = pygame.font.SysFont("Roboto Condensed", 30)

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                pygame.exit()

        screen.fill(back)
        screen.blit(road, (0, 600))

        pygame.display.flip()
        clock.tick(0)

if __name__ == "main":
    run_game()