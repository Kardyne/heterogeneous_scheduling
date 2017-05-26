/* scheduling.c */

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
#include <glpk.h>
#include <math.h>
#include "io.h"
#include "glpk.h"

int main(int argc, char **argv)
{
	struct cli_args parsed_args = parse_args(argc, argv);
	int group_count = parsed_args.group_count;
	int task_count = parsed_args.task_count;
	int array_size = 2*group_count+10*task_count+1;
	glp_prob *lp = glp_create_prob();
	int *ia = calloc(array_size, sizeof(int));
	int *ja = calloc(array_size, sizeof(int));
	double *ar = calloc(array_size, sizeof(double));
	struct task *tasks = calloc(task_count+1, sizeof(struct task));
	srand48(parsed_args.seed);
	int upper_bound = generate_values(tasks, parsed_args), lower_bound = 0;
	print_log(2, "Array size:%d\n", array_size);
	print_log(2, "Line count:%d\n", 2*(group_count+task_count+1));
	print_log(2, "Lambda upper bound: %d\n", upper_bound);
	int lambda = dichotomy(lp, ia, ja, ar, tasks, parsed_args, lower_bound, upper_bound);
	if (log_level >= 1)
		glp_write_lp(lp, NULL, "scheduling.lp");
	for (int i = 1; i<task_count+1; i++) {
		print_log(2, "%s=%d\n", 
			glp_get_col_name(lp, i+2*group_count), 
			(int) round(glp_get_col_prim(lp, i+2*group_count)));
	}
	for (int i = 1; i<2*group_count+1; i++) {
		print_log(2, "%s=%d\n", 
			glp_get_col_name(lp, i), 
			(int) round(glp_get_col_prim(lp, i)));
	}
	char method_name[17], output[21];
	if (lambda == -1) {
		snprintf(output, 21, "NO SOLUTION FOUND");
	} else {
		snprintf(output, 21, "%-10d%-10d", 
			(int) round(glp_get_obj_val(lp)), lambda);
	}
	switch(parsed_args.experience_type) {
	case 0:
		snprintf(method_name, 17, "opt_l");
		break;
	case 1:
		snprintf(method_name, 17, "random");
		break;
	case 2:
		snprintf(method_name, 17, "min_work");
		break;
	}
	print_log(1, "%-16s%-20s", method_name, output);
	if (log_level == 0)
		printf("%-6d%-6d%-8d%-12d%-8d%-6d%-6d%-6d%-8d",
			parsed_args.cpu_count_a,
			parsed_args.cpu_count_b,
			parsed_args.group_count,
			parsed_args.task_count,
			parsed_args.max_task_time,
			parsed_args.seed,
			parsed_args.experience_type,
			(int) round(glp_get_obj_val(lp)),
			lambda);
	glp_delete_prob(lp);
	glp_free_env();
	free(ia);
	free(ja);
	free(ar);
	free(tasks);
	return EXIT_SUCCESS;
}
