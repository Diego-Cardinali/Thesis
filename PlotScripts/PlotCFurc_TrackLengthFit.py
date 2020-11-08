from json import load
from matplotlib.pyplot import subplots
from numpy import sqrt, diag, mean, linspace, vstack, array, diff
from scipy.stats import ks_2samp
from scipy.optimize import curve_fit
from scipy.integrate import cumtrapz

from CFurc_Argparse import ArgumentParser, SmartFormatter
from CFurc_Plot import PlotAndSave
from CFurc_OSManip import NoFiles, GetFiles, CheckValidPath
from CFurc_FitFuncs import ExpLawT, PowLawT
from CFurc_DataManip import DivideEvenUneven

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

NBinsFast = [16, 16, 16, 16]
NBinsSlow = [16, 16, 16, 16]
LimitFast = [0, 1, 0, 1]
LimitSlow = [1, 5, 4, 5]
NFast = [300, 175, 300, 175]
NSlow = [250, 150, 200, 150]
ParamsInitFast = [
    [1., -1., 0.],
    [1., -1., 0.],
    [1., -1., 0.],
    [1., -1., 0.]
    ]

ParamsInitSlow = [
    [1., -1., 0.],
    [1., -1., 0.],
    [1., -1., 0.],
    [1., -1., 0.]
    ]
FitsFast = ["PowLawT","PowLawT","PowLawT","PowLawT"]
FitsSlow = ["ExpLawT","ExpLawT","ExpLawT","ExpLawT"]

Parser = ArgumentParser(description = "", formatter_class = SmartFormatter)
Parser.add_argument("Directory", metavar= "-D", type = str, action = "store", help = "Path to the directory containing the files.")
Parser.add_argument("Keys", metavar = "-K", nargs = "+", help = "A list of keys. All files in the folder containing a Key in the name will be read. If a file contains two or more keys it will be read more than once. If each key is associated only to a single file there are no requirements for confirmation on plotting.")
Parser.add_argument("-Plot", "--P", "--p", action = "store_true", help = "If given the data is plotted.")
Parser.add_argument("-Save Path", "--SP", "--sp", action = "store", help = "Path where to save data, names will be the keys in progressive order. Default is input path. If the path is invalid resort to default.")
Parser.add_argument("-Save", "--S", "--s", action = "store_true", help = "If given the data is saved.")
Args = Parser.parse_args()

AllFiles = GetFiles(Args.Directory, Args.Keys)
if NoFiles(AllFiles):
    exit ("No valid files given.")

if Args.SP is None or not CheckValidPath(Args.SP):
    Args.SP = Args.Directory

