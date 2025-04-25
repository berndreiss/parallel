#!/bin/bash

rm -rf $1/scorep_measurements

scorep-gcc -fopenmp $1/main.c -o $1/main
scan -t -e $1/scorep_measurements $1/main
cube_calltree -m time -p $1/scorep_measurements/profile.cubex
cube_dump -m time -c all $1/scorep_measurements/profile.cubex > $1/metric_per_proc.txt

cube_info -l $1/scorep_measurements/profile.cubex
cube_dump -m max_time -c all $1/scorep_measurements/profile.cubex
otf2-print -A --show-all $1/scorep_measurements/traces.otf2 > $1/trace.txt
