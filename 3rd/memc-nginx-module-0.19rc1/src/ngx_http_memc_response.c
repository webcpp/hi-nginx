
#line 1 "src/ngx_http_memc_response.rl"

/*
 * Copyright (C) Yichun Zhang (agentzh)
 */


#ifndef DDEBUG
#define DDEBUG 0
#endif
#include "ddebug.h"

#include "ngx_http_memc_response.h"
#include "ngx_http_memc_module.h"


#ifdef s_char
#undef s_char
#endif

#define s_char signed char



#line 24 "src/ngx_http_memc_response.rl"

#line 29 "src/ngx_http_memc_response.c"
static const char _memc_storage_key_offsets[] = {
	0, 0, 4, 5, 6, 7, 8, 9, 
	10, 11, 12, 13, 14, 15, 16, 17, 
	19, 21, 22, 23, 24, 25, 26, 27, 
	28, 29, 30, 31, 32, 33, 34, 35, 
	37, 38, 39, 40, 41, 42, 43, 44, 
	45, 46, 47, 48, 50, 51, 52, 53, 
	54, 55, 56, 57, 58, 59, 60
};

static const char _memc_storage_trans_keys[] = {
	67, 69, 78, 83, 76, 73, 69, 78, 
	84, 95, 69, 82, 82, 79, 82, 32, 
	13, 10, 13, 82, 88, 82, 79, 82, 
	13, 10, 73, 83, 84, 83, 13, 10, 
	79, 84, 95, 70, 83, 79, 85, 78, 
	68, 13, 10, 84, 79, 82, 69, 68, 
	69, 84, 82, 86, 69, 82, 79, 82, 
	69, 68, 13, 10, 0
};

static const char _memc_storage_single_lengths[] = {
	0, 4, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 2, 
	2, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 2, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 2, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 0
};

static const char _memc_storage_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const unsigned char _memc_storage_index_offsets[] = {
	0, 0, 5, 7, 9, 11, 13, 15, 
	17, 19, 21, 23, 25, 27, 29, 31, 
	34, 37, 39, 41, 43, 45, 47, 49, 
	51, 53, 55, 57, 59, 61, 63, 65, 
	68, 70, 72, 74, 76, 78, 80, 82, 
	84, 86, 88, 90, 93, 95, 97, 99, 
	101, 103, 105, 107, 109, 111, 113
};

static const char _memc_storage_trans_targs[] = {
	2, 16, 28, 43, 0, 3, 0, 4, 
	0, 5, 0, 6, 0, 7, 0, 8, 
	0, 9, 0, 10, 0, 11, 0, 12, 
	0, 13, 0, 14, 0, 15, 14, 54, 
	15, 14, 17, 22, 0, 18, 0, 19, 
	0, 20, 0, 21, 0, 54, 0, 23, 
	0, 24, 0, 25, 0, 26, 0, 27, 
	0, 54, 0, 29, 0, 30, 0, 31, 
	0, 32, 38, 0, 33, 0, 34, 0, 
	35, 0, 36, 0, 37, 0, 54, 0, 
	39, 0, 40, 0, 41, 0, 42, 0, 
	26, 0, 44, 48, 0, 45, 0, 46, 
	0, 47, 0, 7, 0, 49, 0, 50, 
	0, 51, 0, 52, 0, 53, 0, 54, 
	0, 0, 0
};

static const char _memc_storage_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 1, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 1, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 2, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 3, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 4, 
	0, 0, 0
};

static const int memc_storage_start = 1;
static const int memc_storage_first_final = 54;
static const int memc_storage_error = 0;



#line 25 "src/ngx_http_memc_response.rl"


#line 27 "src/ngx_http_memc_response.rl"

#line 129 "src/ngx_http_memc_response.c"
static const char _memc_flush_all_key_offsets[] = {
	0, 0, 4, 5, 6, 7, 8, 9, 
	10, 11, 12, 13, 14, 15, 16, 17, 
	19, 20, 21, 22, 23, 24, 25, 26, 
	27, 28, 29, 30, 31, 32, 33
};

static const short _memc_flush_all_trans_keys[] = {
	67, 69, 79, 83, 76, 73, 69, 78, 
	84, 95, 69, 82, 82, 79, 82, 32, 
	13, 10, 13, 82, 82, 79, 82, 13, 
	10, 75, 13, 10, 69, 82, 86, 69, 
	82, 0
};

static const char _memc_flush_all_single_lengths[] = {
	0, 4, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 2, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 0
};

static const char _memc_flush_all_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const char _memc_flush_all_index_offsets[] = {
	0, 0, 5, 7, 9, 11, 13, 15, 
	17, 19, 21, 23, 25, 27, 29, 31, 
	34, 36, 38, 40, 42, 44, 46, 48, 
	50, 52, 54, 56, 58, 60, 62
};

static const char _memc_flush_all_indicies[] = {
	0, 2, 3, 4, 1, 5, 1, 6, 
	1, 7, 1, 8, 1, 9, 1, 10, 
	1, 11, 1, 12, 1, 13, 1, 14, 
	1, 15, 1, 16, 1, 17, 16, 18, 
	17, 16, 19, 1, 20, 1, 21, 1, 
	22, 1, 23, 1, 18, 1, 24, 1, 
	25, 1, 26, 1, 27, 1, 28, 1, 
	29, 1, 30, 1, 9, 1, 1, 0
};

static const char _memc_flush_all_trans_targs[] = {
	2, 0, 16, 22, 25, 3, 4, 5, 
	6, 7, 8, 9, 10, 11, 12, 13, 
	14, 15, 30, 17, 18, 19, 20, 21, 
	23, 24, 30, 26, 27, 28, 29
};

