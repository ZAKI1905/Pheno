# Pheno
This is the program used for the collider analysis in "Exotic Lepton-Flavor Violating Higgs Decays" project. 

-Last updated October 11, 2019


To make the binary files:
  1) Edit the make file ("~/Makefile") in the main directory, by adding the path to pythia, fastjet, etc. 
  2) Choose the main c++ file (which is inside "~/main") to be compiled, by typing it in front of "MAINOBJ" in the make file. 
  3) Run "make"


To run the binaries:
  1) Put the LHE file(s) in the desired directory, with the following naming scheme:
  {"XYZ_0.lhe", "XYZ_1.lhe", "XYZ_2.lhe", ...}
  where all "XYZ.lhe" files are from the same process, but chopped for parallel computing by OMP.
  Note: If there is only one LHE file, still add "_0.lhe" to its name.

  Step 2 depends on the main c++ file format, for the ones inside "~/main" do

  2) run "./bin/[BINARY] [LHE_FILE] [NUM_EVENTS] [OMP_THREADS] [REPORT_BOOL]"
  where:

    "BINARY" stands for the name of the binary file (inside ~/bin)
    "LHE_FILE" is the path+name of the LHE file compared to the current path in terminal
    Note that the extra extensions {"_0", "_1", ...} should be omitted.

    "NUM_EVENTS" is the number of events in the LHE file
    "OMP_THREADS" is the number of threads to be requested from OMP
    "REPORT_BOOL" can be '0' (no cut, etc. reporting) or '1' (reports cuts, etc. in .txt files)

  Example: run "./bin/cms_8TeV lhe_dir/my_model.lhe 1000 2 0" in terminal, 
  where the lhe_dir contains 2 lhe files each having 500 events named as:
    {my_model_0.lhe, my_model_1.lhe}

To add your own main c++ file:
  1) write a c++ file with a main() function and put it inside "~/main".
  2) Add the name of the main file in the make file for compiling.
  3) Compile using "make" command
  4) A binary with the same name will be created inside "~/bin"
  5) Run as explained above.

-------------------------------------------
This package uses PYTHIA-8 and FastJet.

PYTHIA-8: 
  - T. Sjöstrand, S. Mrenna and P. Skands, JHEP05 (2006) 026, Comput. Phys. Comm. 178 (2008) 852.
  
FastJet: 
  - M. Cacciari, G.P. Salam and G. Soyez, Eur.Phys.J. C72 (2012) 1896 [arXiv:1111.6097].


