import RPi.GPIO as gpio
import time

gpio.setmode(gpio.BCM)

x = [2, 3, 10, 9, 11, 14, 15, 8]
y = [4, 17, 27, 22, 18, 23, 24, 25]

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
    gpio.output(pin, False)

for pin in y:
    gpio.setup(pin, gpio.OUT)
    gpio.output(pin, True)

def matrix(_x, _y):
    if not m[_x][_y]:
        return
    gpio.output(x[_x], True)
    gpio.output(y[_y], False)
    time.sleep(0.004)
    gpio.output(x[_x], False)
    gpio.output(y[_y], True)

while True:
    for _x in range(0, 8):
        gpio.output(x[_x], True)
        for _y in range(0, 8):
            if not m[_x][_y]:
                continue
            gpio.output(y[_y], False)
            time.sleep(0.004/8)
            gpio.output(y[_y], True)
        gpio.output(x[_x], False)