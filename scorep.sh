#!/bin/bash

set -euo pipefail
echo "Running in directory: $1"
ls -la "$1"

scorep-gcc -fopenmp $1/main.c -o $1/main
scorep -t -e $1/scorep_measurements $1/main
cube_calltree -m time -p $1/scorep_measurements/profile.cubex
cube_dump -m time -c all $1/scorep_measurements/profile.cubex > $1/metric_per_proc.txt

python $PERFORMANCE/wf_profiling/visualization.py --data_file $1/metric_per_proc.txt --output_file $1/metric_per_proc.html
cube_info -l $1/scorep_measurements/profile.cubex
cube_dump -m max_time -c all $1/scorep_measurements/profile.cubex
otf2-print -A --show-all $1/scorep_measurements/traces.otf2 > $1/trace.txt
python $PERFORMANCE/wf_tracing/visualization.py --results_directory $1 --output_file $1/trace.html
