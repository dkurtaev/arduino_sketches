import serial

from OpenGL.GL import *
from OpenGL.GLUT import *

sensor = serial.Serial('/dev/ttyACM0', 9600, timeout=None)

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

def gameLoop():
    print readFromSensor()

def display():
    glClear(GL_COLOR_BUFFER_BIT)

    glColor3ub(180, 30, 125)
    glBegin(GL_TRIANGLES)
    glVertex2f(0.0, 0.5)
    glVertex2f(-0.5, -0.5)
    glVertex2f(0.5, -0.5)
    glEnd()

    glutSwapBuffers()

glutInit()
glutInitWindowSize(200, 200)
glutInitWindowPosition(0, 0)
glutCreateWindow("arcade")

glutDisplayFunc(display)
glutIdleFunc(gameLoop)

glutMainLoop()
