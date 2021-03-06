/*
 * RECON Copyright (C) 2018 Yan-Rong Li
 * A package for measuring spectral power and reconstructing time series in AGN.
 * 
 * Yan-Rong Li, liyanrong@mail.ihep.ac.cn
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "allvars.h"
#include "proto.h"

/*!
 * read parameter set from parameter file.
 */
void read_parset()
{
  if(thistask == roottask)
  {
    #define MAXTAGS 300
    #define DOUBLE 1
    #define STRING 2
    #define INT 3

    FILE *fparam;
    int i, j, nt;
    char str[200], buf1[200], buf2[200], buf3[200];
    int id[MAXTAGS];
    void *addr[MAXTAGS];
    char tag[MAXTAGS][50];

    nt = 0;
    strcpy(tag[nt], "FileDir");
    addr[nt] = &parset.file_dir;
    id[nt++] = STRING;

    strcpy(tag[nt], "FileName");
    addr[nt] = &parset.file_name;
    id[nt++] = STRING;

    strcpy(tag[nt], "V");
    addr[nt] = &parset.V;
    id[nt++] = DOUBLE;

    strcpy(tag[nt], "W");
    addr[nt] = &parset.W;
    id[nt++] = DOUBLE;

    strcpy(tag[nt], "FreqLimit");
    addr[nt] = &parset.freq_limit;
    id[nt++] = DOUBLE;

    strcpy(tag[nt], "PSDType");
    addr[nt] = &parset.psd_type;
    id[nt++] = INT;

    strcpy(tag[nt], "PSDArg");
    addr[nt] = &parset.str_psd_arg;
    id[nt++] = STRING;

    strcpy(tag[nt], "FlagEndMatch");
    addr[nt] = &parset.flag_endmatch;
    id[nt++] = INT;

    strcpy(tag[nt], "FlagWhiteNoise");
    addr[nt] = &parset.flag_whitenoise;
    id[nt++] = INT;

    strcpy(tag[nt], "PeriodPSDProfType");
    addr[nt] = &parset.periodpsd_proftype;
    id[nt++] = INT;

    strcpy(tag[nt], "FileSim");
    addr[nt] = &parset.file_sim;
    id[nt++] = STRING;

    strcpy(tag[nt], "ND_Sim");
    addr[nt] = &parset.nd_sim;
    id[nt++] = INT;

    strcpy(tag[nt], "DT");
    addr[nt] = &parset.DT;
    id[nt++] = DOUBLE;

    strcpy(tag[nt], "FBad");
    addr[nt] = &parset.fbad;
    id[nt++] = DOUBLE;

    strcpy(tag[nt], "FErr");
    addr[nt] = &parset.ferr;
    id[nt++] = DOUBLE;
    
    char fname[200];
    sprintf(fname, "%s", parset.param_file);
    
    fparam = fopen(fname, "r");
    if(fparam == NULL)
    {
      fprintf(stderr, "# Error: Cannot open file %s\n", fname);
      exit(-1);
    }

    //default values
    parset.flag_endmatch = 0;
    parset.flag_whitenoise = 0;

    while(!feof(fparam))
    {
      sprintf(str,"empty");

      fgets(str, 200, fparam);
      if(sscanf(str, "%s%s%s", buf1, buf2, buf3)<2)
        continue;
      if(buf1[0]=='#')
        continue;
      for(i=0, j=-1; i<nt; i++)
        if(strcmp(buf1, tag[i]) == 0)
        {
          j = i;
          tag[i][0] = 0;
          //printf("%s %s\n", buf1, buf2);
          break;
        }
      if(j >=0)
      {
        switch(id[j])
        {
          case DOUBLE:
            *((double *) addr[j]) = atof(buf2);
            break;
          case STRING:
            strcpy(addr[j], buf2);
            break;
          case INT:
            *((int *)addr[j]) = (int) atof(buf2);
            break;
        }
      }
      else
      {
        fprintf(stderr, "# Error in file %s: Tag '%s' is not allowed or multiple defined.\n", 
                      parset.param_file, buf1);
        exit(0);
      }
    }
    fclose(fparam);

    if(parset.V < 1.0)
    {
      printf("Incorrect V=%f.\n V should be larger than 1.\n", parset.V);
      exit(0);
    }

    if(parset.W < 1.0)
    {
      printf("Incorrect W=%f.\n V should be larger than 1.\n", parset.V);
      exit(0);
    }

    if(parset.freq_limit <=0.0)
    {
      printf("Incorrect freq_limit=%f\n freq_limit should be positive.\n", parset.freq_limit);
      exit(0);
    }

    if(parset.ferr <=0.0)
    {
      printf("#Incorrect ferr=%f\n. ferr should be positive.\n", parset.ferr);
      exit(0);
    }

    if(recon_flag_psd == 1)
    {
      parset.flag_endmatch = 0;
    }
  }
  
  MPI_Bcast(&parset, sizeof(parset), MPI_BYTE, roottask, MPI_COMM_WORLD);
  return;
}
