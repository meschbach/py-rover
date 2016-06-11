import _rover
import numpy as np
#
# terrian = np.array([
#     [0,0,0,0,0,0],
#     [0,1,1,1,1,0],
#     [0,1,0,0,1,0],
#     [0,1,1,0,1,0],
#     [0,1,1,0,1,0],
#     [0,1,1,1,1,0],
#     [0,0,0,0,0,0],
# ])
# result = _rover.a_star( terrian, (2,2), (3,4) )
# print(str(result))

#
# terrian = np.array([
#     [0,0,0,0,0,0,0,0],
#     [0,1,1,1,1,1,1,0],
#     [0,1,0,0,0,0,1,0],
#     [0,1,0,0,1,0,1,0],
#     [0,1,0,0,1,0,1,0],
#     [0,1,0,1,1,1,1,0],
#     [0,0,0,0,0,0,0,0],
# ])
# result = _rover.a_star( terrian, (2,2), (5,4) )
# print(str(result))


import random

size = 1024
start = random.randint(0,size), random.randint(0,size)
end = random.randint(0,size), random.randint(0,size)

print("Finding from " + str(start) + " to " + str(end) )

count = random.randint(size,size*16)
board = np.ndarray(shape=(size,size))
for i in range(0,count):
    board[random.randint(0,size-1)][random.randint(0,size-1)] = 1.0
result = _rover.a_star( board.astype(int), start, end )
print(str(result))