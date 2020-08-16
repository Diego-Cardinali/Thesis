# C. Furcatus

The code will mostly be divided into two parts:
+ C++ for computation;
+ Python for plotting.

## Prerequisites
The C++ code requires boost (at least version 1.65.1.0) and the *jsoncons* library, that can be obtained [here](https://github.com/danielaparker/jsoncons). It is also required to compile using the c++17 standard.

## C++
The C++ part of the project is made up of a few small libraries, a main file with its units and is compiled with CMake. This section will give a quick tour of the features of the code as well as the structure for the instructions required by the program. 

### Instructions

#### Main
The main executable requires a .json file as instructions, that needs to be passed as the first or second argument when launching it in the form "-i/path/to/instructions/InstructionsForMain.json". This file has very precise formatting in the form:
```json
{
	"TaskList" : [0, 1, 2, 3, 4, 5],
	"InstructionsPath" : "/path/to/other/instructions/files/",
	"InstructionsFiles" : ["Array", "of", "names", "of", ".json", "instructions", "files"],
	"Verbose" : true
}
```

The parameters are:
* **TaskList**: An array of integers that represent the order of the tasks that need to be executed. For further explanation on what each function does please refer to the *Units* section. The same task can be repeated an arbitrary number of times by writing the same number more than once, provided the instructions are given correctly. At the moment the available functions are: <br/>
  &nbsp; &nbsp; 0\. DataToJson;<br/>
  &nbsp; &nbsp; 1\. CartesianToSpherical;<br/>
  &nbsp; &nbsp; 2\. VariationsOnDeltaT;<br/>
  &nbsp; &nbsp; 3\. Variations;<br/>
  &nbsp; &nbsp; 4\. Histograms;<br/>
  &nbsp; &nbsp; 5\. JoinFiles.<br/>
* **InstructionsPath**: A string consisting of a path to the folder containing instructions for the various tasks. All intructions for the tasks must be in the same folder. *NOTE: Paths should end with a slash ("/"), otherwise the program might not function properly.*
* **InstructionsFiles**: An array of strings, each one must be the name of a .json file containing specific instructions for the corresponding task requested in *TaskList*. *NOTE: If the number of elements of this array and TaskList differs the execution of the program will stop. Please double check that the instructions match the order of the tasks.*
* **Verbose**: A boolean that will make the program print additional information during execution if set to true.

#### Tasks
The instructions for the tasks are all formatted the same, with each task having the possibility of requiring its specific parameters. There are six parameters which are needed for all files:
* **InputPath**: A string containing the path to the directory where the files for the corresponding task are located.
* **InputNames**: An array of strings, each one containing a fragment of name that will be searched in the folder. The program will attempt to input all files that contain that fragment. If a file contains more than one fragment it will be matched more than once. As an example `"InputNames" : ["Test", "Dog"]` will read the file named "Test_Dogbone.json" twice and perform work on it both times. If an element of *InputNames* matches more than one file they will all be read and then progressively produce output according to the *OutputNames* keyword.
* **OutputPath**: A string containing the path to the directory where the output files will be produced.
* **OutputNames**: An array of strings that will progressively be matched with the elements of *InputNames* to name the output files accordingly. *NOTE: if the number of elements in this array and InputNames differs the execution of the program will stop.*
* **TypeOfDataIn**: A string containing the exact type of data that the program will search for in the .json input files.
* **TypeOfDataOut**: A string containing the exact type of data that will be written in the .json output files.

In addition to these some tasks require additional parameters:
* **DoubleParameter**: A floating-point number that will be used by the task. Referring to the numbering in the previous subsection, the tasks needing this parameter are: *VariationsOnDeltaT*.
* **ArrayOfIntParameters**: An array of integers that will be associated with each of the groups of files in the *InputNames* array. *NOTE: If the number of elements in this array and InputNames differs the execution of the program will stop.*

### Main
The Main file is mostly a wrapper for the various functions that are described in the Task section. The default name for the executable file is *CFurcatus*. Once launched, it will either read the instructions (if provided) or ask for direct console input. This second way is not recommended as the syntax is still that of a .json file.

### Tasks
The tasks are functions to process data. Here follows a list:
0. **DataToJson**: This functions reads data from .txt files formatted specifically in four columns and outputs .json files with *TypeOfDataOut* set as *Coordinates*. The first column is ignored and the other three are interpreted as *x*, *y* and *z* cartesian coordinates for the position of a point in 3D space. The parameters *TypeOfDataIn* and *TypeOfDataOut* can be left blank. *Extra instructions required: None.*
1. **CartesianToSpherical**: This function converts cartesian coordinates in the form *(x,y,z)* to spherical 3D coordinates in the form *(a, i, r)*, where *a* is the inclination angle (in radians), *i* is the inclination angle (in radians) and *r* is the radius (in the same unit as the cartesian coordinates). *Extra instructions required: None.* *NOTE: Boost is required.*
2. **VariationsOnDeltaT**: This function computes `(Data[I+1]-Data[I])/DeltaT` for each I and each set of data, i.e. it computes the average velocity along the corresponding axis. *Extra instructions required: DoubleParameter, used as DeltaT.*
3. **Variations**: This function works exactly as **2** but without the time variation. *Extra instructions required: None.*
4. **Histograms**: Coming soon.
5. **JoinFiles**: This function doesn't work on a single file at a time, instead it reads all the files that match the corresponding element of *InputNames* and then produces a single file with all the data concatenated. The data must be all of the same *TypeOfDataIn*. *Extra instructions required: None.*

### Libraries
* **Utility**: Some functions needed in various codes for computation.
* **ExploreDirectory**: Some functions to interact with files in directories; *NOTE: Boost is required.*
* **FileManip**: Some functions to manipulate files.
* **Pipeline**: Functions and variables to handle input and output.

## Python
The python part of the project handles mostly plotting relevant data. Each file requires its parameters, that 
### PlotAngularVariations
This script reads the given data, assumed in spherical coordinates (see *CartesianToSphetical* function in the *C++* section) and plots the first two coordinates of the data, that should be variations of azimuth and inclination angles.
