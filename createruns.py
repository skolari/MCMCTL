import datetime
now = datetime.datetime.now().strftime('%Y-%m-%d_%Hh')

contents = '''#!/bin/bash
#SBATCH --output="MCMCTL.out"
#SBATCH --error="MCMCTL.err"
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 28
#SBATCH --mem 50GB
#SBATCH --account=fsl

#SBATCH --mail-type ALL
#SBATCH --mail-user manuel.stathis@epfl.ch
#SBATCH --time 12:00:00

#working directory
SCRGD="/scratch/$USER/Runs_{1}_MCMCTL"
SCR="/scratch/$USER/Runs_{1}_MCMCTL{0}"

#home directory
HD="/home/$USER/2018-Ising_triangular/"
HDR="/home/$USER/2018-Ising_triangular/"

#create the scratch directories to work
mkdir $SCRGD
mkdir $SCR
#copy data from HD to SCR
cp $HD/*.py $SCR

#go to scratch and run code
cd $SCR

module load gcc
module load python/3.6.1

python3 KagomeRunBasis_2dot7dot2.py --L {0} --J1 1 --J2 0 --J3 0 --J4 0.0539 --nst 1000 --nsm 500 --nips 50 --nb 20 --nt_list 28 29 29 --t_list 0.0005 0.05 0.15 0.5 --output Kag_J1-J4_L{0} --nmaxiter 10 --randominit False --same False

#create the results directory in home
mkdir $HDR
#copy the results to the results directory
cp $SCR/*.pkl $HDR
cp $SCR/*.out $HDR
cp $SCR/*.err $HDR
#remove the files
cd $SCRGD
rm -r $SCR
'''

sizes = [4, 8, 12, 16]
for L in sizes:
    with open("DipolarToJ4_L{0}.run".format(L), 'w') as f:
            f.write(contents.format(L, now))
