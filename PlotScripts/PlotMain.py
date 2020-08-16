import argparse
import glob
import json
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import os
from pathlib import Path

from AngularVariarions import PlotAngularVariations, AAA
from VelModule import PlotVelModule

def StrToBool(B):
    if isinstance(B, bool):
       return B
    if B.lower() in ("yes", "true", "t", "y", "1"):
        return True
    elif B.lower() in ("no", "false", "f", "n", "0"):
        return False
    else:
        raise argparse.ArgumentTypeError("Boolean value expected.")

def NoFiles(FileList):
	for Element in FileList:
		if Element:
			return False
	return True


Parser = argparse.ArgumentParser(description = "Plot the files in the given Directory that match the given Key.")
Parser.add_argument("Mode", type = int, choices=range(0, 2), metavar = "-M", action = "store", help = "Switch between various plot functions: (0) PlotAngularVariations; (1) PlotVelModule.")
Parser.add_argument("Directory", type = str, metavar = "-D", action = "store", help = "Path to the directory containing the files to plot.")
Parser.add_argument("Keys", metavar = "-K", nargs = "+", help = "A list of keys. All files in the folder containing a Key in the name will be plotted. If a file contains two or more keys it will be plotted more than once.")
Parser.add_argument("--Clean", "--C", "--c", type = StrToBool, metavar = "--C", const = True, default = False, nargs = "?", help = "If true the X Axis will be restricted to relevant data, if false all data will be displayed.")
Parser.add_argument("--SavePath", "--S", "--s", type = str, metavar = "--S", nargs = 1, help = "Path to the directory where the graphs will be saved. If no path is provided (or if the path is invalid) all graphs will be saved in the directory containing the files to plot.")
Args = Parser.parse_args()

if not os.path.isdir(Args.Directory):
	exit("Invalid input path.")

AllFiles = np.empty((len(Args.Keys), 0)).tolist()
for I in range(len(Args.Keys)):
	AllFiles[I] = glob.glob(Args.Directory+"*"+Args.Keys[I]+"*.json")
	AllFiles[I].sort()
AllFiles = [X for X in AllFiles if X != []]

if NoFiles(AllFiles):
	exit ("No valid files given.")

Modes = {
	0 : PlotAngularVariations,
	1 : PlotVelModule
}

Modes[Args.Mode]