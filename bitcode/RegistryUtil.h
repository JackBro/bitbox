/**----------------------------------------------------------------------------
 * RegistryUtil.h
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by somma (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * 11:11:2011   0:10 created
**---------------------------------------------------------------------------*/
#pragma once

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

HKEY 
RUOpenKey(
    HKEY  RootKey,
    const wchar_t* SubKey,
    bool ReadOnly
    );

bool
RUCloseKey(
    HKEY Key
    );

HKEY
RUCreateKey(
    HKEY RootKey,
    const wchar_t* SubKey,
    bool ReadOnly
    );


DWORD 
RUReadDword(
    HKEY RootKey,
    const wchar_t* SubKey,
    const wchar_t* ValueName, 
    DWORD DefaultValue
    );

bool 
RUWriteDword(
HKEY RootKey,
    const wchar_t* SubKey,
    const wchar_t* ValueName, 
    DWORD Value
    );


bool
RUReadString(
    IN HKEY	RootKey,
    IN const wchar_t* SubKey,
    IN const wchar_t* ValueName,
    OUT std::wstring& Value
    );

bool
RUSetString(
    HKEY RootKey,
    const wchar_t* SubKey,
    const wchar_t* ValueName,
    const wchar_t* Value,           // byte buffer
    DWORD cbValue          // count byte
    );


bool
RUSetExpandString(
    HKEY RootKey,
    const wchar_t* SubKey,
    const wchar_t* value_name,
    const wchar_t* value,           // byte buffer
    DWORD cbValue          // count byte
    );

bool
RUDeleteValue(
	_In_ HKEY RootKey,
	_In_ const wchar_t* SubKey,
	_In_ const wchar_t* ValueName
	);

bool 
RUDeleteKey(
	_In_ HKEY RootKey,
	_In_ const wchar_t* SubKey
	);


bool
RUIsKeyExists(
    HKEY RootKey, 
    const wchar_t* TargetKey
    );


typedef
bool(*fn_key_callback)(
    _In_ uint32_t index,
    _In_ const wchar_t* sub_key_name,
    _In_ const wchar_t* class_name
    );

typedef
bool(*fn_value_callback)(
    _In_ uint32_t index,
    _In_ uint32_t value_type,
    _In_ const wchar_t* value_name,
    _In_ uint32_t value_data_size,
    _In_ const uint8_t* value_data
    );

bool
reg_enum_key_values(
    _In_ HKEY key,
    _In_ fn_key_callback key_cb,
    _In_ fn_value_callback value_cb
    );