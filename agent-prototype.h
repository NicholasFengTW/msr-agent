/*
 *  msr-agent - Utility for help MSR access
 *  file name: agent-prototype.h
 *
 *  Copyright (c) 2016 Nicholas Feng <nicholasfeng.tw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define LINUX_MSR_DRIVER_PATH "/dev/cpu/%d/msr"
#define MAX_SUPPORTED_CORES 64
#define MAX_SUPPORTED_PATH 64



void print_program_header();

void printhelp();
