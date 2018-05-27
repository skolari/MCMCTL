import datetime
today = datetime.datetime.now().strftime('%Y-%m-%d_%Hh')


J1 = 7.0
J2 = J1 / 100.0
T_end = 20 * J2
deltaJ = [0.1, 0.5]
deltaJ = [round(x * J2, 3) for x in deltaJ]

config = '''Deg=4
outputPath=./Outputfiles/
N_simul=150
N_thermal=1000
N_algo=100
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
#SBATCH --output="MCMCTL_{0}.out"
#SBATCH --error="MCMCTL_{0}.err"
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
SCRPRE="/scratch/$USER/Runs_MCMCTL_{1}"
SCRGD="/scratch/$USER/Runs_MCMCTL_{1}/deltaJ_{0}"
SCR="/scratch/$USER/Runs_MCMCTL_{1}/deltaJ_{0}/Outputfiles"

#home directory
HDPRE="/home/$USER/MCMCTL/Outputfiles_{1}"
HD="/home/$USER/MCMCTL"
HDR="/home/$USER/MCMCTL/Outputfiles_{1}/deltaJ_{0}"
# compile code
cd $HD
make

#create the scratch directories to work
mkdir $SCRPRE
mkdir $SCRGD
mkdir $SCR

#copy data from HD to SCR
cp $HD/MCMCIsingTri $SCRGD
cp $HD/configuration.in $SCRGD

#go to scratch and run code
cd $SCRGD

./MCMCIsingTri configuration_{0}.in

#create the results directory in home
mkdir $HDRPRE
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
    dJ_str = str(dJ)
    dJ_str = dJ_str.replace(".", "")
    with open("run_deltaJ_{0}.run".format(dJ_str), 'w') as f:
        f.write(contents.format(dJ_str, today))
    with open("configuration_deltaJ_{0}.in".format(dJ_str), 'w') as ff:
        ff.write(config.format(dJ, J1, J2, T_end))
