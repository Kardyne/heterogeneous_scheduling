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
