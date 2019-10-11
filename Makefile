# The main c++ file ( should have main() function )
MAINOBJ_0	:= cms_8TeV
MAINOBJ_1 	:= search_LFV_1
MAINOBJ_2_1 := search_LFV_2_1
MAINOBJ_2_2 := search_LFV_2_2
MAINOBJ_3 	:= search_LFV_3
MAINOBJ_4 := search_LFV_4
MAINOBJ_5 	:= faketaus

# MAINOBJ := $(MAINOBJ_1) $(MAINOBJ_2) $(MAINOBJ_3) $(MAINOBJ_4)

MAINOBJ 	:= $(MAINOBJ_5)

# The Target Binary Program 
# This could be anything, but this way you can make
# all the binaries by switching the 'MAINOBJ' above,
# without changing anything else.
TARGET      :=  $(MAINOBJ)


# The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
INCDIR      := inc
BUILDDIR    := obj
TARGETDIR   := bin
# RESDIR      := res
MAINDIR     := main
SRCEXT      := cc
DEPEXT      := d
OBJEXT      := o

# Compiler and Linker
CC          := g++


# Flags, Libraries and Includes
CFLAGS      := -std=c++11 -fopenmp -Wall -Wextra -pedantic -g
# CFLAGS      := -std=c++11 -fopenmp -Wall -Wextra -pedantic -g -lCGAL -lgmp
PYTFASFLAG 	:= $(shell pythia8-config --fastjet3 --libs --cxxflags ) 
LDFLAGS 	:= 
INC         := -I$(INCDIR) -I/usr/local/include $(PYTFASFLAG) $(LDFLAGS)
# INC         := -I$(INCDIR) $(PYTFASFLAG) $(LDFLAGS) 
INCDEP      := -I$(INCDIR) $(PYTFASFLAG) $(LDFLAGS) 

#---------------------------------------------------------------------------------
# DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

# # Making special target
# special: directories $(TARGET)

# Making all targets
all: directories $(TARGET)

# Remake
remake: cleaner all

# #Copy Resources from Resources Directory to Target Directory
# resources: directories
# 	@cp $(RESDIR)/* $(TARGETDIR)/

# Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

# Clean only Objecst
clean:
	@echo "Cleaning objects..."
	@$(RM) -rf $(BUILDDIR)

# Full Clean, Objects and Binaries
cleaner: clean
	@echo "Cleaning up..."
	@$(RM) -rf $(TARGETDIR)

# Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

# Link
$(TARGET): $(OBJECTS) $(MAINOBJ).$(OBJEXT)
	$(CC)  $(CFLAGS) -o $(TARGETDIR)/$(TARGET) $^ $(PYTFASFLAG) $(LDFLAGS)

# Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<  
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT) 
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp
	@echo "Build Date = `date` " >_compile.txt

$(MAINOBJ).$(OBJEXT): $(MAINDIR)/$(MAINOBJ).$(SRCEXT)
	$(CC) -c $(MAINDIR)/$(MAINOBJ).$(SRCEXT)

# Non-File Targets
.PHONY: all remake clean cleaner resources

