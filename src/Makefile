#Name of C++ compiler:
CXX = g++
#Remove variable
RM = rm -f

SOURCES = comp_emu.cpp core.cpp gen_test_programs.cpp
OBJECTS = $(SOURCES:.cpp=.o)

SOURCES2 = gen_test_programs.cpp
OBJECTS2 = $(SOURCES2:.cpp=.o)
#Alternatively: OBJECTS =  $(subst .cpp, .o, $(SOURCES))

all: comp_emu.exe gen_test_programs.exe clean

comp_emu.exe: $(OBJECTS)
	$(CXX) -o comp_emu comp_emu.o core.o

comp_emu.o: comp_emu.cpp core.h
	$(CXX) -c comp_emu.cpp

core.o: core.cpp core.h
	$(CXX) -c core.cpp
	
gen_test_programs.exe: $(OBJECTS2)
	$(CXX) -o gen_test_programs gen_test_programs.o
	
gen_test_programs.o: 
	$(CXX) -c gen_test_programs.cpp
clean:
	$(RM) *.o
#Alternatively: $(RM) $(OBJECTS) $(OBJECTS2)