/* Copyright (C) 2010, 2011 Luk Bettale

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <libanf2cnf.h>
#include "chrono.h"

int
main (int argc, char *argv[])
{
  FILE *stream;
  int err;
  ANF_system *syst;
  if (argc <= 1)
    {
      fprintf (stderr, "%s: Filename needed\n", argv[0]);
      return 1;
    }
  stream = fopen (argv[argc - 1], "r");
  if (!stream)
    {
      char str[1024] = "";
      strcat (str, argv[0]);
      strcat (str, ": ");
      strcat (str, argv[argc - 1]);
      perror (str);
      return 2;
    }
  syst = malloc (sizeof *syst);
  err = ANF_parse (syst, stream);
  fclose (stream);
  if (err)
    {
      ANF_free (syst);
      if (err < 0)
        fprintf (stderr, "%s: Variable should start from 1\n", argv[0]);
      else
        fprintf (stderr, "%s: Bad format (line %d)\n", argv[0], err);
      return 4;
    }
  /* insert code here */
  {
    int sat;
    ANF_system *clone;
    FILE *cnf_file, *sol_file;
    CNF_system *cnf_syst;
    int *cnf_sol;
    char cryptominisat[128] = "";

    char cnf_file_name[] = "/tmp/cnf.XXXXXX";
    char sol_file_name[] = "/tmp/sol.XXXXXX";

    /* create temporary file names */
    if (mkstemp (cnf_file_name) < 0)
      {
        char str[1024] = "";
        strcat (str, argv[0]);
        strcat (str, ": ");
        strcat (str, cnf_file_name);
        perror (str);
        ANF_free (syst);
        return 8;
      }
    if (mkstemp (sol_file_name) < 0)
      {
        char str[1024] = "";
        strcat (str, argv[0]);
        strcat (str, ": ");
        strcat (str, sol_file_name);
        perror (str);
        remove (cnf_file_name);
        ANF_free (syst);
        return 8;
      }

    /* clone the original system for further checking */
    clone = malloc (sizeof *clone);
    ANF_clone (syst, clone);

    /* propagation */
    measure (ANF_propagate (syst), "propagate");

    /* converts anf to cnf */
    cnf_syst = malloc (sizeof *cnf_syst);
    measure (anf2cnf (syst, cnf_syst), "anf2cnf");

    /* print cnf into a file */
    cnf_file = fopen (cnf_file_name, "r+");
    CNF_fprint (cnf_file, cnf_syst);
    fclose (cnf_file);

    /* prepare the cryptominisat command */
    strcat (cryptominisat, "cryptominisat ");
    for (unsigned int i = 1; i < argc - 1; i++)
      {
        strcat (cryptominisat, argv[i]);
        strcat (cryptominisat, " ");
      }
    strcat (cryptominisat, cnf_file_name);
    strcat (cryptominisat, " ");
    strcat (cryptominisat, sol_file_name);

    /* run cryptominisat */
#ifdef DEBUG
    fprintf (stderr, "%s\n", cryptominisat);
#endif
    err = system (cryptominisat);
    if (WEXITSTATUS (err) == 127 || WEXITSTATUS (err) == 1)
      {
        remove (cnf_file_name);
        remove (sol_file_name);
        ANF_free (syst);
        CNF_free (cnf_syst);
        return 127;
      }

    /* parse the output of cryptominisat */
    cnf_sol = calloc (cnf_syst->rank + 1, sizeof *cnf_sol);
    sol_file = fopen (sol_file_name, "r");
    sat = solution_parse (sol_file, cnf_sol);
    fclose (sol_file);

    /* clean the temporary files */
    remove (cnf_file_name);
    remove (sol_file_name);

    if (sat < 0)
      {
        ANF_free (syst);
        CNF_free (cnf_syst);
        printf ("[]\n");
        return 0;
      }

    /* replace variables */
    for (unsigned int i = 0; cnf_sol[i] >= 0; i++)
      {
        if (cnf_syst->map[i].degree == 1)
          {
            variable v = cnf_syst->map[i].variables[0];
            if (syst->x[v - 1] == v)
              syst->x[v - 1] = cnf_sol[i] - 1;
          }
      }
    free (cnf_sol);
    CNF_free (cnf_syst);

    /* evaluate the anf system with newly found variables */
    ANF_clean (syst);
    ANF_evaluate (syst);

    /* update the variables of clone to check the solution */
    for (unsigned int i = 0; i < syst->rank; i++)
      clone->x[i] = syst->x[i];
    ANF_evaluate (clone);

    /* check that the solution is a good one */
    {
      bool check = true;
      for (unsigned int i = 0; i < clone->size; i++)
        check = check && (clone->polynomials[i].length != 1);
      ANF_free (clone);
      if (!check)
        {
          fprintf (stderr, "Very bad: should not happen\n");
          ANF_free (syst);
          return 127;
        }
      printf ("[\n    ");
      ANF_fprint_x (stdout, syst);
      printf ("]\n");
    }
  }
  /* end */
  ANF_free (syst);
  return 0;
}
