import os
import sys
from PIL import Image, ImageDraw
import datetime 

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches

D_NAME = "plot"
D_DPI = 300


class Coords:
    def __init__(self, x, y):
        self.x = x
        self.y = y

class Driver:
    """docstring for Driver"""
    def __init__(self, src_name, out_name):
        self.src_name = src_name
        self.out_name = out_name
        self.data_file = open(src_name,'r')
    @property
    def default_name(self):
        time_label =time.strftime("%b_%d_%H:%M:%S", datetime.now())
        return f"{self.out_name}_{time_label}"

class DriverMatplotlib(Driver):
    def __init__(self, 
        src_name,
        out_name = D_NAME,
        grid = True, 
        boundaries = [(-2,2),(-2,2)],
        draw_bounds = True
        ):
        super(DriverMatplotlib, self).__init__(src_name, out_name)
        self.cell_size = Coords(0.1,0.1)
        self.unique_name = self.out_name != D_NAME
        fig,ax = plt.subplots()
        self.ax = ax  
        self.fig  = fig
        if grid:
            plt.grid()
        self.boundaries = boundaries
        if draw_bounds:
            self.draw_borders()
        #fig, ax = plt.subplots(x,y) 
        #plt.figure()
    def draw_borders(self):
        for i in range(2):
            self.ax.vlines(self.boundaries[0][i],self.boundaries[1][0] , self.boundaries[1][1])
        for i in range(2):
            self.ax.hlines(self.boundaries[1][i],self.boundaries[0][0] , self.boundaries[0][1])
    def save_picture(self, path = " "):
        out_name = self.out_name if self.unique_name else self.default_name
        plt.savefig(self.out_name, dpi = D_DPI)
    def plot_show(self):
        self.ax.plot()
        plt.show()
    def test_draw(self):
        for i in range(0,10,3):
            for j in range(10):
                self.draw_cell2d (Coords(i,j))
        self.plot_show()
    def draw_cell2d(self, coords):
        # Parallelogram
        x = coords.x
        y = coords.y
        cell_size = self.cell_size
        #xy, (x, y,y
        #x = [x, x, x + cell_size.x, x + cell_size.x]
        #y = [y, y - cell_size.y, y - cell_size.y, y]
        rect = patches.Rectangle((x, y-cell_size.y),cell_size.x, cell_size.y, color='black')
        self.ax.add_patch(rect)
 
#ax.add_patch(patches.Polygon(xy=list(zip(x,y)), fill=False))
        


class DriverChainRecurrentSet(Driver):
    def __init__(self, src_name, out_name = "ChainRecurrentSet"):
        super(ClassName, self).__init__(src_name, out_name)

        
class ClassName(object):
    """docstring for ClassName"""
    def __init__(self, arg):
        super(ClassName, self).__init__()
        self.arg = arg
