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
#include <string.h>
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
    char magma_file_name[] = "/tmp/mgm.XXXXXX";
    FILE *magma_file;
    char magma[128] = "magma";

    /* propagation */
    /* measure (ANF_propagate (syst), "propagate"); */
    /* measure (ANF_remap (syst), "remap variables"); */

    /* create temporary magma file */
    mkstemp (magma_file_name);
    magma_file = fopen (magma_file_name, "w+");
    fprintf (magma_file, "R<[x]> := BooleanPolynomialRing(%d);\n", syst->rank);
    fprintf (magma_file, "F :=\n");
    ANF_fprint_syst (magma_file, syst);
    fprintf (magma_file, ";\n");
#ifdef DEBUG
    fprintf (magma_file, "SetVerbose(\"Groebner\",true);");
#endif
    fprintf (magma_file, "VarietySequence(Ideal(ChangeUniverse(F,R)));\n");
    fclose (magma_file);

    /* run magma */
    strcat (magma, " -b");
    strcat (magma, " <");
    strcat (magma, magma_file_name);
    system (magma);
    
    /* remove temporary magma file */
    remove (magma_file_name);
  }
  /* end */
  ANF_free (syst);
  return 0;
}
