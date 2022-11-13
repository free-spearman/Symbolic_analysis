import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import Driver 

test = Driver.DriverMatplotlib("Driver.py")
test.test_draw()
"""
class Coords:
	def __init__(self, x, y):
		self.x = x
		self.y = y

def draw_cell2d(plot, coords):
        # Parallelogram
        x = coords.x
        y = coords.y
        cell_size = Coords(0.2,0.2)
        #xy, (x, y,y
        #x = [x, x, x + cell_size.x, x + cell_size.x]
        #y = [y, y - cell_size.y, y - cell_size.y, y]
        rect = patches.Rectangle((x, y-cell_size.y),cell_size.x, cell_size.y, facecolor='black')
        plot.add_patch(rect)

#fig, axs = plt.subplots(2, 2)
fig, ax = plt.subplots()
for i in range(0,10,3):
	for j in range(10):
		draw_cell2d(ax, Coords(i,j))
ax.plot() 
fig.suptitle('Axes values are scaled individually by default')
x_min = -10
x_max = 10
y = 2
ax.hlines(y, x_min, x_max)
#fig = plt.figure()
#ax1 = fig.add_subplot(211)
plt.grid()
plt.show()
#axs.show()
"""
"""
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle

#define Matplotlib figure and axis
fig, ax = plt.subplots()

#create simple line plot
ax.plot([0, 10],[0, 10])

#add rectangle to plot
ax.add_patch (Rectangle((1, 1), 2, 6))

#display plot
plt.show() """
