NumberOfEmptySpaces = 4
FigWidth = 32
FigHeight = 20

from CleanData import CleanData

def AAA():
	print ("Oh")

def PlotAngularVariations(AllFiles) :
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
	return