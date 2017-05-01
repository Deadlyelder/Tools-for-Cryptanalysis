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
   along with this program. If not, see
   <http://www.gnu.org/licenses/>. */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <libanf2cnf.h>

int
main (int argc, char *argv[])
{
  static unsigned int counter;
  FILE *stream;
  int err;
  ANF_system *syst;
  if (argc <= 1)
    {
      fprintf (stderr, "%s: Filename needed\n", argv[0]);
      return 1;
    }
  stream = fopen (argv[1], "r");
  if (!stream)
    {
      char str[1024] = "";
      strcat (str, argv[0]);
      strcat (str, ": ");
      strcat (str, argv[1]);
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
    static unsigned int unknowns = -1;
    unsigned int new_unknowns;
    int sat;
    ANF_system *clone;
    FILE *anf_file, *cnf_file, *sol_file;
    CNF_system *cnf_syst;
    int *cnf_sol;
    char cryptominisat[128];

    char anf_file_name[] = "/tmp/anf_i.XXXXXX";
    char cnf_file_name[] = "/tmp/cnf_i.XXXXXX";
    char sol_file_name[] = "/tmp/sol_i.XXXXXX";
    char learnts_file_name[] = "/tmp/learnts_i.XXXXXX";

    /* clone the original system for further checking */
    clone = malloc (sizeof *clone);
    ANF_clone (syst, clone);

#ifdef DEBUG
    {
      unsigned int unknowns = 0;
      for (unsigned int i = 0; i < syst->rank; i++)
        {
          if (syst->x[i] == i+1)
            unknowns++;
        }
      fprintf (stderr, "before propagation: %d unknowns.\n", unknowns);
    }
#endif

  begin :
    fprintf (stderr, "%d call%s to cryptominisat.\n", counter, counter > 1 ? "s" : "");
    /* propagation */
    ANF_propagate (syst);
#ifdef DEBUG
    {
      unsigned int unknowns = 0;
      for (unsigned int i = 0; i < syst->rank; i++)
        {
          if (syst->x[i] == i+1)
            unknowns++;
        }
      fprintf (stderr, "after propagation: %d unknowns.\n", unknowns);
    }
#endif

    new_unknowns = 0;
    for (unsigned int i = 0; i < syst->rank; i++)
      {
        if (syst->x[i] == i+1)
          new_unknowns++;
      }
    /* stop when fix point reached */
    if (unknowns == new_unknowns)
      {
        ANF_fprint (stdout, syst);
        ANF_free (syst);
        ANF_free (clone);
        printf ("Inconclusive\n");
        return 0;
      }

    anf_file_name[9] = '0' + counter;
    for (unsigned int i = 11; i < 17; i++)
      anf_file_name[i] = 'X';
    cnf_file_name[9] = '0' + counter;
    for (unsigned int i = 11; i < 17; i++)
      cnf_file_name[i] = 'X';
    sol_file_name[9] = '0' + counter;
    for (unsigned int i = 11; i < 17; i++)
      sol_file_name[i] = 'X';
    learnts_file_name[13] = '0' + counter;
    for (unsigned int i = 15; i < 21; i++)
      learnts_file_name[i] = 'X';

    /* create temporary file names */
    if (mkstemp (cnf_file_name) < 0)
      {
        char str[1024] = "";
        strcat (str, argv[0]);
        strcat (str, ": ");
        strcat (str, cnf_file_name);
        perror (str);
        ANF_free (syst);
        ANF_free (clone);
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
        ANF_free (clone);
        return 8;
      }
    if (mkstemp (learnts_file_name) < 0)
      {
        char str[1024] = "";
        strcat (str, argv[0]);
        strcat (str, ": ");
        strcat (str, learnts_file_name);
        perror (str);
        remove (cnf_file_name);
        remove (sol_file_name);
        ANF_free (syst);
        ANF_free (clone);
        return 8;
      }
    if (mkstemp (anf_file_name) < 0)
      {
        char str[1024] = "";
        strcat (str, argv[0]);
        strcat (str, ": ");
        strcat (str, anf_file_name);
        perror (str);
        remove (cnf_file_name);
        remove (sol_file_name);
        remove (learnts_file_name);
        ANF_free (syst);
        ANF_free (clone);
        return 8;
      }

    /* print anf into a file */
    anf_file = fopen (anf_file_name, "r+");
    ANF_fprint (anf_file, syst);
    fclose (anf_file);

    /* converts anf to cnf */
    cnf_syst = malloc (sizeof *cnf_syst);
    anf2cnf (syst, cnf_syst);

    /* print cnf into a file */
    cnf_file = fopen (cnf_file_name, "r+");
    CNF_fprint (cnf_file, cnf_syst);
    fclose (cnf_file);

    /* prepare the cryptominisat command */
    strcpy (cryptominisat, "cryptominisat");
    strcat (cryptominisat, " --verbosity=0");
    if (unknowns > new_unknowns)
      {
        strcat (cryptominisat, " --restart=static --restarts=20");
        strcat (cryptominisat, " --dumplearnts=");
        strcat (cryptominisat, learnts_file_name);
      }
    strcat (cryptominisat, " ");
    strcat (cryptominisat, cnf_file_name);
    strcat (cryptominisat, " ");
    strcat (cryptominisat, sol_file_name);
    /* strcat (cryptominisat, " >&2"); */

    /* run cryptominisat */
    err = system (cryptominisat);
    counter++;
    if (WEXITSTATUS (err) == 127)
      {
        ANF_free (syst);
        ANF_free (clone);
        CNF_free (cnf_syst);
        remove (cnf_file_name);
        remove (sol_file_name);
        remove (learnts_file_name);
        return 127;
      }

    /* parse the output of cryptominisat */
    cnf_sol = calloc (cnf_syst->rank + 1, sizeof *cnf_sol);
    sol_file = fopen (sol_file_name, "r+");
    sat = solution_parse (sol_file, cnf_sol);
    fclose (sol_file);

#ifndef DEBUG
    remove (anf_file_name);
    remove (cnf_file_name);
    remove (sol_file_name);
#endif

    if (sat < 0)
      {
        ANF_free (syst);
        ANF_free (clone);
        CNF_free (cnf_syst);
        free (cnf_sol);
        remove (learnts_file_name);
        printf ("No solution\n");
        return 0;
      }

    if (sat > 0)
      {
        FILE *learnts_file;
        learnts_file = fopen (learnts_file_name, "r+");
        learnts_parse (learnts_file, syst, cnf_syst);
        fclose (learnts_file);
#ifndef DEBUG
        remove (learnts_file_name);
#endif
        CNF_free (cnf_syst);
        free (cnf_sol);
        unknowns = new_unknowns;
        goto begin;
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
      ANF_fprint_x (stdout, syst);
    }
  }
  /* end */
  ANF_free (syst);
  return 0;
}
