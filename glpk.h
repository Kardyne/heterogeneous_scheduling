#ifndef __GLPK_H__
#define __GLPK_H__

#include <glpk.h>
#include <time.h>
#include <math.h>
#include "io.h"

struct task {
	int p_a;
	int p_b;
	int q_a;
	int q_b;
	int g;
};

extern int rand_int(int min, int max);

extern int generate_values(struct task *tasks, struct cli_args parsed_args);

extern int dichotomy(glp_prob *lp, int *ia, int *ja, double *ar, struct task *tasks, struct 
cli_args parsed_args, int lb, int ub);

#endif
