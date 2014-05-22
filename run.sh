#!/bin/bash

# runner for CPUBENCH
# Runs cpubench for each value of N three times, takes the best

#for i in $(seq 0 10 10000000)
for i in $(seq 10000 10 10000000)
do
    MAX_MFLOPS=0.0
    #for j in 1 2 3
    for j in 1
    do
      #echo "Run $j"
      out=$(./cpubench $i)

      N=$(echo $out | cut -f1 -d" ")
      MFLOPS=$(echo $out | cut -f2 -d" ")

      #echo "N: $N, MFLOPS: $MFLOPS"

      result=$(awk -v mflops=$MFLOPS -v max_mflops=$MAX_MFLOPS 'BEGIN{print (mflops>max_mflops)?1:0}')
      #echo $result

      if [ "$result" -eq 1 ]; then
          MAX_MFLOPS=$MFLOPS
      fi
    done

    echo "N=$N, MFLOPS=$MAX_MFLOPS"

done
