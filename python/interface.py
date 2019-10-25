'''
The interface module with a list of channels and directories 

For implemantation info read the comments below for the first channel.

Last edited on Oct 25, 2019

@author: ZAKI

'''

import os
import sys
import math
import pep8
import socket
import numpy as np
from collections import defaultdict

from channel import Channel

Yellow = "\033[33m"
NoColor = "\033[0m"
Red = "\033[31m"
Cyan = "\033[36m"


# =================================================================
# color dictionary for printing messages
def color_dict(color):
    if color in ['Y', 'Yellow', 'yellow', 'y']:
        return Yellow
    elif color in ['c', 'Cyan', 'cyan', 'C']:
        return Cyan
    elif color in ['r', 'Red', 'red', 'R']:
        return Red


# =================================================================
# messsage function prints messages on the terminal
def message(text=None, options=None):

    if options is None:
        print(Yellow + text + NoColor)
    else:
        for key, val in options.items():
            if key == "mid":
                message(" | %s%s|" % (text, " "*(val-len(text)-1)))
            if key == "top":
                message("\n  " + "="*val)
            if key == "bottom":
                message("  " + "="*val)
            if key == "err":
                print(Red + "Error: " + text + NoColor)
            if key == "color":
                print(color_dict(val) + text + NoColor)


# =================================================================
# Calculate decay width of phi:
def fi_width(chan):
    for name, tmp_lst in chan.var_param_set.items():
        m_lst = tmp_lst

    if chan.model in ["LFV_2", "LFV_4"]:
        factor = 2  # fi real
    else:
        factor = 1  # fi complex

    fi_w_val = {}

    yf1x2 = chan.fix_param_set["yf1x2"]

    for m in m_lst:
        fi_w_val[m] = ((m * (yf1x2**2)) / (factor*8*math.pi))

    return fi_w_val

# =================================================================

# ..................... Initialization Begins  ..................... #
# This saves the path to the location of scripts for future use.
abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)

# .................................................................. #
# ......  Type the model directory here:
MODEL_DIR = dname + "/Model_Files"

# ......  Type the work directory here:  ......
remote_wrk_dir = "/home/zaki/2019/Summer/LFV/wrk_dir"
Desktop_wrk_dir = "/home/zaki/wrk_dir/LFV/Code/wrk_dir"

# ......  Type the Pythia directory here:
remote_pythia_dir = ""
Desktop_pythia_dir = ""

# ......  Type the MadGraph path here:
remote_MG_dir = "/home/zaki/MG/MG5_aMC_v2_6_1"
Desktop_MG_dir = "/home/zaki/HEPTools/MG5_aMC_v2_6_6"

# .................................................................. #
# If you have multiple platforms you can use this to avoid changing the
#  directories every time you switch platform
if socket.gethostname() == 'zaki-pop-os':

    wrk_dir = Desktop_wrk_dir
    pythia_dir = Desktop_pythia_dir
    MG_dir = Desktop_MG_dir

elif socket.gethostname() == 'dm.itp.ac.cn':

    wrk_dir = remote_wrk_dir
    pythia_dir = remote_pythia_dir
    MG_dir = remote_MG_dir

else:
    print("Fix the directories in 'interface.py'.")
    exit()

# ########################### Channels  ########################### #

# All the channels in this module will be added to this set
channels_set = []

# =========================================
# 0 ................  LFV_1 ................#

