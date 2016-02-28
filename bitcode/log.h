/**----------------------------------------------------------------------------
 * log.h
 *-----------------------------------------------------------------------------
 * initialize_log() �Լ��� ��������� ȣ���ϸ�
 *	- log level, log target (file, debugger, console, etc) ����/���� ����
 *	- log format ����/���� ����
 *  - multi thread ȯ�濡�� serialization �� ��
 *
 * log_err, con_err ���� ��ũ�θ� ����ϸ� 
 *	- debugger, console �� �޼��� ��� ����
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh,Yonghwan (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * 2015:1:12 15:26 created
**---------------------------------------------------------------------------*/
#ifndef _log_h_
#define _log_h_

#pragma warning(disable:4005)


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <sal.h>

/// @brief log level
#define log_level_debug         0
#define log_level_info          1
#define log_level_warn          2
#define log_level_critical      3
#define log_level_error         log_level_critical

//
// C like APIs
//

void
set_log_format(
	_In_ bool show_process_name, 
	_In_ bool show_pid_tid,
	_In_ bool show_function_name
	);

void
log_write_fmt(
    _In_ uint32_t log_level,
	_In_z_ const char* function,
    _In_z_ const char* fmt, 
    _In_ ...
    );


//
// define macro for convenience
//
#define log_err		log_write_fmt( log_level_error, __FUNCTION__, 
#define log_warn	log_write_fmt( log_level_warn, __FUNCTION__,  
#define log_info	log_write_fmt( log_level_info, __FUNCTION__, 
#define log_dbg		log_write_fmt( log_level_debug, __FUNCTION__, 

#define log_end		);

#endif//_log_h_