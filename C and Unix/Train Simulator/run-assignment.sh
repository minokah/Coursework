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
c8a132b88f39e8e3058c9680d3d2c2eb  Makefile' || exit 1;

echo
echo "Building environment"
echo "----------------------------"
make || exit 1;

echo
echo "Assignment ${ASSIGNMENT}"
echo "----------------------------"
sleep 3
time ./assignment-${ASSIGNMENT}
sleep 3
time ./assignment-${ASSIGNMENT}

echo
echo "Cleaning environment"
echo "----------------------------"
make clean

echo
echo "ASSIGNMENT ${ASSIGNMENT} COMPLETED - $(date)"
