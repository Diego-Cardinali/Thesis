import argparse
import glob
import json
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import os
from pathlib import Path

NumberOfEmptySpaces = 4
FigWidth = 32
FigHeight = 20

def StrToBool(B):
    if isinstance(B, bool):
       return B
    if B.lower() in ("yes", "true", "t", "y", "1"):
        return True
    elif B.lower() in ("no", "false", "f", "n", "0"):
        return False
    else:
        raise argparse.ArgumentTypeError("Boolean value expected.")

def CleanData(DataList):
	Indexes = []
	for I in range (0, len(DataList[1])-NumberOfEmptySpaces):
		if all([Datum != 0 for Datum in DataList[1][I:I+NumberOfEmptySpaces]]):
			Indexes.append(I)
			break
	for I in range (len(DataList[1]), NumberOfEmptySpaces, -1):
		if all([Datum != 0 for Datum in DataList[1][I-NumberOfEmptySpaces:I]]):
			Indexes.append(I)
			break
	DataList[0] = DataList[0][Indexes[0]:Indexes[1]]
	DataList[1] = DataList[1][Indexes[0]:Indexes[1]]
	return DataList

def NoFiles(FileList):
	for Element in FileList:
		if Element:
			return False
	return True

Parser = argparse.ArgumentParser(description = "Plot the files in the given Directory that match the given Key.")
Parser.add_argument("Directory", type = str, metavar = "-D", action = "store", nargs = 1, help = "Path to the directory containing the files to plot.")
Parser.add_argument("Keys", metavar = "-K", action = "append", nargs = "+", help = "A list of keys. All files in the folder containing a Key in the name will be plotted. If a file contains two or more keys it will be plotted more than once.")
Parser.add_argument("--Clean", "--C", "--c", type = StrToBool, metavar = "--C", const = True, default = False, nargs = "?", help = "If true the X Axis will be restricted to relevant data, if false all data will be displayed.")
Parser.add_argument("--SavePath", "--S", "--s", type = str, metavar = "--S", nargs = 1, help = "Path to the directory where the graphs will be saved. If no path is provided (or if the path is invalid) all graphs will be saved in the directory containing the files to plot.")
Args = Parser.parse_args()

if not os.path.isdir(Args.Directory[0]):
	exit("Invalid input path.")

AllFiles = np.empty((len(Args.Keys[0]), 0)).tolist()
for I in range(len(Args.Keys[0])):
	AllFiles[I] = glob.glob(Args.Directory[0]+"*"+Args.Keys[0][I]+"*.json")
	AllFiles[I].sort()

if NoFiles(AllFiles):
	exit ("No valid files given.")

for Files in AllFiles:	
	for I in range(len(Files)):
		Data = json.load(open(Files[I], 'r'))
		XYData = []
		Width = []
		#Z in radius in spherical coordinates, the order is (Azimuth, Inclination, Radius)
		for Var in ["X", "Y"]:
			XYData.append([np.linspace(Data[Var]["Min"], Data[Var]["Max"], Data["NBins"]).tolist(), Data[Var]["FrequencyHistogram"]])
			Width.append(abs(Data[Var]["Max"]-Data[Var]["Min"])/Data["NBins"])
			if Args.Clean:
				for J in range (len(XYData)):
					XYData[J] = CleanData(XYData[J])
		Fig, Ax = plt.subplots(2, figsize = (FigWidth, FigHeight))
		for J in range (len(XYData[0])):
			AxesSettings = plt.gca()
			AxesSettings.set_xlim([XYData[J][0][0],XYData[J][0][len(XYData[J][0])-1]])
			Ax[J].bar(XYData[J][0], XYData[J][1], width = Width[J]*1.01)
			Ax[J].set_title(["Azimuth Angle", "Inclination Angle"][J]+" variation distribution", fontsize = 24)
			Ax[J].xaxis.set_major_locator(mpl.ticker.MultipleLocator(10*Width[J]))
			Ax[J].set_xlabel("Variation (rad)", fontsize = 20)
			Ax[J].set_ylabel("Occurrences", fontsize = 20)
		#figManager = plt.get_current_fig_manager()
		#figManager.window.showMaximized()
		#
		if Args.SavePath is None or not os.path.isdir(Args.SavePath[0]):
			Name = Args.Directory[0]+Path(Files[I]).stem+".png"
		else:
			Name = Args.SavePath[0]+Path(Files[I]).stem+".png"
		#Fig.subplots_adjust(hspace=.4)
		#plt.show()
		Fig.savefig(Name, bbox_inches = "tight")