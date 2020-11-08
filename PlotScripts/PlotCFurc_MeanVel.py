from json import load
from matplotlib.pyplot import figure
from numpy import array

from CFurc_Argparse import ArgumentParser, SmartFormatter
from CFurc_Plot import PlotAndSave
from CFurc_OSManip import NoFiles, GetFiles

PLim = 0.95
KCut = 0.05

Exps = ["E1", "E2", "E3", "E4"]
Food = ["food", "no food", "food", "no food"]

ExtKeys = [
	"Slow",
	"Fast"
	]
Values = {
	"foodSlow" : {"Min": 0., "Max" : 6.},
	"no foodSlow" : {"Min": 0., "Max" : 2.1},
	"foodFast" : {"Min": 6., "Max" : 13.},
	"no foodFast" : {"Min": 2.1, "Max" : 13.}
}
NBins = {
	"Slow" : [12, 12, 12, 12],
	"Fast" : [12, 12, 12, 12]
	}
XLim = {
	"Slow" : [[.4, 6.], [.4, 2.1], [.4, 6.], [.4, 2.1]],
	"Fast" : [[6., 13.], [2.1, 11.], [6., 13.], [2.1, 11.]]
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
		for EKey in ExtKeys:
			Data = load(open(File, 'r'))
			GoodData = [(X[0], X[1]) for X in zip(Data[EKey]["Means"], Data[EKey]["NSamples"]) if Values[Food[Ind]+EKey]["Min"]<X[0]< Values[Food[Ind]+EKey]["Max"]]
			Means, NSamples = zip(*GoodData)
			Means = array(Means)
			NSamples = array(NSamples)
			Fig = figure()
			Grid =Fig.add_gridspec(2, 1, hspace = .5)
			Ax0 = Fig.add_subplot(Grid[0, 0])
			Ax1 = Fig.add_subplot(Grid[1, 0])
			Ax0.hist(Means, bins = NBins[EKey][Ind], density =  True, stacked = True)
			Ax0.set_xlabel("Mean velocity (mm/s)", fontsize = 14)
			Ax0.set_ylabel("PDF", fontsize = 14)
			Ax0.set_yscale("log")
			Ax0.set_title(Exps[Ind] + " ("+ EKey.lower()+", "+Food[Ind].lower()+") mean velocity PDF for each individual", fontsize = 18)
			#Ax0.set_xlim(XLim[EKey][Ind])
			
			Ax1.scatter([float(X)/15. for X in NSamples], Means, s=8, c = "red", edgecolors = "black")
			Ax1.set_xlabel("Track duration (s)", fontsize = 14)
			Ax1.set_ylabel("Mean velocity (mm/s)", fontsize = 14)
			Ax1.set_title(EKey+" mean velocity scatter vs track duration", fontsize = 18)

			PlotAndSave(Args.P, Args.S)