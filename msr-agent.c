/*
 *  msr-agent - Utility for help MSR access
 *  file name: msr-agent.c
 *
 *  Copyright (c) 2016 Nicholas Feng <nicholasfeng.tw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

// include
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
// include (msr-agent)
#include "ver.h"
#include "agent-prototype.h"

// define

// global variable



int main(int argc, char *argv[])
{
  print_program_header();

  exit(0);
}

void print_program_header()
{
  fprintf(stderr, VERSION_STRING_MSR_AGENT);
  fprintf(stderr, COPYRIGHT_STRING);
//fprintf(stderr, STRING_MSR_AGENT "\n");

}
