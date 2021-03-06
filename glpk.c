/* glpk.c */

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


#include <stdlib.h>
#include <stdbool.h>
#include <glpk.h>
#include <math.h>
#include "io.h"
#include "glpk.h"

#define RANDOM_TYPE 1
#define MIN_WORK_TYPE 2

int rand_int(int min, int max)
{
	return lrand48()%(max-min)+min;
}

int generate_values(struct task *tasks, struct cli_args parsed_args)
{
	int group;
	int lambda = 0;
	for (int i = 1; i<parsed_args.task_count+1; i++) {
		struct task *current = &tasks[i];
		current->p_a = rand_int(1, parsed_args.max_task_time+1);
		current->p_b = rand_int(1, parsed_args.max_task_time+1);
		current->q_a = rand_int(1, parsed_args.cpu_count_a+1);
		current->q_b = rand_int(1, parsed_args.cpu_count_b+1);
		group = rand_int(1, parsed_args.group_count+1);
		current->g = group;
		if (current->p_a > current->p_b) {
			lambda += current->p_a;
		} else {
			lambda += current->p_b;
		}
		print_log(2, "[%d]\t{%d}\t(a) %d, %d\t(b) %d, %d\n", i, current->g, 
				current->p_a, current->q_a, current->p_b, current->q_b);
	}
	return 2*lambda;
}