# ------ Case 1 ------
ch = Channel()
# This should match the model name inside "/Model Files/"
# i.e. same as what you would type in MadGraph interface as:
#       "import model XXXX"
ch.add_model("LFV_1")
# This name is just for file management
ch.add_name("LFV_1_1")
# This is the C++ script for running pythia, fastjet, etc. 
# It should match the scripts inside "../main/" folder
ch.set_script("search_LFV_1")
# ch.set_script("cms_8TeV")
# This is the parton level final state
# however it's not currently used in "pheno.py" (OCT/25/2019)
ch.add_final_state("e+ mu+ e- mu-")
# If the save file options is set to false, this set
# acts as an exception set of parameters for which 
# the files will be kept. For example for debugging purposes
# you may wanna keep the files for one value of the parameters.
# in this case 150 doesn't exist in our set, and it will be ignored.
ch.add_keep_files([150])
# Using "add_param" method you can add the model parameter values
# the input should be a dictionary with a valid name that can 
# be recognized by MAdGraph, i.e. same as in the model definition
# inside "/Model_Files"
ch.add_param({'yf1x2': 3.000000e-04})
# This is the parton level process, i.e. same as
# what you would type in MadGraph interface:
#       "generate p p > XXXXX"
ch.add_process("p p > h, (h > fi fi~, fi > e- mu+, fi~ > e+ mu-)")
# if a parameter input is a set then "pheno.py" will take it
# as a variable parameter, and it will loop over the set
# Currently only one parameter can be looped over, i.e.
# after this line any other parameter set will be ignored.
# Note that the python collection data type used here is a "list"
# i.e. using brackets "[ XXX ]". You can still use other types
# like python sets for constant variable sets. 
# Check "channel.py" for more info. 
ch.add_param({'mass 9000006': [5, 7, 9, 12, 15, 20, 25, 30, 35, 40, 45, 50,
                               55, 57, 6, 8, 10, 14, 17, 22, 27, 32, 37, 42,
                               47, 52, 60]})

ch.add_param({'kq': 3.000000e-03})
# you can use functions (here "fi_width") in case a parameter is
# dependent on the choice of the variable parameter which is being looped over
ch.add_param({'wfi': fi_width})

# ................
channels_set.append(ch)

# 1 ------ Case 2 ------
ch = Channel()
ch.add_model("LFV_1")
ch.add_name("LFV_1_2")
ch.set_script("search_LFV_1")
# ch.set_script("cms_8TeV")
ch.add_final_state("e+ mu+ e- mu-")
ch.add_keep_files([150])
ch.add_param({'yf1x2': 3.000000e-04})
ch.add_process("p p > h, (h > fi e+ mu-, fi > e- mu+)")
ch.add_param({'mass 9000006': [5, 6, 7, 8, 9, 10, 12, 15, 20, 25, 30, 35, 40,
                               45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95,
                               100, 105, 110, 115, 116]})

ch.add_param({'kq': 0})
ch.add_param({'wfi': fi_width})

# ................
channels_set.append(ch)

# 2 ------ Case 3 ------
ch = Channel()
ch.add_model("LFV_1")
ch.add_name("LFV_1_3")
ch.set_script("search_LFV_1")
# ch.set_script("cms_8TeV")
ch.add_final_state("e+ mu+ e- mu-")
ch.add_keep_files([150])
ch.add_param({'yf1x2': 3.000000e-04})
ch.add_process("p p > Z, (Z > fi e+ mu-, fi > e- mu+)")
ch.add_param({'mass 9000006': [5, 6, 7, 8, 9, 10, 12, 15, 20, 25, 30, 35, 40,
                               45, 50, 55, 60, 65, 70, 75, 80, 85, 90]})

ch.add_param({'kq': 0})
ch.add_param({'wfi': fi_width})

# ................
channels_set.append(ch)
# =========================================

# ................  LFV_2 ................#

# 3 ------ Case 1 ------
ch = Channel()
ch.add_model("LFV_2")
ch.add_name("LFV_2_1")
ch.set_script("search_LFV_2_1")
# ch.set_script("cms_8TeV")
ch.add_final_state("e+ mu- e+ mu-")
ch.add_keep_files([150])
ch.add_param({'yf1x2': 3.000000e-04})
ch.add_process("p p > h, (h > fi fi, (fi > e+ mu-))")
# ch.add_param({'mass 9000006': [5, 7, 9, 12, 15, 20, 25, 30, 35, 40, 45, 50, 55,
#                               57, 6, 8, 10, 14, 17, 22, 27, 32, 37, 42, 47,
#                               52, 60]})
#                           
ch.add_param({'mass 9000006': [47]})

ch.add_param({'kq': 3.000000e-03})
ch.add_param({'wfi': fi_width})

