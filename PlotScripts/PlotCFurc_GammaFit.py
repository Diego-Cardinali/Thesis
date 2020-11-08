from json import load
from matplotlib.pyplot import subplots
from numpy import array, mean, vstack, sqrt, linspace
from scipy.stats import ks_2samp
from scipy.optimize import curve_fit

from CFurc_Argparse import ArgumentParser, SmartFormatter
from CFurc_Plot import PlotAndSave
#from CFurc_FitFunc import 
#from CFurc_Colors import 

Parser = ArgumentParser(description = "", formatter_class=SmartFormatter)
Parser.add_argument("-P", "--P", "--p", action = "store_true", help = "If given the data is plotted.")
Args = Parser.parse_args()



PLim = 0.95

Files = [
	"/home/diego/Desktop/Code/Thesis/ThesisData/JoinedData/E1_JoinedSVel.json",
	"/home/diego/Desktop/Code/Thesis/ThesisData/JoinedData/E3_JoinedSVel.json",
	]
Lim = [8., 8.]#[7.5, 8.]
ParamsInit = [
 	[1, 15, -2.3],
 	[1, 15, -2.3]
	]
ParamsInitCent = [
	[-1.02849840e+000, 6.16655173e-162,  2.69778635e+002,  2.52004871e+001],#[6, 1, 3, -2.3],
 	[-9.33988072e+000, 2.76953432e-194,  2.22794420e+002,  1.11806290e+001]#[6, 1, 3, -2.3]
	]

LimAsc = [6., 6.]
ParamsInitAsc = [
 	[1, 15, -2.3],
 	[1, 15, -2.3]
	]

LimDesc = [10.3, 11.05]
ParamsInitDesc = [
 	[1, 15, -2.3],
 	[1, 15, -2.3]
	]

Bins = [50, 50]
Exps = ["E1", "E3"]

