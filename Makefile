# Makefile

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


OBJS = scheduling.o io.o glpk.o

LIBS = -lglpk -lm

CFLAGS = -Wall -Wextra -Werror -O2 -Wno-strict-aliasing
CC = gcc

scheduling: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o scheduling $(LIBS)

$(OBJS): %.o: %.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o scheduling scheduling.lp *~
