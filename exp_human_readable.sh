#! /bin/bash

# constants
RESULTS_FILE=./results.txt
SCH_BIN=./scheduling
TIME_BIN=/usr/bin/time
TIME_FLAGS="--format=%e --append --output=$RESULTS_FILE"
MAX_TASK_TIME=100

:> $RESULTS_FILE

for CPU_A in 10 #100
do
	for CPU_B in 10 #100
	do
		for GROUP_COUNT in 2 #3 4 16
		do
			for TASK_COUNT in 10 #20 100 #1000
			do
				for SEED in {0..29}
				do
					echo "[ a=$CPU_A b=$CPU_B g=$GROUP_COUNT n=$TASK_COUNT s=$SEED t=$MAX_TASK_TIME ]" >> $RESULTS_FILE
					printf "%-16s%-10s%-10stime(s)\n" "method_name" "sum_l" "lambda" >> $RESULTS_FILE
					for TYPE in 0 1 2
					do
						$TIME_BIN $TIME_FLAGS $SCH_BIN -a $CPU_A -b $CPU_B -e $TYPE -g $GROUP_COUNT -n $TASK_COUNT -s $SEED -t $MAX_TASK_TIME >> $RESULTS_FILE
					done
				done
			done
		done
	done
done

#$SCH_BIN -a 2 -b 2 -e 0 -g 2 -n 2 -s 0 -t 100 -v 1; echo
#$SCH_BIN -a 2 -b 2 -e 1 -g 2 -n 2 -s 0 -t 100 -v 1; echo
#$SCH_BIN -a 2 -b 2 -e 2 -g 2 -n 10 -s 0 -t 100 -v 1; echo
