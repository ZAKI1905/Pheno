'''
Module for defining channels

check "interface.py" for a list of channels defined

@author: ZAKI

'''

# import pep8
import logging


logger = logging.getLogger('Channel Object')
logging.basicConfig(filename="pheno.log",
                    format='%(asctime)s - %(name)s ' +
                           '- %(levelname)s : %(message)s',
                    filemode='w', level=logging.DEBUG)


# test function for dependent variables
def ftest(ch):
    for name, x_lst in ch.var_param_set.items():
        tmp_lst = {}
        for x in x_lst:
            tmp_lst[x] = 3*x
    return tmp_lst


# =================================================================
class Channel:

    def __init__(self):
        self.name = "ch_1"
        self.model = "sm"

        self.process = []
        self.final_state = ""

        self.var_param_set = {}
        self.dep_param_set = {}
        self.fix_param_set = {}

        self.inter_param_set = {}

        self.keep_files = []

        self.script = ""

    # ---------------------------------------
    def __str__(self):
        """String representation of the object. Outputs valid Python
        with improved format."""

        my_str = "\n{" + str(self.__class__.__name__) + " class info: \n"
        for key, val in self.__dict__.items():
            my_str += " " + key + " : " + str(val) + "\n"

        my_str += "}"

        return my_str

    # ---------------------------------------
    def add_model(self, model):
        self.model = model

    # ---------------------------------------
    def add_name(self, name):
        self.name = name

    # ---------------------------------------
    def add_process(self, process):
        self.process.append(process)

    # ---------------------------------------
    def add_final_state(self, final_state):
        self.final_state = final_state

    # ---------------------------------------
    def add_keep_files(self, par_list):
        self.keep_files = par_list

    # ---------------------------------------
    def set_script(self, script):
        self.script = script

    # ---------------------------------------
    # MG parameters
    def add_param(self, par_dict):
        for key, val in par_dict.items():
            if isinstance(val, list):
                if len(self.var_param_set) == 0:
                    self.var_param_set = par_dict
                else:
                    logger.error("Parameter '%s' wasn't added." % key)
                    message("There can be only 1 variable parameter," +
                            "\n use set instead of list for " +
                            "multi-dimensional parameters.", {"err": 0})

            elif type(val) in [int, float, str, tuple, set]:
                self.fix_param_set.update(par_dict)
            else:
                self.dep_param_set[key] = val(self)

    # ---------------------------------------
    # Internal parameters
    def add_inter_param(self, par_dict):
        for key, val in par_dict.items():

            if type(val) in [list, int, float, str, tuple, set]:
                self.inter_param_set.update(par_dict)
            else:
                self.inter_param_set[key] = val(self)
    # ---------------------------------------

    __repr__ = __str__

# =================================================================

if __name__ == "__main__":

    ch = Channel()

    ch.add_model("LFV_2")
    ch.add_name("LFV_2_1")

    ch.add_process("p p > h, (h > fi fi, (fi > e+ mu-))")
    ch.add_final_state("e+ mu- e+ mu-")

    ch.add_param({'kq': 3.000000e-03})
    ch.add_param({'mass 9000006': [30, 45, 60]})
    ch.add_param({'yf1x2': 3.000000e-04})

    ch.add_inter_param({'3x': ftest})

    print(ch)

# -------------------------------
