# Thesis

#### List of codes

##### Bash
+ **Compile.sh**: A compiler for the .cpp files

##### C++
All the main files require instructions to work. Tha instructions consist of a .json file with clearly named elements. There are all the elements that can appear in instructions and what they mean:
1. **InputPath**: String. Path to the folder where the files to read are, ***NOTE: Please finish the path with a /***;
2. **InputNames**: Array. A list of strings that contain an element of the name of the files to read, for example if the array is *\["Test", "Dog"\]*, all files containing the word *Test* or *dog* in the folder will be read;
3. **Output**: String. Path to the folder where the files need to be written, ***NOTE: Please finish the path with a /***;
4. **OutputNames**: Array. A list of strings that will be progressively used to name the output files, with a one-to-one association to *InputNames*. ***NOTE: If the number of elements in this array differs from the number of elements in *InputNames* the program will not proceed.***
5. **TypeOfData**: String. The name of the type of input data the code is working on, e.g. *"Coordinates"* or *"Velocities"*, it will be used to read the .json data.

###### Main files (Require instructions)
+ **DataToJson.cpp**: Takes specifically formatted data and converts it to .json.
*Instructions required*: 1, 2, 3, 4.
+ **CartesianToSpherical**: Converts data of the specified type from cartesian to spherical coordinates, ***NOTE: requires boost***.
*Instructions required*: 1, 2, 3, 4, 5.
###### Support files
+ **ThesisCode/Utility.cpp(.h)**: Some functions needed in various codes for computation.
+ **ThesisCode/ExploreDirectory.cpp(.h)**: Some functions to interact with files in directories; ***NOTE: requires boost***.
+ **ThesisCode/FileManip.cpp(.h)**: Some functions to manipulate files.
+ **ThesisCode/\[IO\]Pipeline.cpp(.h)**: Funtions and variables to handle input and output.

