import os
import sys
from PIL import Image, ImageDraw
import datetime 

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches

D_NAME = "plot"
D_DPI = 300
PARAM_LABEL = '#'
SEP_AXIS = ';'
SEP_VAL = ','
SEP_EQ = ':'
SEP_SPACE = " "
FILE_PARAMS = ["boundaries",
    "cell_size",
    "components"
]

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

    def parse_boundaries(self, data = False):
        if not data:
            data = self.data_file.readline()
        boundaries = {}
        print(f"data = {data}")
        for item in data.split(SEP_AXIS)[:-1]:
            print(f"item = {item}")
            axis, vals = item.split(SEP_EQ)
            self.axes.append(axis)
            boundaries[axis] = tuple([float(val) for val in vals.split(SEP_VAL)])
        self.boundaries = boundaries
        return self.boundaries
    
    def parse_cell_size(self, data = False):
        if not data:
            data = self.data_file.readline()
        cell_size = {}
        for item in data.split(SEP_AXIS)[:-1]:
            axis, val = item.split(SEP_EQ)
            cell_size[axis] = float(val)
        print(f"cell_size { cell_size}")
        self.cell_size = cell_size
        return self.cell_size
    def parse_components(self, data = False):
        #добавить тест флагов
        cells = []
        for line in self.data_file:
            cells.append([float(val) for val in line.split(SEP_SPACE)])
        self.cells = cells
        return self.cells
    def parse_data_file(self):
        for line in self.data_file:
            if line[0] != PARAM_LABEL:
                continue
            name = line[1:-1]
            try:
                parser = getattr(self, f"parse_{name}")   
            except Exception as e:
                print(f"not attr: {name}")
                print(e)
            parser()

class DriverMatplotlib(Driver):
    def __init__(self, 
        src_name,
        out_name = D_NAME,
        grid = True, 
        boundaries = {"x":(-2,2),"y": (-2,2)},
        draw_bounds = True
        ):
        super(DriverMatplotlib, self).__init__(src_name, out_name)
        self.unique_name = self.out_name != D_NAME
        fig,ax = plt.subplots()
        self.ax = ax  
        self.fig  = fig
        if grid:
            plt.grid()
        self.boundaries = boundaries
        self.axes = []
        #for axis in boundaries.keys():
        #    self.axes.add(axis)
        if draw_bounds:
            self.draw_borders()
        #fig, ax = plt.subplots(x,y) 
        #plt.figure()
    def draw_borders(self):
        if len(self.axes) > 2:
            return
        for i in range(2):
            self.ax.vlines(self.boundaries["x"][i],self.boundaries["y"][0] , self.boundaries["y"][1])
        for i in range(2):
            self.ax.hlines(self.boundaries["y"][i],self.boundaries["x"][0] , self.boundaries["x"][1])
    def save_picture(self, path = ""):
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
        # нужно доделать
        x = coords.x
        y = coords.y
        cell_size = Coords(self.cell_size["x"],self.cell_size["y"])
        #xy, (x, y,y
        #x = [x, x, x + cell_size.x, x + cell_size.x]
        #y = [y, y - cell_size.y, y - cell_size.y, y]
        #print(f"cell_size = {cell_size.x} , {cell_size.y}")
        rect = patches.Rectangle((x, y-cell_size.y),cell_size.x, cell_size.y, color='black')
        self.ax.add_patch(rect)
    def draw_sym_im(self):
        for cell in self.cells:
            self.draw_cell2d(Coords(cell[0],cell[1]))
        self.plot_show()
            
 
#ax.add_patch(patches.Polygon(xy=list(zip(x,y)), fill=False))
        


class DriverChainRecurrentSet(Driver):
    def __init__(self, src_name, out_name = "ChainRecurrentSet"):
        super(ClassName, self).__init__(src_name, out_name)

        
class ClassName(object):
    """docstring for ClassName"""
    def __init__(self, arg):
        super(ClassName, self).__init__()
        self.arg = arg
