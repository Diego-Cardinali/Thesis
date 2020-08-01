#!/bin/bash
if [ ! -d ./Executables ]
then
	mkdir Executables
	echo "Folder Executables created"
else
	echo "Folder Executables already found, compilation will overwrite previous executable files."
fi

CompilePath="ThesisCode/"
ExecutablesPath="Executables/"
#Addons="${CompilePath}FileManip.h ${CompilePath}FileManip.cpp ${CompilePath}Utility.h ${CompilePath}Utility.cpp ${CompilePath}ExploreDirectory.h ${CompilePath}ExploreDirectory.cpp"
Addons="${CompilePath}FileManip.h ${CompilePath}FileManip.cpp ${CompilePath}ExploreDirectory.h ${CompilePath}ExploreDirectory.cpp ${CompilePath}[IO]Pipeline.h ${CompilePath}[IO]Pipeline.cpp"
Linking="-lboost_system -lboost_filesystem"
Options="-O3 -std=c++17 -W -fdiagnostics-color=always"
#FilesToCompile=("DataToJson" "CartesianToSpherical")
FilesToCompile=("CartesianToSpherical")
touch CompilerLog.txt

for I in ${!FilesToCompile[@]}
do
	File=${FilesToCompile[$I]}
	printf "Compiling ${File}.cpp to ${File}... "
	if g++ $Options ${CompilePath}${File}.cpp $Addons $Linking -o ${ExecutablesPath}${File} 2>>CompilerLog.txt
	then
		echo -e "\e[32mSuccess\e[0m."
	else
		echo -e "\e[31mFailed\e[0m."
	fi
done

if [ -s CompilerLog.txt ]
then
        while true
        do
    		read -p "There were errors and/or warnings during compilations, do you want to display them? (y/n) " yn
    		case $yn in
        		[Yy]* ) cat CompilerLog.txt
				break;;
        		
        		[Nn]* ) exit;;
        		* ) echo "Invalid choice, please use y (Y) or n (N).";;
    		esac
		done
fi
rm -f CompilerLog.txt