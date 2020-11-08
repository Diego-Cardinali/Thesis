from json import load
from matplotlib.pyplot import figure, close, hist
from numpy import sqrt, diag, mean, linspace, vstack, array
from scipy.integrate import cumtrapz

from CFurc_Argparse import ArgumentParser, SmartFormatter
from CFurc_Plot import PlotAndSave
from CFurc_OSManip import NoFiles, GetFiles, CheckValidPath
from CFurc_FitFuncs import ExpLawT, PowLawT
from CFurc_DataManip import DivideEvenUneven, GetCostFunc

NFast = [175, 175, 175, 175]
NSlow = [0, 0, 0, 0]
NBins = [22, 22, 22, 22]
YInf = [1e-2, 1e-2, 1e-2, 3e-3]
#FastLim = [14, 14, 14, 14]
Exps = ["E1", "E2", "E3", "E4"]
Food = ["food", "no food", "food", "no food"]

Parser = ArgumentParser(description = "", formatter_class = SmartFormatter)
Parser.add_argument("Directory", metavar= "-D", type = str, action = "store", help = "Path to the directory containing the files.")
Parser.add_argument("Keys", metavar = "-K", nargs = "+", help = "A list of keys. All files in the folder containing a Key in the name will be read. If a file contains two or more keys it will be read more than once. If each key is associated only to a single file there are no requirements for confirmation on plotting.")
Parser.add_argument("-Plot", "--P", "--p", action = "store_true", help = "If given the data is plotted.")
Parser.add_argument("-Save", "--S", "--s", action = "store_true", help = "If given the data is saved.")
Parser.add_argument("-Save Path", "--SP", "--sp", action = "store", help = "Path where to save data, names will be the keys in progressive order. Default is input path. If the path is invalid resort to default.")
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

        Fig = figure()
        Grid = Fig.add_gridspec(1, 1, hspace = 0.3)
        Ax = Fig.add_subplot(Grid[0, 0])

        FastHist, FastBins, Patches = Ax.hist(FastData, bins = NBins[Ind], density =  True, stacked = True, alpha = 0.3)
        
        FastBinsCenter = mean(vstack([FastBins[0:-1],FastBins[1:]]), axis=0)
        
        FastCDF = cumtrapz(x=FastBinsCenter, y=FastHist)
        FastCDF = FastCDF/max(FastCDF)
        FastCDFBar = 1-FastCDF
        FastHazard = GetCostFunc(FastCDFBar)

        XCut = next(LL[0] for LL in enumerate(FastBinsCenter) if LL[1] > 5.)

        FastHist = FastHist[0:XCut]
        FastBins = FastBins[0:XCut+1]
        FastBinsCenter = mean(vstack([FastBins[0:-1],FastBins[1:]]), axis=0)

        Ax.plot(FastBinsCenter, FastHist, label =  "Track duration PDF", color = "blue")
        Ax.plot(FastBinsCenter, FastCDFBar[:XCut], label = "Track duration 1-CDF", color = "red")
        Ax.plot(FastBinsCenter, FastHazard[:XCut], label = "Track duration hazard function", color = "black")
        Ax.set_xlabel("Duration (s)", fontsize = 20)
        Ax.set_yscale("log")
        Ax.set_xlim(0, FastBins[XCut]+.1)
        Ax.set_ylim(YInf[Ind], 1.2)
        Ax.legend(loc='upper right', frameon=False)

        Fig.suptitle(Exps[Ind]+" (fast, "+Food[Ind]+") probability functions", fontsize = 24)
        PlotAndSave(Args.P, Args.S, Args.SP+Args.Keys[Ind]+"_"+str(Files.index(File)+1).zfill(4), "pdf")