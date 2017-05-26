/* io.h */

/******************************************************************************
* Copyright (C) 2017 Alexis Janon
* 
* This code is part of a program computing task lists for
* heterogeneous scheduling problems.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version. 
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/


#ifndef __IO_H__
#define __IO_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <getopt.h>

extern int log_level;

extern int print_log(int level, const char *format, ...);

struct cli_args {
	int cpu_count_a;
	int cpu_count_b;
	int experience_type;
	int group_count;
	int task_count;
	int seed;
	int max_task_time;
};

extern struct cli_args parse_args(int argc, char **argv);

#endif