static const char _memc_flush_all_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 1, 0, 0, 0, 0, 0, 
	0, 0, 2, 0, 0, 0, 0
};

static const int memc_flush_all_start = 1;
static const int memc_flush_all_first_final = 30;
static const int memc_flush_all_error = 0;



#line 28 "src/ngx_http_memc_response.rl"


#line 30 "src/ngx_http_memc_response.rl"

#line 203 "src/ngx_http_memc_response.c"
static const char _memc_version_key_offsets[] = {
	0, 0, 4, 5, 6, 7, 8, 9, 
	10, 11, 12, 13, 14, 15, 16, 17, 
	19, 20, 21, 22, 23, 24, 25, 26, 
	27, 28, 29, 30, 31, 32, 33, 34, 
	35, 36, 37, 38, 39, 41, 43, 43, 
	43
};

static const short _memc_version_trans_keys[] = {
	67, 69, 83, 86, 76, 73, 69, 78, 
	84, 95, 69, 82, 82, 79, 82, 32, 
	13, 10, 13, 82, 82, 79, 82, 13, 
	10, 69, 82, 86, 69, 82, 69, 82, 
	83, 73, 79, 78, 32, 13, 13, 10, 
	13, 10, 13, 13, 0
};

static const char _memc_version_single_lengths[] = {
	0, 4, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 2, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 2, 2, 0, 0, 
	1
};

static const char _memc_version_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0
};

static const char _memc_version_index_offsets[] = {
	0, 0, 5, 7, 9, 11, 13, 15, 
	17, 19, 21, 23, 25, 27, 29, 31, 
	34, 36, 38, 40, 42, 44, 46, 48, 
	50, 52, 54, 56, 58, 60, 62, 64, 
	66, 68, 70, 72, 74, 77, 80, 81, 
	82
};

static const char _memc_version_indicies[] = {
	0, 2, 3, 4, 1, 5, 1, 6, 
	1, 7, 1, 8, 1, 9, 1, 10, 
	1, 11, 1, 12, 1, 13, 1, 14, 
	1, 15, 1, 16, 1, 17, 16, 18, 
	17, 16, 19, 1, 20, 1, 21, 1, 
	22, 1, 23, 1, 18, 1, 24, 1, 
	25, 1, 26, 1, 27, 1, 9, 1, 
	28, 1, 29, 1, 30, 1, 31, 1, 
	32, 1, 33, 1, 34, 1, 36, 35, 
	37, 35, 38, 37, 35, 39, 37, 35, 
	35, 1, 37, 35, 0
};

static const char _memc_version_trans_targs[] = {
	2, 0, 16, 22, 27, 3, 4, 5, 
	6, 7, 8, 9, 10, 11, 12, 13, 
	14, 15, 39, 17, 18, 19, 20, 21, 
	23, 24, 25, 26, 28, 29, 30, 31, 
	32, 33, 34, 35, 37, 36, 40, 38
};

static const char _memc_version_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 1, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 2, 0
};

static const int memc_version_start = 1;
static const int memc_version_first_final = 39;
static const int memc_version_error = 0;



#line 31 "src/ngx_http_memc_response.rl"


#line 33 "src/ngx_http_memc_response.rl"

#line 291 "src/ngx_http_memc_response.c"
static const char _memc_stats_key_offsets[] = {
	0, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 11, 12, 13, 14, 15, 16, 
	18, 20, 21, 22, 23, 24, 25, 26, 
	27, 28, 30, 31, 32, 33, 34, 35, 
	36, 37, 38, 39, 41, 43, 45, 47, 
	48, 50, 52, 52, 52
};

static const short _memc_stats_trans_keys[] = {
	67, 69, 83, 76, 73, 69, 78, 84, 
	95, 69, 82, 82, 79, 82, 32, 13, 
	10, 13, 78, 82, 68, 13, 10, 82, 
	79, 82, 13, 10, 69, 84, 82, 86, 
	69, 82, 65, 84, 32, 13, 13, 10, 
	13, 13, 69, 13, 78, 13, 68, 13, 
	10, 13, 10, 13, 13, 69, 0
};

static const char _memc_stats_single_lengths[] = {
	0, 3, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 2, 
	2, 1, 1, 1, 1, 1, 1, 1, 
	1, 2, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 2, 2, 2, 2, 1, 
	2, 2, 0, 0, 2
};

static const char _memc_stats_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0
};

static const unsigned char _memc_stats_index_offsets[] = {
	0, 0, 4, 6, 8, 10, 12, 14, 
	16, 18, 20, 22, 24, 26, 28, 30, 
	33, 36, 38, 40, 42, 44, 46, 48, 
	50, 52, 55, 57, 59, 61, 63, 65, 
	67, 69, 71, 73, 76, 79, 82, 85, 
	87, 90, 93, 94, 95
};

static const char _memc_stats_indicies[] = {
	0, 2, 3, 1, 4, 1, 5, 1, 
	6, 1, 7, 1, 8, 1, 9, 1, 
	10, 1, 11, 1, 12, 1, 13, 1, 
	14, 1, 15, 1, 16, 15, 17, 16, 
	15, 18, 19, 1, 20, 1, 21, 1, 
	22, 1, 23, 1, 24, 1, 25, 1, 
	26, 1, 17, 1, 27, 28, 1, 29, 
	1, 30, 1, 31, 1, 8, 1, 32, 
	1, 33, 1, 34, 1, 36, 35, 37, 
	35, 38, 37, 35, 37, 39, 35, 37, 
	40, 35, 37, 41, 35, 42, 35, 43, 
	37, 35, 44, 37, 35, 35, 1, 37, 
	39, 35, 0
};

