PYTFASFLAG = $(shell pythia8-config --fastjet3 --libs --cxxflags ) -lCGAL -lgmp

objects = Basics.o ExParticle.o EV.o Cut.o GenJet.o Isolation.o IdEff.o M2Cut.o M4Cut.o PtCut.o PrapCut.o Binner.o STBinner.o test.o Pheno.o

CC = g++
CCFLAGS  = -std=c++11 -fopenmp -Wall -Wextra -pedantic -g
EXE = run

#------------------------------------objects----------------------------------------
$(EXE) : $(objects)
	$(CC) $(CCFLAGS) -o $(EXE) $(objects) $(PYTFASFLAG)
	echo "Build Date = `date` " >_compile.txt
	-rm $(objects)

test.o : Basics.o EV.o IdEff.o
	$(CC) -std=c++11 -c test.cc 

Basics.o : src/Basics.cc include/Basics.h
	$(info Compiling...) 
	$(CC) -std=c++11 -c src/Basics.cc 

ExParticle.o : Basics.o src/ExParticle.cc include/ExParticle.h
	$(CC) -std=c++11 -c src/ExParticle.cc 

EV.o : ExParticle.o Basics.o src/EV.cc include/EV.h
	$(CC) -std=c++11 -c src/EV.cc 

GenJet.o : EV.o Basics.o src/GenJet.cc include/GenJet.h
	$(CC) -std=c++11 -c src/GenJet.cc 

Cut.o : EV.o Basics.o src/Cut.cc include/Cut.h
	$(CC) -std=c++11 -c src/Cut.cc 

Isolation.o : Basics.o EV.o Cut.o src/Isolation.cc include/Isolation.h
	$(CC) -std=c++11 -c src/Isolation.cc 

IdEff.o : Basics.o EV.o Cut.o src/IdEff.cc include/IdEff.h
	$(CC) -std=c++11 -c src/IdEff.cc 

M2Cut.o : Basics.o EV.o Cut.o src/M2Cut.cc include/M2Cut.h
	$(CC) -std=c++11 -c src/M2Cut.cc 

M4Cut.o : Basics.o EV.o Cut.o src/M4Cut.cc include/M4Cut.h
	$(CC) -std=c++11 -c src/M4Cut.cc 

PtCut.o : Basics.o EV.o Cut.o src/PtCut.cc include/PtCut.h
	$(CC) -std=c++11 -c src/PtCut.cc 

PrapCut.o : Basics.o EV.o Cut.o src/PrapCut.cc include/PrapCut.h
	$(CC) -std=c++11 -c src/PrapCut.cc 

Binner.o : EV.o Basics.o src/Binner.cc include/Binner.h
	$(CC) -std=c++11 -c src/Binner.cc 

STBinner.o : EV.o Basics.o Binner.o src/STBinner.cc include/STBinner.h
	$(CC) -std=c++11 -c src/STBinner.cc 

Pheno.o : Basics.o src/Pheno.cc include/Pheno.h
	$(CC) $(CCFLAGS) -std=c++11 -c src/Pheno.cc

#-------------------------------------------------------------------------------

.PHONY : clean

clean :
	$(info Cleaning...)
	-rm $(EXE) $(objects)
