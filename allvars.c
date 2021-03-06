/*
 * RECON Copyright (C) 2018 Yan-Rong Li
 * A package for measuring spectral power and reconstructing time series in AGN.
 * 
 * Yan-Rong Li, liyanrong@mail.ihep.ac.cn
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <fftw3.h>
#include <gsl/gsl_interp.h>

#include "allvars.h"

int thistask, totaltask;
int roottask;
int namelen;
char proc_name[MPI_MAX_PROCESSOR_NAME];

int ndata;
double *flux_data, *time_data, *err_data;
double time_media, time_cad_min, time_cad_media, flux_data_min, flux_data_max, flux_scale, flux_mean;

double freq_limit_data_lower, freq_limit_data_upper;

int nd_sim;
double DT, V, W;
double *time_sim, *flux_sim, *flux_data_sim;
double *flux_sim_mean, *err_sim_mean;

gsl_interp_accel *gsl_acc_sim;
gsl_interp  *gsl_linear_sim;

fftw_plan pfft;
fftw_complex *fft_work;

int num_params, num_params_psd, num_recon;
double **par_range_model, **var_range_model, *par_fix_val;
int *par_fix;

int which_particle_update, which_parameter_update;
int which_level_update, num_particles;
unsigned long long int which_mcmc_steps;
int *perturb_accept;


int recon_flag_postprc; 
double recon_temperature;
int recon_flag_restart;
int recon_flag_sample_info;
int recon_flag_temp;
int recon_flag_sim;
int recon_flag_help;
int recon_flag_end;
int recon_flag_limits;
int recon_flag_prior_exam;
int recon_flag_seed;
int recon_seed;
int recon_flag_psd;

PARSET parset;

FILE *finfo;