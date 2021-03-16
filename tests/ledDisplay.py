import RPi.GPIO as gpio
import time

gpio.setmode(gpio.BCM)

x = [11, 5, 6, 13, 19, 26, 21, 20]
y = [2, 3, 4, 17, 27, 22, 10, 9]

m = [
    [1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [1, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 1, 1, 1, 0, 0, 0],
    ]

for pin in x:
    gpio.setup(pin, gpio.OUT)
    gpio.output(pin, True)

for pin in y:
    gpio.setup(pin, gpio.OUT)
    gpio.output(pin, True)

while True:
    for _y in range(0, 8):
        gpio.output(y[_y], False)
        for _x in range(0, 8):
            if m[_x][_y]:
                gpio.output(x[_x], True)
            else:
                gpio.output(x[_x], False)
        time.sleep(0.001)
        gpio.output(y[_y], True)