static const char _memc_stats_trans_targs[] = {
	2, 0, 16, 25, 3, 4, 5, 6, 
	7, 8, 9, 10, 11, 12, 13, 14, 
	15, 43, 17, 20, 18, 19, 43, 21, 
	22, 23, 24, 26, 30, 27, 28, 29, 
	31, 32, 33, 34, 41, 35, 36, 37, 
	38, 39, 40, 44, 42
};

static const char _memc_stats_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 1, 0, 0, 0, 0, 2, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 2, 0
};

static const int memc_stats_start = 1;
static const int memc_stats_first_final = 43;
static const int memc_stats_error = 0;



#line 34 "src/ngx_http_memc_response.rl"


#line 36 "src/ngx_http_memc_response.rl"

#line 384 "src/ngx_http_memc_response.c"
static const char _memc_delete_key_offsets[] = {
	0, 0, 5, 6, 7, 8, 9, 10, 
	11, 12, 13, 14, 15, 16, 17, 18, 
	20, 21, 22, 23, 24, 25, 26, 27, 
	28, 29, 30, 31, 32, 33, 34, 35, 
	36, 37, 38, 39, 40, 41, 42, 43, 
	44, 45, 46, 47, 48, 49
};

static const short _memc_delete_trans_keys[] = {
	67, 68, 69, 78, 83, 76, 73, 69, 
	78, 84, 95, 69, 82, 82, 79, 82, 
	32, 13, 10, 13, 69, 76, 69, 84, 
	69, 68, 13, 10, 82, 82, 79, 82, 
	13, 10, 79, 84, 95, 70, 79, 85, 
	78, 68, 13, 10, 69, 82, 86, 69, 
	82, 0
};

static const char _memc_delete_single_lengths[] = {
	0, 5, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 2, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 0
};

static const char _memc_delete_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0
};

static const unsigned char _memc_delete_index_offsets[] = {
	0, 0, 6, 8, 10, 12, 14, 16, 
	18, 20, 22, 24, 26, 28, 30, 32, 
	35, 37, 39, 41, 43, 45, 47, 49, 
	51, 53, 55, 57, 59, 61, 63, 65, 
	67, 69, 71, 73, 75, 77, 79, 81, 
	83, 85, 87, 89, 91, 93
};

static const char _memc_delete_trans_targs[] = {
	2, 16, 24, 30, 40, 0, 3, 0, 
	4, 0, 5, 0, 6, 0, 7, 0, 
	8, 0, 9, 0, 10, 0, 11, 0, 
	12, 0, 13, 0, 14, 0, 15, 14, 
	45, 15, 14, 17, 0, 18, 0, 19, 
	0, 20, 0, 21, 0, 22, 0, 23, 
	0, 45, 0, 25, 0, 26, 0, 27, 
	0, 28, 0, 29, 0, 45, 0, 31, 
	0, 32, 0, 33, 0, 34, 0, 35, 
	0, 36, 0, 37, 0, 38, 0, 39, 
	0, 45, 0, 41, 0, 42, 0, 43, 
	0, 44, 0, 7, 0, 0, 0
};

static const char _memc_delete_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 2, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 1, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 3, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const int memc_delete_start = 1;
static const int memc_delete_first_final = 45;
static const int memc_delete_error = 0;



#line 37 "src/ngx_http_memc_response.rl"


#line 39 "src/ngx_http_memc_response.rl"

#line 473 "src/ngx_http_memc_response.c"
static const char _memc_incr_decr_key_offsets[] = {
	0, 0, 6, 10, 11, 13, 14, 15, 
	16, 17, 18, 19, 20, 21, 22, 23, 
	24, 25, 26, 28, 29, 30, 31, 32, 
	33, 34, 35, 36, 37, 38, 39, 40, 
	41, 42, 43, 44, 45, 46, 47, 48, 
	49
};

static const short _memc_incr_decr_trans_keys[] = {
	67, 69, 78, 83, 48, 57, 13, 32, 
	48, 57, 10, 13, 32, 76, 73, 69, 
	78, 84, 95, 69, 82, 82, 79, 82, 
	32, 13, 10, 13, 82, 82, 79, 82, 
	13, 10, 79, 84, 95, 70, 79, 85, 
	78, 68, 13, 10, 69, 82, 86, 69, 
	82, 0
};

static const char _memc_incr_decr_single_lengths[] = {
	0, 4, 2, 1, 2, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 2, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	0
};

static const char _memc_incr_decr_range_lengths[] = {
	0, 1, 1, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0
};

static const char _memc_incr_decr_index_offsets[] = {
	0, 0, 6, 10, 12, 15, 17, 19, 
	21, 23, 25, 27, 29, 31, 33, 35, 
	37, 39, 41, 44, 46, 48, 50, 52, 
	54, 56, 58, 60, 62, 64, 66, 68, 
	70, 72, 74, 76, 78, 80, 82, 84, 
	86
};

static const char _memc_incr_decr_indicies[] = {
	2, 3, 4, 5, 0, 1, 6, 7, 
	0, 1, 8, 1, 6, 7, 1, 9, 
	1, 10, 1, 11, 1, 12, 1, 13, 
	1, 14, 1, 15, 1, 16, 1, 17, 
	1, 18, 1, 19, 1, 20, 1, 21, 
	20, 22, 21, 20, 23, 1, 24, 1, 
	25, 1, 26, 1, 27, 1, 22, 1, 
	28, 1, 29, 1, 30, 1, 31, 1, 
	32, 1, 33, 1, 34, 1, 35, 1, 
	36, 1, 37, 1, 38, 1, 39, 1, 
	40, 1, 41, 1, 13, 1, 1, 0
};

