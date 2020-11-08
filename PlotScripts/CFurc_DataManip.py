from numpy import array, dot, abs, floor
from numpy.linalg import norm
from CFurc_Colors import BLUE, RED, YELLOW, GREEN, END

def GetNonNullData(Data, Key):
    NewData = [[], [], []]
    NewData[0] = [X[0] if X[0] is not None else 0. for X in Data[Key]]
    NewData[1] = [X[1] if X[1] is not None else 0. for X in Data[Key]]
    NewData[2] = [X[2] if X[2] is not None else 0. for X in Data[Key]]
    return NewData

#Code to check if vector is unitary
def CheckNorm(Data, Verbose = False, Mode = False, NormLimMin = .9, NormLimMax = 1.1, NormTolMin = .95, NormTolMax = 1.05):
    Data =  [array(X) for X in zip(*Data)]
    Norms = [norm(X) for X in Data]
    CountTol = 0
    CountLim = 0
    for X in Norms:
        if not NormLimMin < X < NormLimMax:
            CountLim += 1
        elif not NormTolMin < X < NormTolMax:
            CountTol += 1
        if Verbose and Mode:
            print(GREEN if NormTolMin < X < NormTolMax  else YELLOW if NormLimMin < X < NormLimMax else RED, "{:.4f}".format(X), END, sep = "", end = " ")
    if Mode:
        if Verbose:
            print()
        print ("Number of vectors: ", BLUE, len(Data), END, sep = "")
        print ("Number of non-unitary vectors ("+"{:.0f}".format((1-NormTolMin)*100)+"%): ", YELLOW if CountTol else GREEN, CountTol, END, sep = "")
        print ("Number of non-unitary vectors ("+"{:.0f}".format((1-NormLimMin)*100)+"%): ", RED if CountLim else GREEN, CountLim, END, sep = "")
    return array([CountTol, CountLim, len(Data)])

#Code to check if vectors are orthogonal
def CheckOrtho(DataA, DataB, Verbose, Mode, Tolerance = .05, Limit = .1):
    DataA =  [array(X) for X in zip(*DataA)]
    DataB =  [array(X) for X in zip(*DataB)]
    Dots = [dot(X[0], X[1]) for X in zip(DataA, DataB)]
    CountTol = 0
    CountLim = 0
    for X in Dots:
        if abs(X) > Limit:
            CountLim += 1
        elif abs(X) > Tolerance:
            CountTol += 1
        if Verbose and Mode:
            print(GREEN if abs(X) < Tolerance  else YELLOW if abs(X) < Limit else RED, "{:.4f}".format(abs(X)), END, sep = "", end = " ")
    if Mode:
        if Verbose:
            print()
        print ("Number of vectors: ", BLUE, len(DataA), END, sep = "")
        print ("Number of non-orthogonal vectors ("+"{:.0f}".format((Tolerance)*100)+"%): ", YELLOW if CountTol else GREEN, CountTol, END, sep = "")
        print ("Number of non-orthogonal vectors ("+"{:.0f}".format((Limit)*100)+"%): ", RED if CountLim else GREEN, CountLim, END, sep = "")
    return array([CountTol, CountLim, len(DataA), len(DataB)])

def DivideEvenUneven (Data, LimEven, LimUneven):
    Even = array([Data[I] for I in range(len(Data)) if Data[I] and not I%2 and Data[I] < LimEven])
    Uneven = array([Data[I] for I in range(len(Data)) if Data[I] and I%2 and Data[I] < LimUneven])
    return (Even, Uneven)

def GetCostFunc (Data):
    return array([1-Data[I+1]/Data[I] for I in range(len(Data)-1)])