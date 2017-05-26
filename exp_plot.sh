#! /bin/bash

# exp_plot.sh

###############################################################################
# Copyright (C) 2017 Alexis Janon
# 
# This code is part of a program computing task lists for
# heterogeneous scheduling problems.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version. 
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
###############################################################################

# constants
RESULTS_FILE=./results_plot.txt
SCH_BIN=./scheduling
TIME_BIN=/usr/bin/time
TIME_FLAGS="--format=%e --append --output=$RESULTS_FILE"
MAX_TASK_TIME=100

:> $RESULTS_FILE

printf "%-6s%-6s%-8s%-12s%-8s%-6s%-6s%-6s%-8sTIME\n" "CPU_A" "CPU_B" "GROUP" "TASK_COUNT" "T_TIME" "SEED" "TYPE" "SUM_L" "LAMBDA" >> $RESULTS_FILE
for CPU_A in 10 100
do
	for CPU_B in 10 100
	do
		for GROUP_COUNT in 2 #3 4 16
		do
			for TASK_COUNT in 1000 #10 20 100 1000
			do
				for SEED in {0..29}
				do
					for TYPE in 0 1 2
					do
						$TIME_BIN $TIME_FLAGS $SCH_BIN -a $CPU_A -b $CPU_B -e $TYPE -g $GROUP_COUNT -n $TASK_COUNT -s $SEED -t $MAX_TASK_TIME >> $RESULTS_FILE
					done
				done
			done
		done
	done
done
