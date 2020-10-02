import json
import matplotlib.pyplot as plt
import math
import numpy as np

import argparse
from argparse import ArgumentParser
class SmartFormatter(argparse.HelpFormatter):
    def _split_lines(self, text, width):
        if text.startswith('R|'):
            return text[2:].splitlines()  
        return argparse.HelpFormatter._split_lines(self, text, width)

Parser = ArgumentParser(description = "", formatter_class=SmartFormatter)
Parser.add_argument("-P", "--P", "--p", action = "store_true", help = "If given the data is plotted.")
Args = Parser.parse_args()

import scipy.stats as ss
from scipy.optimize import curve_fit

END = "\033[0m"
BOLD = "\033[1m"
RED = "\033[91m"
GREEN = "\033[92m"
YELLOW = "\033[93m"
BLUE = "\033[94m"

PLim = 0.95

def Plot(P):
	if (P):	
		figManager = plt.get_current_fig_manager()
		figManager.window.showMaximized()
		plt.show()
	plt.close()
	return

Files = [
	"/home/diego/Desktop/Code/Thesis/ThesisData/E2E4Stop/E2_JoinedDisplacement_4f.json",
	"/home/diego/Desktop/Code/Thesis/ThesisData/E2E4Stop/E4_JoinedDisplacement_4f.json",
	]


NBinsFast = [22, 22]
NBinsSlow = [20, 20]
LimitFast = [3, 3]
LimitSlow = [5, 6] #5 vs 6
DataSetsFast = []
DataSetsSlow = []
NFast = [40., 40.]
NSlow = [15.5, 15.]
ParamsInitFastPow = [
	[1, -1],
	[1, -1]
	]
ParamsInitFastExp = [
	[1, -1],
	[1, -1]
	]
ParamsInitSlowPow = [
	[1, -1],
	[1, -1]
	]
ParamsInitSlowExp = [
	[1, -1],
	[1, -1]
	]

def PowLaw (X, Mod, Exp):
	return Mod*(X**Exp)
def ExpLaw (X, Mod, Exp):
	return Mod*np.exp(X*Exp)

