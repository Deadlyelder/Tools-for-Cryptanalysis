/* Copyright (C) 2010 Luk Bettale

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.
   
   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program. If not, see
   <http://www.gnu.org/licenses/>. */

#ifndef CHRONO_H
#define CHRONO_H

#include <time.h>

#define measure(command, label) {                               \
    clock_t start, end;                                         \
    double cpu_time_used;                                       \
    start = clock ();                                           \
    command;                                                    \
    end = clock ();                                             \
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;  \
    fprintf (stderr, "%s: ", label);                            \
    fprintf (stderr, "%lf\n", cpu_time_used);                   \
  }

#endif  /* CHRONO_H */