void fill_problem(glp_prob *lp, int ia[], int ja[], double ar[], struct task tasks[],
int lambda, struct cli_args parsed_args)
{
	int group_count = parsed_args.group_count;
	int task_count = parsed_args.task_count;
	double *sum_group_b = calloc(group_count+1, sizeof(double));
	double sum_total_b = 0.0;
	glp_add_cols(lp, 2*group_count+task_count);
	print_log(3, "Matrix coef\n");
	for (int i = 1; i<group_count+1; i++) {
		int k = 2*i-1;
		char col_name_A[15], col_name_B[15];
		snprintf(col_name_A, 15, "la%d", i);
		snprintf(col_name_B, 15, "lb%d", i);
		glp_set_col_name(lp, k, col_name_A);
		glp_set_col_name(lp, k+1, col_name_B);
		glp_set_col_bnds(lp, k, GLP_DB, 0.0, parsed_args.cpu_count_a);
		glp_set_col_bnds(lp, k+1, GLP_DB, 0.0, parsed_args.cpu_count_b);
		glp_set_col_kind(lp, k, GLP_IV);
		glp_set_col_kind(lp, k+1, GLP_IV);
		glp_set_obj_coef(lp, k, 1);
		glp_set_obj_coef(lp, k+1, 1);
		ia[k] = 2*i-1;
		ja[k] = k;
		ar[k] = -lambda;
		ia[k+1] = 2*i;
		ja[k+1] = k+1;
		ar[k+1] = -lambda;
		print_log(3, "(%d) %d, %d, %f\n", k, ia[k], ja[k], ar[k]);
		print_log(3, "(%d) %d, %d, %f\n", k+1, ia[k+1], ja[k+1], ar[k+1]);
	}
	for (int i = 1; i<task_count+1; i++) {
		int j = i+2*group_count;
		int k = 2*i+2*group_count-1;
		char col_name[15];
		struct task *current = &tasks[i];
		snprintf(col_name, 15, "x%d", i);
		glp_set_col_name(lp, j, col_name);
		glp_set_col_kind(lp, j, GLP_BV);
		glp_set_obj_coef(lp, j, 0);
		ia[k] = 2*current->g-1;
		ja[k] = j;
		ar[k] = current->p_a*current->q_a;
		ia[k+1] = 2*current->g;
		ja[k+1] = j;
		ar[k+1] = -current->p_b*current->q_b;
		sum_group_b[current->g] += current->p_b*current->q_b;
		sum_total_b += current->p_b*current->q_b;
		print_log(3, "(%d)[%d] %d, %d, %f\n", k, i, ia[k], ja[k], ar[k]);
		print_log(3, "(%d)[%d] %d, %d, %f\n", k+1, i, ia[k+1], ja[k+1], ar[k+1]);
	}
	for (int i = 1; i<task_count+1; i++) {
		int j = i+2*group_count;
		int k = 6*i+2*group_count+2*task_count-5;
		int l = 4*i+2*group_count-3;
		struct task *current = &tasks[i];
		/* q_a row */
		ia[k] = l;
		ja[k] = j;
		ar[k] = current->q_a;
		ia[k+1] = l;
		ja[k+1] = 2*current->g-1;
		ar[k+1] = -1;
		/* p_a row */
		ia[k+2] = l+1;
		ja[k+2] = j;
		ar[k+2] = current->p_a;
		/* q_b row */
		ia[k+3] = l+2;
		ja[k+3] = j;
		ar[k+3] = -current->q_b;
		ia[k+4] = l+2;
		ja[k+4] = 2*current->g;
		ar[k+4] = -1;
		/* p_b row */
		ia[k+5] = l+3;
		ja[k+5] = j;
		ar[k+5] = -current->p_b;
		print_log(3, "(%d)[%d] %d, %d, %f\n", k, i, ia[k], ja[k], ar[k]);
		print_log(3, "(%d)[%d] %d, %d, %f\n", k+1, i, ia[k+1], ja[k+1], ar[k+1]);
		print_log(3, "(%d)[%d] %d, %d, %f\n", k+2, i, ia[k+2], ja[k+2], ar[k+2]);
		print_log(3, "(%d)[%d] %d, %d, %f\n", k+3, i, ia[k+3], ja[k+3], ar[k+3]);
		print_log(3, "(%d)[%d] %d, %d, %f\n", k+4, i, ia[k+4], ja[k+4], ar[k+4]);
		print_log(3, "(%d)[%d] %d, %d, %f\n", k+5, i, ia[k+5], ja[k+5], ar[k+5]);
	}
	for (int i = 1; i<task_count+1; i++) {
		int j = i+2*group_count;
		int k = 2*i+2*group_count+8*task_count-1;
		int l = 2*group_count+4*task_count+1;
		struct task *current = &tasks[i];
		/* sum_a row */
		ia[k] = l;
		ja[k] = j;
		ar[k] = current->p_a*current->q_a;
		/* sum_b row */
		ia[k+1] = l+1;
		ja[k+1] = j;
		ar[k+1] = -current->p_b*current->q_b;
		print_log(3, "(%d)[%d] %d, %d, %f\n", k, i, ia[k], ja[k], ar[k]);
		print_log(3, "(%d)[%d] %d, %d, %f\n", k+1, i, ia[k+1], ja[k+1], ar[k+1]);
	}
	glp_add_rows(lp, 2*group_count+4*task_count+2);
	print_log(3, "Row limits\n");
	for (int i = 1; i<group_count+1; i++) {
		char row_name_a[15], row_name_b[15];
		snprintf(row_name_a, 15, "a_g%d", i);
		snprintf(row_name_b, 15, "b_g%d", i);
		glp_set_row_name(lp, 2*i-1, row_name_a);
		glp_set_row_name(lp, 2*i, row_name_b);
		glp_set_row_bnds(lp, 2*i-1, GLP_UP, 0.0, 0.0);
		glp_set_row_bnds(lp, 2*i, GLP_UP, 0.0, -sum_group_b[i]);
		print_log(3, "(%d) (a)\t %f\t<= \t<= %f\n", 2*i-1, 0.0, glp_get_row_ub(lp, 2*i-1));
		print_log(3, "(%d) (b)\t %f\t<= \t<= %f\n", 2*i, 0.0, glp_get_row_ub(lp, 2*i));
	}
	for (int i = 1; i<task_count+1; i++) {
		int l = 4*i+2*group_count-3;
		struct task *current = &tasks[i];
		char row_name_qa[15], row_name_qb[15],
			row_name_pa[15], row_name_pb[15];
		snprintf(row_name_qa, 15, "qa_x%d", i);
		snprintf(row_name_pa, 15, "pa_x%d", i);
		snprintf(row_name_qb, 15, "qb_x%d", i);
		snprintf(row_name_pb, 15, "pb_x%d", i);
		glp_set_row_name(lp, l, row_name_qa);
		glp_set_row_name(lp, l+1, row_name_pa);
		glp_set_row_name(lp, l+2, row_name_qb);
		glp_set_row_name(lp, l+3, row_name_pb);
		glp_set_row_bnds(lp, l, GLP_UP, 0.0, 0.0);
		glp_set_row_bnds(lp, l+1, GLP_UP, 0.0, lambda);
		glp_set_row_bnds(lp, l+2, GLP_UP, 0.0, -current->q_b);
		glp_set_row_bnds(lp, l+3, GLP_UP, 0.0, lambda-current->p_b);
		print_log(3, "(%d) (a)\t %f\t<= \t<= %f\n", l, 0.0, glp_get_row_ub(lp, l));
		print_log(3, "(%d) (b)\t %f\t<= \t<= %f\n", l+1, 0.0, glp_get_row_ub(lp, l+1));
	}
	glp_set_row_name(lp, 2*group_count+4*task_count+1, "sum_a");
	glp_set_row_name(lp, 2*group_count+4*task_count+2, "sum_b");
	glp_set_row_bnds(lp, 2*group_count+4*task_count+1, GLP_UP, 0.0, 
		lambda*parsed_args.cpu_count_a);
	glp_set_row_bnds(lp, 2*group_count+4*task_count+2, GLP_UP, 0.0, 
		lambda*parsed_args.cpu_count_b-sum_total_b);
	print_log(3, "(%d) (a)\t %f\t<= \t<= %f\n", 2*group_count+4*task_count+1, 0.0, glp_get_row_ub(lp, 2*group_count+4*task_count+1));
	print_log(3, "(%d) (b)\t %f\t<= \t<= %f\n", 2*group_count+4*task_count+2, 0.0, glp_get_row_ub(lp, 2*group_count+4*task_count+2));

	glp_load_matrix(lp, 2*group_count+10*task_count, ia, ja, ar);
	free(sum_group_b);
}

