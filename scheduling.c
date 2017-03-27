#include <stdio.h>
#include <glpk.h>
#include <time.h>
#include <math.h>
#include "io.h"
#include "glpk.h"

int main(int argc, char **argv)
{
	struct cli_args parsed_args = parse_args(argc, argv);
	int group_count = parsed_args.group_count;
	int task_count = parsed_args.task_count;
	int array_size = 2*group_count+8*task_count+1;
	glp_prob *lp = glp_create_prob();
	int *ia = calloc(array_size, sizeof(int));
	int *ja = calloc(array_size, sizeof(int));
	double *ar = calloc(array_size, sizeof(double));
	struct task *tasks = calloc(task_count+1, sizeof(struct task));
	int upper_bound = generate_values(tasks, parsed_args), lower_bound = 0;
	srand(parsed_args.seed);
	print_log(2, "Array size:%d\n", array_size);
	print_log(2, "Line count:%d\n", 2*(group_count+task_count+1));
	print_log(2, "Lambda upper bound: %d\n", upper_bound);
	int lambda = dichotomy(lp, ia, ja, ar, tasks, parsed_args, lower_bound, upper_bound);
	if (log_level >= 1)
		glp_write_lp(lp, NULL, "scheduling.lp");
	for (int i = 1; i<task_count+1; i++) {
		print_log(1, "%s=%d\n", glp_get_col_name(lp, i+2*group_count), 
(int) round(glp_get_col_prim(lp, i+2*group_count)));
	}
	for (int i = 1; i<2*group_count+1; i++) {
		print_log(1, "%s=%d\n", glp_get_col_name(lp, i), (int) round(glp_get_col_prim(lp, 
i)));
	}
	switch(parsed_args.experience_type) {
	case 0:
		printf("opt: %d, %d", (int) round(glp_get_obj_val(lp)), lambda);
		break;
	case 1:
		printf("random: %d, %d", (int) round(glp_get_obj_val(lp)), lambda);
		break;
	case 2:
		printf("min_work: %d, %d", (int) round(glp_get_obj_val(lp)), lambda);
		break;
	}
	glp_delete_prob(lp);
	glp_free_env();
	free(ia);
	free(ja);
	free(ar);
	free(tasks);
	return EXIT_SUCCESS;
}