static const char _memc_incr_decr_trans_targs[] = {
	2, 0, 5, 19, 25, 35, 3, 4, 
	40, 6, 7, 8, 9, 10, 11, 12, 
	13, 14, 15, 16, 17, 18, 40, 20, 
	21, 22, 23, 24, 26, 27, 28, 29, 
	30, 31, 32, 33, 34, 40, 36, 37, 
	38, 39
};

static const char _memc_incr_decr_trans_actions[] = {
	1, 0, 1, 1, 1, 1, 1, 1, 
	2, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 3, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 4, 1, 1, 
	1, 1
};

static const int memc_incr_decr_start = 1;
static const int memc_incr_decr_first_final = 40;
static const int memc_incr_decr_error = 0;



#line 40 "src/ngx_http_memc_response.rl"


u_char  ngx_http_memc_end[] = CRLF "END" CRLF;


static u_char *parse_memc_storage(int *cs_addr, u_char *p, u_char *pe,
        ngx_uint_t *status_addr, unsigned *done_addr);
static u_char *parse_memc_flush_all(int *cs_addr, u_char *p, u_char *pe,
        ngx_uint_t *status_addr, unsigned *done_addr);
static u_char *parse_memc_version(int *cs_addr, u_char *p, u_char *pe,
        ngx_uint_t *status_addr, unsigned *done_addr);
static u_char *parse_memc_stats(int *cs_addr, u_char *p, u_char *pe,
        ngx_uint_t *status_addr, unsigned *done_addr);
static u_char *parse_memc_delete(int *cs_addr, u_char *p, u_char *pe,
        ngx_uint_t *status_addr, unsigned *done_addr);
static u_char *parse_memc_incr_decr(int *cs_addr, u_char *p, u_char *pe,
        ngx_uint_t *status_addr, unsigned *done_addr);
static ngx_int_t ngx_http_memc_write_simple_response(ngx_http_request_t *r,
        ngx_http_upstream_t *u, ngx_http_memc_ctx_t *ctx,
        ngx_uint_t status, ngx_str_t *resp);


ngx_int_t
ngx_http_memc_process_simple_header(ngx_http_request_t *r)
{
    ngx_int_t                rc;
    int                      cs;
    s_char                  *p;
    s_char                  *pe;
    s_char                  *orig;
    ngx_str_t                resp;
    ngx_http_upstream_t     *u;
    ngx_http_memc_ctx_t     *ctx;
    ngx_uint_t               status;
    unsigned                 done = 0;
    int                      error_state;
    int                      final_state;

    status = NGX_HTTP_OK;

    dd("process simple cmd header");

    ctx = ngx_http_get_module_ctx(r, ngx_http_memc_module);

    if (ctx->parser_state == NGX_ERROR) {
        dd("reinit state");

        if (ctx->is_storage_cmd) {
            dd("init memc_storage machine...");

            
#line 91 "src/ngx_http_memc_response.rl"
            
#line 613 "src/ngx_http_memc_response.c"
	{
	cs = memc_storage_start;
	}

#line 92 "src/ngx_http_memc_response.rl"

        } else if (ctx->cmd == ngx_http_memc_cmd_flush_all) {
            dd("init memc_flush_all machine...");

            
#line 97 "src/ngx_http_memc_response.rl"
            
#line 626 "src/ngx_http_memc_response.c"
	{
	cs = memc_flush_all_start;
	}

#line 98 "src/ngx_http_memc_response.rl"

        } else if (ctx->cmd == ngx_http_memc_cmd_version) {
            dd("init memc_version machine...");

            
#line 103 "src/ngx_http_memc_response.rl"
            
#line 639 "src/ngx_http_memc_response.c"
	{
	cs = memc_version_start;
	}

#line 104 "src/ngx_http_memc_response.rl"

        } else if (ctx->cmd == ngx_http_memc_cmd_stats) {
            dd("init memc_stats machine...");

            
#line 109 "src/ngx_http_memc_response.rl"
            
#line 652 "src/ngx_http_memc_response.c"
	{
	cs = memc_stats_start;
	}

#line 110 "src/ngx_http_memc_response.rl"

        } else if (ctx->cmd == ngx_http_memc_cmd_delete) {
            dd("init memc_delete machine...");

            
#line 115 "src/ngx_http_memc_response.rl"
            
#line 665 "src/ngx_http_memc_response.c"
	{
	cs = memc_delete_start;
	}

#line 116 "src/ngx_http_memc_response.rl"

        } else if (ctx->cmd == ngx_http_memc_cmd_incr
                || ctx->cmd == ngx_http_memc_cmd_decr)
        {
            dd("init memc_incr_decr machine...");

            
#line 123 "src/ngx_http_memc_response.rl"
            
#line 680 "src/ngx_http_memc_response.c"
	{
	cs = memc_incr_decr_start;
	}

#line 124 "src/ngx_http_memc_response.rl"

        } else {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
              "unrecognized memcached command in "
              "ngx_http_memc_process_simple_header: \"%V\"",
              &ctx->cmd_str);

            return NGX_ERROR; /* this results in 500 status */
        }

    } else {
        cs = ctx->parser_state;
    }

    u = r->upstream;

    orig = (s_char *) u->buffer.pos;

    p  = (s_char *) u->buffer.pos;
    pe = (s_char *) u->buffer.last;

    dd("buffer len: %d", (int) (pe - p));

    if (ctx->is_storage_cmd) {
        error_state = memc_storage_error;
        final_state = memc_storage_first_final;

        p = (s_char *) parse_memc_storage(&cs, (u_char *) p, (u_char *) pe,
                &status, &done);

    } else if (ctx->cmd == ngx_http_memc_cmd_flush_all) {
        error_state = memc_flush_all_error;
        final_state = memc_flush_all_first_final;

        p = (s_char *) parse_memc_flush_all(&cs, (u_char *) p, (u_char *) pe,
                &status, &done);

    } else if (ctx->cmd == ngx_http_memc_cmd_version) {
        error_state = memc_version_error;
        final_state = memc_version_first_final;

        p = (s_char *) parse_memc_version(&cs, (u_char *) p, (u_char *) pe,
                &status, &done);

    } else if (ctx->cmd == ngx_http_memc_cmd_stats) {
        error_state = memc_stats_error;
        final_state = memc_stats_first_final;

        p = (s_char *) parse_memc_stats(&cs, (u_char *) p, (u_char *) pe,
                &status, &done);

    } else if (ctx->cmd == ngx_http_memc_cmd_delete) {
        error_state = memc_delete_error;
        final_state = memc_delete_first_final;

        p = (s_char *) parse_memc_delete(&cs, (u_char *) p, (u_char *) pe,
                &status, &done);

    } else if (ctx->cmd == ngx_http_memc_cmd_incr
            || ctx->cmd == ngx_http_memc_cmd_decr)
    {
        error_state = memc_incr_decr_error;
        final_state = memc_incr_decr_first_final;

        p = (s_char *) parse_memc_incr_decr(&cs, (u_char *) p, (u_char *) pe,
                &status, &done);

    } else {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
          "unrecognized memcached command in "
          "ngx_http_memc_process_simple_header: \"%V\"",
          &ctx->cmd_str);

        return NGX_ERROR; /* this results in 500 status */
    }

    ctx->parser_state = cs;

    resp.data = u->buffer.start;
    resp.len  = (u_char *) p - resp.data;

    u->buffer.pos = (u_char *) p;

    dd("machine state: %d (done: %d)", cs, done);
    dd("memcached response: (len: %d) %s", (int) resp.len, resp.data);

    if (done || cs >= final_state) {
        dd("memcached response parsed (resp.len: %d)", (int) resp.len);

        rc = ngx_http_memc_write_simple_response(r, u, ctx, status, &resp);

        return rc;
    }

    if (cs == error_state) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                  "memcached sent invalid response for command \"%V\" "
                  "at pos %O: %V", &ctx->cmd_str, (off_t) (p - orig), &resp);

        status = NGX_HTTP_BAD_GATEWAY;
        u->headers_in.status_n = status;
        u->state->status = status;

        /* u->headers_in.status_n will be the final status */
        return NGX_OK;
    }

    dd("we need more data to proceed (returned NGX_AGAIN)");

    return NGX_AGAIN;
}


