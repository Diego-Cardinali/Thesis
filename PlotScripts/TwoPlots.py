import matplotlib.pyplot as plt
import json
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
from pathlib import Path

from CleanData import CleanData

FigWidth = 32
FigHeight = 20

###########################################################################################
###     NEEDS REWORKING     NEEDS REWORKING     NEEDS REWORKING     NEEDS REWORKING     ###
###########################################################################################

def TwoPlots(AllFiles, Clean, Display, Settings, SavePath) :
    for Files in AllFiles:
        for I in range(len(Files)):
            Data = json.load(open(Files[I], 'r'))
            XYData = []
            Width = []
            #Z in radius in spherical coordinates, the order is (Azimuth, Inclination, Radius)
            for Var in ["X", "Y"]:
                XYData.append([np.linspace(Data[Var]["Min"], Data[Var]["Max"], Data["NBins"]).tolist(), Data[Var]["FrequencyHistogram"]])
                Width.append(abs(Data[Var]["Max"]-Data[Var]["Min"])/Data["NBins"])
            if Clean:
                for J in range (len(XYData)):
                    XYData[J] = CleanData(XYData[J])
            Fig, Ax = plt.subplots(2, figsize = (FigWidth, FigHeight))
            for J in range (len(XYData[0])):
                Ax[J].bar(XYData[J][0], XYData[J][1], width = Width[J]*1.01)
                plt.gca().set_xlim([XYData[J][0][0],XYData[J][0][len(XYData[J][0])-1]])
                Ax[J].set_title(Settings["Title"], fontsize = Settings["TitleSize"])
                Ax[J].xaxis.set_major_locator(mpl.ticker.MultipleLocator(10*Width[J]))
                Ax[J].set_xlabel(Settings["XAxisName"], fontsize = Settings["XAxisNameSize"])
                Ax[J].set_ylabel(Settings["YAxisName"], fontsize = Settings["YAxisNameSize"])
            if Display:
                figManager = plt.get_current_fig_manager()
                figManager.window.showMaximized()
                plt.show()
            else:
                Fig.savefig(SavePath+Path(Files[I]).stem+("_Clean.png" if Clean else ".png"), bbox_inches = "tight")
    return