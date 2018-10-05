#----------------------------------------------------------
# prova_modulation.py
#----------------------------------------------------------

import numpy as np
import time

import nest

######################################################################################################
######################################################################################################
######################################################################################################


dt = 0.1
THREADS = 4
SEED = 1
STIME = 5*100/dt

##################################################################################################
##################################################################################################
##################################################################################################

# initialize nest kernel
#nest.ResetKernel()
# set nest number of processes and integration step
nest.SetKernelStatus({
    "local_num_threads" : THREADS, 
    "resolution" : dt})
    
##################################################################################################
## RANDOM GENERATORS #############################################################################
################################################################################################## 

msd = SEED    # master seed number
N_vp = nest.GetKernelStatus(['total_num_virtual_procs'])[0]    # number of nest processes

# initialize pylab random  generators
pyrngs = [np.random.RandomState(s) for s in range(msd, msd+N_vp)]   
# initialize nest master random generator
nest.SetKernelStatus({"grng_seed" : msd+N_vp})
# initialize nest thread random generators
nest.SetKernelStatus({"rng_seeds" : range(msd+N_vp+1, msd+2*N_vp+1)})

######################################################################################################
## UTILS #############################################################################################
######################################################################################################

def bernoulli_distribution(x, RO=0.1) :
    return x*(np.random.random(x.shape)<RO)

######################################################################################################
######################################################################################################
######################################################################################################

# install th module 
nest.Install("modmodule")

NEURONS_PRE_N = 5
NEURONS_POST_N = 5
NEURONS_MOD_N = 1

# create a poisson generator
POISSON_GENERATOR = nest.Create("poisson_generator", 100)
nest.SetStatus(POISSON_GENERATOR, [{'rate'  : 10000.0   }])
nest.SetStatus(POISSON_GENERATOR, [{'origin': 0.0    }])
nest.SetStatus(POISSON_GENERATOR, [{'start' : 0.0 }])
nest.SetStatus(POISSON_GENERATOR, [{'stop'  : 4900.0 }])
    
# create three sets of neurons 
NEURONS_PRE = nest.Create("iaf_psc_exp",NEURONS_PRE_N)
NEURONS_POST = nest.Create("iaf_psc_exp",NEURONS_POST_N)
NEURONS_MOD = nest.Create("iaf_psc_exp",NEURONS_MOD_N)

# create the volume transmitter to be attached to the connection
VOL = nest.Create("volume_transmitter")
nest.SetStatus(VOL,"deliver_interval", 300)
::::
# create array of weights
w_array = 1.0*np.ones([NEURONS_POST_N, NEURONS_PRE_N])
# sparsify
w_array = bernoulli_distribution(w_array, RO=0.1)

# create the modulatory connection 
conn_dict = {"rule": "all_to_all"}
syn_dict = {"model": "d1_synapse", "vt": VOL[0], "alpha": 3.0, "max_modulation": NEURONS_MOD_N  }
nest.Connect(NEURONS_PRE,NEURONS_POST, conn_spec=conn_dict, syn_spec=syn_dict)

# connect poisson and neurons
nest.Connect(POISSON_GENERATOR, NEURONS_MOD)
nest.Connect(POISSON_GENERATOR, NEURONS_PRE)
nest.Connect(NEURONS_PRE, VOL)

nest.Simulate(STIME)
