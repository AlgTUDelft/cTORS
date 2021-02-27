import tkinter as tk
import queue
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
import numpy as np

class RLVisualizer(tk.Tk):
    
    def __init__(self, screenName="RL Visualizer", baseName=None, className='Tk', 
        useTk=1, sync=0, use=None):
        tk.Tk.__init__(self, screenName=screenName, baseName=baseName, className=className, useTk=useTk, sync=sync, use=use)
        self.plot_frame = RLPerformancePlot(self)
        self.plot_frame.pack()
        self.thread = None
        self.queue = queue.Queue()
        
    def start(self, thread):
        self.thread = thread
        self.thread.start()
        self.after(100, self.process_queue)
        self.mainloop()
        
    def process_queue(self):
        while self.queue.qsize() > 0:
            try:
                methodname, args, kwargs = self.queue.get(block=False)
                try:
                    getattr(self, methodname)(*args, **kwargs)
                except Exception as e:
                    print("Error in function call {} with arguments {} , {}".format(methodname, args, kwargs))
                    print(e)
            except queue.Empty:
                break
        if self.thread and self.thread.isAlive():
            self.after(100, self.process_queue)
        else:
            self.quit()
        
    def quit(self):
        tk.Tk.quit(self)
    
    def post(self, methodname, *args, **kwargs):
        self.queue.put((methodname, args, kwargs))
        
    def add_point(self, y, eps):
        self.plot_frame.add_point(y, eps)
        
class RLPerformancePlot(tk.Frame):
    
    def __init__(self, master=None, cnf={}, **kw):
        tk.Frame.__init__(self, master=master, cnf=cnf, **kw)
        plt.ion()
        self.figure = Figure(figsize=(4,2), dpi=300, constrained_layout=True)
        grid_spec = self.figure.add_gridspec(2, 1, height_ratios=[4,1])
        self.res_ax = self.figure.add_subplot(grid_spec[0,0])
        self.par_ax = self.figure.add_subplot(grid_spec[1,0], sharex=self.res_ax)
        plt.setp(self.res_ax.get_xticklabels(), visible=False)
        self.result_plot = self.res_ax.plot([], [])[0]
        self.total_plot = self.res_ax.plot([], [])[0]
        self.par_ax.set_ylim(0,1)
        self.eps_plot = self.par_ax.plot([],[])[0]
        self.canvas = FigureCanvasTkAgg(self.figure, master=self)
        self.canvas.draw()
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)
        self.toolbar = NavigationToolbar2Tk(self.canvas, self)
        self.toolbar.update()
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)
        
    def add_point(self, y, eps):
        xdata = self.result_plot.get_xdata()
        ydata = self.result_plot.get_ydata()
        x = len(xdata) + 1
        xdata = np.append(xdata, x)
        ydata = np.append(ydata, y)
        toty = sum(ydata[-25:]) / min(25, len(ydata))
        self.result_plot.set_xdata(xdata)
        self.result_plot.set_ydata(ydata)
        self.total_plot.set_xdata(xdata)
        self.total_plot.set_ydata(np.append(self.total_plot.get_ydata(), toty))
        xlim = max(1, x-50),max(2, x)
        self.res_ax.set_xlim(*xlim)
        self.res_ax.set_ylim(min(0,min(ydata)),max(0.1, max(ydata)))
        epsdata = self.eps_plot.get_ydata()
        epsdata = np.append(epsdata, eps)
        self.eps_plot.set_ydata(epsdata)
        self.eps_plot.set_xdata(xdata)
        self.par_ax.set_xlim(*xlim)
        self.canvas.draw()
        self.canvas.flush_events()
        