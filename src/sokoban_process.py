#!/usr/bin/env python3

# the purpose of this is to extract the sokoban level information into a .c file
# * is a box = 0
# X is a wall = 1
# @ is the player = 2
# . is the dest = 3
# space is path = 4
# there are only 4 states. Can be represented in 2 bits. Do I wanna? No.
# 

with open("sokoban_levels.txt") as f:
    lines = f.read().split('\n')
levels=[]
levelpartial=[]
for line in lines:
    if line=='EOF':
        levels.append(levelpartial)
        levelpartial=[]
        continue
    levelpartial.append(line)

max_w = max(max(map(len,level)) for level in levels)
max_h = max(len(level) for level in levels)

written_contents = f"unsigned char levels[{len(levels)}][{max_h}][{max_w}] = {{"
mapping = {
    ' ': 0,
    '*': 1,
    'X': 2,
    '@': 3,
    '.': 4,
}

levels_arr='{'
for level in levels:
    c_arrs = '{'
    for line in level:
        line = line.ljust(max_w,' ')
        c_arrs+='{'+str([mapping[c] for c in line])[1:-1]+'},\n'
    c_arrs += '},\n'
    levels_arr+=c_arrs
levels_arr+='};'

prefix = f"unsigned char sokoban_levels[{len(levels)}][{max_h}][{max_w}] = "
prefix += levels_arr
with open("sokoban_data.c","w") as f:
    f.write(prefix)