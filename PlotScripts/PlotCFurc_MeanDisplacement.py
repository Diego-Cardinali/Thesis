from json import load
from matplotlib.pyplot import figure
from numpy import array, sqrt, linspace, diag
from scipy.stats import ks_2samp
from scipy.optimize import curve_fit

from CFurc_Argparse import ArgumentParser, SmartFormatter
from CFurc_Plot import PlotAndSave
from CFurc_FitFuncs import Line
from CFurc_OSManip import GetFiles, NoFiles

PLim = 0.95
KCut = 0.05


ExtKeys = [
    "Fast",
    "Slow"
    ]
IntKeys = [
    "Displacement",
    "DistanceTravelled"
    ]
Types = [
    "Means",
    "NSamples"
    ]
Labels = [
    "Mean displacement",
    "Mean travelled distance"
    ]
Colors = [
    "red",
    "blue"
    ]
FitLim = {
    "Fast" : [[0, 16], [0, 23], [0, 12], [0, 15]],
    "Slow" : [[0, 45], [0, 150], [1, 80], [1, 90]]
    }
Exps = ["E1","E2","E3","E4"]
Food = ["food","no food","food","no food"]
Y1Lim = {
    "Fast" : [[.34, .79],[.22, .48],[.46, .85],[.32, .48]],
    "Slow" : [[.07, .28],[.03, .12],[.07, .23],[.03, .18]]
    }

Parser = ArgumentParser(description = "", formatter_class = SmartFormatter)
Parser.add_argument("Directory", metavar= "-D", type = str, action = "store", help = "Path to the directory containing the files.")
Parser.add_argument("Keys", metavar = "-K", nargs = "+", help = "A list of keys. All files in the folder containing a Key in the name will be read. If a file contains two or more keys it will be read more than once. If each key is associated only to a single file there are no requirements for confirmation on plotting.")
Parser.add_argument("-Plot", "--P", "--p", action = "store_true", help = "If given the data is plotted.")
Parser.add_argument("-Save", "--S", "--s", action = "store_true", help = "If given the data is saved.")
Args = Parser.parse_args()

AllFiles = GetFiles(Args.Directory, Args.Keys)
if NoFiles(AllFiles):
    exit ("No valid files given.")

for Files in AllFiles:
    Ind = AllFiles.index(Files)
    for File in Files:
        Data = load(open(File, 'r'))
        for EK in ExtKeys:
            Fig = figure()
            Grid = Fig.add_gridspec(4, 1, hspace = 0)
            Ax0 = Fig.add_subplot(Grid[0:2, 0])
            Ax1 = Fig.add_subplot(Grid[2, 0])
            Ax2 = Fig.add_subplot(Grid[3, 0])
            Time = linspace(0, len(Data["Means"]["Mean"+EK+IntKeys[0]][Types[0]])/15, len(Data["Means"]["Mean"+EK+IntKeys[0]][Types[0]]))
            Ax0.plot(Time[0:], Data["Means"]["Mean"+EK+IntKeys[0]][Types[0]][0:], label = Labels[0], color = Colors[0])
            Ax1.plot(Time[2:], Data["Means"]["Mean"+EK+IntKeys[1]][Types[0]][2:], label = Labels[1], color = Colors[1])
            Ax2.plot(Time[0:], [X/float(Data["Means"]["Mean"+EK+IntKeys[0]][Types[1]][0])*100. for X in Data["Means"]["Mean"+EK+IntKeys[0]][Types[1]][0:]], color = "orange", label = "Data usage percentage")
            
            XFivePercent = next(LL[0] for LL in enumerate(Data["Means"]["Mean"+EK+IntKeys[1]][Types[1]]) if LL[1] < Data["Means"]["Mean"+EK+IntKeys[1]][Types[1]][0]*0.05)

            '''
            #XOnePercent = next(LL[0] for LL in enumerate(Data["Means"]["Mean"+EK+IntKeys[1]][Types[1]]) if LL[1] < Data["Means"]["Mean"+EK+IntKeys[1]][Types[1]][0]*0.01)

            #YMin0 = Data["Means"]["Mean"+EK+IntKeys[0]][Types[0]][0]
            #YMax0 = 10*Data["Means"]["Mean"+EK+IntKeys[0]][Types[0]][len(Data["Means"]["Mean"+EK+IntKeys[0]][Types[0]])-1]
            #YMin1 = Data["Means"]["Mean"+EK+IntKeys[0]][Types[1]][len(Data["Means"]["Mean"+EK+IntKeys[0]][Types[1]])-1]
            #YMax1 = Data["Means"]["Mean"+EK+IntKeys[0]][Types[1]][0]

            Ax0.vlines(
                Time[XFivePercent], YMin0, YMax0, linestyles = "dotted", color = "green")#, label = "Less than 5% of data")
            Ax1.vlines(
                Time[XFivePercent], YMin1, YMax1, linestyles = "dotted", label = "Less than 5% of data", color = "green")
            Ax0.vlines(
                Time[XOnePercent], YMin0, YMax0, linestyles = "dotted", color = "orange")#, label = "Less than 1% of data")
            Ax1.vlines(
                Time[XOnePercent], YMin1, YMax1, linestyles = "dotted", label = "Less than 1% of data", color = "orange")
            '''

            Ax0.set_ylabel("Distance (mm)")
            Ax0.set_xticklabels([])
            Ax0.set_xlim(xmin = 0., xmax = Time[XFivePercent])
            Ax0.set_ylim(ymin = 0., ymax = Data["Means"]["Mean"+EK+IntKeys[0]][Types[0]][XFivePercent]*1.2)

            Ax1.set_ylabel("Distance (mm)")
            Ax1.set_xticklabels([])
            Ax1.set_xlim(xmin = 0., xmax = Time[XFivePercent])
            Ax1.set_ylim(ymin = Y1Lim[EK][Ind][0], ymax = Y1Lim[EK][Ind][1])

            Ax2.set_xlabel("Time (s)")
            Ax2.set_ylabel("Percentage")
            Ax2.set_xlim(xmin = 0., xmax = Time[XFivePercent])
            Ax2.set_ylim(ymin = 0., ymax = 101.)

            
            XFit = Time[FitLim[EK][Ind][0]:FitLim[EK][Ind][1]]
            YFit = Data["Means"]["Mean"+EK+IntKeys[0]][Types[0]][FitLim[EK][Ind][0]:FitLim[EK][Ind][1]]
            Out, Covs = curve_fit(Line, XFit, YFit)
            YTest = Line(XFit, Out[0], Out[1])
            Stat, Prob = ks_2samp(YFit, YTest)
            FitLabel = "$Y$ $=$ $AX+B$"+"\nKS test probability: {:.3f}".format(Prob)+"\n$A$ $=$ ${:2.2f}\\pm {:.2f}$\n$B$ $=$ ${:2.2f}\\pm {:.2f}$".format(Out[0], sqrt(diag(Covs))[0], Out[1], sqrt(diag(Covs))[1])
            Ax0.plot(XFit, YTest, "--", color = "black", label = FitLabel)
            
            Fig.suptitle(Exps[Ind]+" ("+EK.lower()+", "+Food[Ind]+") mean displacement and travelled distance", fontsize = 20)
            Fig.legend(loc=(0.1275, 0.73), frameon = False, ncol = 2)
            PlotAndSave (Args.P, Args.S)