# ................
channels_set.append(ch)

# 4 ------ Case 2 ------
ch = Channel()
ch.add_model("LFV_2")
ch.add_name("LFV_2_2")
ch.set_script("search_LFV_2_2")
# ch.set_script("cms_8TeV")
ch.add_final_state("e+ mu- e+ mu-")
ch.add_keep_files([150])
ch.add_param({'yf1x2': 3.000000e-04})
ch.add_process("p p > h, (h > fi e+ mu-, (fi > e+ mu-))")
ch.add_param({'mass 9000006': [5, 6, 7, 8, 9, 10, 12, 15, 20, 25, 30, 35, 40,
                               45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100,
                               105, 110, 115, 116]})

ch.add_param({'kq': 0})
ch.add_param({'wfi': fi_width})

# ................
channels_set.append(ch)

# 5 ------ Case 3 ------
ch = Channel()
ch.add_model("LFV_2")
ch.add_name("LFV_2_3")
ch.set_script("search_LFV_2_2")
# ch.set_script("cms_8TeV")
ch.add_final_state("e+ mu- e+ mu-")
ch.add_keep_files([150])
ch.add_param({'yf1x2': 3.000000e-04})
ch.add_process("p p > Z, (Z > fi e+ mu-, (fi > e+ mu-))")
ch.add_param({'mass 9000006': [5, 6, 7, 8, 9, 10, 12, 15, 20, 25, 30, 35, 40,
                               45, 50, 55, 60, 65, 70, 75, 80, 85, 90]})

ch.add_param({'kq': 0})
ch.add_param({'wfi': fi_width})

# ................
channels_set.append(ch)
# =========================================

# ................  LFV_3 ................#

# 6 ------ Case 1 ------
ch = Channel()
ch.add_model("LFV_3")
ch.add_name("LFV_3_1")
ch.set_script("search_LFV_3")
# ch.set_script("cms_8TeV")
ch.add_final_state("mu+ mu- ta+ ta-")
ch.add_keep_files([150])
ch.add_param({'yf1x2': 3.000000e-04})
ch.add_process("p p > h, (h > fi fi~, fi > mu- ta+, fi~ > mu+ ta-)")
# ch.add_param({'mass 9000006': [5, 7, 9, 12, 15, 20, 25, 30, 35, 40, 45, 50,
#                                55, 57, 6, 8, 10, 14, 17, 22, 27, 32, 37, 42,
#                                47, 52, 60]})
ch.add_param({'mass 9000006': [5, 6, 7]})

ch.add_param({'kq': 3.000000e-03})
# ch.add_param({'wfi': fi_width})
ch.add_param({'wfi': ((40 * (3.000000e-04)**2) / (1*8*math.pi))})

# ................
channels_set.append(ch)

# 7 ------ Case 2 ------
ch = Channel()
ch.add_model("LFV_3")
ch.add_name("LFV_3_2")
ch.set_script("search_LFV_3")
# ch.set_script("cms_8TeV")
ch.add_final_state("mu+ mu- ta+ ta-")
ch.add_keep_files([150])
ch.add_param({'yf1x2': 3.000000e-04})
ch.add_process("p p > h, (h > fi ta- mu+, fi > ta+ mu-)")
# ch.add_param({'mass 9000006': [5, 6, 7, 8, 9, 10, 12, 15, 20, 25, 30, 35, 40,
#                               45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95,
#                               100, 105, 110, 115, 116]})
ch.add_param({'mass 9000006': [75, 110, 115, 116]})

ch.add_param({'kq': 0})
ch.add_param({'wfi': fi_width})

# ................
channels_set.append(ch)

# 8 ------ Case 3 ------
ch = Channel()
ch.add_model("LFV_3")
ch.add_name("LFV_3_3")
ch.set_script("search_LFV_3")
# ch.set_script("cms_8TeV")
ch.add_final_state("mu+ mu- ta+ ta-")
ch.add_keep_files([150])
ch.add_param({'yf1x2': 3.000000e-04})
ch.add_process("p p > Z, (Z > fi ta- mu+, fi > mu- ta+)")
ch.add_param({'mass 9000006': [5, 6, 7, 8, 9, 10, 12, 15, 20, 25, 30, 35, 40,
                               45, 50, 55, 60, 65, 70, 75, 80, 85, 87]})