ngx_int_t
ngx_http_memc_empty_filter_init(void *data)
{
    ngx_http_memc_ctx_t  *ctx = data;
    ngx_http_upstream_t  *u;

    u = ctx->request->upstream;

    u->length = 0;

    /* to persuade ngx_http_upstream_keepalive (if any)
       to cache the connection if the status is neither
       200 nor 404. */
    if (u->headers_in.status_n == NGX_HTTP_CREATED) {
        u->headers_in.status_n = NGX_HTTP_OK;
    }

    return NGX_OK;
}

ngx_int_t
ngx_http_memc_empty_filter(void *data, ssize_t bytes)
{
    ngx_http_memc_ctx_t  *ctx = data;
    ngx_http_upstream_t  *u;

    u = ctx->request->upstream;

    /* recover the buffer for subrequests in memory */
    u->buffer.last += ctx->body_length;

    return NGX_OK;
}


ngx_int_t
ngx_http_memc_get_cmd_filter_init(void *data)
{
    ngx_http_memc_ctx_t  *ctx = data;

    ngx_http_upstream_t  *u;

    u = ctx->request->upstream;

    dd("filter init: u->length: %d", (int) u->length);

    u->length = u->headers_in.content_length_n + NGX_HTTP_MEMC_END;

    dd("filter init (2): u->length: %d", (int) u->length);

    return NGX_OK;
}


ngx_int_t
ngx_http_memc_get_cmd_filter(void *data, ssize_t bytes)
{
    ngx_http_memc_ctx_t  *ctx = data;

    u_char               *last;
    ngx_buf_t            *b;
    ngx_chain_t          *cl, **ll;
    ngx_http_upstream_t  *u;

    u = ctx->request->upstream;
    b = &u->buffer;

    if (u->length == ctx->rest) {

        if (ngx_strncmp(b->last,
                        ngx_http_memc_end + NGX_HTTP_MEMC_END - ctx->rest,
                        bytes) != 0)
        {
            ngx_log_error(NGX_LOG_ERR, ctx->request->connection->log, 0,
                          "memcached sent invalid trailer");

            u->length = 0;
            ctx->rest = 0;

            return NGX_OK;
        }

        u->length -= bytes;
        ctx->rest -= bytes;

#if defined(nginx_version) && nginx_version >= 1001004
        if (u->length == 0) {
            u->keepalive = 1;
        }
#endif

        return NGX_OK;
    }

    for (cl = u->out_bufs, ll = &u->out_bufs; cl; cl = cl->next) {
        ll = &cl->next;
    }

    cl = ngx_chain_get_free_buf(ctx->request->pool, &u->free_bufs);
    if (cl == NULL) {
        return NGX_ERROR;
    }

    cl->buf->flush = 1;
    cl->buf->memory = 1;

    *ll = cl;

    last = b->last;
    cl->buf->pos = last;
    b->last += bytes;
    cl->buf->last = b->last;
    cl->buf->tag = u->output.tag;

    ngx_log_debug4(NGX_LOG_DEBUG_HTTP, ctx->request->connection->log, 0,
                   "memcached filter bytes:%z size:%z length:%z rest:%z",
                   bytes, b->last - b->pos, u->length, ctx->rest);

    if (bytes <= (ssize_t) (u->length - NGX_HTTP_MEMC_END)) {
        u->length -= bytes;
        return NGX_OK;
    }

    last += u->length - NGX_HTTP_MEMC_END;

    if (ngx_strncmp(last, ngx_http_memc_end, b->last - last) != 0) {
        ngx_log_error(NGX_LOG_ERR, ctx->request->connection->log, 0,
                      "memcached sent invalid trailer");

#if defined(nginx_version) && nginx_version >= 1001004
        b->last = last;
        cl->buf->last = last;
        u->length = 0;
        ctx->rest = 0;

        return NGX_OK;
#endif
    }

    ctx->rest -= b->last - last;
    b->last = last;
    cl->buf->last = last;
    u->length = ctx->rest;

#if defined(nginx_version) && nginx_version >= 1001004
    if (u->length == 0) {
        u->keepalive = 1;
    }
#endif

    return NGX_OK;
}


