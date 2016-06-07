/*
 *  msr-agent - Utility for help MSR access
 *  file name: agent-prototype.h
 *
 *  Copyright (c) 2016 Nicholas Feng <nicholasfeng.tw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 */

void initialize_global_variable(int argc, char *argv[]);
void print_program_header();
void printhelp();
void error_handler(int errnumber);
void msr_handle_install();
void msr_handle_uninstall();
void op_msr_read();
void op_msr_write();
