#!/bin/bash

rm -rf $1/scorep_measurements

scorep mpicc -fopenmp $1/main.c -o $1/mainScorep
scalasca --analyze -e $1/scorep_measurements mpirun -n $2 $1/mainScorep $3 $4 $5
cube_calltree -m time -p $1/scorep_measurements/profile.cubex
cube_dump -m time -c all $1/scorep_measurements/profile.cubex > $1/metric_per_proc.txt

cube_info -l $1/scorep_measurements/profile.cubex
cube_dump -m max_time -c all $1/scorep_measurements/profile.cubex
#otf2-print -A --show-all $1/scorep_measurements/traces.otf2 > $1/trace.txt
