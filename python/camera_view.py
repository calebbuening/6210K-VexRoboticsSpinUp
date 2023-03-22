import turtle
import subprocess

# Set up the turtle screen
screen = turtle.Screen()
screen.setup(640, 480)
screen.bgcolor('white')

# Set up the subprocess to run the C++ program
p = subprocess.Popen(["pros terminal"], stdout=subprocess.PIPE)

# Create a turtle to draw with
t = turtle.Turtle()

# Loop to read the output from the subprocess and update the turtle
while True:
    # Read a line of output from the subprocess
    line = p.stdout.readline().decode().strip()
    if not line:
        break
    if line.split("")[1] not in ['0','1', '2', '3', '4', '5', '6', '7', '8', '9']:
        pass
    # Split the line into fields
    fields = line.split(";")
    U = fields[0].split(",")[3]
    V = fields[0].split(",")[6]
    Y = .5
    R = Y + 1.4075 * (V - 128)
    G = Y - 0.3455 * (U - 128) - (0.7169 * (V - 128))
    B = Y + 1.7790 * (U - 128)
    color = (R,G,B)
    x = int(fields[3])
    y = int(fields[4])
    width = int(fields[5])
    height = int(fields[6])
    # Draw a rectangle representing the object
    t.penup()
    t.goto(x - width / 2, y - height / 2)
    t.pendown()
    t.fillcolor(color)
    t.begin_fill()
    t.goto(x + width / 2, y - height / 2)
    t.goto(x + width / 2, y + height / 2)
    t.goto(x - width / 2, y + height / 2)
    t.goto(x - width / 2, y - height / 2)
    t.end_fill()

# Close the subprocess
p.kill()