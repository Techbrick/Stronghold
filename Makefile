CPP_files = $(wildcard *.cpp)
H_files   = $(wildcard *.h)

main: $(CPP_files) $(H_files)
	arm-frc-linux-gnueabi-g++ -std=c++14 $(CPP_files) -lwpi -o bin/FRCUserProgram

open:
	mousepad $(CPP_files) $(H_files) &