for Files in AllFiles:
    Ind = AllFiles.index(Files)
    for File in Files:
        Data = load(open(File, 'r'))
        #Fast is even, slow is uneven
        FastData, SlowData = DivideEvenUneven(Data["Length"], NFast[Ind], NSlow[Ind])
        FastData = FastData/15
        SlowData = SlowData/15

        Fig, Ax = subplots(1)
        FastHist, FastBins, Patches = Ax.hist(FastData, bins = NBinsFast[Ind], density =  True, stacked = True)#, label = "Track duration PDF")
        Ax.set_title(Exps[Ind] +" Track duration PDF (fast, "+Food[Ind]+")", fontsize = 24)
        Ax.set_xlabel("Duration (s)", fontsize = 20)
        Ax.set_ylabel("PDF", fontsize = 20)

        try:
            FastXCut = next(LL[0] for LL in enumerate(FastHist) if LL[1] < .01)
            FastHist = FastHist[LimitFast[Ind]:FastXCut]
            FastBins = FastBins[LimitFast[Ind]:FastXCut+1]
        except StopIteration:
            FastHist = FastHist[LimitFast[Ind]:]
            FastBins = FastBins[LimitFast[Ind]:]
        FastBinsCenter = mean(vstack([FastBins[0:-1],FastBins[1:]]), axis=0)

        ParFast, CovFast = curve_fit(PossibleFits[FitsFast[Ind]], FastBinsCenter, FastHist, p0=ParamsInitFast[Ind], sigma = sqrt(FastHist), maxfev = 100000000)

        XXFast = linspace(FastBinsCenter[0], FastBins[len(FastBins)-1], 5000)
        YYFast = PossibleFits[FitsFast[Ind]](XXFast, *ParFast)
        Stat, ProbFast = ks_2samp(FastHist, YYFast)

        FastFitLabel = Labels[FitsFast[Ind]]+"\nKS test probability: {:.3f}".format(ProbFast)+"\n${:}$ $=$ ${:2.2f}\\pm {:.2f}$".format(Names[FitsFast[Ind]][1], ParFast[1], sqrt(diag(CovFast))[1])

        Ax.plot(XXFast, YYFast , color = "black", label = FastFitLabel)
        Ax.set_yscale("log")
        Ax.set_xlim(0., FastBins[-1]*1.05)
        Ax.set_ylim(1e-3, 1.)
        Ax.legend(loc='upper right', frameon=False)

        PlotAndSave(Args.P, Args.S, Args.SP+Args.Keys[Ind]+"_fast_"+str(Files.index(File)+1).zfill(4), "pdf")

        Fig, Ax = subplots(1)
        SlowHist, SlowBins, Patches = Ax.hist(SlowData, bins = NBinsSlow[Ind], density =  True, stacked = True)#, label = "Track duration PDF")
        Ax.set_title(Exps[Ind] +" Track duration PDF (slow, "+Food[Ind]+")", fontsize = 24)
        Ax.set_xlabel("Duration (s)", fontsize = 20)
        Ax.set_ylabel("PDF", fontsize = 20)

        try:
            SlowXCut = next(LL[0] for LL in enumerate(SlowHist) if LL[1] < .01)
        except StopIteration:
            SlowXCut = SlowHist[-1]

        try:
            SlowXCut = next(LL[0] for LL in enumerate(SlowHist) if LL[1] < .01)
            SlowHist = SlowHist[LimitSlow[Ind]:SlowXCut]
            SlowBins = SlowBins[LimitSlow[Ind]:SlowXCut+1]
        except StopIteration:
            SlowHist = SlowHist[LimitSlow[Ind]:]
            SlowBins = SlowBins[LimitSlow[Ind]:]
        SlowBinsCenter = mean(vstack([SlowBins[0:-1],SlowBins[1:]]), axis=0)

        ParSlow, CovSlow = curve_fit(PossibleFits[FitsSlow[Ind]], SlowBinsCenter, SlowHist, p0=ParamsInitSlow[Ind], sigma = sqrt(SlowHist), maxfev = 100000000)

        XXSlow = linspace(SlowBinsCenter[0], SlowBins[len(SlowBins)-1], 5000)
        YYSlow = PossibleFits[FitsSlow[Ind]](XXSlow, *ParSlow)
        Stat, ProbSlow = ks_2samp(SlowHist, YYSlow)

        SlowFitLabel = Labels[FitsSlow[Ind]]+"\nKS test probability: {:.3f}".format(ProbSlow)+"\n${:}$ $=$ ${:2.2f}\\pm {:.2f}$".format(Names[FitsSlow[Ind]][1], ParSlow[1], sqrt(diag(CovSlow))[1])

        Ax.plot(XXSlow, YYSlow , color = "red", label = SlowFitLabel)
        Ax.set_yscale("log")
        Ax.set_xlim(0., SlowBins[-1]*1.05)
        Ax.set_ylim(1e-3, 1.)
        Ax.legend(loc='upper right', frameon=False)

        PlotAndSave(Args.P, Args.S, Args.SP+Args.Keys[Ind]+"_slow_"+str(Files.index(File)+1).zfill(4), "pdf")
