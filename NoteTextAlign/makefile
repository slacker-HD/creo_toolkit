#             PTC             File
# -----------------------------------------------------------------------------
# File Name
MAKEFILENAME = NoteTextAlign

# Machine Type
PRO_MACHINE_TYPE = x86e_win64

# Executable names
EXE_DLL = NoteTextAlign.dll

# Pro/Toolkit Source & Machine Loadpoint  (EXTERNAL USE - DEFAULT)
PROTOOL_SRC = ./src

# Include File Paths
INCS = -I. -I"C:/PTC/Creo 2.0/Common Files/M060/protoolkit/includes" -I$(PROTOOL_SRC)/includes

# Compiler Flags
CC = cl
MACH = -DPRO_MACHINE=36 -DPRO_OS=4
CCFLAGS = -c -GS -fp:precise -D_WSTDIO_DEFINED  /Od /Z7
CFLAGS = $(CCFLAGS) $(INCS) $(MACH)

# Libraries
PTCLIBS = "C:/PTC/Creo 2.0/Common Files/M060/protoolkit/x86e_win64/obj/protoolkit.lib"
PTCLIBS_DLL = "C:/PTC/Creo 2.0/Common Files/M060/protoolkit/x86e_win64/obj/protk_dll.lib"
LIBS = libcmt.lib kernel32.lib user32.lib wsock32.lib advapi32.lib mpr.lib winspool.lib netapi32.lib psapi.lib gdi32.lib shell32.lib comdlg32.lib ole32.lib ws2_32.lib

# Object files
OBJS = main.obj

# Linker
LINK = link

# Dll target
$(EXE_DLL) :  $(OBJS) $(PTCLIBS_DLL)
	$(LINK) /subsystem:console -out:$(EXE_DLL) /debug /machine:amd64 /dll @<<longline.list
$(OBJS) $(PTCLIBS_DLL) $(LIBS)
<<

# object dependencies
main.obj:  $(PROTOOL_SRC)/main.c
	$(CC) $(CFLAGS) $(PROTOOL_SRC)/main.c

clean :
	del $(OBJS)
	del $(EXE_DLL)
	del $(MAKEFILENAME).pdb
	del $(MAKEFILENAME).ilk
	del $(MAKEFILENAME).exp
	del $(MAKEFILENAME).lib