
# File Name
MAKEFILENAME = AsyncProjectNOVS

# Machine Type
PRO_MACHINE_TYPE = x86e_win64

# Executable names
EXE = AsyncProjectNOVS.exe

# Pro/Toolkit Source & Machine Loadpoint  (EXTERNAL USE - DEFAULT)
PROTOOL_SRC = ./src
# PROTOOL_SYS = "C:/PTC/Creo 2.0/Common Files/M060/protoolkit/x86e_win64"
# PROTOOL_HEAD = "C:/PTC/Creo 2.0/Common Files/M060/protoolkit/includes"

# Include File Paths
INCS = -I. -I"C:/PTC/Creo 2.0/Common Files/M060/protoolkit/includes" -I$(PROTOOL_SRC)/includes

# Compiler Flags
CC = cl
MACH = -DPRO_MACHINE=36 -DPRO_OS=4
CCFLAGS = /wd4430  /TP -c -GS -fp:precise -D_WSTDIO_DEFINED -DPRO_USE_VAR_ARGS -DPRO_USE_VAR_ARG  /Od /Z7
CFLAGS = $(CCFLAGS) $(INCS) $(MACH)

# Libraries
PTCLIBS = "C:/PTC/Creo 2.0/Common Files/M060/protoolkit/x86e_win64/obj/protoolkit.lib" "C:/PTC/Creo 2.0/Common Files/M060/protoolkit/x86e_win64/obj/pt_asynchronous.lib"
LIBS = kernel32.lib user32.lib wsock32.lib advapi32.lib mpr.lib winspool.lib netapi32.lib psapi.lib gdi32.lib shell32.lib comdlg32.lib ole32.lib ws2_32.lib
PDCURSESLIB = "D:/mydoc/creo_toolkit/AsyncProjectWithPdcurses/libs/pdcurses.lib"

# Object files
OBJS = Test.obj 

# Linker
LINK = link


$(EXE) :  $(OBJS) $(PTCLIBS) $(PDCURSESLIB)
#Executable applications compiled using WinMain() instead of main() should set the subsystem to windows instead of console.
	$(LINK) /subsystem:console -out:$(EXE) /debug /debugtype:cv /machine:amd64 @<<longline.list 
$(OBJS) $(PDCURSESLIB) $(PTCLIBS) $(LIBS)
<<

# object dependencies
Test.obj:  $(PROTOOL_SRC)/Test.c
	$(CC) $(CFLAGS) $(PROTOOL_SRC)/Test.c

clean :
	del $(OBJS)
	del $(EXE)
	del $(MAKEFILENAME).pdb
	del $(MAKEFILENAME).ilk
	del $(MAKEFILENAME).exp
	del $(MAKEFILENAME).lib

