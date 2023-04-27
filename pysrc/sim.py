import turtle
import math
import pandas
import numpy
import random

global log, count, pixels, values_list, movement_list, movement_count, finished
finished = False
log = True
count = 0
pixels = 0
movement_count = 0
movement_list = [0,0,0,0,0]
values_list = [0,0,9999,9999,9999,0]

screen = turtle.Screen()
screen.bgpic('field.gif')
screen.tracer(0,20)

opponent = turtle.Turtle(shape='square', visible=True)
opponent.color('red')
opponent.shapesize(1.2, 1.3, 1)
opponent.pu()

k = turtle.Turtle(shape='square', visible=True)
k.color('blue')
k.shapesize(1.4, 1.4, 1)
k.pu()
k.goto(-90, 60)
k.pd()

def run_auton(auton_num):
    global movement_list, finished
    finished = False
    if auton_num == 0:
        #Jugglenauts
        opponent.goto(-20, 90)
        opponent.seth(245)
        opponent.pd()
        movement_list = [2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,-2,-2,-2,-2,-2,45,2,2,2,2,2,2,2,2,2,2,-45,0,0,0,0,0,-20,-2,-2,-2,-2,-2,2,2,2,2,2,20,0,0,0,0,0]
    elif auton_num == 1:
        #Speed of light
        opponent.goto(-20, 90)
        opponent.seth(180)
        opponent.pd()
        movement_list = [2,2,2,2,2,2,2,2,2,2,90,2,2,2,2,2,0,0,0,0,0,-2,-2,-2,-2,-2,135,2,2,2,2,-90,0,0,0,0,0,90,2,2,2,2,2-90,0,0,0,0,0]
    elif auton_num == 2:
        # Gears
        opponent.goto(-15,85)
        opponent.seth(270)
        opponent.pd()
        movement_list = [2,2,2,2,2,-150,0,0,0,0,0,0,135,2,2,2,2,2,2,-2,-2,-135,0,0,0,0,0,135,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,0,0,0,0,0]




def print_coords(x,y):
    x_coord = x
    y_coord = y
    print(x_coord, y_coord)

def full_forward():
    global log, count, pixels, values_list, movement_list, movement_count, finished
    LSD = 9999
    MSD = 9999
    BSD = 9999
    k.fd(2)
    screen.update()
    pixels += 2
    count += 20
    lsd_points = [(round(k.xcor() + 9*math.sqrt(2)*0.564705882352941 + a), round(k.ycor() + a)) for a in range(0,225)]
    msd_points = [(round(k.xcor() + a), round(k.ycor() + a)) for a in range(0,225)]
    bsd_points = [(round(k.xcor() + a), round(k.ycor() + 9*math.sqrt(2)*0.564705882352941 + a)) for a in range(0,225)]
    accepted_points = [(round(opponent.xcor() + 8*1.5625), round(opponent.ycor() + a)) for a in range(round(-8*1.5625), round(8*1.5625))]
    for x in range (0, 9):
        for c in [(round(opponent.xcor() + x*1.5625), round(opponent.ycor() + a)) for a in range(round(-8*1.5625), round(8*1.5625))]:
            accepted_points.append(c)
    for i in range (0, 9):
        for b in [(round(opponent.xcor() + a), round(opponent.ycor() + i*1.5625)) for a in range(round(-8*1.5625), round(8*1.5625))]:
            accepted_points.append(b)
    for x in accepted_points:
        if x in lsd_points:
            LSD = round(16.255999479808015 * k.distance(opponent)) + random.randint(-200, 200)
        if x in msd_points:
            MSD = round(16.255999479808015 * k.distance(opponent)) + random.randint(-200, 200)
            count = 0
        if x in bsd_points:
            BSD = round(16.255999479808015 * k.distance(opponent)) + random.randint(-200, 200)
    if log: 
        values_list[0] = 4
        values_list[1] = pixels * 0.030557749073644
        values_list[2] = LSD
        values_list[3] = MSD
        values_list[4] = BSD
        values_list[5] = count
    if movement_count == len(movement_list) - 1: 
        finished = True
        movement_count = 0
    else: 
        if abs(movement_list[movement_count]) > 2:
            opponent.rt(movement_list[movement_count])
        else: 
            opponent.fd(movement_list[movement_count])
    movement_count += 1
    
    if finished: 
        k.goto(-80,60)
        run_auton(random.randint(0,2))
    
    accepted_points.clear()
    print(values_list)
    