ch.add_param({'kq': 0})
ch.add_param({'wfi': fi_width})

# ................
channels_set.append(ch)
# =========================================

# ................  LFV_4 ................#

# 9 ------ Case 1 ------
ch = Channel()
ch.add_model("LFV_4")
ch.add_name("LFV_4_1")
ch.set_script("search_LFV_4")
# ch.set_script("cms_8TeV")
ch.add_final_state("mu+ mu+ ta- ta-")
ch.add_keep_files([150])
ch.add_param({'yf1x2': 3.000000e-04})
ch.add_process("p p > h, (h > fi fi, fi > mu+ ta-)")
# ch.add_param({'mass 9000006': [5, 7, 9, 12, 15, 20, 25, 30, 35, 40, 45, 50, 55,
#                               57, 6, 8, 10, 14, 17, 22, 27, 32, 37, 42, 47,
#                               52, 60]})
ch.add_param({'mass 9000006': [6, 12, 14]})
ch.add_param({'kq': 3.000000e-03})
ch.add_param({'wfi': fi_width})

# ................
channels_set.append(ch)

# 10 ------ Case 2 ------
ch = Channel()
ch.add_model("LFV_4")
ch.add_name("LFV_4_2")
ch.set_script("search_LFV_4")
# ch.set_script("cms_8TeV")
ch.add_final_state("mu+ mu+ ta- ta-")
ch.add_keep_files([150])
ch.add_param({'yf1x2': 3.000000e-04})
ch.add_process("p p > h, (h > fi ta- mu+, fi > ta- mu+)")
ch.add_param({'mass 9000006': [5, 6, 7, 8, 9, 10, 12, 15, 20, 25, 30, 35, 40,
                               45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100,
                               105, 110, 115, 116]})
ch.add_param({'kq': 0})
ch.add_param({'wfi': fi_width})

# ................
channels_set.append(ch)

# 11 ------ Case 3 ------
ch = Channel()
ch.add_model("LFV_4")
ch.add_name("LFV_4_3")
ch.set_script("search_LFV_4")
# ch.set_script("cms_8TeV")
ch.add_final_state("mu+ mu+ ta- ta-")
ch.add_keep_files([150])
ch.add_param({'yf1x2': 3.000000e-04})
ch.add_process("p p > Z, (Z > fi ta- mu+, fi > mu+ ta-)")
ch.add_param({'mass 9000006': [5, 6, 7, 8, 9, 10, 12, 15, 20, 25, 30, 35, 40,
                               45, 50, 55, 60, 65, 70, 75, 80, 85, 87]})
ch.add_param({'kq': 0})
ch.add_param({'wfi': fi_width})

# ................
channels_set.append(ch)

# ===========================================================

# ................  SM Background ................#

# 12 ------ Case 1 ------
ch = Channel()
ch.add_model("LFV_Background")
ch.add_name("e+e-mu+mu-")
ch.set_script("search_LFV_1")
# ch.set_script("cms_8TeV")
ch.add_final_state("e+ mu+ e- mu-")
ch.add_process("p p > e+ mu+ e- mu-")
ch.add_param({'bg': [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                     17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                     31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
                     45, 46, 47, 48, 49, 50]})

# ch.add_param({'bg': [0]})

# ................
channels_set.append(ch)

# 13 ------ Case 1 ------
ch = Channel()
ch.add_model("LFV_Background")
ch.add_name("WWZ_emu")
ch.set_script("search_LFV_1")
# ch.set_script("cms_8TeV")
ch.add_final_state("e+ mu+ e- mu-")

# ch.add_process("p p > w+ w- z, z > l+ l-, w+ > l+ vl, w- > l- vl~")

ch.add_process("p p > w+ w- z, z > e+ e-, w+ > mu+ vl, w- > mu- vl~")
ch.add_process("p p > w+ w- z, z > mu+ mu-, w+ > e+ vl, w- > e- vl~")

