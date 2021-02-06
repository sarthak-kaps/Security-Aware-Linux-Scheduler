#!/bin/bash

export NCORES=`cat /proc/cpuinfo | grep -c processor`
echo "Number of cores is $NCORES"
export MAXCORE=`expr $NCORES - 1`

sudo modprobe msr
# Enable all counters in IA32_PERF_GLOBAL_CTRL
#   bits 34:32 enabled the three fixed function counters
#	bits 7:0 enable the four programmable counters
echo "Checking IA32_PERF_GLOBAL_CTRL on all cores"
echo "  (should be 00000007000000ff)"
for core in `seq 0 $MAXCORE`
do
	echo -n "$core "
	sudo rdmsr -p $core -x -0 0x38f
	# sudo wrmsr -p $core 0x38f 0x00000007000000ff
done

# Core Performance Counter Event Select MSRs
#   Counter	 MSR
#	   0    0x186
#	   1    0x187
#	   2    0x188
#	   3    0x189
#	   4    0x18a
#	   5    0x18b
#	   6    0x18c
#	   7    0x18d

# Dump all performance counter event select registers on all cores
if [ 0 == 1 ]
then
	echo "Printing out all performance counter event select registers"
	echo "MSR    CORE    CurrentValue"
	for PMC_MSR in 186 187 188 189
	do
		for CORE in `seq 0 $MAXCORE`
		do
			echo -n "$PMC_MSR $CORE "
			sudo rdmsr -p $core -0 -x 0x${PMC_MSR}
		 done
	done
fi

# Counter 0 Uops Dispatched on Port 0		0x004301a1
# Counter 1 Uops Dispatched on Port 1		0x004302a1
# Counter 2 Uops Dispatched on Port 2		0x004304a1
# Counter 3 Uops Dispatched on Port 3		0x004308a1
# Counter 4 actual core cycles unhalted		0x0043003c
# Counter 5 Uops Dispatched on Port 5		0x004320a1
# Counter 6 cycles with no uops delivered from back end to
#   front end & there is no back end stall	0x0143019c
# Counter 7 Uops issued from RAT to RS		0x0043010e

echo "Programming counters 0,1,2,3"
for core in `seq 0 $MAXCORE`
do
    sudo wrmsr -p $core 0x186 0x004108d1
	sudo wrmsr -p $core 0x187 0x004108d1
	sudo wrmsr -p $core 0x188 0x004108d1
	sudo wrmsr -p $core 0x189 0x004108d1
done