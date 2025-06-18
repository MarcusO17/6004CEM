#!/bin/bash

mpicc  -o t1e t1e.c

read -p "Enter the number of processes: " n

mpirun -np $n ./t1e