ch.add_param({'bg': [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                     17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                     31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
                     45, 46, 47, 48, 49, 50]})

# ch.add_param({'bg': [41]})

# ................
channels_set.append(ch)

# 14 ------ Case 1 ------
ch = Channel()
ch.add_model("LFV_Background")
ch.add_name("ttZ_emu")
ch.set_script("search_LFV_1")
# ch.set_script("cms_8TeV")
ch.add_final_state("e+ mu+ e- mu-")

# ch.add_process("p p > t t~ z, z > l+ l-, ( t > b w+, w+ > l+ vl ),  ( t~ > b~ w-, w- > l- vl~ )")

ch.add_process("p p > t t~ z, z > e+ e-, ( t > b w+, w+ > mu+ vl ),  ( t~ > b~ w-, w- > mu- vl~ )")
ch.add_process("p p > t t~ z, z > mu+ mu-, ( t > b w+, w+ > e+ vl ),  ( t~ > b~ w-, w- > e- vl~ )")

ch.add_param({'bg': [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                     17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                     31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
                     45, 46, 47, 48, 49, 50]})

# ch.add_param({'bg': [41]})

# ................
channels_set.append(ch)

# 15 ------ Case 2 ------
ch = Channel()
ch.add_model("LFV_Background")
ch.add_name("mu+mu-ta+ta-")
ch.set_script("search_LFV_3")
# ch.set_script("cms_8TeV")
ch.add_final_state("mu+ mu- ta+ ta-")
ch.add_process("p p > mu+ mu- ta+ ta-")
# ch.add_param({'bg': [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
#                      17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
#                      31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
#                      45, 46, 47, 48, 49, 50]})
ch.add_param({'bg': [9]})

# ................
channels_set.append(ch)

# 16 ------ Case 2 ------
ch = Channel()
ch.add_model("LFV_Background")
ch.add_name("ta+ta-ta+ta-")
ch.set_script("search_LFV_3")
# ch.set_script("cms_8TeV")
ch.add_final_state("ta+ ta- ta+ ta-")
ch.add_process("p p > ta+ ta- ta+ ta-")
# ch.add_param({'bg': [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
#                      17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
#                      31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
#                      45, 46, 47, 48, 49, 50]})
ch.add_param({'bg': [27, 32, 36]})

# ................
channels_set.append(ch)

# 17 ------ Case 2 ------
ch = Channel()
ch.add_model("LFV_Background")
ch.set_script("search_LFV_3")
# ch.set_script("cms_8TeV")
ch.add_final_state("mu+ mu- ta+ ta-")
ch.add_name("WWZ_ta")
# ch.add_process("p p > w+ w- z, z > l+ l-, w+ > l+ vl, w- > l- vl~")

ch.add_process("p p > w+ w- z, z > ta+ ta-, w+ > l+ vl, w- > l- vl~")
ch.add_process("p p > w+ w- z, z > ta+ ta-, w+ > ta+ vl, w- > l- vl~")
ch.add_process("p p > w+ w- z, z > ta+ ta-, w+ > l+ vl, w- > ta- vl~")
ch.add_process("p p > w+ w- z, z > ta+ ta-, w+ > ta+ vl, w- > ta- vl~")
ch.add_process("p p > w+ w- z, z > l+ l-, w+ > ta+ vl, w- > l- vl~")
ch.add_process("p p > w+ w- z, z > l+ l-, w+ > l+ vl, w- > ta- vl~")
ch.add_process("p p > w+ w- z, z > l+ l-, w+ > ta+ vl, w- > ta- vl~")

# ch.add_param({'bg': [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
#                      17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
#                      31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
#                      45, 46, 47, 48, 49, 50]})

ch.add_param({'bg': [41]})

# ................
channels_set.append(ch)


# 18 ------ Case 2 ------
ch = Channel()
ch.add_model("LFV_Background")
ch.add_final_state("mu+ mu- ta+ ta-")
ch.set_script("search_LFV_3")
# ch.set_script("cms_8TeV")
ch.add_name("ttZ_ta")