for File in Files:
	Data = json.load(open(File, 'r'))
	ListSlow = []
	ListFast = []

	for Datum in Data["Slow"]:
		if Datum["Displacement"] < NSlow[Files.index(File)]:
			ListSlow.append(Datum["Displacement"])
	for Datum in Data["Fast"]:
		if Datum["Displacement"] < NFast[Files.index(File)]:
			ListFast.append(Datum["Displacement"])

	Fig, Ax = plt.subplots(1, figsize = (32, 18))
	FastHist, FastBins, Patches = Ax.hist(ListFast, bins = NBinsFast[Files.index(File)], density =  True, stacked = True, label = "Distance from origin PDF")
	Ax.set_title(["E2", "E4"][Files.index(File)] +" distance from starting point probabilty\n density (fast, 4 frames minimum gap)", fontsize = 24)
	Ax.set_xlabel("Distance (mm)", fontsize = 20)
	Ax.set_ylabel("Probability", fontsize = 20)

	DataSetsFast.append(FastHist)
	FastHist = FastHist[LimitFast[Files.index(File)]:]
	FastBins = FastBins[LimitFast[Files.index(File)]:]
	FastBinsCenter = np.mean(np.vstack([FastBins[0:-1],FastBins[1:]]), axis=0)

	OutFastPow, CovFastPow = curve_fit(PowLaw, FastBinsCenter, FastHist, p0=ParamsInitFastPow[Files.index(File)], sigma = np.sqrt(FastHist))
	OutFastExp, CovFastExp = curve_fit(ExpLaw, FastBinsCenter, FastHist, p0=ParamsInitFastExp[Files.index(File)], sigma = np.sqrt(FastHist))


	print (BLUE, '\b'+BOLD, "\bFAST", '\b'+END, "A*X^B:",'\b'+END)
	print ("A = ", "{:.4f}".format(OutFastPow[0]), u"\u00B1", "{:.4f}".format(np.sqrt(CovFastPow[0][0])), sep='', end="; ")
	print ("B = ", "{:.4f}".format(OutFastPow[1]), u"\u00B1", "{:.4f}".format(np.sqrt(CovFastPow[1][1])), sep='')
	print (BLUE, '\b'+BOLD, "\bFAST", '\b'+END, "A*exp(X*B):",'\b'+END)
	print ("A = ", "{:.4f}".format(OutFastExp[0]), u"\u00B1", "{:.4f}".format(np.sqrt(CovFastExp[0][0])), sep='', end="; ")
	print ("B = ", "{:.4f}".format(OutFastExp[1]), u"\u00B1", "{:.4f}".format(np.sqrt(CovFastExp[1][1])), sep='')

	XXFast = np.linspace(FastBinsCenter[0], FastBins[len(FastBins)-1], 50)
	YYFastPow = PowLaw(XXFast, OutFastPow[0], OutFastPow[1])
	YYFastExp = ExpLaw(XXFast, OutFastExp[0], OutFastExp[1])
	FastPowLabel =       "Power law fit: ("+"{:.3f}".format(OutFastPow[0])+u"\u00B1"+"{:.3f}".format(np.sqrt(CovFastPow[0][0]))+")X^("+"{:.3f}".format(OutFastPow[1])+u"\u00B1"+"{:.3f}".format(np.sqrt(CovFastPow[1][1]))+")"
	FastExpLabel = "Exponential law fit: ("+"{:.3f}".format(OutFastExp[0])+u"\u00B1"+"{:.3f}".format(np.sqrt(CovFastExp[0][0]))+")X^("+"{:.3f}".format(OutFastExp[1])+u"\u00B1"+"{:.3f}".format(np.sqrt(CovFastExp[1][1]))+")"
	Ax.plot(XXFast, YYFastPow , color = "black", label = FastPowLabel)
	#Ax.plot(XXFast, YYFastExp , color = "red", label = FastExpLabel)
	#Ax.set_xscale("log")
	Ax.set_yscale("log")
	Ax.set_xlim(right = 41.)
	Ax.set_xticks([2*I for I in range (21)], minor = False)
	Ax.set_xticks([I for I in range (41)], minor = True)
	Ax.legend(loc='upper right', frameon=False)

	Stat, Prob = ss.ks_2samp(FastHist, YYFastPow)
	print(BLUE, '\b'+BOLD, "\bFAST", '\b'+END, "P-Value for Kolmogorov-Smirnoff test on polinomial fit goodness:", RED+'\b' if Prob<PLim else GREEN+'\b', "{:.4f}".format(Prob), END)
	Stat, Prob = ss.ks_2samp(FastHist, YYFastExp)
	print(BLUE, '\b'+BOLD, "\bFAST", '\b'+END, "P-Value for Kolmogorov-Smirnoff test on exponential fit goodness:", RED+'\b' if Prob<PLim else GREEN+'\b', "{:.4f}".format(Prob), END)
	Plot(Args.P)

	print ()
	Fig, Ax = plt.subplots(1, figsize = (32, 18))
	SlowHist, SlowBins, Patches = Ax.hist(ListSlow, bins = NBinsSlow[Files.index(File)], density =  True, stacked = True, label = "Distance from origin PDF")
	Ax.set_title(["E2", "E4"][Files.index(File)] +" distance from starting point probabilty\n density (slow, 4 frames minimum gap)", fontsize = 24)
	Ax.set_xlabel("Distance (mm)", fontsize = 20)
	Ax.set_ylabel("Probability", fontsize = 20)

	DataSetsSlow.append(SlowHist)
	SlowHist = SlowHist[LimitSlow[Files.index(File)]:]
	LogSlowHist = np.log10(SlowHist)
	SlowBins = SlowBins[LimitSlow[Files.index(File)]:]
	LogSlowBins = np.log10(SlowBins)
	SlowBinsCenter = np.mean(np.vstack([SlowBins[0:-1],SlowBins[1:]]), axis=0)
	LogSlowBinsCenter = np.log10(SlowBinsCenter)

	OutSlowPow, CovSlowPow = curve_fit(PowLaw, SlowBinsCenter, SlowHist, p0=ParamsInitSlowPow[Files.index(File)], sigma = np.sqrt(SlowHist))
	OutSlowExp, CovSlowExp = curve_fit(ExpLaw, SlowBinsCenter, SlowHist, p0=ParamsInitSlowExp[Files.index(File)], sigma = np.sqrt(SlowHist))
	print (YELLOW, '\b'+BOLD, "\bSLOW", '\b'+END, "A*X^B:",'\b'+END)
	print ("A = ", "{:.4f}".format(OutSlowPow[0]), u"\u00B1", "{:.4f}".format(np.sqrt(CovSlowPow[0][0])), sep='', end="; ")
	print ("B = ", "{:.4f}".format(OutSlowPow[1]), u"\u00B1", "{:.4f}".format(np.sqrt(CovSlowPow[1][1])), sep='')
	print (YELLOW, '\b'+BOLD, "\bSLOW", '\b'+END, "A*exp(X*B):",'\b'+END)
	print ("A = ", "{:.4f}".format(OutSlowExp[0]), u"\u00B1", "{:.4f}".format(np.sqrt(CovSlowExp[0][0])), sep='', end="; ")
	print ("B = ", "{:.4f}".format(OutSlowExp[1]), u"\u00B1", "{:.4f}".format(np.sqrt(CovSlowExp[1][1])), sep='')

	XXSlow = np.linspace(SlowBinsCenter[0], SlowBins[len(SlowBins)-1], 50)
	YYSlowPow = PowLaw (XXSlow, OutSlowPow[0], OutSlowPow[1])
	YYSlowExp = ExpLaw (XXSlow, OutSlowExp[0], OutSlowExp[1])
	SlowPowLabel =       "Power law fit: ("+"{:.3f}".format(OutSlowPow[0])+u"\u00B1"+"{:.3f}".format(np.sqrt(CovSlowPow[0][0]))+")X^("+"{:.3f}".format(OutSlowPow[1])+u"\u00B1"+"{:.3f}".format(np.sqrt(CovSlowPow[1][1]))+")"
	SlowExpLabel = "Exponential law fit: ("+"{:.3f}".format(OutSlowExp[0])+u"\u00B1"+"{:.3f}".format(np.sqrt(CovSlowExp[0][0]))+")exp(X*("+"{:.3f}".format(OutSlowExp[1])+u"\u00B1"+"{:.3f}".format(np.sqrt(CovSlowExp[1][1]))+"))"
	Ax.plot(XXSlow, YYSlowPow , color = "black", label = SlowPowLabel)
	Ax.plot(XXSlow, YYSlowExp , color = "red", label = SlowExpLabel)
	#Ax.set_xscale("log")
	Ax.set_yscale("log")
	Ax.set_xlim(right = 16.)
	Ax.set_xticks([I for I in range (17)], minor = False)
	Ax.set_xticks([I/2 for I in range (32)], minor = True)
	Ax.legend(loc='upper right', frameon=False)

	Stat, Prob = ss.ks_2samp(SlowHist, YYSlowPow)
	print(YELLOW, '\b'+BOLD, "\bSLOW", '\b'+END, "P-Value for Kolmogorov-Smirnoff test on polinomial fit goodness:", RED+'\b' if Prob<PLim else GREEN+'\b', "{:.4f}".format(Prob), END)
	Stat, Prob = ss.ks_2samp(SlowHist, YYSlowExp)
	print(YELLOW, '\b'+BOLD, "\bSLOW", '\b'+END, "P-Value for Kolmogorov-Smirnoff test on exponential fit goodness:", RED+'\b' if Prob<PLim else GREEN+'\b', "{:.4f}".format(Prob), END)
	Plot(Args.P)
	print ()