void random_schedule(glp_prob *lp, struct cli_args parsed_args, int *positions)
{
	for (int i=1; 
		i<parsed_args.task_count+1; i++)  {
		int j = i+2*parsed_args.group_count;
		glp_set_col_bnds(lp, j, GLP_FX, positions[i], positions[i]);
	}
}


void minimum_work_schedule(glp_prob *lp, struct task *tasks, struct cli_args parsed_args)
{
	for (int i=1; i<parsed_args.task_count+1; i++)  {
		struct task *current = &tasks[i];
		int j = i+2*parsed_args.group_count;
		if (current->p_a*current->q_a < current->p_b*current->q_b) {
			glp_set_col_bnds(lp, j, GLP_FX, 1, 1);
		} else {
			glp_set_col_bnds(lp, j, GLP_FX, 0, 0);
		}
	}
}

int dichotomy(glp_prob *lp, int *ia, int *ja, double *ar, struct task *tasks, struct 
cli_args parsed_args, int lb, int ub)
{
	int lower_bound = lb, upper_bound = ub, lambda = 0;
	int *positions = NULL;
	if (parsed_args.experience_type == RANDOM_TYPE) {
		positions = calloc(parsed_args.task_count+1, sizeof(int));
		for (int i=1; i<parsed_args.task_count+1; i++) {
			positions[i] = rand_int(0, 2);;
		}
	}
	bool sol_found = false;
	while (upper_bound-lower_bound > 1) {
		glp_smcp param;
		glp_init_smcp(&param);
		switch(log_level) {
		case 1:
			param.msg_lev = GLP_MSG_ERR;
			break;
		case 2:
			param.msg_lev = GLP_MSG_ON;
			break;
		case 3:
			param.msg_lev = GLP_MSG_ALL;
			break;
		default:
			param.msg_lev = GLP_MSG_OFF;
			break;
		}
		lambda = (upper_bound + lower_bound)/2;
		glp_erase_prob(lp);
		glp_set_prob_name(lp, "scheduling");
		glp_set_obj_dir(lp, GLP_MIN);
		glp_set_obj_name(lp, "cpu_count");
		fill_problem(lp, ia, ja, ar, tasks, lambda, parsed_args);
		switch(parsed_args.experience_type) {
		case RANDOM_TYPE:
			random_schedule(lp, parsed_args, positions);
			break;
		case MIN_WORK_TYPE:
			minimum_work_schedule(lp, tasks, parsed_args);
			break;
		}
		print_log(2, "[%d, %d]: %d\n", 
			upper_bound, lower_bound, lambda);
		glp_simplex(lp, &param);
		if (glp_get_status(lp) == GLP_OPT) {
			upper_bound = lambda;
			sol_found = true;
		} else {
			lower_bound = lambda;
		}
	}
	if(!sol_found) {
		print_log(1, "NO SOLUTION FOUND\tlambda: %d\t\tlb: %d | %d\t\tub: %d | %d\n", 
			lambda, lb, lower_bound, ub, upper_bound);
		return -1;
	}
	return upper_bound;
}
