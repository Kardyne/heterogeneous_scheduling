/* glpk.h */

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
