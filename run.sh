#!/bin/bash

# runner for CPUBENCH
# Runs cpubench for each value of N three times, takes the best


for i in {100..10000}
do
    MAX_MFLOPS=0.0
    for j in 1 2 3
    do
      out=$(./cpubench $i)

      N=$(echo $out | cut -f1 -d" ")
      MFLOPS=$(echo $out | cut -f2 -d" ")

      echo "N: $N, MFLOPS: $MFLOPS"
      #if [ $(bc <<< "$MFLOPS > $MAX_MFLOPS") -eq 1 ]

      #res=$(awk '{ if ("'$MFLOPS'">"'$MAX_MFLOPS'") print "0"; else print "1"}')
      #res=$(awk -v mflops=$MFLOPS -v max_mflops=$MAX_MFLOPS 'BEGIN{print mflops>max_mflops?1:0}')
      res=$(awk -v mflops=$MFLOPS -v max_mflops=$MAX_MFLOPS 'BEGIN{print mflops<max_mflops?1:0}')
      echo $res

      if [ "$res" -eq 1 ]; then
          MAX_MFLOPS=$MFLOPS
      fi
    done

    echo "N=$N, MFLOPS=$MAX_MFLOPS"

done
