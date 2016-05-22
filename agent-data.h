/*
 *  msr-agent - Utility for help MSR access
 *  file name: agent-data.h
 *
 *  Copyright (c) 2016 Nicholas Feng <nicholasfeng.tw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 */

#define MSR_AGENT_DEBUG 1
#define LINUX_MSR_DRIVER_PATH "/dev/cpu/%d/msr"
#define MAX_SUPPORTED_CORES 64
#define MAX_SUPPORTED_PATH 64
#define MAX_STRENG_LENGTH 256

#define EXIT_SUCCESS 0
#define EXIT_GENERAL_ERROR 1
#define EXIT_INCORRECT_USAGE 2
#define EXIT_COMMAND_NOT_FOUND 127

struct msr_handle {
	int    FdMsr;
	bool   IsFdValid;
  bool   IsHTCore;
	char   msr_driver_path[MAX_SUPPORTED_PATH];
} msr_handle_default = {0, false, false, };

struct msr_main_handle {
  int  ActiveCoreCounts;
  bool IsIntelHTEnabled;
  struct msr_handle msr_fd[MAX_SUPPORTED_CORES];
} msr_main_handle_default = {0, false, };

//typedef struct msr_main_handle pcmbfs_perf_recd;
