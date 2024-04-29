#!/bin/bash

# Angel Badillo, Chad Callender
# Parallel (16, 1024) Frontera script

#SBATCH -J myparalleljob_16_1024           # Job name
#SBATCH -o myparalleljob_16_1024.o%j       # Name of stdout output file
#SBATCH -e myparalleljob_16_1024.e%j       # Name of stderr error file
#SBATCH -p development     # Queue (partition) name
#SBATCH -N 1               # Total # of nodes (1 for OpenMP or Threads)
#SBATCH -n 1               # Total # of mpi tasks (1 for OpenMP or Threads)
#SBATCH -t 00:2:00         # Run time (hh:mm:ss)
#SBATCH --mail-type=all    # Send email at begin and end of job
#SBATCH -A ASC23018       # Project/Allocation name (req'd if you have more than 1)
#SBATCH --mail-user=username@tacc.utexas.edu

# Any other commands must follow all #SBATCH directives...
module list
pwd
date

# Compile parallel code
g++ BadilloCallender_T16_1024.cpp -lpthread -o parallel_16_1024

# Set thread count
export OMP_NUM_THREADS=16

# Launch C++ multithreaded code
./parallel_16_1024

# # Launch parallel code...
# # loop runs code three times
# for value in {1..3}
# do
#     ibrun ./parallel_16_1024       # Use ibrun instead of mpirun or mpiexec
#     echo ''              # newline
# done