ngx_int_t
ngx_http_memc_process_get_cmd_header(ngx_http_request_t *r)
{
    ngx_http_memc_loc_conf_t        *conf;
    u_char                          *p, *len;
    ngx_str_t                        line;
    ngx_http_upstream_t             *u;
    ngx_http_memc_ctx_t             *ctx;
    ngx_http_variable_value_t       *flags_vv;

    u = r->upstream;

    dd("process header: u->length: %u", (unsigned) u->length);

    for (p = u->buffer.pos; p < u->buffer.last; p++) {
        if (*p == LF) {
            goto found;
        }
    }

    return NGX_AGAIN;

found:

    *p = '\0';

    line.len = p - u->buffer.pos - 1;
    line.data = u->buffer.pos;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "memcached: \"%V\"", &line);

    p = u->buffer.pos;

    ctx = ngx_http_get_module_ctx(r, ngx_http_memc_module);

    if (ngx_strncmp(p, "VALUE ", sizeof("VALUE ") - 1) == 0) {

        p += sizeof("VALUE ") - 1;

        if (ngx_strncmp(p, ctx->key.data, ctx->key.len) != 0) {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                          "memcached sent invalid key in response \"%V\" "
                          "for key \"%V\"",
                          &line, &ctx->key);

            return NGX_HTTP_UPSTREAM_INVALID_HEADER;
        }

        p += ctx->key.len;

        if (*p++ != ' ') {
            goto no_valid;
        }

        /* save flags */

        flags_vv = ctx->memc_flags_vv;

        if (flags_vv == NULL) {
            return NGX_ERROR;
        }

        if (flags_vv->not_found) {
            flags_vv->not_found = 0;
            flags_vv->valid = 1;
            flags_vv->no_cacheable = 0;
        }

        flags_vv->data = p;

        while (*p) {
            if (*p++ == ' ') {
                flags_vv->len = p - 1 - flags_vv->data;
                conf = ngx_http_get_module_loc_conf(r, ngx_http_memc_module);

                if (conf->flags_to_last_modified) {
                    r->headers_out.last_modified_time =
                            ngx_atotm(flags_vv->data, flags_vv->len);
                }

                goto length;
            }
        }

        goto no_valid;

    length:

        len = p;

        while (*p && *p++ != CR) { /* void */ }

#if defined(nginx_version) && nginx_version >= 1001004
        u->headers_in.content_length_n = ngx_atoof(len, p - len - 1);
        if (u->headers_in.content_length_n == -1) {
#else
        r->headers_out.content_length_n = ngx_atoof(len, p - len - 1);
        if (r->headers_out.content_length_n == -1) {
#endif
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                          "memcached sent invalid length in response \"%V\" "
                          "for key \"%V\"",
                          &line, &ctx->key);
            return NGX_HTTP_UPSTREAM_INVALID_HEADER;
        }

        u->headers_in.status_n = NGX_HTTP_OK;
        u->state->status = NGX_HTTP_OK;
        u->buffer.pos = p + 1;

        return NGX_OK;
    }

    if (ngx_strcmp(p, "END\x0d") == 0) {
        ngx_log_error(NGX_LOG_INFO, r->connection->log, 0,
                      "key: \"%V\" was not found by memcached", &ctx->key);

        u->headers_in.status_n = NGX_HTTP_NOT_FOUND;
        u->state->status = NGX_HTTP_NOT_FOUND;

#if defined(nginx_version) && nginx_version >= 1001004
        u->keepalive = 1;
#endif

        return NGX_OK;
    }

no_valid:

    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                  "memcached sent invalid response: \"%V\"", &line);

    return NGX_HTTP_UPSTREAM_INVALID_HEADER;
}


