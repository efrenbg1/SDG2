import time
import os

p = [[0, 0, 0, 0, 0, 0, 0],
     [0, 0, 0, 0, 0, 0, 0],
     [0, 0, 0, 0, 0, 0, 0],
     [0, 0, 0, 0, 0, 0, 0],
     [0, 0, 0, 0, 0, 0, 0],
     [0, 0, 0, 0, 0, 0, 0],
     [0, 0, 0, 0, 0, 0, 0],
     [0, 0, 0, 0, 0, 0, 0]]

i = 0
j = 0

ii = 0
jj = 0


def paint():
    global i, j, ii, jj
    i += 1
    if i > 6:
        i = 0
        j += 1
    if j > 1:
        ii += 1
        if ii > 6:
            ii = 0
            jj += 1
        p[jj][ii] = 0
    p[j][i] = 1
    os.system('clear')
    for row in p:
        print(row)


for _ in range(1, 7*8):
    paint()
    time.sleep(0.1)
