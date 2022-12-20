import turtle
import random
from csv import writer

MODE = "drive"
NOISE = False

import turtle

# Set up the turtle and screen
t = turtle.Turtle("square", visible=True)
turtle.tracer(0,0)
s = turtle.Screen()
s.screensize(1400, 1400)

# Draw the rectangle
t.pu()
t.back(60)
t.right(90)
t.forward(60)
t.left(90)
t.pd()
wall_points = []
for i in range (4):
    n = 0
    while n < 120:
        wall_points.append((t.xcor(), t.ycor()))
        t.forward(.1)
        n+=.1
    t.left(90)
turtle.update()
t.pu()
t.goto(0,0)
turtle.update()

# define function to record theoretical sensor values
def record_data():
    csv_row = []
    t.hideturtle()
    for i in range (4):
        if NOISE:
            # Set the mean and standard deviation of the noise
            mean = 0
            std = 0.1
            # Generate a random noise value
            x_noise = random.gauss(mean, std)
            y_noise = random.gauss(mean, std)
            distance_noise = random.gauss(mean, std)
        else:
            x_noise = 0
            y_noise = 0
            distance_noise = 0
        original_pos = t.pos()
        gps_x = t.xcor() + x_noise
        gps_y = t.ycor() + y_noise
        while (t.pos()) not in wall_points:
            t.forward(.1)
        sensor_x = t.xcor()
        sensor_y = t.ycor()
        t.goto(original_pos)
        #convert distance to mm
        #get distance in pixel
        raw_distance = t.distance(sensor_x, sensor_y)
        distance = (raw_distance/10*304.8) + distance_noise
        csv_row.append(gps_x)
        csv_row.append(gps_y)
        csv_row.append(distance)
        t.left(90)
    with open('C:\\Users\\mcurn\\OneDrive\\Documents\\GitHub\\6210K-VexRoboticsSpinUp\\nn_data\\data.csv', 'a') as f_object:

        # Pass this file object to csv.writer()
        # and get a writer object
        writer_object = writer(f_object)
    
        # Pass the list as an argument into
        # the writerow()
        writer_object.writerow(csv_row)
    
        # Close the file object
        f_object.close()
    t.showturtle()
# Set up the click event handler
def on_w():
    if t.pos() not in wall_points:
        t.forward(1)
        turtle.update()
    record_data()
def on_s():
    if t.pos() not in wall_points:
        t.back(1)
        turtle.update()
    record_data()
def on_a():
    t.left(1)
    turtle.update()
    record_data()
def on_d():
    t.right(1)
    turtle.update()
    record_data()
def go_to(x,y):
    if (x,y) not in wall_points:
        t.goto(x,y)
        record_data()

if MODE == "drive":
    s.onkey(on_w, "w")
    s.onkey(on_s, "s")
    s.onkey(on_a, "a")
    s.onkey(on_d, "d")
    s.listen()

    # Wait for user input
    turtle.done()

if MODE == "click":
    s.onclick(t.goto)
    turtle.update()

if MODE == "auto":
    x = random.randrange(-120, 120, 0.1)
    y = random.randrange(-120, 120, 0.1)
    t.goto(x,y)
    turtle.update()
    record_data()