for File in Files:
	Data = load(open(File, 'r'))
	Vel = [X["Velocities"][2] for X in Data["List"]]
	Fig, Ax = subplots(1, figsize = (32, 18))
	VelHist, VelBins, Patches = Ax.hist(Vel, bins = Bins[Files.index(File)], density = True, stacked = True)
	Ax.set_title(Exps[Files.index(File)] +" velocity module distribution probabilty density", fontsize = 24)
	Ax.set_xlabel("Velocity (mm/s)", fontsize = 20)
	Ax.set_ylabel("PDF", fontsize = 20)
	#Ax.set_xscale("log")
	#Ax.set_yscale("log")

	Cut = next(LL[0] for LL in enumerate(VelBins) if LL[1] > Lim[Files.index(File)])-1
	VelBinsGamma = VelBins[Cut:]
	VelHistGamma = VelHist[Cut:]
	VelBinsCenterGamma = mean(vstack([VelBinsGamma[0:-1],VelBinsGamma[1:]]), axis=0)

	#OutGamma, CovGamma = curve_fit(Gamma, VelBinsCenterGamma, VelHistGamma, p0=ParamsInit[Files.index(File)], sigma=sqrt(VelHistGamma), maxfev=100000)
	#print (BLUE, '\b'+BOLD, "\bFAST", '\b'+END, "A*X^B*exp(X*C):",'\b'+END)
	#print ("A = ", str(OutGamma[0]), u"\u00B1", str(sqrt(CovGamma[0][0])), sep='', end="; ")
	#print ("B = ", "{:.4f}".format(OutGamma[1]), u"\u00B1", "{:.4f}".format(sqrt(CovGamma[1][1])), sep='', end="; ")
	#print ("C = ", "{:.4f}".format(OutGamma[2]), u"\u00B1", "{:.4f}".format(sqrt(CovGamma[2][2])), sep='')
	OutCenGa, CovCenGa = curve_fit(CenteredGamma, VelBinsCenterGamma, VelHistGamma, p0=ParamsInitCent[Files.index(File)], sigma=sqrt(VelHistGamma), maxfev=100000000, method = "dogbox")
	print (OutCenGa)
	#GammaLabel = "Gamma law fit: ("+str(OutGamma[0])+u"\u00B1"+str(sqrt(CovGamma[0][0]))+")X^("+"{:.3f}".format(OutGamma[1])+u"\u00B1"+"{:.3f}".format(sqrt(CovGamma[1][1]))+")*exp(X*"+"{:.3f}".format(OutGamma[2])+u"\u00B1"+"{:.3f}".format(sqrt(CovGamma[2][2]))+")"
	#GammaLabel = "Gamma"
	VelXXGamma = linspace(VelBinsGamma[0], VelBinsGamma[len(VelBinsGamma)-1], 500)
	#VelYYGamma = Gamma(VelXXGamma, OutGamma[0], OutGamma[1], OutGamma[2])#, OutGamma[3], OutGamma[4])
	VelYYCenGa = CenteredGamma(VelXXGamma, OutCenGa[0], OutCenGa[1], OutCenGa[2], OutCenGa[3])#, OutGamma[3], OutGamma[4])
	
	#VelHistAsc  = [X for X in VelHist if (X > LimAsc[Files.index(File)][0]  and X < LimAsc[Files.index(File)][1])]
	#VelHistDesc = [X for X in VelHist if X > LimDesc[Files.index(File)]]
	#CutAsc  = next(LL[0] for LL in enumerate(VelBins) if LL[1] > LimAsc[Files.index(File)])-1
	#CutDesc = next(LL[0] for LL in enumerate(VelBins) if LL[1] > LimDesc[Files.index(File)])-1
	#VelHistAsc  = VelHist[CutAsc:CutDesc]
	#VelHistDesc = VelHist[CutDesc:]
	#VelBinsAsc  = VelBins[CutAsc:CutDesc+1]
	#VelBinsDesc = VelBins[CutDesc:]
	#print (VelHist[CutAsc:])
	#print (VelHistAsc)
	#print (VelHistDesc)
	#print (len(VelBinsAsc), len(VelBinsCenterAsc), len(VelHistAsc))
	#print (len(VelBinsDesc), len(VelBinsCenterDesc), len(VelHistDesc))
	#VelBinsCenterAsc  = mean(vstack([VelBinsAsc [0:-1],VelBinsAsc [1:]]), axis=0)
	#VelBinsCenterDesc = mean(vstack([VelBinsDesc[0:-1],VelBinsDesc[1:]]), axis=0)
	#OutAsc, CovAsc =   curve_fit(Gamma, VelBinsCenterAsc,  VelHistAsc,  p0=ParamsInitAsc [Files.index(File)], sigma=sqrt(VelHistAsc),  maxfev=100000)
	#OutDesc, CovDesc = curve_fit(Gamma, VelBinsCenterDesc, VelHistDesc, p0=ParamsInitDesc[Files.index(File)], sigma=sqrt(VelHistDesc), maxfev=100000)
	#print (BLUE, '\b'+BOLD, "\bRISE", '\b'+END, "A*X^B*exp(X*C):",'\b'+END)
	#print ("A = ", str(OutAsc[0]), u"\u00B1", str(sqrt(CovAsc[0][0])), sep='', end="; ")
	#print ("B = ", "{:.4f}".format(OutAsc[1]), u"\u00B1", "{:.4f}".format(sqrt(CovAsc[1][1])), sep='', end="; ")
	#print ("C = ", "{:.4f}".format(OutAsc[2]), u"\u00B1", "{:.4f}".format(sqrt(CovAsc[2][2])), sep='')
	#print (BLUE, '\b'+BOLD, "\bFALL", '\b'+END, "A*X^B*exp(X*C):",'\b'+END)
	#print ("A = ", str(OutDesc[0]), u"\u00B1", str(sqrt(CovDesc[0][0])), sep='', end="; ")
	#print ("B = ", "{:.4f}".format(OutDesc[1]), u"\u00B1", "{:.4f}".format(sqrt(CovDesc[1][1])), sep='', end="; ")
	#print ("C = ", "{:.4f}".format(OutDesc[2]), u"\u00B1", "{:.4f}".format(sqrt(CovDesc[2][2])), sep='')
	#AscLabel  = "Rise"
	#DescLabel = "Fall"
	#VelXXAsc  = linspace(VelBinsAsc[0], VelBinsAsc[len(VelBinsAsc)-1], 500)
	#VelYYAsc  = Gamma(VelXXAsc, OutAsc[0], OutAsc[1], OutAsc[2])#, OutGamma[3], OutGamma[4])
	#VelXXDesc = linspace(VelBinsDesc[0], VelBinsDesc[len(VelBinsDesc)-1], 500)
	#VelYYDesc = Gamma(VelXXDesc, OutDesc[0], OutDesc[1], OutDesc[2])#, OutGamma[3], OutGamma[4])

	print()
	#Stat, Prob = ks_2samp(VelHistGamma, VelYYGamma)
	#print(BLUE, '\b'+BOLD, "\bFAST", '\b'+END, "P-Value for Kolmogorov-Smirnoff test on gamma fit goodness:", RED+'\b' if Prob<PLim else GREEN+'\b', "{:.4f}".format(Prob), END)
	#Stat, Prob = ks_2samp(VelHistAsc, VelYYAsc)
	#print(BLUE, '\b'+BOLD, "\bRISE", '\b'+END, "P-Value for Kolmogorov-Smirnoff test on rising gamma fit goodness:", RED+'\b' if Prob<PLim else GREEN+'\b', "{:.4f}".format(Prob), END)
	#Stat, Prob = ks_2samp(VelHistDesc, VelYYDesc)
	#print(BLUE, '\b'+BOLD, "\bFALL", '\b'+END, "P-Value for Kolmogorov-Smirnoff test on falling gamma fit goodness:", RED+'\b' if Prob<PLim else GREEN+'\b', "{:.4f}".format(Prob), END)
	print()
	#Ax.plot(VelXXGamma, VelYYGamma, color = "black", label = GammaLabel)
	#Ax.plot(VelXXAsc, VelYYAsc, color = "red", label = AscLabel)
	#Ax.plot(VelXXDesc, VelYYDesc, color = "orange", label = DescLabel)
	Ax.plot(VelXXGamma, VelYYCenGa, color = "green")
	#Ax.legend(loc='upper right', frameon=False)
	Plot(Args.P)

