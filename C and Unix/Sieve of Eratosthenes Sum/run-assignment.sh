#!/bin/bash
#
# Run this script with the following command (N is the assignment #):
#   script -c 'run-assignment.sh N' assignment-N.out
#
ASSIGNMENT=${1}
clear

echo
echo "ASSIGNMENT ${ASSIGNMENT} STARTED - $(date)"

echo
echo "Cleaning environment"
echo "----------------------------"
make clean

echo
echo "Checking environment"
echo "----------------------------"
md5sum ${0}
md5sum assignment-${ASSIGNMENT}.c || exit 1;
md5sum -c <<<'
1857d3ace53049fabbf4de5b9fe79c84  Makefile' || exit 1;

echo
echo "Building environment"
echo "----------------------------"
make || exit 1;

echo
echo "Assignment ${ASSIGNMENT}"
echo "----------------------------"
sleep 3
./assignment-${ASSIGNMENT}
./assignment-${ASSIGNMENT} 10
./assignment-${ASSIGNMENT} 10 20 30
sleep 3

time ./assignment-${ASSIGNMENT} 1 100
sleep 3

time ./assignment-${ASSIGNMENT} 5 100
sleep 3

time ./assignment-${ASSIGNMENT} 1 1000
sleep 3

time ./assignment-${ASSIGNMENT} 50 1000
sleep 3

time ./assignment-${ASSIGNMENT} 1 1000000
sleep 3

time ./assignment-${ASSIGNMENT} 50 1000000
sleep 3

echo
echo "Cleaning environment"
echo "----------------------------"
make clean

echo
echo "ASSIGNMENT ${ASSIGNMENT} COMPLETED - $(date)"