# ch.add_process("p p > t t~ z, z > l+ l-, ( t > b w+, w+ > l+ vl ),  ( t~ > b~ w-, w- > l- vl~ )")

ch.add_process("p p > t t~ z, z > ta+ ta-, ( t > b w+, w+ > l+ vl ),  ( t~ > b~ w-, w- > l- vl~ )")
ch.add_process("p p > t t~ z, z > ta+ ta-, ( t > b w+, w+ > ta+ vl ),  ( t~ > b~ w-, w- > l- vl~ )")
ch.add_process("p p > t t~ z, z > ta+ ta-, ( t > b w+, w+ > l+ vl ),  ( t~ > b~ w-, w- > ta- vl~ )")
ch.add_process("p p > t t~ z, z > ta+ ta-, ( t > b w+, w+ > ta+ vl ),  ( t~ > b~ w-, w- > ta- vl~ )")
ch.add_process("p p > t t~ z, z > l+ l-, ( t > b w+, w+ > ta+ vl ),  ( t~ > b~ w-, w- > l- vl~ )")
ch.add_process("p p > t t~ z, z > l+ l-, ( t > b w+, w+ > l+ vl ),  ( t~ > b~ w-, w- > ta- vl~ )")
ch.add_process("p p > t t~ z, z > l+ l-, ( t > b w+, w+ > ta+ vl ),  ( t~ > b~ w-, w- > ta- vl~ )")

ch.add_param({'bg': [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                     17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
                     31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
                     45, 46, 47, 48, 49, 50]})

# ch.add_param({'bg': [0]})

# ................
channels_set.append(ch)

# ..........................FAKE TAUS.........................
# 19 ------ Case 2 ------
# Jets faking taus
ch = Channel()
ch.add_model("LFV_Background")
ch.add_name("ttW_fake_tau")
ch.set_script("faketaus")
# ch.set_script("cms_8TeV")
ch.add_final_state("mu+ mu- ta+ ta-")

ch.add_process("p p > t t~ w+, w+ > l+ vl, ( t > b w+, w+ > l+ vl ),  ( t~ > b~ w-, w- > l- vl~ )")
ch.add_process("p p > t t~ w-, w- > l- vl~, ( t > b w+, w+ > l+ vl ),  ( t~ > b~ w-, w- > l- vl~ )")

ch.add_param({'bg': [1, 2, 3, 4, 5]})

# ................
channels_set.append(ch)


# 20 ------ Case 2 ------
# Jets faking taus
ch = Channel()
ch.add_model("LFV_Background")
ch.add_name("WZ_fake_tau")
ch.set_script("faketaus")
# ch.set_script("cms_8TeV")
ch.add_final_state("mu+ mu- ta+ ta-")

ch.add_process("p p > w+ z, w+ > l+ vl, z > ta+ ta-")
ch.add_process("p p > w- z, w- > l- vl~, z > ta+ ta-")

ch.add_param({'bg': [1, 2, 3, 4, 5]})

# ................
channels_set.append(ch)


# 21 ------ Case 2 ------
# Jets faking taus
ch = Channel()
ch.add_model("LFV_Background")
ch.add_name("ttZ_fake_tau")
ch.set_script("faketaus")
# ch.set_script("cms_8TeV")
ch.add_final_state("mu+ mu- ta+ ta-")

ch.add_process("p p > t t~ z, z > ta+ ta-, ( t > b w+, w+ > l+ vl ),  ( t~ > b~ w-, w- > l- vl~ )")

ch.add_param({'bg': [1, 2, 3, 4, 5]})

# ................
channels_set.append(ch)


# =========================================
# ########################### End of Channels  ########################### #


yes_set = ['y', 'yes', 'yay', 'yeah', 'yea', 'ok', 'ye', 'ya', 'yep', 'Y']


####################################################
def PY_Input_Function(input_txt):
    if sys.version_info[0] < 3:
        return raw_input(input_txt)
    else:
        return input(input_txt)

####################################################

if __name__ == "__main__":

    a = PY_Input_Function("Hi!\n")
    print(a)