Stat, Prob = ss.ks_2samp(DataSetsFast[0], DataSetsFast[1])
print(BLUE, '\b'+BOLD, "\bFAST", '\b'+END, "P-Value for Kolmogorov-Smirnoff test on the two fast data samples:", RED+'\b' if Prob<PLim else GREEN+'\b', "{:.4f}".format(Prob), END)
Stat, Prob = ss.ks_2samp(DataSetsFast[0][LimitFast[0]:], DataSetsFast[1][LimitFast[1]:])
print(BLUE, '\b'+BOLD, "\bFAST", '\b'+END, "P-Value for Kolmogorov-Smirnoff test on the two fast fit data samples:", RED+'\b' if Prob<PLim else GREEN+'\b', "{:.4f}".format(Prob), END)
Stat, Prob = ss.ks_2samp(DataSetsSlow[0], DataSetsSlow[1])
print(YELLOW, '\b'+BOLD, "\bSLOW", '\b'+END, "P-Value for Kolmogorov-Smirnoff test on the two slow data samples:", RED+'\b' if Prob<PLim else GREEN+'\b', "{:.4f}".format(Prob), END)
Stat, Prob = ss.ks_2samp(DataSetsSlow[0][LimitSlow[0]:], DataSetsSlow[1][LimitSlow[1]:])
print(YELLOW, '\b'+BOLD, "\bSLOW", '\b'+END, "P-Value for Kolmogorov-Smirnoff test on the two slow fit data samples:", RED+'\b' if Prob<PLim else GREEN+'\b', "{:.4f}".format(Prob), END)