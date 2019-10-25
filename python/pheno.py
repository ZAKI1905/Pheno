'''
The main module that:
    1) runs MadGraph,
    2) chops the lhe files from MG into smaller pieces (for multithreading),
    3) runs the corresponding script (C++ file inside "../main/") for each channel
    4) and also does all the necessary file management

Last edited on Sep 3, 2019

@author: ZAKI
'''

import re
import os
import sys
import math
# import pep8
import gzip
import tarfile
import errno
import shutil
import random
import socket
import logging
import subprocess
import numpy as np
from unipath import Path
from os.path import isfile, join
from time import strftime, localtime, time
from multiprocessing import Pool


from interface import message, dname, MG_dir, yes_set, channels_set, fi_width,\
                      wrk_dir
from channel import Channel
from run_par import RunPar, default_run_pars

logger = logging.getLogger('Pheno Object')
logging.basicConfig(filename="pheno.log",
                    format='%(asctime)s - %(name)s ' +
                           '- %(levelname)s : %(message)s',
                    filemode='w', level=logging.DEBUG)


dname_path = Path(dname)


# =================================================================
def create_dir(dir_name):
    try:
        os.makedirs(dir_name)
        logger.debug("Creating directory '%s'." % dir_name)
    except OSError as exc:
        if exc.errno != errno.EEXIST:
            logger.error("Directory '%s' can't be created!" % dir_name,
                         exc_info=True)
            raise
        logger.warning("Directory '%s' already exist." % dir_name)
        pass


