import pygame
import random
import sys
import math

width = 1280
height = 720
back = (255, 255, 255, 255)

score = 0
score_speedup = 20
game_speed = 2.0
skins = ["default", "aqua", "black", "bloody", "cobalt", "gold", "insta",
         "lime", "magenta", "magma", "navy", "neon", "orange", "pinky",
         "purple", "rgb", "silver", "subaru", "sunny", "toxic"]
names = ["Флафи", "Фалафель", "Ведьмак", "Лютик", "Пучеглазик", "Слайм", "Шустрый", "Следопыт",
         "Малыш", "Субарик", "Т-Рекс", "Птенец", "Рядовой", "Опытный", "Ветеран", "Геймер",
         "Самурай", "Странник"]
generation = 0

def run_game():
    enemies = [Cactus(width + 300 / random.uniform(0.8, 3), height - 85),
               Cactus(width * 2 + 200 / random.uniform(0.8, 3), height - 85),
               Cactus(width * 3 + 400 / random.uniform(0.8, 3), height - 85)]
    
    road_chunks = [
        [pygame.image.load('sprites/road.png'), [0, height - 100]],
        [pygame.image.load('sprites/road.png'), [2404, height - 100]]
    ]

    pygame.init()
    screen = pygame.display.set_mode((width, height))
    clock = pygame.time.Clock()
    road = pygame.image.load('sprites/road.png')
    font = pygame.font.SysFont("Roboto Condensed", 30)
    dino = Dino(30, 550, "insta", "Шашик")
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

        screen.fill(back)
        for road_chunk in road_chunks:
            if road_chunk[1][0] <= -2400:
                road_chunk[1][0] = road_chunks[len(road_chunks) - 1][1][0] + 2400

                road_chunks[0], road_chunks[1] = road_chunks[1], road_chunks[0]
                break

            road_chunk[1][0] -= game_speed
            screen.blit(road_chunk[0], (road_chunk[1][0], road_chunk[1][1]))

   


        dino.update()
        dino.draw(screen, font)

        user_input = pygame.key.get_pressed()
        if user_input[pygame.K_SPACE]:
            if not dino.state == DinoState.JUMP:
                dino.jump()
        pygame.display.flip()
        clock.tick(0)

class DinoState():
    RUN  = 1
    JUMP = 2

class Dino():
    name = "Бабулех"
    jump_power = 10
    cur_jump_power = jump_power
    color = "default"
    sprites = {
        "run": [],
        "jump": []
    }
    image = None
    run_animation_index = [0, 20]
    hitbox = None
    state = DinoState.RUN

    def __init__(self, x, y, color = "default", name = None):
        self.color = color
        self.load_sprites()
        self.hitbox = pygame.Rect(x, y, self.sprites["run"][0].get_width(), self.sprites["run"][0].get_height())
        self.image = self.sprites["run"][0]

        if name is not None:
            self.name = name



    def load_sprites(self):
        self.sprites["jump"].append(pygame.image.load(f"sprites/dino/{self.color}_jump.png"))
        self.sprites["run"].append(pygame.image.load(f"sprites/dino/{self.color}_run1.png"))
        self.sprites["run"].append(pygame.image.load(f"sprites/dino/{self.color}_run2.png"))

    def update(self):
        if self.state == DinoState.RUN:
            self.run()
        elif self.state == DinoState.JUMP:
            self.jump()

    def run(self):
        self.sprites["run"][0] = pygame.image.load(f"sprites/dino/{self.color}_run1.png")
        self.sprites["run"][1] = pygame.image.load(f"sprites/dino/{self.color}_run2.png")

        self.image = self.sprites["run"][self.run_animation_index[0] // self.run_animation_index[1]]

        self.run_animation_index[0] += 1
        if self.run_animation_index[0] >= self.run_animation_index[1] * 2:
            self.run_animation_index[0] = 0

    def jump(self): 
        if self.state == DinoState.JUMP:
            self.hitbox.y -= self.cur_jump_power * (2 * (game_speed / 8))
            self.cur_jump_power -= 0.5 * (game_speed / 8)

            if self.hitbox.y >= height - 170:
                self.hitbox.y = height - 170
                self.state = DinoState.RUN
                self.cur_jump_power = self.jump_power
        else:
            self.state = DinoState.JUMP
            self.image = pygame.image.load(f"sprites/dino/{self.color}_jump.png")

            
    def draw(self, scr, fnt=None):
        scr.blit(self.image, (self.hitbox.x, self.hitbox.y))

        if fnt is not None:
            c_label = fnt.render(self.name.capitalize(), True, (100, 100, 100))
            c_label_rect = c_label.get_rect()
            c_label_rect.center = (self.hitbox.x + 45, self.hitbox.y - 30)
            scr.blit(c_label, c_label_rect)




class Cactus:
    available_types = ["1", "2", "3", "4", "5", "6"]
    cactus_type = None
    image = None
    hitbox = None
    is_active = True

    def __init__(self, x, y, forced_type=None):
        if forced_type is not None:
            self.cactus_type = forced_type

        self.load_image()
        self.hitbox.x = x
        self.hitbox.y = y - self.hitbox.height  # origin from bottom

    def randomize_cactus(self):
        self.cactus_type = random.choice(self.available_types)

    def load_image(self):
        if self.cactus_type is None:
            self.randomize_cactus()

        self.image = pygame.image.load(f"sprites/cactus/{self.cactus_type}.png")
        self.hitbox = self.image.get_rect()

    def update(self):
        self.hitbox.x -= game_speed
        if self.hitbox.x < -self.hitbox.width:
            # remove this cactus
            self.is_active = False

    def draw(self, scr):
        scr.blit(self.image, self.hitbox)










if __name__ == "__main__":
    run_game()