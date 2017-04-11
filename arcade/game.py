import serial
import random

from OpenGL.GL import *
from OpenGL.GLUT import *

CHARACTER_HEIGHT = 0.1
CHARACTER_WIDTH = 0.3
ENEMY_WIDTH = 0.1
ENEMY_HEIGHT = 0.1
MAX_NUM_ENEMIES = 3
SPEED = 0.01

sensor = serial.Serial('/dev/ttyACM0', 9600, timeout=None)
# (x, y) coordinates
enemyPositions = []

def readFromSensor():
    LOWER_VALUE = 100.0
    UPPER_VALUE = 2000.0
    # Read value by symbols.
    value = ''
    char = sensor.read()
    while char != '\n':
        value += char
        char = sensor.read()
    # Convert from string to integer.
    value = int(value)
    # Clamp to range [LOWER_VALUE, UPPER_VALUE].
    value = max(LOWER_VALUE, min(value, UPPER_VALUE))
    # Map to [0, 1].
    return (value - LOWER_VALUE) / (UPPER_VALUE - LOWER_VALUE)

def drawRect(top, left, height, width):
    glBegin(GL_QUADS)
    glVertex2f(left, top)
    glVertex2f(left, top - height)
    glVertex2f(left + width, top - height)
    glVertex2f(left + width, top)
    glEnd()

def gameLoop():
    # Update player position.
    playerPosition = readFromSensor()

    # Update enemies position.
    for pos in enemyPositions:
        pos[0] -= SPEED

    # Remove hidden enemies.
    for i, pos in enumerate(enemyPositions):
        if pos[0] < -ENEMY_WIDTH:
            del enemyPositions[i]
            break

    # Create new enemies.
    if len(enemyPositions) < MAX_NUM_ENEMIES and random.randint(0, 20) == 0:
        enemyPositions.append([1.0, random.random()])

    # Drawing.
    glClear(GL_COLOR_BUFFER_BIT)

    # Draw character.
    glColor3ub(60, 130, 200)
    drawRect(playerPosition, 0, CHARACTER_HEIGHT, CHARACTER_WIDTH)

    # Draw enemies.
    glColor3ub(200, 30, 60)
    for pos in enemyPositions:
        drawRect(pos[1], pos[0], ENEMY_HEIGHT, ENEMY_WIDTH)

    glutSwapBuffers()

glutInit()
glutInitWindowSize(500, 500)
glutInitWindowPosition(0, 0)
glutCreateWindow("arcade")

glOrtho(0, 1, -CHARACTER_HEIGHT, 1, 0, 1)

glutIdleFunc(gameLoop)

glutMainLoop()
