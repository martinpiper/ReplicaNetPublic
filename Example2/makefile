# This is an example makefile for Linux
ALL =		LinuxExample2

# These file should get made first from the ROL files, so that the ROL output cpp/h files are not missing for the rest of the compile
COBJFILES = 	_Def_Example2.o _RO_Test1.o
# These files are made after the ROL compiler files
OBJFILES =	Test1.o main.o ../ReplicaNetPublic/AmalgamatedReplicaNet.o

# These files are the ones created by the ROLCompiler
ROLOBJFILES = 	_Def_Example2.cpp _RO_Test1.cpp \
				_Def_Example2.h _RO_Test1.h


# These flags help to reduce the ELF size when it links.
CFLAGS =	-O2 -Os -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,-s -pedantic -I../ReplicaNetPublic/Includes/
# These flags are simpler, but don't reduce the ELF size as well as the flags above.
#CFLAGS =	-O2 -pedantic -I../ReplicaNetPublic/Includes/
CPPFLAGS =	$(CFLAGS)

# The first target
all:		$(ALL)


# The clean target
clean:
		rm -f core ${ALL} $(OBJFILES) $(COBJFILES)
# Remove the ROL compiled files
		rm -f $(ROLOBJFILES)

# A rule to make cpp files from the rol files, the ROLCompiler takes the additional include directory to the lang directory
%.cpp:%.rol
	../ReplicaNetPublic/RNROLCompiler/bin/RNROLCompiler -v1 -I../ReplicaNetPublic/RNROLCompiler/lang/ $^


# Have the COBJFILES files first so that they get built before the other source files
LinuxExample2:	$(COBJFILES) $(OBJFILES)
		$(CC) $(CFLAGS) -o $@ $^ -lstdc++ -lpthread -lm