# =================================================================
class Pheno:

    # --------------------------------------
    def __init__(self, wrk_dir):

        # Creating the work directory
        create_dir(wrk_dir)

        # main work directory
        self.wrk_dir = wrk_dir

        # trash folder for throwing old files
        self.trash_dir = self.wrk_dir + '/_trash'

        # directory of the MG model files
        self.model_dir = dname + '/Model_Files'

        # temporary directory to keep the MG input files
        self.MG_input_dir = "_tmp_MG_input"

        # directory to the pythia binary files
        self.pythia_run_file = 'bin'

        # For keeping track of the directories made in a run
        self.run_folder_list = []

        # LHE file directory inside the home directory, i.e. 'dname' parent
        self.LHE_dir = dname_path.parent + '/_tmp_lhe'

        # This time can be reset by the user e.g. between runs
        self.start_t = time()
        self.start_T = localtime()

        self.start_t_global = self.start_t
        self.start_T_global = self.start_T

        # The channel counter
        self.run_count = 0

        # MG command
        if socket.gethostname() == 'dm.itp.ac.cn':
            self.MG_command = 'mg5_aMC'
        else:
            self.MG_command = MG_dir+'/bin/mg5_aMC'

        # The set of random seeds used during the run
        self.rnd_seed_set = {}

        # Changing the seed every 100K events (suggested by MG)
        self.seed_ev_chunk = 100000

        # The channels to be iterated over
        self.channel_set = []

        # placeholder for the name of the var in the iteraions
        self.var_name = ""

        # A dictionary of failed steps for the whole run
        # Example : { channel : {par : {"MG" : 2} } }
        self.fail = {}

        # Maximum number of times pythia is run after failure
        self.pythFail_MAX = 2

        # number of processes for iterations
        self.process_num = 1

        # number of OMP threads in pythia
        self.OMP_thrds = 5

        # Runs in batch mode, i.e. no interruption in case of errors
        self.batch_mode = True

        # runs lhe_reader module
        self.lhe_reader_step = False

        # runs pythia
        self.pythia_step = False

        # report cuts
        self.cut_report = False

        # keeping files options are '0', '1' and '2'
        self.keep_files = '0'
        self.tmp_keep_files = self.keep_files
        self.keep_lhe_chops = True
        # LHE file compression option
        # Can be 'zip' or 'gz' or anything else
        #  e.g. '' for no compression.
        self.zip_lhe_chops = ''
        self.lhe_chop_dir = 'LHE_Files'

        # temporary run channel in the iterations
        self.current_run_chan = None

        self.init_clean_up('Eff_')
        self.init_clean_up('pheno.log')

    # --------------------------------------
    def __str__(self):
        """String representation of the object. Outputs valid Python
        with improved format."""

        my_str = "\n{" + str(self.__class__.__name__) + " class info: \n"
        for key, val in self.__dict__.items():
            my_str += " " + key + " : " + str(val) + "\n"

        my_str += "\n}"

        return my_str

    # --------------------------------------
    def init_clean_up(self, file_str):
        # .............. Cleaning up wrk_dir ................. #
        lst_of_f = os.listdir(self.wrk_dir)

        for f in lst_of_f:
            if f.startswith(file_str):
                try:
                    os.makedirs(self.trash_dir)
                    logger.debug("'%s' was created inside '%s'." %
                                 (self.trash_dir, self.wrk_dir))
                except FileExistsError:
                    pass

                logger.debug("File named '%s' was moved to '%s'." %
                             (f, self.trash_dir))
                shutil.move(self.wrk_dir + '/' + f, self.trash_dir + '/' + f)

    # --------------------------------------
    def add_channel(self, chan, run_par=default_run_pars):
        self.channel_set.append([chan, run_par])

    # --------------------------------------
    def show_banner(self):

        message("\n  "+"="*70+"\n |   *** %-4d" % len(self.channel_set) +
                "channel(s) are being considered. ***%s|" % (" "*23) + "\n  " +
                "="*70 + "\n")

        for chan in self.channel_set:

            MG_run_params = chan[1].MG_run_pars

            message("\n  "+"="*70 +
                    "\n | -The number of events: %-8d%s|" %
                    (MG_run_params["nevents"], " "*38))

            message(" |%s|\n | -The model file to be used: %-41s|" %
                    (" "*70, chan[0].model))

            message(" |%s|\n | -The processes are: %s|" %
                    (" "*70, " "*49))
            for proc in chan[0].process:
                message(" |  -- %-60s%s|" % (proc, " "*5))

            message(" |%s|\n |" % (" "*70) +
                    " -The multiprocessing option for python is:" +
                    " %-10s%s|" % (self.process_num != 1, " "*16))
            if self.process_num != 1:
                message(" |\t -The number of cores requested:" +
                        " %-10s%s|" % (self.process_num,  " "*21))

            message(" |%s|\n |" % (" "*70) +
                    " -The multi-threading option for OMP is: " +
                    "%-10s%s|" % ((self.OMP_thrds > 1), " "*19))

            if self.OMP_thrds > 1:
                message(" |\t -The number of threads requested:" +
                        " %-10s%s|" % (self.OMP_thrds, " "*19))

            message(" |%s|\n |" % (" "*70) +
                    " -This parameter set will be iterated over: %s|" %
                    (" "*26))

            for key, val in chan[0].var_param_set.items():
                message(" |  -- %-20s with %-3d values %s|" %
                        (key, len(val), " "*28))

            message(" |%s|\n  " % (" "*70) + "="*70)

    # --------------------------------------
    def show_settings(self, usr_in=None):

        if usr_in == "1":
            self.lhe_reader_step = not (self.lhe_reader_step)
        elif usr_in == "2":
            self.pythia_step = not (self.pythia_step)
        elif usr_in == "3":
            self.cut_report = not (self.cut_report)
        elif usr_in is not None and usr_in.split(" ")[0] == "4":
            if len(usr_in.split(" ")) > 1 and \
             usr_in.split(" ")[1] in {"0", "1", "2"}:
                self.keep_files = usr_in.split(" ")[1]
            else:
                message("\nInvalid input, try again!", {"err": 0})
                logger.warning("'%s' is an invalid input." % usr_in)

        elif usr_in not in {None, ""}:
            message("\nInvalid input, try again!", {"err": 0})
            logger.warning("'%s' is an invalid input." % usr_in)
        elif usr_in == "":
            return 0

        message("\nThe following settings will be used." +
                " To change bool options just type their number.\n" +
                " For others type their number followed by a space" +
                " & the new value.")

        message("", {"top": 70})

        message("1- Export LHE files into txt files: %-10s" %
                (self.lhe_reader_step), {"mid": 70})

        message("2- Running pythia & applying cuts: %-10s" %
                (self.pythia_step), {"mid": 70})
        if self.pythia_step:
            message("3- Saving cut reports: %-10s" %
                    (self.cut_report), {"mid": 70})

        message("4- Keeping files: %-10s" % (self.keep_files), {"mid": 70})
        message(" '0': Only keep the reports for each run.", {"mid": 70})
        message(" '1': Zip files to save space.", {"mid": 70})
        message(" '2': Keep the original files from MadGraph.", {"mid": 70})

        message("", {"bottom": 70})

    # --------------------------------------
    def ask_input(self):

        user_in = input("")
        self.show_settings(user_in)

        if user_in != "":
            self.ask_input()

    # --------------------------------------
    # Keeps track of the number of times each step failed
    def record_failure(self, step_str, param):

        if self.current_run_chan[0] not in self.fail:

            self.fail[self.current_run_chan[0]] = {}

            self.fail[self.current_run_chan[0]][param] = {}

        else:
            if param not in self.fail[self.current_run_chan[0]]:
                self.fail[self.current_run_chan[0]][param] = {}

        tmp_dic = self.fail[self.current_run_chan[0]][param]

        if step_str in tmp_dic:
            tmp_dic[step_str] += 1
        else:
            tmp_dic[step_str] = 1

        logger.error("The failure in step '%s' for parameter '%s'" %
                     (step_str, param) + " was recorded.")
        return tmp_dic[step_str]

    # --------------------------------------
    def param_dir(self, par):

        tmp_out = "%s/%s=%s" % (self.wrk_dir, self.var_name.split(" ")[0], par)
        return tmp_out

    # --------------------------------------
    def write_MG_input(self, par):

        par_dir = self.param_dir(par)

        tmp_chan = self.current_run_chan[0]
        tmp_run = self.current_run_chan[1]

        # Writing the MG input file in "MG_pars_%_%.txt":
        os.chdir(dname)
        create_dir(dname + "/" + self.MG_input_dir)

        for _ in range(int(tmp_run.MG_run_pars["nevents"] /
                           self.seed_ev_chunk)):

            MG_iseed = random.randint(1, 20001)

            # Making sure it's not used before
            if par in self.rnd_seed_set:
                while MG_iseed in self.rnd_seed_set[par]:
                    MG_iseed = random.randint(1, 20001)

            with open(self.MG_input_dir+"/MG_pars_%s_%d_%d.txt" %
                      (self.var_name, par, MG_iseed), "w") as f:
                logger.debug("File 'MG_pars_%s_%d_%d.txt' created." %
                             (self.var_name, par, MG_iseed))
                MG_lines = []

                # Turning automatic_html_opening off
                MG_lines.append("set automatic_html_opening False\n")
                logger.debug("MG 'automatic_html_opening' set to 'False'.")

                # ...................
                # Imports the model file
                MG_lines.append("import %s/%s\n" %
                                (self.model_dir, tmp_chan.model))
                logger.debug("MG model imported: %s/%s." %
                             (self.model_dir, tmp_chan.model))

                # MG_lines.append("define l+ =  e+ mu+ ta+\n")
                # logger.debug("MG command: 'define l+ =  e+ mu+ ta+'.")
                # MG_lines.append("define l- =  e- mu- ta-\n")
                # logger.debug("MG command: 'define l- =  e- mu- ta-'.")

                # ...................
                # Adding the processes
                for proc in tmp_chan.process:
                    MG_lines.append("add process %s\n" % proc)
                    logger.debug("MG process added: %s." % proc)

                # ...................
                # Specifying the output directory
                MG_lines.append("output %s/%s=%s_%d\n" %
                                (par_dir, self.var_name.split(" ")[0], par,
                                 MG_iseed))
                logger.debug("MG command: 'output %s/%s=%s_%d'" %
                             (par_dir, self.var_name.split(" ")[0], par,
                              MG_iseed))

                # ...................
                # Launching followed by an "Enter" key
                MG_lines.append("launch\n\n")
                logger.debug("MG 'launch' command added.")

                # Turning automatic_html_opening to the default value
                # MG_lines.append("set automatic_html_opening default\n")

                # ...................
                # Setting the random seed:
                if par not in self.rnd_seed_set:
                    self.rnd_seed_set[par] = []

                self.rnd_seed_set[par].append(MG_iseed)

                MG_lines.append("set iseed %d\n" % MG_iseed)
                logger.debug("Random seed set to '%d'." % MG_iseed)
                # ...................

                # ...................
                # Setting the run parameters
                for opt, val in tmp_run.MG_run_pars.items():

                    if opt == "nevents":
                        MG_lines.append("set run_card %s %d\n" %
                                        (opt, self.seed_ev_chunk))
                    elif isinstance(val, int):
                        MG_lines.append("set run_card %s %d\n" % (opt, val))
                    elif isinstance(val, float):
                        MG_lines.append("set run_card %s %f\n" % (opt, val))

                    logger.debug("MG run parameter '%s' added." % opt)

                # ...................
                # Setting the fixed model parameters
                for opt, val in tmp_chan.fix_param_set.items():

                    if isinstance(val, int):
                        MG_lines.append("set param_card %s %d\n" % (opt, val))
                    elif isinstance(val, float):
                        MG_lines.append("set param_card %s %.10f\n" %
                                        (opt, val))
                    logger.debug("Fixed model parameter '%s' added." % opt)

                # ...................
                # Setting the dependent model parameter
                for opt, val in tmp_chan.dep_param_set.items():

                    if isinstance(val[par], float):
                        MG_lines.append("set param_card %s %e\n"
                                        % (opt, val[par]))
                    elif isinstance(val[par], int):
                        MG_lines.append("set param_card %s %d\n" %
                                        (opt, val[par]))
                    logger.debug("Dependent parameter '%s' added." % opt)

                # ...................
                # Setting the variable model parameters
                if isinstance(par, float):
                    MG_lines.append("set param_card %s %e\n"
                                    % (self.var_name, par))
                elif isinstance(par, int):
                    MG_lines.append("set param_card %s %d\n" %
                                    (self.var_name, par))
                logger.debug("Variable parameter '%s' added." % self.var_name)
                # ...................
                f.writelines(MG_lines)
                f.close()

    # --------------------------------------
    def run_MG(self, par):

        # tmp_chan = self.current_run_chan[0]
        # tmp_run = self.current_run_chan[1]

        # for tmp1, tmp2 in tmp_chan.var_param_set.items():
        #     run_set = tmp2

        # ...................
        par_dir = self.param_dir(par)

        # create_dir(par_dir)

        lst_of_f = os.listdir(self.wrk_dir)
        for f in lst_of_f:
            if f == ("%s=%s" % (self.var_name.split(" ")[0], par)):
                logger.warning("'%s' directory already exists!" % par_dir)
                self.init_clean_up(f)

        # ...................
        message("\n--> Running MadGraph for parameter '%s'= %-3.2f..."
                % (self.var_name, par), {"color": 'cyan'})

        self.write_MG_input(par)
        # ..............................
        # # Writing the MG input file in "MG_pars_.txt":

        # os.chdir(dname)
        # create_dir(dname + '/_tmp_MG_input')
        # ...................
        # with open("_tmp_MG_input/MG_pars_%s_%d.txt" % (self.var_name, par), "w") as f:
        #     logger.debug("File 'MG_pars_%s_%d.txt' created." %
        #                  (self.var_name, par))
        #     MG_lines = []

        #     # Turning automatic_html_opening off
        #     MG_lines.append("set automatic_html_opening False\n")
        #     logger.debug("MG 'automatic_html_opening' set to 'False'.")

        #     # ...................
        #     # Imports the model file
        #     MG_lines.append("import %s/%s\n" %
        #                     (self.model_dir, tmp_chan.model))
        #     logger.debug("MG model imported: %s/%s." %
        #                  (self.model_dir, tmp_chan.model))

        #     # MG_lines.append("define l+ =  e+ mu+ ta+\n")
        #     # logger.debug("MG command: 'define l+ =  e+ mu+ ta+'.")
        #     # MG_lines.append("define l- =  e- mu- ta-\n")
        #     # logger.debug("MG command: 'define l- =  e- mu- ta-'.")

        #     # ...................
        #     # Adding the processes
        #     for proc in tmp_chan.process:
        #         MG_lines.append("add process %s\n" % proc)
        #         logger.debug("MG process added: %s." % proc)

        #     # ...................
        #     # Specifying the output directory
        #     MG_lines.append("output %s\n" % (par_dir))
        #     logger.debug("MG command: 'output %s'" % (par_dir))

        #     # ...................
        #     # Launching followed by an "Enter" key
        #     MG_lines.append("launch\n\n")
        #     logger.debug("MG 'launch' command started.")

        #     # Turning automatic_html_opening to the default value
        #     # MG_lines.append("set automatic_html_opening default\n")

        #     # ...................
        #     # Setting the random seed:
        #     MG_iseed = random.randint(1, 20001)
        #     # Making sure it's not used before
        #     while MG_iseed in self.rnd_seed_set:
        #         MG_iseed = random.randint(1, 20001)

        #     self.rnd_seed_set.append(MG_iseed)

        #     MG_lines.append("set iseed %d\n" % MG_iseed)
        #     logger.debug("Random seed set to '%d'." % MG_iseed)
        #     # ...................

        #     # ...................
        #     # Setting the run parameters
        #     for opt, val in tmp_run.MG_run_pars.items():

        #         if isinstance(val, int):
        #             MG_lines.append("set run_card %s %d\n" % (opt, val))
        #         if isinstance(val, float):
        #             MG_lines.append("set run_card %s %f\n" % (opt, val))

        #         logger.debug("MG run parameter '%s' added." % opt)

        #     # ...................
        #     # Setting the fixed model parameters
        #     for opt, val in tmp_chan.fix_param_set.items():

        #         if isinstance(val, int):
        #             MG_lines.append("set param_card %s %d\n" % (opt, val))
        #         elif isinstance(val, float):
        #             MG_lines.append("set param_card %s %.10f\n" % (opt, val))
        #         logger.debug("Fixed model parameter '%s' added." % opt)

        #     # ...................
        #     # Setting the dependent model parameter
        #     for opt, val in tmp_chan.dep_param_set.items():

        #         if isinstance(val[par], float):
        #             MG_lines.append("set param_card %s %e\n"
        #                             % (opt, val[par]))
        #         elif isinstance(val[par], int):
        #             MG_lines.append("set param_card %s %d\n" %
        #                             (opt, val[par]))
        #         logger.debug("Dependent parameter '%s' added." % opt)

        #     # ...................
        #     # Setting the variable model parameters
        #     if isinstance(par, float):
        #         MG_lines.append("set param_card %s %e\n"
        #                         % (self.var_name, par))
        #     elif isinstance(par, int):
        #         MG_lines.append("set param_card %s %d\n" %
        #                         (self.var_name, par))
        #     logger.debug("Variable parameter '%s' added." % self.var_name)
        #     # ...................
        #     f.writelines(MG_lines)
        #     f.close()

        # ...................
        for mg_seed in self.rnd_seed_set[par]:

            os.chdir(dname)
            logger.debug("Openning 'MG_pars_%s_%d_%d.txt'." %
                         (self.var_name, par, mg_seed))

            file_obj = open(self.MG_input_dir+"/MG_pars_%s_%d_%d.txt" %
                            (self.var_name, par, mg_seed), 'r')

            os.chdir(self.wrk_dir)

            logger.debug("Running MG with '%s' command." % self.MG_command)

            subprocess.call([self.MG_command], shell=True, stdin=file_obj)

            file_obj.close()

        # for f in os.listdir(self.wrk_dir):
        #     if f == ("%s=%s_%d" % (self.var_name.split(" ")[0], par, mg_seed)):
        #         shutil.move(dname_path.parent + '/' + f,
        #                     par_dir + '/Results')
        logger.debug("Ran MG.")

    # --------------------------------------
    def yield_events(self, LHE_file, par, mg_seed, Num_Ev):

        logger.debug("Running yield_events for '%s= %-3d_%d'." %
                     (self.var_name, par, mg_seed))

        MG_seed_rank = self.rnd_seed_set[par].index(mg_seed)
        # ...................
        par_dir = self.param_dir(par)
        # ...................

        hed = 1
        events_split = []
        inside_event = False
        temp = []
        lhe_header = []
        lhe_current_event = []
        event_counter = 0
        chop_len = Num_Ev / self.OMP_thrds

        if self.OMP_thrds > 1:
            message("\n--> Chopping the LHE file for '%s= %-3d_%d' in %d"
                    % (self.var_name, par, mg_seed, self.OMP_thrds) +
                    " pieces...", {'color': 'cyan'})

        logger.debug("Reading through the LHE file for '%s= %-3d_%d'." %
                     (self.var_name, par, mg_seed))

        for line in LHE_file:

            if '<event>' in line:
                event_counter += 1
                inside_event = True
                hed = 0

            if hed == 1:
                lhe_header.append(line)

            if hed == 0:

                if self.lhe_reader_step and '<mgrwt>' not in line and \
                        '<event>' not in line and \
                        '</LesHouchesEvents>' not in line and len(line) > 75:
                    events_split = line.split()

                    if events_split[1] == '1':  # Final states

                        line_counter = str(event_counter) + line
                        temp.append(line_counter)  # Final states

            if inside_event:
                lhe_current_event.append(line)

                # ..... Reaching end of file ........
                if '</event>' in line and event_counter % chop_len == 0:
                    inside_event = False
                    lhe_current_event = lhe_header + lhe_current_event
                    lhe_current_event.append("</LesHouchesEvents>")

                    if self.OMP_thrds > 1:
                        np.savetxt(self.LHE_dir + '/' + '%d_%d.lhe' %
                                   (par, event_counter/chop_len-1 +
                                    MG_seed_rank*self.OMP_thrds),
                                   lhe_current_event, delimiter=',', fmt='%s')

                        message("\n\t--> Piece number %d ready for '%s= %-3d_%d'."
                                % (event_counter/chop_len +
                                   MG_seed_rank*self.OMP_thrds,
                                   self.var_name, par, mg_seed),
                                {'color': 'cyan'})
                        logger.debug("Saved the LHE file in '%s/%d_%d.lhe'" %
                                     (self.LHE_dir, par,
                                      event_counter/chop_len-1 +
                                      MG_seed_rank*self.OMP_thrds))

                    else:
                        np.savetxt(self.LHE_dir + '/' + '%d_%d.lhe' % (par,
                                   MG_seed_rank),
                                   lhe_current_event, delimiter=',', fmt='%s')
                        logger.debug("Saved the LHE file in '%s/%d_%d.lhe'." %
                                     (self.LHE_dir, par, MG_seed_rank))

                    lhe_current_event = []

        chop_output = 1

        # .. Not enough events!
        if event_counter < Num_Ev:
            message("\nThe LHE file contains only" +
                    "%d event of the total %d" % (event_counter, Num_Ev) +
                    " events.", {"err": 0})
            logger.error("The LHE file contains only" +
                         "%d event of the total %d" % (event_counter, Num_Ev) +
                         " events.")
            chop_output = event_counter

        if self.lhe_reader_step:
            message('\n--> Writing the LHE file for ' +
                    "'%s= %-3d_%d'" % (self.var_name, par, mg_seed) +
                    'in a text file...', {"color": "cyan"})

            temp2 = np.column_stack((np.loadtxt(temp, unpack=True)[0],
                                    np.loadtxt(temp, unpack=True)[1],
                                    np.loadtxt(temp, unpack=True)[7],
                                    np.loadtxt(temp, unpack=True)[8],
                                    np.loadtxt(temp, unpack=True)[9],
                                    np.loadtxt(temp, unpack=True)[10],
                                    np.loadtxt(temp, unpack=True)[13]))

            tmp_dir = (par_dir + "/" + self.var_name.split(" ")[0] +
                       "=" + str(par) + "_" + str(mg_seed))

            event_output_dir = (tmp_dir + '/Results/Events_%d.txt' % par)
            np.savetxt(event_output_dir, temp2, delimiter=',', fmt='%g')

            message("\n\t--> LHE file for '%s=%-d_%d' is written in" %
                    (self.var_name, par, mg_seed) + " '%s'." %
                    event_output_dir,
                    {"color": "cyan"})

        return chop_output

    # --------------------------------------
    def load_lhe_to_pythia(self, lhe_file, par):

        lhe_file_data = gzip.open(lhe_file, 'rb')
        # create_dir(self.LHE_dir)
        lhe_file_out = open("%s/%d.lhe" % (self.LHE_dir, par), 'wb')
        lhe_file_out.write(lhe_file_data.read())
        lhe_file_data.close()
        lhe_file_out.close()

    # --------------------------------------
    # This function opens the lhe file and reads it line-by-line,
    #  then inputs the splitted lhe_file to 'yield_events' function.
    def load_LHE(self, lhe_file, par, mg_seed, Num_Ev):

        message("\n--> Opening the LHE file for '%s= %-3d_%d'..." %
                (self.var_name, par, mg_seed), {"color": 'cyan'})
        logger.debug("Loading the LHE file for '%s= %-3d_%d'." %
                     (self.var_name, par, mg_seed))

        with gzip.open(lhe_file) as f:
            lhe_data = f.read()

        split_lhe_data = lhe_data.decode('utf8').split('\n')
        logger.debug("Extracting the LHE content for '%s= %-3d_%d'." %
                     (self.var_name, par, mg_seed))

        return self.yield_events(split_lhe_data, par, mg_seed, Num_Ev)

    # --------------------------------------
    def run_lhe_reader(self, par):

        tmp_run = self.current_run_chan[1]

        # ...................
        par_dir = self.param_dir(par)
        # ...................
        os.chdir(dname)
        # ...................
        for mg_seed in self.rnd_seed_set[par]:
            try:
                os.remove(self.MG_input_dir+"/MG_pars_%s_%d_%d.txt" %
                          (self.var_name, par, mg_seed))
            except OSError:
                logger.warning("File '%s/MG_pars_%s_%d_%d.txt', doesn't exist! " %
                               (self.MG_input_dir, self.var_name, par, mg_seed))
        os.chdir("..")  # This contains the pythia binary file (./bin/run)

        create_dir(self.LHE_dir)

        # ...................

        for mg_seed in self.rnd_seed_set[par]:

            local_lhe = (par_dir + "/" + self.var_name.split(" ")[0] +
                         "=" + str(par) + "_" + str(mg_seed))

            result_dir = local_lhe + '/Results'
            create_dir(result_dir)

            local_lhe = local_lhe + '/Events/run_01/unweighted_events.lhe.gz'

            chop_stat = 1
            if self.lhe_reader_step or self.OMP_thrds > 1:
                chop_stat = self.load_LHE(local_lhe, par, mg_seed,
                                          self.seed_ev_chunk)
            # tmp_run.MG_run_pars["nevents"])

            elif self.pythia_step:
                load_lhe_to_pythia(local_lhe, par)

            # ... Checking the generated files against the expected number of
            #  events
            if chop_stat != 1:
                    message("\nThe lhe file for '%s= %d_%d'" %
                            (self.var_name, par, mg_seed) +
                            " was corrupted.", {"err": 0})
                    logger.error("The lhe file for '%s= %d_%d'" %
                                 (self.var_name, par, mg_seed) +
                                 " was corrupted.")

                    f_count = self.record_failure("MG", par)

                    if not self.batch_mode:
                        MG_again = input("\n- Do you want to run MadGraph" +
                                         " again? (y/n) ")
                    else:
                        MG_again = 'No!'

                    if(MG_again in yes_set):
                        tmp_dir = (par_dir + "/" + self.var_name.split(" ")[0] +
                                   "=" + str(par) + "_" + str(mg_seed))
                        os.rename(tmp_dir,  tmp_dir + '_corrupted_%d' %
                                  f_count)
                        self.loop_func(par)
                    else:
                        message("\n- Using the %d events generated" %
                                chop_stat +
                                " instead of %d events..." %
                                tmp_run.MG_run_pars["nevents"])
                        logger.warning("'%d' events are being used" %
                                       chop_stat +
                                       " instead of %d." %
                                       tmp_run.MG_run_pars["nevents"])

                        tmp_run.inter_run_pars["new_nevents"] = (chop_stat -
                                                                 chop_stat %
                                            (tmp_run.MG_run_pars["nevents"] /
                                                self.OMP_thrds))
                        self.new_OMP_thrds = (chop_stat /
                                              (tmp_run.MG_run_pars["nevents"] /
                                               self.OMP_thrds))

    # --------------------------------------
    def cleanup_pythia_files(self, par):

        message("\n--> Cleaning up pythia files for  '%s= %d'" %
                (self.var_name, par), {"color": "cyan"})
        logger.debug("Running 'cleanup_pythia_files' for '%s= %d'." %
                     (self.var_name, par))
        # ...................
        par_dir = self.param_dir(par)
        # ...................

        onlyfiles_1 = [f for f in os.listdir(dname_path.parent)
                       if isfile(join(dname_path.parent, f))
                       ]

        onlyfiles_2 = [f for f in os.listdir(self.LHE_dir)
                       if isfile(join(self.LHE_dir[:-1], f))
                       ]

        # patterns for the individual thread files
        mv_name_patt = [r"^Tau_Report_%d_[0-9].txt$" % (par),
                        r"^Tau_Report_%d_[0-9][0-9].txt$" % (par),

                        r"^Event_Report_%d_[0-9].txt$" % (par),
                        r"^Event_Report_%d_[0-9][0-9].txt$" % (par),
                        r"^Event_Report_%d_[0-9][0-9][0-9].txt$" %
                        (par),
                        r"^Event_Report_%d_[0-9][0-9][0-9][0-9].txt$" %
                        (par),
                        r"^Event_Report_%d_[0-9][0-9][0-9][0-9][0-9].txt$"
                        % (par),
                        r"^Event_Report_%d_[0-9][0-9][0-9][0-9][0-9][0-9].\
                            txt$" % (par),
                        r"^Event_Report_%d_[0-9][0-9][0-9][0-9][0-9][0-9]\
                            [0-9].txt$" % (par),

                        r"^Pythia_Cut_Report_%d_[0-9].txt$" % (par),
                        r"^Pythia_Cut_Report_%d_[0-9][0-9].txt$" %
                        (par),

                        r"^Jet_Report_%d_[0-9].txt$" % (par),
                        r"^Jet_Report_%d_[0-9][0-9].txt$" % (par),
                        ]

        # patterns for files common between threads
        mov_2_par_dir = [r"^Report_%d.txt$" % (par),
                         r"^_main_inputs.*_%d.dat$" % (par),
                         r"^_rec_.*_%d.dat$" % (par)]

        # rmv_name_patt = [r"xxx%d_[0-9].lhe$" % (par),
        #                  r"xxx%d_[0-9][0-9].lhe$" % (par)]
        # for pat in rmv_name_patt:
        #     if re.match(pat, f):
        #         os.remove(self.LHE_dir + '/' + f)

        onlyfiles = onlyfiles_1 + onlyfiles_2

        for f in onlyfiles:
            for pat in mv_name_patt:
                if re.match(pat, f):

                    tmp_num = int(f.split("_")[-1].split(".")[0])
                    tmp_num = tmp_num // self.OMP_thrds
                    tmp_num = self.rnd_seed_set[par][tmp_num]

                    shutil.move(dname_path.parent + '/' + f,
                                par_dir + "/" + self.var_name.split(" ")[0] +
                                "=" + str(par) + "_" + str(tmp_num) +
                                "/Results")

            for pat in mov_2_par_dir:
                if re.match(pat, f):
                    shutil.move(dname_path.parent + '/' + f,
                                par_dir)

    # --------------------------------------
    def check_pyth_out(self, par):

        # .... Checking the Results (all events for each par) ....#

        message("\n--> Checking pythia output for  '%s= %d'" %
                (self.var_name, par), {"color": "cyan"})
        logger.debug("Running 'check_pyth_out' for '%s= %d'." %
                     (self.var_name, par))
        # ...................
        par_dir = self.param_dir(par)
        tmp_run = self.current_run_chan[1]
        tmp_chan = self.current_run_chan[0]
        # ...................

        # Finding the final number of events passing the cuts:
        result_txt_link = dname_path.parent + "/_report_%d" % par

        try:
            event_cut = np.loadtxt(result_txt_link, delimiter=',',
                                   usecols=(1, 2, 3), unpack=True)
        except IOError:
            message(result_txt_link + " doesn't exist," +
                    " stopped checking pythia output!", {"err": 0})
            logger.error(result_txt_link + " doesn't exist," +
                         " stopped checking pythia output!")
            return 0
            # return -1

        event_cut = np.transpose(event_cut)

        os.remove(dname_path.parent + "/_report_%d" % par)
        logger.debug("Removing " + dname_path.parent + "/_report_%d." % par)

        eff = event_cut / tmp_run.inter_run_pars["new_nevents"]
        logger.debug("Efficiency calculated from " + dname_path.parent +
                     "/_report_%d." % par)

        Final_Report_Line = []

        ev_err_str = ''
        if tmp_run.inter_run_pars["new_nevents"] != tmp_run.MG_run_pars["nevents"]:
            ev_err_str = '    #From %d events only!' % (tmp_run.inter_run_pars["new_nevents"])

            Final_Report_Line.append(" WARNING: The events generated for " +
                                     "this value were only %d" %
                                     tmp_run.inter_run_pars["new_nevents"] +
                                     " instead of %d.\n" %
                                     tmp_run.MG_run_pars["nevents"])

        # ... Saving the 'Final_Report_Line's to 'Report_(par).txt':
        final_report_file = par_dir + '/Report_%d.txt' % par
        with open(final_report_file, 'a') as rp_file:
            rp_file.writelines(Final_Report_Line)

        # .... Saving the params, and efficiencies of events
        # to a Eff_%s_%d.txt file:

        with open(self.wrk_dir+'/Eff_%s.txt' % tmp_chan.name, 'a+') as g:
            for counter in range(45):
                g.write(str(par)+',' +
                        str(eff[counter, 0])+',' +
                        str(eff[counter, 1])+',' +
                        str(eff[counter, 2])+ev_err_str+'\n')
        g.close()

        return 0

    # --------------------------------------
    def run_pythia(self, par):

        message("\n--> Running pythia for '%s= %-3d'..." %
                (self.var_name, par), {"color": "cyan"})
        logger.debug("Running 'run_pythia' for '%s= %d'." %
                     (self.var_name, par))

        # ...................
        par_dir = self.param_dir(par)
        tmp_chan = self.current_run_chan[0]
        tmp_run = self.current_run_chan[1]
        # ...................

        local_pyth_scr = self.pythia_run_file + "/" + tmp_chan.script
        if not os.path.exists("%s" % local_pyth_scr):
            logger.error("Pythia run file '%s' doesn't exist! " %
                         local_pyth_scr)
            message("Pythia run file '%s' doesn't exist!" %
                    self.pythia_run_file, {"err": 0})
            message("run_pythia aborted!", {"err": 0})
            return

        p = subprocess.Popen(["./%s %s/%d.lhe %d %d %d" %
                             (local_pyth_scr,
                              self.LHE_dir, par,
                              tmp_run.inter_run_pars["new_nevents"],
                              self.new_OMP_thrds*len(self.rnd_seed_set[par]),
                              self.cut_report)],
                             shell=True, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, bufsize=1)

        for line in iter(p.stdout.readline, b''):
            print(line)
            sys.stdout.flush()
        # # .............. Pythia Terminal Output ..................#
        # # The following lines show the output from Pythia:
        # if pythia_stdout:
        #     for line in iter(p.stdout.readline, b''):
        #         ''' This part is for selecting part of the output that is
        #             important to you, e.g. lines with
        #               'Pythia_Terminal_Output'
        #             tag:
        #         '''
        #         if 'Applying cuts' in line:
        #             print('\033[35m'+"(M = %d) " % par +
        #                 line.split('...')[0]+'...')
        #             sys.stdout.flush()
        #     p.wait()
        # # .............. Pythia Terminal Output ..................#

        # Chceking pythia return code
        stdoutdata, stderrdata = p.communicate()
        pyth_status = p.returncode
        if pyth_status:
            # something went wrong
            logger.error("Pythia script '%s'" % local_pyth_scr +
                         " for '%s= %d' failed." % (self.var_name, par))
            message("Pythia script '%s'" % local_pyth_scr +
                    " for '%s= %d' failed." % (self.var_name, par), {"err": 0})
        else:
            # we are ok
            logger.debug("Pythia script '%s'" % local_pyth_scr +
                         " for '%s= %d' ran succesfully!" %
                         (self.var_name, par))
            message("Pythia script '%s'" % local_pyth_scr +
                    " for '%s= %d' ran succesfully!" % (self.var_name, par),
                    {"color": "cyan"})
            pass
        # ............................

        p.stdout.close()

        # # Cleaning up
        self.cleanup_pythia_files(par)

        pythFail_count = 0
        # Checking the results
        if self.check_pyth_out(par) == -1:

            pythFail_count = self.record_failure("Pythia", par)
            message("\nRunning pythia failed for '%s= %d'" %
                    (self.var_name, par), {"err": 0})

            if not self.batch_mode:
                pyth_again = input("\n- Do you want to run Pythia" +
                                   " again? (y/n) ")
            else:
                pyth_again = 'yes'

            os.rename(par_dir + '/Results',  par_dir + '/Results_pythFail_%d' %
                      pythFail_count)

            if pyth_again in yes_set:

                if pythFail_count <= self.pythFail_MAX:

                    result_dir = par_dir + '/Results'
                    create_dir(result_dir)
                    return self.run_pythia(par)
                else:
                    logger.error("Maximum pythia runs for parameter" +
                                 " %s reached." % self.var_name)
                    message("Maximum pythia runs for parameter" +
                            " %s reached." % self.var_name, {"err": 0})
            else:
                pythFail_count = self.pythFail_MAX * 10
                message("\n- Running pythia has failed for '%s= %-3d'." %
                        (self.var_name, par) + " Skipping to the next value.",
                        {"err": 0})
                logger.error("Running pythia has failed for '%s= %-3d'." %
                             (self.var_name, par) +
                             " Skipping to the next value.")

        return pythFail_count

    # --------------------------------------
    def find_xsec(self, par):

        message("\n--> Recording the cross section for '%s= %-3d'..." %
                (self.var_name, par), {"color": "cyan"})
        # ...................
        par_dir = self.param_dir(par)
        # ...................

        for mg_seed in self.rnd_seed_set[par]:

            ev_ban_link = (par_dir + "/" + self.var_name.split(" ")[0] +
                           "=" + str(par) + "_" + str(mg_seed) +
                           '/Events/run_01/run_01_tag_1_banner.txt')

        # ev_ban_link = par_dir + '/Events/run_01/run_01_tag_1_banner.txt'

            ev_ban = open(ev_ban_link, 'r')

            for line in ev_ban:
                if 'Integrated weight' in line:
                    splitted_line_ev_ban = line.split()
                    Integrated_Weight = float(splitted_line_ev_ban[5])

            with open(par_dir + '/XSec.txt', 'a+') as g:
                g.write(str(par) + ',' + str(mg_seed) + ',' +
                        str(Integrated_Weight)+'\n')
        g.close()

    # --------------------------------------
    def cleanup_files(self, par):

        # .... Begin: Removing/Keeping Extra Files .... #
        message("\n--> Cleaning up the files for '%s= %-3d'..." %
                (self.var_name, par), {"color": "cyan"})

        # Removing the MG command input dir
        try:
            logger.debug("Removing %s." % (dname + '/' + self.MG_input_dir))
            shutil.rmtree(dname + '/' + self.MG_input_dir)
        except:
            logger.warning("The directory " + dname + '/' + self.MG_input_dir +
                           " couldn't be removed.")

        # ...................
        par_dir = self.param_dir(par)
        tmp_chan = self.current_run_chan[0]
        # ...................

        # ... Removing the lhe files
        lhe_name_patt = [r"%d_[0-9].lhe$" % (par),
                         r"%d_[0-9][0-9].lhe$" % (par),
                         r"%d_[0-9][0-9][0-9].lhe$" % (par)]

        lhe_dir_files = [f for f in os.listdir(self.LHE_dir)
                         if isfile(join(self.LHE_dir, f))
                         ]

        lhe_dest_dir = (dname_path.parent + '/' + self.lhe_chop_dir + '/' +
                        tmp_chan.name + '/' + "%d" % par)

        for f in lhe_dir_files:
            for pat in lhe_name_patt:
                if re.match(pat, f):
                    try:
                        if self.keep_lhe_chops:
                            # create_dir(par_dir + '/Results/' +
                            #            self.lhe_chop_dir)
                            create_dir(lhe_dest_dir)

                            # shutil.move(dname_path.parent + '/' +
                            #             self.LHE_dir + f,
                            #             par_dir + '/Results/' +
                            #             self.lhe_chop_dir)

                            shutil.move(self.LHE_dir + '/' + f,
                                        lhe_dest_dir)

                            # os.remove(dname_path.parent + '/' +
                            #           self.LHE_dir + '/' + f)
                        else:
                            os.remove(self.LHE_dir + '/' + f)
                    except OSError:
                        logger.warning("Couldn't remove" +
                                       self.LHE_dir + f)

        if self.keep_lhe_chops and self.zip_lhe_chops in ['zip', 'gz']:
            message(" Compressing the LHE files for %s=%-3d..." %
                    (self.var_name, par), {"color": "cyan"})
            logger.debug(" Compressing the LHE files for %s=%-3d..." %
                         (self.var_name, par))

            local_gz_dir = (dname_path.parent + '/' + self.lhe_chop_dir + '/' +
                            tmp_chan.name)

            if self.zip_lhe_chops == 'gz':
                local_gz_dir = (dname_path.parent + '/' + self.lhe_chop_dir +
                                '/' + tmp_chan.name)
                os.chdir(local_gz_dir)
                tar = tarfile.open("%d" % par + ".tar.gz", "w:gz")
                tar.add("%d" % par)
                tar.close()
                os.chdir(dname)
            elif self.zip_lhe_chops == 'zip':
                shutil.make_archive(lhe_dest_dir, 'zip', lhe_dest_dir)

            shutil.rmtree(lhe_dest_dir)
        # ...................

        # # # Making folders
        # os.makedirs(par_dir + '/%s=%d' % (self.var_name, par))

        # for f in os.listdir(par_dir):
        #     if f not in ['Results', '%s=%d' % (self.var_name, par)]:
        #         shutil.move(par_dir + '/' + f, par_dir + '/%s=%d' %
        #                     (self.var_name, par))

        if par in self.current_run_chan[0].keep_files:
            self.tmp_keep_files = '1'

        # ...................................................
        if self.tmp_keep_files == '0':
            message(" Removing the MG files for " + "%s=%-3d..." %
                    (self.var_name, par), {"color": "cyan"})
            logger.debug(" Removing the MG files for " + "%s=%-3d..." %
                         (self.var_name, par))

        if self.tmp_keep_files == '2':
            message(" Keeping the MG files for %s=%-3d..." %
                    (self.var_name, par), {"color": "cyan"})
            logger.debug(" Keeping the MG files for %s=%-3d..." %
                         (self.var_name, par))

        if self.tmp_keep_files == '1':
            message(" Compressing the MG files for %s=%-3d..." %
                    (self.var_name, par), {"color": "cyan"})
            logger.debug(" Compressing the LHE files for %s=%-3d..." %
                         (self.var_name, par))
        # ...................................................

        for mg_seed in self.rnd_seed_set[par]:
            tmp_dir = (par_dir + "/" + self.var_name.split(" ")[0] +
                       "=" + str(par) + "_" + str(mg_seed))

            if self.tmp_keep_files != '0':
                try:
                    os.makedirs(tmp_dir + '/MG')
                except:
                    pass

            for f in os.listdir(tmp_dir):
                if f not in ['Results']:

                    # Removing all the MG files
                    if self.tmp_keep_files == '0':
                        if os.path.isfile(tmp_dir + '/' + f):
                            os.remove(tmp_dir + '/' + f)
                        elif os.path.isdir(tmp_dir + '/' + f):
                            shutil.rmtree(tmp_dir + '/' + f)

                    # Keeping the MG files in '~/MG'
                    else:
                        shutil.move(tmp_dir + '/' + f, tmp_dir + '/MG')

            # Zipping the MG files
            if self.tmp_keep_files == '1':
                shutil.make_archive(tmp_dir + '/MG', 'zip', tmp_dir + '/MG')
                shutil.rmtree(tmp_dir + '/MG')

    # --------------------------------------
    def loop_func(self, par):

        self.new_OMP_thrds = self.OMP_thrds
        self.tmp_keep_files = self.keep_files

        tmp_run = self.current_run_chan[1]
        tmp_run.inter_run_pars["new_nevents"] = tmp_run.MG_run_pars["nevents"]

        self.run_MG(par)
        self.run_lhe_reader(par)

        pythFail_status = 0
        if self.pythia_step:
            pythFail_status = self.run_pythia(par)

        if pythFail_status <= self.pythFail_MAX:
            pass

        # Finding the Cross-section (pb)
        self.find_xsec(par)

        # Removing/Keeping Extra Files
        self.cleanup_files(par)

    # --------------------------------------
    def par_run(self):
        tmp_chan = self.current_run_chan[0]

        for tmp1, tmp2 in tmp_chan.var_param_set.items():
            run_set = tmp2
            self.var_name = tmp1

        chunk_size = math.ceil(len(run_set) / self.process_num)
        loop_process = Pool(maxtasksperchild=50)
        loop_process.imap(self.loop_func, run_set,
                          chunksize=chunk_size)

        loop_process.close()
        loop_process.join()

    # --------------------------------------
    def seq_run(self):
        tmp_chan = self.current_run_chan[0]

        for par_name, run_set in tmp_chan.var_param_set.items():
            self.var_name = par_name
            for x in run_set:
                self.loop_func(x)

    # --------------------------------------
    def organize_files(self, chan):

        folder_name = '%s-%s' % (chan[0].name,
                                 strftime("%Y-%m-%d %H-%M", localtime()))
        final_directory = (self.wrk_dir + '/' + folder_name)

        os.makedirs(final_directory)
        self.run_folder_list.append(folder_name)
        os.chdir(final_directory)

        for f in os.listdir(self.wrk_dir):
            if f.startswith(self.var_name.split(" ")[0]):
                shutil.move(self.wrk_dir + '/' + f, final_directory)

        if self.pythia_step:
            try:
                shutil.move(self.wrk_dir+'/Eff_%s.txt' % chan[0].name,
                            final_directory)
            except IOError:
                message(self.wrk_dir + "/Eff_%s.txt'" % chan[0].name +
                        "  doesn't exist!", {"err": 0})

        # Saving computation time, number of events, and the mass set to
        #  Run_Report.txt:
        Run_Rep_Line = []
        Run_Rep_Line.append("=> Start Time: %s.\n\n" %
                            strftime("%Y-%m-%d %H:%M:%S", self.start_T))
        Run_Rep_Line.append("\n  " + "="*75 +
                            "\n | -The number of events: %-6d %44s|\n |" %
                            (chan[1].MG_run_pars["nevents"], " "))
        Run_Rep_Line.append("%75s|\n | -The model file used: %-51s |\n |" %
                            (" ", chan[0].model))

        Run_Rep_Line.append(" %s|\n |" % (" "*74) +
                            " -The processes were: %53s|\n |" % " ")
        for proc in chan[0].process:
            Run_Rep_Line.append(" -%-50s%s|\n |" % (proc, (" "*23)))

        Run_Rep_Line.append(" %s|\n |" % (" "*74) +
                            " -This parameter set was iterated over: %s|" %
                            (" "*35))
        for key, val in chan[0].var_param_set.items():
            Run_Rep_Line.append("\n |  -- %-20s with %-3d values %33s|" %
                                (key, len(val), " "))

        Run_Rep_Line.append(" %s|\n |" % (" "*74) +
                            " -These random seeds were used: %s|" %
                            (" "*43))

        Run_Rep_Line.append("\n |  -- %-70s|" % self.rnd_seed_set)

        Run_Rep_Line.append("\n |%75s|\n |" % (" ") +
                            " -The computation took " +
                            "{:-4.0f} minutes and {:-2.0f}".format(
                             (time()-self.start_t)/60,
                             (time() - self.start_t) % 60) +
                            " seconds.%24s|\n |%75s|\n  " % (" ", " ") +
                            "="*75)

        Run_Rep_Line.append("\n\n=> End Time: %s." %
                            strftime("%Y-%m-%d %H:%M:%S", localtime()))

        run_report_file = final_directory+'/Run_Report.txt'
        with open(run_report_file, 'w') as run_f:
            run_f.writelines(Run_Rep_Line)

        message("\n  " + "="*90 +
                "\n | Finished: The results are written" +
                " in:\n |\n | ==>  %s\n |" % final_directory)
        message(" |\n |           The whole process took --- " +
                "{:.0f} minutes".format((time() - self.start_t)/60) +
                "{:.0f} seconds! ---\n  ".format((time() - self.start_t) %
                                                 60) + "="*90)

    # --------------------------------------
    def reset_clock(self):

        self.start_t = time()
        self.start_T = localtime()
        logger.debug(" --- clock reset --- ")

    # --------------------------------------
    def reset_seeds(self):
        self.rnd_seed_set = {}
        logger.debug(" --- random seed set reset --- ")

    # --------------------------------------
    def run(self):

        for chan in self.channel_set:
            self.reset_clock()
            self.reset_seeds()
            self.run_count += 1
            self.current_run_chan = chan
            if self.process_num > 1:
                self.par_run()
            else:
                self.seq_run()

            self.organize_files(chan)

        # Removing the lhe_folder
        try:
            message("Removing the lhe folder...", {"color": "cyan"})
            shutil.rmtree(self.LHE_dir)
        except:
            logger.warning("The LHE directory " + self.LHE_dir +
                           " couldn't be removed.")

        run_directory = (self.wrk_dir + '/Run_%s' %
                         (strftime("%Y-%m-%d %H-%M", localtime())))

        os.makedirs(run_directory)
        os.chdir(run_directory)

        run_t_line = []
        run_t_line.append("\n \t ****** Run finished ****** \n")
        run_t_line.append("\n  Run began at : %s." %
                          strftime("%Y-%m-%d %H:%M:%S", self.start_T_global))
        run_t_line.append("\n  it ended at  : %s." %
                          strftime("%Y-%m-%d %H:%M:%S", localtime()))
        run_t_line.append("\n  and it took  : " +
                          "{:.0f}".format((time() - self.start_t_global)/60) +
                          " minutes and " +
                          "{:.0f}".format((time() -
                                          self.start_t_global) % 60) +
                          " seconds.\n  ")
        run_t_line.append("\n \t ************************** ")

        for line in run_t_line:
            message(line)

        run_time_file = run_directory+'/run_time.txt'
        with open(run_time_file, 'w') as run_f:
            run_f.writelines(run_t_line)

        for f in os.listdir(self.wrk_dir):
            if f in self.run_folder_list:
                print(f)
                shutil.move(self.wrk_dir+'/' + f, run_directory)

        logger.debug("Directories moved into %s." % run_directory)
        logger.debug(" *** Run finished *** ")

        try:
            shutil.move(dname + '/' + "pheno.log", run_directory + '/' +
                        "pheno.log")
        except:
            pass


# =================================================================
if __name__ == "__main__":

    # my_dir = "/home/zaki/wrk_dir/LFV/Code/wrk_dir"
    p1 = Pheno(wrk_dir)

    my_chan_iter_set = [10]

    for i in my_chan_iter_set:
        p1.add_channel(channels_set[i])

    p1.show_banner()
    p1.show_settings()
    p1.ask_input()
    p1.run()

# -------------------------------
