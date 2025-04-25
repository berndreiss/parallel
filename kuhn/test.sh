#!/bin/bash 

make clean
rm -rf results
mkdir results
make

for arg in "$@"; do
  mpirun -n 1 main_serial 1 0 $arg > results/$arg.serial.out
  mpirun -n 2 main 1 0 $arg > results/$arg.2.out
  mpirun -n 4 main 1 0 $arg > results/$arg.4.out
  mpirun -n 8 main 1 0 $arg > results/$arg.8.out
  mpirun -n 16 main 1 0 $arg > results/$arg.16.out
done

echo "TIME , serial, 2, 4, 8, 16" > results/results.out
for arg in "$@"; do
  SERIAL=$(cat results/$arg.serial.out)
  TWO=$(cat results/$arg.2.out)
  FOUR=$(cat results/$arg.4.out)
  EIGHT=$(cat results/$arg.8.out)
  SIXTEEN=$(cat results/$arg.16.out)
  echo "$arg, $SERIAL, $TWO, $FOUR, $EIGHT, $SIXTEEN" >> results/results.out
done

echo "SPEEDUP , serial, 2, 4, 8, 16" >> results/results.out
for arg in "$@"; do
  SERIAL=$(cat results/$arg.serial.out)
  TWO=$(cat results/$arg.2.out)
  FOUR=$(cat results/$arg.4.out)
  EIGHT=$(cat results/$arg.8.out)
  SIXTEEN=$(cat results/$arg.16.out)

  RESULT_TWO=$(awk -v x="$SERIAL" -v y="$TWO" 'BEGIN { print x / y }')
  RESULT_FOUR=$(awk -v x="$SERIAL" -v y="$FOUR" 'BEGIN { print x / y }')
  RESULT_EIGHT=$(awk -v x="$SERIAL" -v y="$EIGHT" 'BEGIN { print x / y }')
  RESULT_SIXTEEN=$(awk -v x="$SERIAL" -v y="$SIXTEEN" 'BEGIN { print x / y }')
  
  echo "$arg, 1, $RESULT_TWO, $RESULT_FOUR, $RESULT_EIGHT, $RESULT_SIXTEEN" >> results/results.out
done

