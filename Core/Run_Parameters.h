#pragma once


static const unsigned int _Megabytes_To_Allocate=500;
static const unsigned int _num_iterations=10;
static const int _num_threads=1;

static const int _Max_Message_Size=5*1048576;

static const int _Page_Size=4096;
static const unsigned int _Max_Pages=(_Megabytes_To_Allocate*1048576)/_Page_Size;

#define STATE_CHECKS 0

#define DEBUG_3