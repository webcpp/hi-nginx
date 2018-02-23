
#line 1 "src/ngx_http_xss_util.rl"

/*
 * Copyright (C) agentzh
 */


#ifndef DDEBUG
#define DDEBUG 0
#endif
#include "ddebug.h"

#include "ngx_http_xss_util.h"



#line 16 "src/ngx_http_xss_util.rl"

#line 21 "src/ngx_http_xss_util.c"
static const char _javascript_key_offsets[] = {
	0, 0, 6, 9, 11, 14, 18, 28
};

static const char _javascript_trans_keys[] = {
	36, 95, 65, 90, 97, 122, 46, 48, 
	57, 48, 57, 93, 48, 57, 46, 93, 
	48, 57, 36, 46, 91, 95, 48, 57, 
	65, 90, 97, 122, 0
};

static const char _javascript_single_lengths[] = {
	0, 2, 1, 0, 1, 2, 4, 0
};

static const char _javascript_range_lengths[] = {
	0, 2, 1, 1, 1, 1, 3, 0
};

static const char _javascript_index_offsets[] = {
	0, 0, 5, 8, 10, 13, 17, 25
};

static const char _javascript_trans_targs[] = {
	6, 6, 6, 6, 0, 3, 5, 0, 
	4, 0, 7, 4, 0, 3, 7, 5, 
	0, 6, 1, 2, 6, 6, 6, 6, 
	0, 0, 0
};

static const int javascript_start = 1;



#line 17 "src/ngx_http_xss_util.rl"

ngx_int_t ngx_http_xss_test_callback(u_char *data, size_t len)
{
    signed char *p = (signed char *) data;
    signed char *pe;
    int cs;

    pe = p + len;

    
#line 70 "src/ngx_http_xss_util.c"
	{
	cs = javascript_start;
	}

#line 75 "src/ngx_http_xss_util.c"
	{
	int _klen;
	const char *_keys;
	int _trans;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _javascript_trans_keys + _javascript_key_offsets[cs];
	_trans = _javascript_index_offsets[cs];

	_klen = _javascript_single_lengths[cs];
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

	_klen = _javascript_range_lengths[cs];
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
	cs = _javascript_trans_targs[_trans];

	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 38 "src/ngx_http_xss_util.rl"


    if (cs < 6 || p != pe) {
        return NGX_DECLINED;
    }

    return NGX_OK;
}
