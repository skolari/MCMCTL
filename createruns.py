# import datetime
#now = datetime.datetime.now().strftime('%Y-%m-%d_%Hh')


J1 = 7.0
J2 = J1 / 100.0
T_end = 20 * J2
deltaJ = [0.1, 0.5]
deltaJ = [round(x * J2, 3) for x in deltaJ]

config = '''Deg=4
outputPath=./Outputfiles/
N_simul=150
N_thermal=10000
N_algo=10000
N_temp=10
N_measure=1
Dipolar=false
J1={1}
J2={2}
J3=0.0
J5=0.0
delta_J={0}
T_start=0.1
T_end={3}
'''

contents = '''#!/bin/bash
#SBATCH --output="MCMCTL.out"
#SBATCH --error="MCMCTL.err"
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 28
#SBATCH --mem 50GB

#SBATCH --mail-type ALL
#SBATCH --mail-user manuel.stathis@epfl.ch
#SBATCH --time 3:00:00

#date
today=`date +%Y-%m-%d-%H-%M`;

#working directory
SCRGD="/scratch/$USER/Runs_MCMCTL_$today/deltaJ_{0}"
SCR="/scratch/$USER/Runs_MCMCTL_$today/deltaJ_{0}/Outputfiles"

#home directory
HD="/home/$USER/MCMCTL"
HDR="/home/$USER/MCMCTL/Outputfiles_$today/deltaJ_{0}"
# compile code
cd $HD
make

#create the scratch directories to work
mkdir $SCRGD
mkdir $SCR

#copy data from HD to SCR
cp $HD/MCMCIsingTri $SCRGD
cp $HD/configuration.in $SCRGD

#go to scratch and run code
cd $SCRGD

./MCMCIsingTri configuration_{0}.in > $SCR/file.out

#create the results directory in home
mkdir $HDR

#copy the results to the results directory
cp $SCR/*.xml $HDR
cp $SCR/*.dat $HDR
cp $SCRGD/*.out $HDR
cp $SCR/*.out $HDR
cp $SCRGD/*.err $HDR
#remove the files
cd $SCRGD
rm -r $SCR
rm -r MCMCIsingTri
'''

for dJ in deltaJ:
    with open("run_deltaJ_{0}.run".format(dJ), 'w') as f:
        f.write(contents.format(dJ))
    with open("configuration_deltaJ_{0}.in".format(dJ), 'w') as ff:
        ff.write(config.format(dJ, J1, J2, T_end))
