CPP_files = $(wildcard *.cpp)
H_files   = $(wildcard *.h)
NAVX_Link_file = "navx-mxp/lib/"
NAVX_Includes = "navx-mxp/include/"

main: $(CPP_files) $(H_files)
	arm-frc-linux-gnueabi-g++ -std=c++14 $(CPP_files) -I$(NAVX_Includes) -L$(NAVX_Link_file) -lnavx_frc_cpp -lwpi -o bin/FRCUserProgram

open:
	vim $(CPP_files) $(H_files)
