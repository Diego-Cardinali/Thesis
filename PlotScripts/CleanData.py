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