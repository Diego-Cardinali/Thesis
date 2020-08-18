def CleanData(DataList):
    Indexes = []
    NumberOfEmptySpaces = int(round(len(DataList[1])/100*10))
    Extra = int(round(len(DataList[1])/100*2))
    for I in range (0, len(DataList[1])-NumberOfEmptySpaces):
        if all([Datum != 0 for Datum in DataList[1][I:I+NumberOfEmptySpaces]]):
            Indexes.append(I)
            break
    if not len(Indexes):
        Indexes.append(0)
    for I in range (len(DataList[1])-1, NumberOfEmptySpaces, -1):
        if all([Datum != 0 for Datum in DataList[1][I-NumberOfEmptySpaces:I]]):
            Indexes.append(I)
            break
    if len(Indexes) == 1:
        Indexes.append(len(DataList[1]))
    DataList[0] = DataList[0][(Indexes[0]-Extra if Indexes[0]-Extra >= 0 else Indexes[0]) : (Indexes[1]+Extra if Indexes[1]+Extra <= len(DataList) else Indexes[1])]
    DataList[1] = DataList[1][(Indexes[0]-Extra if Indexes[0]-Extra >= 0 else Indexes[0]) : (Indexes[1]+Extra if Indexes[1]+Extra <= len(DataList) else Indexes[1])]
    return DataList