from json import load
from matplotlib.pyplot import subplots
from numpy import sqrt, diag, mean, linspace, vstack, array
from scipy.stats import ks_2samp
from scipy.optimize import curve_fit

from CFurc_Argparse import ArgumentParser, SmartFormatter
from CFurc_Plot import PlotAndSave
from CFurc_OSManip import NoFiles, GetFiles
from CFurc_FitFuncs import ExpLawT, PowLawT

Labels = {
    "ExpLawT" : "$Y$ $=$ $C$ $\\exp\\left[\\beta(X-X_{0})\\right]$",
    "PowLawT" : "$Y$ $=$ $C$ $\\left(X-X_{0}\\right)^{\\alpha}$",
}
PossibleFits = {
    "ExpLawT" : ExpLawT,
    "PowLawT" : PowLawT,
}
Names = {
    "ExpLawT" : ["C", "\\beta", "X_{0}"],
    "PowLawT" : ["C", "\\alpha", "X_{0}"],
}

Exps = ["E1", "E2", "E3", "E4"]
Food = ["food", "no food", "food", "no food"]

PLim = 0.95

NBinsFast = [15, 15, 15, 15]
NBinsSlow = [15, 15, 15, 15]
LimitFast = [
    [1, 12],
    [1, 10],
    [1, 10],
    [1, 7]
    ]
LimitSlow = [
    [2, 12],
    [4, 12],
    [4, 12],
    [3, 11]
    ]
NFast = [30., 40., 40., 40.]
NSlow = [14.5, 14.5, 14.5, 14.5]
ParamsInitFast = [
    [1., -1., 0.],
    [1., -1., 0.],
    [1., -1., 0.],
    [1., -1., 0.]
    ]
#ParamsInitSlow = [
#    [1., -1., 0.],
#    [1., -1., 0.],
#    [1., -1., 0.],
#    [1., -1., 0.]
#    ]
FitsFast = ["ExpLawT","ExpLawT","ExpLawT","ExpLawT"]
#FitsSlow = ["PowLawT","ExpLawT","ExpLawT","ExpLawT"]

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
        FastData = array([X["Displacement"] for X in Data["Fast"] if X["Displacement"] < NFast[Ind]])
        SlowData = array([X["Displacement"] for X in Data["Slow"] if X["Displacement"] < NSlow[Ind]])

        Fig, Ax = subplots(1)
        FastHist, FastBins, Patches = Ax.hist(FastData, bins = NBinsFast[Ind], density =  True, stacked = True)#, label = "Displacement PDF")
        Ax.set_title(Exps[Ind] +" (fast, "+Food[Ind]+") displacement PDF", fontsize = 24)
        Ax.set_xlabel("Distance (mm)", fontsize = 20)
        Ax.set_ylabel("PDF", fontsize = 20)
        Ax.set_xlim(0., FastBins[LimitFast[Ind][1]])
        Ax.set_ylim(1e-3, 1.)

        FastHist = FastHist[LimitFast[Ind][0]:LimitFast[Ind][1]]
        FastBins = FastBins[LimitFast[Ind][0]:LimitFast[Ind][1]+1]
        FastBinsCenter = mean(vstack([FastBins[0:-1],FastBins[1:]]), axis=0)

        ParFast, CovFast = curve_fit(PossibleFits[FitsFast[Ind]], FastBinsCenter, FastHist, p0=ParamsInitFast[Ind], sigma = sqrt(FastHist), maxfev = 1000000000)

        XXFast = linspace(FastBinsCenter[0], FastBinsCenter[-1], 5000)
        YYFast = PossibleFits[FitsFast[Ind]](XXFast, *ParFast)

        Stat, ProbFast = ks_2samp(FastHist, YYFast)

        FitLabel = Labels[FitsFast[Ind]]+"\nKS test probability: {:.3f}".format(ProbFast)+"\n${:}$ $=$ ${:2.2f}\\pm {:.2f}$".format(Names[FitsFast[Ind]][1], ParFast[1], sqrt(diag(CovFast))[1])

        Ax.plot(XXFast, YYFast , color = "red", label = FitLabel)
        Ax.set_yscale("log")
        #Ax.set_xlim(right = 41.)
        #Ax.set_xticks([2*I for I in range (21)], minor = False)
        #Ax.set_xticks([I for I in range (41)], minor = True)
        Ax.legend(loc='upper right', frameon=False)

        PlotAndSave(Args.P, Args.S)

        Fig, Ax = subplots(1)
        SlowHist, SlowBins, Patches = Ax.hist(SlowData, bins = NBinsSlow[Ind], density =  True, stacked = True)#, label = "Displacement PDF")
        Ax.set_title(Exps[Ind] +"  (slow, "+Food[Ind]+") displacement PDF", fontsize = 24)
        Ax.set_xlabel("Distance (mm)", fontsize = 20)
        Ax.set_ylabel("PDF", fontsize = 20)
        Ax.set_xlim(0., SlowBins[LimitSlow[Ind][1]])
        Ax.set_ylim(5e-3, 1.)

        #SlowHist = SlowHist[LimitSlow[Ind][0]:LimitSlow[Ind][1]]
        #SlowBins = SlowBins[LimitSlow[Ind][0]:LimitSlow[Ind][1]+1]
        #SlowBinsCenter = mean(vstack([SlowBins[0:-1],SlowBins[1:]]), axis=0)

        #ParSlow, CovSlow = curve_fit(PossibleFits[FitsSlow[Ind]], SlowBinsCenter, SlowHist, p0=ParamsInitSlow[Ind], sigma = sqrt(SlowHist), maxfev = 1000000000)

        #XXSlow = linspace(SlowBinsCenter[0], SlowBins[len(SlowBins)-1], 5000)
        #YYSlow = PossibleFits[FitsSlow[Ind]](XXSlow, *ParSlow)

        #Stat, ProbSlow = ks_2samp(SlowHist, YYSlow)

        #FitLabel = Labels[FitsFast[Ind]]+"\nKS test probability: {:.3f}".format(ProbSlow)+"\n${:}$ $=$ ${:2.2f}\\pm {:.2f}$".format(Names[FitsFast[Ind]][1], ParSlow[1], sqrt(diag(CovSlow))[1])

        #Ax.plot(XXSlow, YYSlow , color = "red", label = FitLabel)
        Ax.set_yscale("log")
        #Ax.set_xlim(right = 16.)
        #Ax.set_xticks([I for I in range (17)], minor = False)
        #Ax.set_xticks([I/2 for I in range (32)], minor = True)
        Ax.legend(loc='upper right', frameon=False)

        PlotAndSave(Args.P, Args.S)