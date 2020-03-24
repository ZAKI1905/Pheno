# ----------------------------------
# Targets
# LINK_TARGET	:= cms_8TeV
# MAIN 	:= cms_8TeV

# LINK_TARGET	:= faketaus
# MAIN 	:= faketaus

# LINK_TARGET	:= search_LFV_1
# MAIN 	:= search_LFV_1

# LINK_TARGET	:= search_LFV_2_1
# MAIN 	:= search_LFV_2_1

# LINK_TARGET	:= search_LFV_2_2
# MAIN 	:= search_LFV_2_2

# LINK_TARGET	:= search_LFV_3
# MAIN 	:= search_LFV_3

# LINK_TARGET	:= search_LFV_4
# MAIN 	:= search_LFV_4

LINK_TARGET	:= user_cuts
MAIN 	:= user_cuts



# ----------------------------------
# Compiler and Linker
CC          := clang++-mp-9.0

# Extensions
SRC_EXT      := cc
OBJ_EXT		 := o

# Directories
SRC_DIR		:= src
MAIN_DIR	:= main
OBJ_DIR		:= obj
BIN_DIR		:= bin
INC_DIR		:= include

# Flags, Libraries and Includes
DEP_INC		:= -Idependencies/include 
DEP_LIB_DIR	:= -Ldependencies/lib

PYTH8_FLAG		:= $(DEP_LIB_DIR) -lpythia8 -lboost_iostreams -lz
FASTJ_FLAG 		:=  -lfastjet
CCFLAGS			:=  -std=c++11 -Wall -g -fopenmp -fPIC

# Making library
OUT_LIB		:= Pheno
OUT_LIB_DIR	:= lib

# Compile command
COMPILE		:= $(CC) $(CCFLAGS) $(DEP_INC) -I$(INC_DIR) 
COMPILE_FLAG:= $(PYTH8_FLAG) $(FASTJ_FLAG)
# ================================================================================

SOURCES     := $(shell find $(SRC_DIR) -type f -name '*.$(SRC_EXT)')
OBJECTS     := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(SOURCES:.$(SRC_EXT)=.$(OBJ_EXT)))

# .......................................................................

all: directories $(BIN_DIR)/$(LINK_TARGET)

directories :
	@echo "Creating the directories..." 
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/$(LINK_TARGET):	$(OBJECTS)  $(OBJ_DIR)/$(MAIN).$(OBJ_EXT)
	@echo "Linking the objects..."
	$(COMPILE) $(COMPILE_FLAG) $^ -o $@

$(OBJ_DIR)/%.$(OBJ_EXT) : $(SRC_DIR)/%.$(SRC_EXT) $(INC_DIR)/%.h
	@echo "Compiling the dependencies..."
	$(COMPILE) -c -o $@ $<

$(OBJ_DIR)/$(MAIN).$(OBJ_EXT) : $(MAIN_DIR)/$(MAIN).$(SRC_EXT)
	@echo "Compiling the Main file..."
	$(COMPILE) -c -o $@ $<

# in case only header file changes
$(INC_DIR)/%.h : 
	$(COMPILE) -c -o $(OBJ_DIR)/%.$(OBJ_EXT) $(SRC_DIR)/%.$(SRC_EXT)
# .......................................................................

genlib: mk_lib_dirs complib

complib: $(OBJECTS)
	@echo "Creating static library..."
	ar -cvq $(OUT_LIB_DIR)/lib$(OUT_LIB).a $^
	# gcc -shared  $^ -o $(OUT_LIB_DIR)/lib$(OUT_LIB).so

mk_lib_dirs: 
	@echo "Creating directories..."
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OUT_LIB_DIR)

# .......................................................................

# Clean only Objects
clean:
	@echo "Cleaning objects..."
	@$(RM) -rf $(OBJ_DIR)

# .......................................................................

# Full Clean, Objects and Binaries
cleaner: clean
	@echo "Cleaning up objects and binaries..."
	@$(RM) -rf $(BIN_DIR)
	# @$(RM) -rf $(OUT_LIB_DIR)

# .......................................................................

# Non-File Targets
.PHONY: all clean cleaner directories genlib mk_lib_dirs complib

# .......................................................................