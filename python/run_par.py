'''

The module for defining the run parameters for MadGraph

Last edited on July 7, 2019

@author: ZAKI
'''
# import os
# import pep8
# import errno
# import shutil
import logging
# from interface import wrk_dir

logger = logging.getLogger('RunPar Object')
logging.basicConfig(filename="pheno.log",
                    format='%(asctime)s - %(name)s ' +
                           '- %(levelname)s : %(message)s',
                    filemode='w', level=logging.DEBUG)


# =================================================================
class RunPar:

    def __init__(self):
        self.MG_run_pars = {}
        self.inter_run_pars = {}

    # ---------------------------------------
    def __str__(self):
        """String representation of the object. Outputs valid Python
        with improved format."""

        my_str = "\n{" + str(self.__class__.__name__) + " class info: \n"
        for key, val in self.__dict__.items():
            my_str += " " + key + " : " + str(val) + ""

        my_str += "}"

        return my_str

    # ---------------------------------------
    def add_MG_par(self, par_dict):

        self.MG_run_pars = par_dict

    # ---------------------------------------
    def add_inter_par(self, par_dict):
        self.inter_run_pars = par_dict

    # ---------------------------------------
    __repr__ = __str__

# =================================================================

default_MG_run_pars = (
        {
            # Sets beam 1 energy to ... TeV
            "ebeam1": 6500,
            # Sets beam 2 energy to ... TeV
            "ebeam2": 6500,
            # Sets the number of events
            "nevents": 500000,
            # minimum pT cut for jets
            "ptj": 5,
            # minimum pT cut for photons
            "pta": 5,
            # minimum pT cut for charged leptons
            "ptl": 5,
            # maximum rapidity cut for jets
            "etaj": 5,
            # maximum rapidity cut for photons
            "etaa": 5,
            # maximum rapidity cut for charged leptons
            "etal": 5,
            # minimum distance between jets
            "drjj": 0.4,
            # minimum distance between charged leptons
            "drll": 0.4,
            # minimum distance between photons
            "draa": 0.4,
            # minimum distance between photons & jets
            "draj": 0.4,
            # minimum distance between charged leptons & jets
            "drjl": 0.4,
            # minimum distance between charged leptons & photons
            "dral": 0.4,
        })

default_inter_run_pars = {}

default_run_pars = RunPar()
default_run_pars.add_MG_par(default_MG_run_pars)
default_run_pars.add_inter_par(default_inter_run_pars)

if __name__ == "__main__":

    print(default_run_pars)

# -------------------------------
