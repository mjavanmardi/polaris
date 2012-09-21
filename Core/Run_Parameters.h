#pragma once


static const unsigned int Megabytes_To_Allocate=1000;
static const unsigned int num_iterations=5;
static const int num_threads=1;

static const int Page_Size=4096;
static const unsigned int Max_Pages=(Megabytes_To_Allocate*1048576)/Page_Size;

#define STATE_CHECKS 0