from os.path import isdir
from glob import glob

def NoFiles(FileList):
    for Element in FileList:
        if Element:
            return False
    return True

def CheckValidPath(Path):
	return isdir(Path)

def GetFiles (Path, Keys):
	if not CheckValidPath(Path):
	    print ("Invalid input path.")
	    return [[]]
	AllFiles = [None]*len(Keys)#np.empty((len(Args.Keys), 0)).tolist()
	for I in range(len(Keys)):
	    AllFiles[I] = glob(Path+"*"+Keys[I]+"*.json")
	    AllFiles[I].sort()
	AllFiles = [X for X in AllFiles if X != []]
	return AllFiles

def CheckSizes(Data, Size = 1):
	It = iter(Data)
	return all(len(Datum) == Size for Datum in It)