def full_backward():
    global log, count, pixels, values_list, movement_list, movement_count, finished    
    LSD = 9999
    MSD = 9999
    BSD = 9999
    k.back(2)
    screen.update()
    pixels -= 2
    count += 20
    lsd_points = [(round(k.xcor() + 9*math.sqrt(2)*0.564705882352941 + a), round(k.ycor() + a)) for a in range(0,225)]
    msd_points = [(round(k.xcor() + a), round(k.ycor() + a)) for a in range(0,225)]
    bsd_points = [(round(k.xcor() + a), round(k.ycor() + 9*math.sqrt(2)*0.564705882352941 + a)) for a in range(0,225)]
    accepted_points = [(round(opponent.xcor() + 8*1.5625), round(opponent.ycor() + a)) for a in range(round(-8*1.5625), round(8*1.5625))]
    for x in range (0, 9):
        for c in [(round(opponent.xcor() + x*1.5625), round(opponent.ycor() + a)) for a in range(round(-8*1.5625), round(8*1.5625))]:
            accepted_points.append(c)
    for i in range (0, 9):
        for b in [(round(opponent.xcor() + a), round(opponent.ycor() + i*1.5625)) for a in range(round(-8*1.5625), round(8*1.5625))]:
            accepted_points.append(b)
    for x in accepted_points:
        if x in lsd_points:
            LSD = round(16.255999479808015 * k.distance(opponent)) + random.randint(-200, 200)
        if x in msd_points:
            MSD = round(16.255999479808015 * k.distance(opponent)) + random.randint(-200, 200)
            count = 0
        if x in bsd_points:
            BSD = round(16.255999479808015 * k.distance(opponent)) + random.randint(-200, 200)
    if log: 
        values_list[0] = 0
        values_list[1] = pixels * 0.030557749073644
        values_list[2] = LSD
        values_list[3] = MSD
        values_list[4] = BSD
        values_list[5] = count
    if movement_count == len(movement_list) - 1: 
        finished = True
        movement_count = 0
    else: 
        if abs(movement_list[movement_count]) > 2:
            opponent.rt(movement_list[movement_count])
        else: 
            opponent.fd(movement_list[movement_count])
    movement_count += 1
    
    if finished: 
        k.goto(-80,60)
        run_auton(random.randint(0,2))
    
    accepted_points.clear()
    print(values_list)

def half_forward():
    global log, count, pixels, values_list, movement_list, movement_count, finished
    LSD = 9999
    MSD = 9999
    BSD = 9999
    k.fd(1)
    screen.update()
    pixels += 1
    count += 20
    lsd_points = [(round(k.xcor() + 9*math.sqrt(2)*0.564705882352941 + a), round(k.ycor() + a)) for a in range(0,225)]
    msd_points = [(round(k.xcor() + a), round(k.ycor() + a)) for a in range(0,225)]
    bsd_points = [(round(k.xcor() + a), round(k.ycor() + 9*math.sqrt(2)*0.564705882352941 + a)) for a in range(0,225)]
    accepted_points = [(round(opponent.xcor() + 8*1.5625), round(opponent.ycor() + a)) for a in range(round(-8*1.5625), round(8*1.5625))]
    for x in range (0, 9):
        for c in [(round(opponent.xcor() + x*1.5625), round(opponent.ycor() + a)) for a in range(round(-8*1.5625), round(8*1.5625))]:
            accepted_points.append(c)
    for i in range (0, 9):
        for b in [(round(opponent.xcor() + a), round(opponent.ycor() + i*1.5625)) for a in range(round(-8*1.5625), round(8*1.5625))]:
            accepted_points.append(b)
    for x in accepted_points:
        if x in lsd_points:
            LSD = round(16.255999479808015 * k.distance(opponent)) + random.randint(-200, 200)
        if x in msd_points:
            MSD = round(16.255999479808015 * k.distance(opponent)) + random.randint(-200, 200)
            count = 0
        if x in bsd_points:
            BSD = round(16.255999479808015 * k.distance(opponent)) + random.randint(-200, 200)
    if log: 
        values_list[0] = 3
        values_list[1] = pixels * 0.030557749073644
        values_list[2] = LSD
        values_list[3] = MSD
        values_list[4] = BSD
        values_list[5] = count
    if movement_count == len(movement_list) - 1: 
        finished = True
        movement_count = 0
    else: 
        if abs(movement_list[movement_count]) > 2:
            opponent.rt(movement_list[movement_count])
        else: 
            opponent.fd(movement_list[movement_count])
    movement_count += 1
    
    if finished: 
        k.goto(-80,60)
        run_auton(random.randint(0,2))
    
    accepted_points.clear()
    print(values_list)
    
