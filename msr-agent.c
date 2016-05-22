/*
 *  msr-agent - Utility for help MSR access
 *  file name: msr-agent.c
 *
 *  Copyright (c) 2016 Nicholas Feng <nicholasfeng.tw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 */

// include
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h> // for parsing command line parameter
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <sysexits.h>
// include (msr-agent)
#include "ver.h"
#include "agent-prototype.h"
#include "agent-data.h"

// define

// global variable
int CpuCountsOnline;
const char *this;
static const char short_op_str[] = "hRvrwapc0";
static const struct option long_op_str[] = {
	{"help",		      0, 0, 'h'},
	{"revision",		  0, 0, 'R'},
	{"verbose",		    0, 0, 'v'},
	{"read",		      0, 0, 'r'},
	{"write",   		  0, 0, 'w'},
	{"all",			      0, 0, 'a'},
	{"processor",		  1, 0, 'p'},
	{"cpu",			      1, 0, 'c'},
	{ 0,              0, 0,  0 }
};

uint64_t glbMSRvalue;
uint64_t *ptrglbMSRvalue = NULL;
bool allCpu = false;

struct msr_main_handle main_handle;
char errstr [MAX_STRENG_LENGTH];


// code section
int main(int argc, char *argv[])
{
	int iCmdLineParam;
  int msroffset;
	int iCoreNum = 0;

  this = argv[0];

  CpuCountsOnline = sysconf(_SC_NPROCESSORS_ONLN);
  msr_handle_install();
//#if MSR_AGENT_DEBUG
#if 0
  fprintf(stdout, "CpuCore(Online): 0x%X\n", CpuCountsOnline);
#endif

	// set global flag first
  while((iCmdLineParam = getopt_long(argc, argv, short_op_str, long_op_str, NULL)) != -1) {
     if(iCmdLineParam == 'a'){
        allCpu = true;
        break;
		 }
  }

	// reset optind
	optind = 0;

  while((iCmdLineParam = getopt_long(argc, argv, short_op_str, long_op_str, NULL)) != -1){
   switch(iCmdLineParam){
     case 'h':

       printhelp();
		   exit(EXIT_SUCCESS);

		 case 'V':

     print_program_header();
			exit(EXIT_SUCCESS);

     case 'r':
 		  print_program_header();
	    msroffset = strtoul(argv[optind], NULL, 0);
			if(allCpu){
			  for(iCoreNum = 0; (iCoreNum < CpuCountsOnline) && (iCoreNum < MAX_SUPPORTED_CORES); iCoreNum++){
				  	 op_msr_read(iCoreNum, msroffset, ptrglbMSRvalue);
			  }
      } else {
				op_msr_read(0, msroffset, ptrglbMSRvalue);
			}
			break;

     case 'w':
  		print_program_header();
 	    msroffset = strtoul(argv[optind], NULL, 0);
			*ptrglbMSRvalue = strtoul(argv[optind+1], NULL, 0);
			if(allCpu){
 			  for(iCoreNum = 0; (iCoreNum < CpuCountsOnline) && (iCoreNum < MAX_SUPPORTED_CORES); iCoreNum++){
 				  	 op_msr_write(iCoreNum, msroffset, ptrglbMSRvalue);
 			  }
		  } else {
        op_msr_write(0, msroffset, ptrglbMSRvalue);
			}
     break;

     case 'a':
		   // dummy
       break;

     default:
       printhelp();
       exit(EXIT_SUCCESS);

   } // switch(iCmdLineParam)
 } // while((iCmdLineParam = getopt_long(argc, argv, short_op_str, long_op_str, NULL)) != -1)


//  printhelp();
  msr_handle_uninstall();
  exit(EXIT_SUCCESS);
}

void print_program_header()
{
	//fprintf(stdout, "\n");
  fprintf(stdout, VERSION_STRING_MSR_AGENT);
  fprintf(stdout, COPYRIGHT_STRING);
//fprintf(stderr, STRING_MSR_AGENT "\n");

}

void printhelp()
{
  fprintf(stdout,
		"Usage: [options] MSR#\n"
		"  --help         -h  Print help message.\n"
		"  --revision     -R  Show reversion.\n"
		"  --verbose      -V  Verbose mode\n"
    "  --read         -r  Read MSR\n"
    "  --write        -w  Write MSR\n"
		"  --all          -a  To access MSR of all processors\n"
		"  --processor #  -p  To access MSR of specific processor (processor #0 as default)\n"
  );
}

void print_errmsg(int errno)
{


}

