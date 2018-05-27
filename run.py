from glob import glob
import os

listruns = glob('run_deltaJ*.run')

for run in listruns:
    os.system('sbatch ' + run)
