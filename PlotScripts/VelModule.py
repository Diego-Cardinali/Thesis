import matplotlib.pyplot as plt
import json
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
from pathlib import Path

from CleanData import CleanData

FigWidth = 32
FigHeight = 24

def PlotVelocityModule(AllFiles, Clean, Display, SavePath) :
    for Files in AllFiles:    
        for I in range(len(Files)):
            Data = json.load(open(Files[I], 'r'))
            PlotData = [np.linspace(Data["Z"]["Min"],Data["Z"]["Max"],Data["NBins"]).tolist(), Data["Z"]["FrequencyHistogram"]]
            Width = abs(Data["Z"]["Max"]-Data["Z"]["Min"])/Data["NBins"]
            if Clean:
                PlotData = CleanData(PlotData)
            Fig, Ax = plt.subplots(1, figsize = (FigWidth, FigHeight))
            Ax.bar(PlotData[0], PlotData[1], width = Width*1.01)
            Ax.set_xlim([PlotData[0][0],PlotData[0][len(PlotData[0])-1]])
            Ax.set_title("Velocity module distribution", fontsize = 24)
            Ax.xaxis.set_major_locator(mpl.ticker.MultipleLocator(20*Width))
            Ax.set_xlabel("Velocity (mm/s)", fontsize = 20)
            Ax.set_ylabel("Occurrences", fontsize = 20)
            if Display:
                figManager = plt.get_current_fig_manager()
                figManager.window.showMaximized()
                plt.show()
            else:
                Fig.savefig(SavePath+Path(Files[I]).stem+("_Clean.png" if Clean else ".png"), bbox_inches = "tight")
    return