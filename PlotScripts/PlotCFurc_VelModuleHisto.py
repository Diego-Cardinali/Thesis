from json import load
from matplotlib.pyplot import subplots

from CFurc_Argparse import ArgumentParser, SmartFormatter
from CFurc_Plot import PlotAndSave
from CFurc_OSManip import NoFiles, GetFiles, CheckValidPath

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

Bins = [50, 50, 50, 50, 50, 50]
Exps = ["E1 (food)", "E1 (food, running average)", "E1 (food, double step)", "E2 (no food)", "E3 (food)", "E4 (no food)"]

for Files in AllFiles:
    Ind = AllFiles.index(Files)
    for File in Files:
        Data = load(open(File, 'r'))
        Vel = [X["Velocities"][2] for X in Data["List"]]
        Fig, Ax = subplots(1)
        Ax.hist(Vel, bins = Bins[Ind], density = True, stacked = True)
        Ax.set_title(Exps[Ind] +" velocity module PDF", fontsize = 24)
        Ax.set_xlabel("Velocity (mm/s)", fontsize = 20)
        Ax.set_ylabel("PDF", fontsize = 20)
        Ax.set_yscale("log")
        PlotAndSave(Args.P, Args.S, Args.SP+Args.Keys[Ind]+"_"+str(Files.index(File)+1).zfill(4), "pdf")