static ngx_int_t
ngx_http_memc_write_simple_response(ngx_http_request_t *r,
    ngx_http_upstream_t *u, ngx_http_memc_ctx_t *ctx, ngx_uint_t status,
    ngx_str_t *resp)
{
    ngx_chain_t             *cl, **ll;

    for (cl = u->out_bufs, ll = &u->out_bufs; cl; cl = cl->next) {
        ll = &cl->next;
    }

    cl = ngx_chain_get_free_buf(r->pool, &u->free_bufs);
    if (cl == NULL) {
        return NGX_ERROR;
    }

    cl->buf->flush = 1;
    cl->buf->memory = 1;
    cl->buf->pos = resp->data;
    cl->buf->last = cl->buf->pos + resp->len;

    *ll = cl;

    /* for subrequests in memory */
    u->buffer.pos = resp->data;
    u->buffer.last = resp->data + resp->len;
    ctx->body_length = resp->len;

#if defined(nginx_version) && nginx_version >= 1001004
    u->headers_in.content_length_n = resp->len;
    u->keepalive = 1;
#else
    r->headers_out.content_length_n = resp->len;
#endif

    u->headers_in.status_n = status;
    u->state->status = status;

    return NGX_OK;
}


static u_char *
parse_memc_storage(int *cs_addr, u_char *p, u_char *pe,
    ngx_uint_t *status_addr, unsigned *done_addr)
{
    int cs = *cs_addr;

    
#line 577 "src/ngx_http_memc_response.rl"
    
#line 578 "src/ngx_http_memc_response.rl"
    
#line 1143 "src/ngx_http_memc_response.c"
	{
	int _klen;
	const char *_keys;
	int _trans;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _memc_storage_trans_keys + _memc_storage_key_offsets[cs];
	_trans = _memc_storage_index_offsets[cs];

	_klen = _memc_storage_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _memc_storage_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	cs = _memc_storage_trans_targs[_trans];

	if ( _memc_storage_trans_actions[_trans] == 0 )
		goto _again;

	switch ( _memc_storage_trans_actions[_trans] ) {
	case 2:
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
	break;
	case 1:
#line 4 "src/memc_common.rl"
	{
        dd("caught error...");
        dd("machine state: %d", cs);

        *status_addr = NGX_HTTP_BAD_GATEWAY;
    }
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
	break;
	case 4:
#line 31 "src/memc_common.rl"
	{
        dd("status set to 201");

        *status_addr = NGX_HTTP_CREATED;
    }
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
	break;
	case 3:
#line 37 "src/memc_common.rl"
	{
        dd("status set to 404");

        *status_addr = NGX_HTTP_NOT_FOUND;
    }
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
	break;
#line 1256 "src/ngx_http_memc_response.c"
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 579 "src/ngx_http_memc_response.rl"

    *cs_addr = cs;

    return p;
}


static u_char *
parse_memc_flush_all(int *cs_addr, u_char *p, u_char *pe,
    ngx_uint_t *status_addr, unsigned *done_addr)
{
    int cs = *cs_addr;

    
#line 593 "src/ngx_http_memc_response.rl"
    
#line 594 "src/ngx_http_memc_response.rl"
    
#line 1287 "src/ngx_http_memc_response.c"
	{
	int _klen;
	const short *_keys;
	int _trans;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _memc_flush_all_trans_keys + _memc_flush_all_key_offsets[cs];
	_trans = _memc_flush_all_index_offsets[cs];

	_klen = _memc_flush_all_single_lengths[cs];
	if ( _klen > 0 ) {
		const short *_lower = _keys;
		const short *_mid;
		const short *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _memc_flush_all_range_lengths[cs];
	if ( _klen > 0 ) {
		const short *_lower = _keys;
		const short *_mid;
		const short *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _memc_flush_all_indicies[_trans];
	cs = _memc_flush_all_trans_targs[_trans];

	if ( _memc_flush_all_trans_actions[_trans] == 0 )
		goto _again;

	switch ( _memc_flush_all_trans_actions[_trans] ) {
	case 2:
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
	break;
	case 1:
#line 4 "src/memc_common.rl"
	{
        dd("caught error...");
        dd("machine state: %d", cs);

        *status_addr = NGX_HTTP_BAD_GATEWAY;
    }
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
	break;
#line 1375 "src/ngx_http_memc_response.c"
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 595 "src/ngx_http_memc_response.rl"

    *cs_addr = cs;

    return p;
}


static u_char *
parse_memc_version(int *cs_addr, u_char *p, u_char *pe,
    ngx_uint_t *status_addr, unsigned *done_addr)
{
    int cs = *cs_addr;

    
#line 609 "src/ngx_http_memc_response.rl"
    
#line 610 "src/ngx_http_memc_response.rl"
    
#line 1406 "src/ngx_http_memc_response.c"
	{
	int _klen;
	const short *_keys;
	int _trans;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _memc_version_trans_keys + _memc_version_key_offsets[cs];
	_trans = _memc_version_index_offsets[cs];

	_klen = _memc_version_single_lengths[cs];
	if ( _klen > 0 ) {
		const short *_lower = _keys;
		const short *_mid;
		const short *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _memc_version_range_lengths[cs];
	if ( _klen > 0 ) {
		const short *_lower = _keys;
		const short *_mid;
		const short *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _memc_version_indicies[_trans];
	cs = _memc_version_trans_targs[_trans];

	if ( _memc_version_trans_actions[_trans] == 0 )
		goto _again;

	switch ( _memc_version_trans_actions[_trans] ) {
	case 2:
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
	break;
	case 1:
#line 4 "src/memc_common.rl"
	{
        dd("caught error...");
        dd("machine state: %d", cs);

        *status_addr = NGX_HTTP_BAD_GATEWAY;
    }
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
	break;
#line 1494 "src/ngx_http_memc_response.c"
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 611 "src/ngx_http_memc_response.rl"

    *cs_addr = cs;

    return p;
}


static u_char *
parse_memc_stats(int *cs_addr, u_char *p, u_char *pe, ngx_uint_t *status_addr,
    unsigned *done_addr)
{
    int cs = *cs_addr;

    
#line 625 "src/ngx_http_memc_response.rl"
    
#line 626 "src/ngx_http_memc_response.rl"
    
#line 1525 "src/ngx_http_memc_response.c"
	{
	int _klen;
	const short *_keys;
	int _trans;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _memc_stats_trans_keys + _memc_stats_key_offsets[cs];
	_trans = _memc_stats_index_offsets[cs];

	_klen = _memc_stats_single_lengths[cs];
	if ( _klen > 0 ) {
		const short *_lower = _keys;
		const short *_mid;
		const short *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _memc_stats_range_lengths[cs];
	if ( _klen > 0 ) {
		const short *_lower = _keys;
		const short *_mid;
		const short *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _memc_stats_indicies[_trans];
	cs = _memc_stats_trans_targs[_trans];

	if ( _memc_stats_trans_actions[_trans] == 0 )
		goto _again;

	switch ( _memc_stats_trans_actions[_trans] ) {
	case 2:
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
	break;
	case 1:
#line 4 "src/memc_common.rl"
	{
        dd("caught error...");
        dd("machine state: %d", cs);

        *status_addr = NGX_HTTP_BAD_GATEWAY;
    }
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
	break;
#line 1613 "src/ngx_http_memc_response.c"
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 627 "src/ngx_http_memc_response.rl"

    *cs_addr = cs;

    return p;
}


static u_char *
parse_memc_delete(int *cs_addr, u_char *p, u_char *pe, ngx_uint_t *status_addr,
    unsigned *done_addr)
{
    int cs = *cs_addr;

    
#line 641 "src/ngx_http_memc_response.rl"
    
#line 642 "src/ngx_http_memc_response.rl"
    
#line 1644 "src/ngx_http_memc_response.c"
	{
	int _klen;
	const short *_keys;
	int _trans;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _memc_delete_trans_keys + _memc_delete_key_offsets[cs];
	_trans = _memc_delete_index_offsets[cs];

	_klen = _memc_delete_single_lengths[cs];
	if ( _klen > 0 ) {
		const short *_lower = _keys;
		const short *_mid;
		const short *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _memc_delete_range_lengths[cs];
	if ( _klen > 0 ) {
		const short *_lower = _keys;
		const short *_mid;
		const short *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	cs = _memc_delete_trans_targs[_trans];

	if ( _memc_delete_trans_actions[_trans] == 0 )
		goto _again;

	switch ( _memc_delete_trans_actions[_trans] ) {
	case 2:
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
	break;
	case 1:
#line 4 "src/memc_common.rl"
	{
        dd("caught error...");
        dd("machine state: %d", cs);

        *status_addr = NGX_HTTP_BAD_GATEWAY;
    }
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
	break;
	case 3:
#line 37 "src/memc_common.rl"
	{
        dd("status set to 404");

        *status_addr = NGX_HTTP_NOT_FOUND;
    }
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
	break;
#line 1744 "src/ngx_http_memc_response.c"
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 643 "src/ngx_http_memc_response.rl"

    *cs_addr = cs;

    return p;
}


static u_char *
parse_memc_incr_decr(int *cs_addr, u_char *p, u_char *pe,
    ngx_uint_t *status_addr, unsigned *done_addr)
{
    int cs = *cs_addr;

    
#line 657 "src/ngx_http_memc_response.rl"
    
#line 658 "src/ngx_http_memc_response.rl"
    
#line 1775 "src/ngx_http_memc_response.c"
	{
	int _klen;
	const short *_keys;
	int _trans;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _memc_incr_decr_trans_keys + _memc_incr_decr_key_offsets[cs];
	_trans = _memc_incr_decr_index_offsets[cs];

	_klen = _memc_incr_decr_single_lengths[cs];
	if ( _klen > 0 ) {
		const short *_lower = _keys;
		const short *_mid;
		const short *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _memc_incr_decr_range_lengths[cs];
	if ( _klen > 0 ) {
		const short *_lower = _keys;
		const short *_mid;
		const short *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _memc_incr_decr_indicies[_trans];
	cs = _memc_incr_decr_trans_targs[_trans];

	if ( _memc_incr_decr_trans_actions[_trans] == 0 )
		goto _again;

	switch ( _memc_incr_decr_trans_actions[_trans] ) {
	case 1:
#line 26 "src/memc_common.rl"
	{
        dd("state %d, left %d, reading char '%c'", cs,
        (int) (pe - p), *p);
    }
	break;
	case 3:
#line 4 "src/memc_common.rl"
	{
        dd("caught error...");
        dd("machine state: %d", cs);

        *status_addr = NGX_HTTP_BAD_GATEWAY;
    }
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
#line 26 "src/memc_common.rl"
	{
        dd("state %d, left %d, reading char '%c'", cs,
        (int) (pe - p), *p);
    }
	break;
	case 2:
#line 31 "src/memc_common.rl"
	{
        dd("status set to 201");

        *status_addr = NGX_HTTP_CREATED;
    }
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
#line 26 "src/memc_common.rl"
	{
        dd("state %d, left %d, reading char '%c'", cs,
        (int) (pe - p), *p);
    }
	break;
	case 4:
#line 37 "src/memc_common.rl"
	{
        dd("status set to 404");

        *status_addr = NGX_HTTP_NOT_FOUND;
    }
#line 21 "src/memc_common.rl"
	{
        dd("done it!");
        *done_addr = 1;
    }
#line 26 "src/memc_common.rl"
	{
        dd("state %d, left %d, reading char '%c'", cs,
        (int) (pe - p), *p);
    }
	break;
#line 1904 "src/ngx_http_memc_response.c"
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 659 "src/ngx_http_memc_response.rl"

    *cs_addr = cs;

    return p;
}