def half_back():
    global log, count, pixels, values_list, movement_list, movement_count, finished
    LSD = 9999
    MSD = 9999
    BSD = 9999
    k.back(1)
    screen.update()
    pixels -= 1
    count += 20
    lsd_points = [(round(k.xcor() + 9*math.sqrt(2)*0.564705882352941 + a), round(k.ycor() + a)) for a in range(0,225)]
    msd_points = [(round(k.xcor() + a), round(k.ycor() + a)) for a in range(0,225)]
    bsd_points = [(round(k.xcor() + a), round(k.ycor() + 9*math.sqrt(2)*0.564705882352941 + a)) for a in range(0,225)]
    accepted_points = [(round(opponent.xcor() + 8*1.5625), round(opponent.ycor() + a)) for a in range(round(-8*1.5625), round(8*1.5625))]
    for x in range (0, 9):
        for c in [(round(opponent.xcor() + x*1.5625), round(opponent.ycor() + a)) for a in range(round(-8*1.5625), round(8*1.5625))]:
            accepted_points.append(c)
    for i in range (0, 9):
        for b in [(round(opponent.xcor() + a), round(opponent.ycor() + i*1.5625)) for a in range(round(-8*1.5625), round(8*1.5625))]:
            accepted_points.append(b)
    for x in accepted_points:
        if x in lsd_points:
            LSD = round(16.255999479808015 * k.distance(opponent)) + random.randint(-100, 100)
        if x in msd_points:
            MSD = round(16.255999479808015 * k.distance(opponent)) + random.randint(-100, 100)
            count = 0
        if x in bsd_points:
            BSD = round(16.255999479808015 * k.distance(opponent)) + random.randint(-100, 100)
    if log: 
        values_list[0] = 1
        values_list[1] = pixels * 0.030557749073644
        values_list[2] = LSD
        values_list[3] = MSD
        values_list[4] = BSD
        values_list[5] = count
    if movement_count == len(movement_list) - 1: 
        finished = True
        movement_count = 0
    else: 
        if abs(movement_list[movement_count]) > 2:
            opponent.rt(movement_list[movement_count])
        else: 
            opponent.fd(movement_list[movement_count])
    movement_count += 1
    
    if finished: 
        k.goto(-80,60)
        run_auton(random.randint(0,2))
    
    accepted_points.clear()
    print(values_list)

def stay():
    global log, count, pixels, values_list, movement_list, movement_count, finished
    LSD = 9999
    MSD = 9999
    BSD = 9999
    screen.update()
    count += 20
    lsd_points = [(round(k.xcor() + 9*math.sqrt(2)*0.564705882352941 + a), round(k.ycor() + a)) for a in range(0,225)]
    msd_points = [(round(k.xcor() + a), round(k.ycor() + a)) for a in range(0,225)]
    bsd_points = [(round(k.xcor() + a), round(k.ycor() + 9*math.sqrt(2)*0.564705882352941 + a)) for a in range(0,225)]
    accepted_points = [(round(opponent.xcor() + 8*1.5625), round(opponent.ycor() + a)) for a in range(round(-8*1.5625), round(8*1.5625))]
    for x in range (0, 9):
        for c in [(round(opponent.xcor() + x*1.5625), round(opponent.ycor() + a)) for a in range(round(-8*1.5625), round(8*1.5625))]:
            accepted_points.append(c)
    for i in range (0, 9):
        for b in [(round(opponent.xcor() + a), round(opponent.ycor() + i*1.5625)) for a in range(round(-8*1.5625), round(8*1.5625))]:
            accepted_points.append(b)
    for x in accepted_points:
        if x in lsd_points:
            LSD = round(16.255999479808015 * k.distance(opponent)) + random.randint(-100, 100)
        if x in msd_points:
            MSD = round(16.255999479808015 * k.distance(opponent)) + random.randint(-100, 100)
            count = 0
        if x in bsd_points:
            BSD = round(16.255999479808015 * k.distance(opponent)) + random.randint(-100, 100)
    if log: 
        values_list[0] = 2
        values_list[1] = pixels * 0.030557749073644
        values_list[2] = LSD
        values_list[3] = MSD
        values_list[4] = BSD
        values_list[5] = count
    if movement_count == len(movement_list) - 1: 
        finished = True
        movement_count = 0
    else: 
        if abs(movement_list[movement_count]) > 2:
            opponent.rt(movement_list[movement_count])
        else: 
            opponent.fd(movement_list[movement_count])
    movement_count += 1
    
    if finished: 
        k.goto(-80,60)
        run_auton(random.randint(0,2))
    
    accepted_points.clear()
    print(values_list)

def auton_block(x,y):
    run_auton(random.randint(0,2))
    k.fd(10)
    k.rt(45)
    screen.update()
    screen.onkeypress(full_forward, 'w')
    screen.onkeypress(full_backward, 's')
    screen.onkeypress(half_forward, 'i')
    screen.onkeypress(half_back, 'k')
    screen.onkeypress(stay, ' ')
    screen.listen()



screen.update()
screen.onclick(auton_block)
screen.mainloop()