void msr_handle_install()
{
  int iCoreNum = 0;

	ptrglbMSRvalue = &glbMSRvalue;
  main_handle.ActiveCoreCounts = CpuCountsOnline;
  main_handle.IsIntelHTEnabled = false;

  for(iCoreNum = 0; (iCoreNum < CpuCountsOnline) && (iCoreNum < MAX_SUPPORTED_CORES); iCoreNum++){
      // Initialize the string for MSR driver file opening
      memset(main_handle.msr_fd[iCoreNum].msr_driver_path, 0, MAX_SUPPORTED_PATH);
      sprintf(main_handle.msr_fd[iCoreNum].msr_driver_path, LINUX_MSR_DRIVER_PATH, iCoreNum);

      // Assign handle of MSR driver for each core
      if(main_handle.msr_fd[iCoreNum].FdMsr == 0) {

        main_handle.msr_fd[iCoreNum].FdMsr = open(main_handle.msr_fd[iCoreNum].msr_driver_path, O_RDWR);

          // Set Valid bit if the handle is usable
        if(main_handle.msr_fd[iCoreNum].FdMsr > 0) {

            main_handle.msr_fd[iCoreNum].IsFdValid = true;

          } else { //  if(main_handle.msr_fd[iCoreNum].FdMsr > 0)

            main_handle.msr_fd[iCoreNum].IsFdValid = false;

            if(errno) {
              // uninstall data structure if any error.
							msr_handle_uninstall();

              switch(errno){
								case EIO:
								  fprintf(stderr, "CPU# %d doesn't exist.\n", iCoreNum);
								  exit(EXIT_INCORRECT_USAGE);

								case ENXIO:
								  fprintf(stderr, "MSRs are not supported by CPU#. %d\n", iCoreNum);
	                exit(EXIT_GENERAL_ERROR);

								default:
								  memset(errstr, 0, MAX_STRENG_LENGTH);
								  sprintf(errstr, "Open MSR driver for CPU %d", iCoreNum);
								  perror(errstr);
								  sprintf(errstr, " Program exit with error.\n");
								  exit(EXIT_COMMAND_NOT_FOUND);
							}
						} // if(errno)
        } // if(main_handle.msr_fd[iCoreNum].FdMsr > 0)
      } // if(main_handle.msr_fd[iCoreNum].FdMsr == 0)
  } // for(CoreNum = 0; (CoreNum < MaxActiveCores) && (CoreNum < MAX_SUPPORTED_CORES); CoreNum++)

//#if MSR_AGENT_DEBUG
#if 0
  for(iCoreNum = 0; (iCoreNum < CpuCountsOnline) && (iCoreNum < MAX_SUPPORTED_CORES); iCoreNum++){

  fprintf(stdout, "main_handle.msr_fd[0x%X].FdMsr=0x%X\n", iCoreNum, main_handle.msr_fd[iCoreNum].FdMsr);

  } // for(iCoreNum = 0; (iCoreNum < CpuCountsOnline) && (iCoreNum < MAX_SUPPORTED_CORES); iCoreNum++)
#endif // MSR_AGENT_DEBUG
}

void msr_handle_uninstall()
{
  int iCoreNum = 0;

  for(iCoreNum = 0; (iCoreNum < CpuCountsOnline) && (iCoreNum < MAX_SUPPORTED_CORES); iCoreNum++){
    if(main_handle.msr_fd[iCoreNum].IsFdValid == true)
    {
      main_handle.msr_fd[iCoreNum].IsFdValid = false;
      close(main_handle.msr_fd[iCoreNum].FdMsr);
      main_handle.msr_fd[iCoreNum].FdMsr = 0;
    }
  }
}

void op_msr_read(int cpu, int msr_number, uint64_t * value)
{
  *value = 0;

  if( pread(main_handle.msr_fd[cpu].FdMsr, value, sizeof(uint64_t), msr_number) != sizeof(*value) ) {
    if (errno == EIO) {
      fprintf(stderr, "MSR 0x%X of CPU %d is not readable.\n", msr_number, cpu);
      exit(EXIT_GENERAL_ERROR);
    } else {
      perror("pread");
      exit(EXIT_COMMAND_NOT_FOUND);
    }
  }

//#if MSR_AGENT_DEBUG
	fprintf(stdout, "op_msr_read - cpu#%x-msr(0x%X) = 0x%llX \n", cpu, msr_number, *value);
//#endif
}

void op_msr_write(int cpu, int msr_number, uint64_t *value){

  if( pwrite(main_handle.msr_fd[cpu].FdMsr, value, sizeof(uint64_t), msr_number) != sizeof(*value) )
  {
    if (errno == EIO) {
      fprintf(stderr, "MSR 0x%X of CPU %d is not writeable.\n", msr_number, cpu);
      exit(EXIT_GENERAL_ERROR);
    } else {
      perror("pwrite");
      exit(EXIT_COMMAND_NOT_FOUND);
    }
	}
}
