/* io.c */

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


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <getopt.h>
#include "io.h"

int log_level = 0;

int print_log(int level, const char *format, ...)
{
	int return_value = 0;
	va_list args;
	va_start(args, format);
	if (log_level >= level)
		return_value = vfprintf(stderr, format, args);
	va_end(args);
	return return_value;
}

struct cli_args parse_args(int argc, char **argv)
{
	int opt;
	struct cli_args parsed_args;
	while((opt = getopt(argc, argv, "a:b:e:g:n:s:t:v:")) != -1) {
		switch(opt) {
		case 'a':
			parsed_args.cpu_count_a = atoi(optarg);
			break;
		case 'b':
			parsed_args.cpu_count_b = atoi(optarg);
			break;
		case 'e':
			parsed_args.experience_type = atoi(optarg);
			break;
		case 'g':
			parsed_args.group_count = atoi(optarg);
			break;
		case 'n':
			parsed_args.task_count = atoi(optarg);
			break;
		case 's':
			parsed_args.seed = atoi(optarg);
			break;
		case 't':
			parsed_args.max_task_time = atoi(optarg);
			break;
		case 'v':
			log_level = atoi(optarg);
			break;
		default:
			fprintf(stderr, "Usage: %s -a cpu_count_a -b cpu_count_b -e experience_type -g group_count -n task_count -s seed -t max_task_time -v log_level\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	return parsed_args;
}
