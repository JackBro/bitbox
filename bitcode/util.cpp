#include "stdafx.h"
#include "log.h"
#include "util.h"
#include <VersionHelpers.h>

/// @brief
void write_to_console(_In_ uint32_t color, _In_z_ const char* log_message)
{
    _ASSERTE(NULL != log_message);
    if (NULL == log_message) return;

    static HANDLE	con_stdout_handle = INVALID_HANDLE_VALUE;
    static WORD		con_old_color = 0;

    if (INVALID_HANDLE_VALUE == con_stdout_handle)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;

        con_stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(con_stdout_handle, &csbi);
        con_old_color = csbi.wAttributes;
    }

    DWORD len;
    switch (color)
    {
    case wtc_red:
        SetConsoleTextAttribute(con_stdout_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
        WriteConsoleA(con_stdout_handle, log_message, (DWORD)strlen(log_message), &len, NULL);
        SetConsoleTextAttribute(con_stdout_handle, con_old_color);
        break;
    case wtc_green:
        SetConsoleTextAttribute(con_stdout_handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        WriteConsoleA(con_stdout_handle, log_message, (DWORD)strlen(log_message), &len, NULL);
        SetConsoleTextAttribute(con_stdout_handle, con_old_color);
        break;
    case wtc_none:
    default:
        WriteConsoleA(con_stdout_handle, log_message, (DWORD)strlen(log_message), &len, NULL);
    }
}


/// @brief  ���� �������� ���μ����� ���� ���� �����Ѵ�.
std::wstring get_current_module_fileEx()
{
    std::wstring out;
    if (true != get_current_module_file(out))
    {
        return _null_stringw;
    }
    else
    {
        return out;
    }
}

/// @brief 
bool get_module_path(_In_ const wchar_t* module_name, _Out_ std::wstring& module_path)
{
    DWORD  ret = 0;
    DWORD  buf_len = MAX_PATH;
    LPTSTR buf = (LPTSTR)malloc(buf_len * sizeof(TCHAR));
    if (NULL == buf) return false;

    for (;;)
    {
        ret = GetModuleFileName(GetModuleHandle(module_name), buf, buf_len);
        if (ret == buf_len)
        {
            // buf �� ���� ��� buf_len ��ŭ ���۰� �߸���, buf_len ���� (������ ����)
            // ���� ����� 2�� �÷��� �� �õ�
            free(buf);

            buf_len *= 2;
            buf = (LPTSTR)malloc(buf_len * sizeof(TCHAR));
            if (NULL == buf) return false;
        }
        else
        {
            module_path = buf;
            free(buf);
            return true;
        }
    }

    //return false;	// never reach here!
}

/// @brief
bool get_current_module_path(_Out_ std::wstring& module_path)
{
    return get_module_path(NULL, module_path);
}

/// @brief
std::wstring get_current_module_pathEx()
{
    std::wstring out;
    if (true != get_current_module_path(out))
    {
        return _null_stringw;
    }
    else
    {
        return out;
    }
}

/// @brief  
bool get_current_module_file(_Out_ std::wstring& module_file)
{
    std::wstring module_path;
    if (true != get_current_module_path(module_path))
    {
        log_err "get_current_module_path()" log_end
        return false;
    }

    if (true != extract_last_tokenW(module_path, L"\\", module_file, false, false))
    {
        log_err "extract_last_tokenW( org=%s )", module_path.c_str() log_end
            module_file = L"";
        return false;
    }

    return true;
}

/**
 * \brief	org_string ���� token �� �˻��ؼ� ���ڿ��� �߶󳽴�. 
			(org_string �� �տ������� token �� �˻�)

			ABCDEFG.HIJ.KLMN	: org_string
				   .			: token
			ABCDEFG             : out_string if forward = TRUE
					HIJ.KLMN	: out_string if forward = FALSE

			delete_token �� True �� ��� org_string ���� out_string + token �� ����

 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
bool
extract_first_tokenW(
	_In_ std::wstring& org_string,
	_In_ const std::wstring& token,
	_Out_ std::wstring& out_string, 
	_In_ bool forward,
	_In_ bool delete_token
	)
{
    if (true == delete_token)
    {
        if (&org_string == &out_string) 
        {
#ifndef TEST_EXPORTS
            _ASSERTE(!"prarameters conflict! ");
#endif
            return false;
        }
    }

	size_t pos = org_string.find(token);
	if (std::wstring::npos == pos)
	{
        out_string = org_string;
        return true;
	}

	if (true== forward)
	{
		out_string = org_string.substr(0, pos);
        if (delete_token) org_string.erase(0, pos + token.size());
	}
	else
	{
		out_string = org_string.substr(pos + token.size(), org_string.size());
        if (delete_token) org_string.erase(pos, org_string.size());
	}
    return true;
}

/**
 * \brief	org_string ���� token �� �˻��ؼ� ���ڿ��� �߶󳽴�. 
			(org_string �� �տ������� token �� �˻�)

			ABCDEFG.HIJ.KLMN	: org_string
				   .			: token
			ABCDEFG             : out_string if forward = TRUE
					HIJ.KLMN	: out_string if forward = FALSE
 * @param	
 * @see		
 * @remarks ���� �� �и��� ���ڿ��� ��Ʈ�� ��ü�� ����
			���� �� _nullstringw ����
 * @code		
 * @endcode	
 * @return	
**/
std::wstring 
extract_first_tokenExW(
	_In_ const wchar_t* org,
	_In_ const wchar_t* token,	
	_In_ bool forward
	)
{
	_ASSERTE(NULL != org);
	_ASSERTE(NULL != token);
	if (NULL == org || NULL == token) return _null_stringw;

	std::wstring org_string = org;
	std::wstring out_string;
	if (true != extract_first_tokenW(org_string, token, out_string, forward, false)) 
		return _null_stringw;
	else
		return out_string;
}


/**
 * @brief	org_string ���� token �� �˻��ؼ� ���ڿ��� �߶󳽴�. 
			(org_string �� �տ������� token �� �˻�)
			
			ABCDEFG.HIJ.KLMN	: org_string
			       .			: token
		    ABCDEFG             : out_string if forward = TRUE
			        HIJ.KLMN	: out_string if forward = FALSE

            delete_token �� True �� ��� org_string ���� out_string + token �� ����
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
bool
extract_first_tokenA(
	_In_ std::string& org_string,
	_In_ const std::string& token,
	_Out_ std::string& out_string, 
	_In_ bool forward,
	_In_ bool delete_token
    )	
{
    if (true == delete_token)
    {
        if (&org_string == &out_string) 
        {
#ifndef TEST_EXPORTS
            _ASSERTE(!"prarameters conflict! ");
#endif
            return false;;
        }
    }

	size_t pos = org_string.find(token);
	if (std::string::npos == pos)
	{
        out_string = org_string;
        return true;
	}

	if (TRUE == forward)
	{
		out_string = org_string.substr(0, pos);
        if (delete_token) org_string.erase(0, pos + token.size());
	}
	else
	{
		out_string = org_string.substr(pos + token.size(), org_string.size());
        if (delete_token) org_string.erase(pos, org_string.size());
	}
    return true;
}

/**
 * @brief	org_string ���� token �� �˻��ؼ� ���ڿ��� �߶󳽴�. 
			(org_string �� �տ������� token �� �˻�)
			
			ABCDEFG.HIJ.KLMN	: org_string
			       .			: token
		    ABCDEFG             : out_string if forward = TRUE
			        HIJ.KLMN	: out_string if forward = FALSE

 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
std::string
extract_first_tokenExA(
		_In_ const char* org,
		_In_ const char* token,		
		_In_ bool forward
		)
{
	_ASSERTE(NULL != org);
	_ASSERTE(NULL != token);
	if (NULL == org || NULL == token) return _null_stringa;

	std::string org_string = org;
	std::string out_string;
	if (true != extract_first_tokenA(org_string, token, out_string, forward, false)) 
		return _null_stringa;
	else
		return out_string;
}

/**
 * @brief	org_string ���� token �� �˻��ؼ� ���ڿ��� �߶󳽴�. 
			(org_string �� �ڿ������� token �� �˻�)

			ABCDEFG.HIJ.KLMN	: org_string
					   .		: token
			ABCDEFG.HIJ			: out_string if forward = TRUE
					    KLMN	: out_string if forward = FALSE

			delete_token �� True �� ��� org_string ���� out_string + token �� ����
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
bool 
extract_last_tokenW(
	_In_ std::wstring& org_string,
	_In_ const std::wstring& token,
	_Out_ std::wstring& out_string, 
	_In_ bool forward,
	_In_ bool delete_token
	)
{
    if (true== delete_token)
    {
        if (&org_string == &out_string) 
        {
#ifndef TEST_EXPORTS
            _ASSERTE(!"prarameters conflict! ");
#endif
            return false;
        }
    }

    size_t pos = org_string.rfind(token);
	if (std::wstring::npos == pos)
	{
        out_string = org_string;
        return true;
	}

	if (true== forward)
	{
		out_string = org_string.substr(0, pos);
        if (delete_token) org_string.erase(0, pos + token.size());
	}
	else
	{
        out_string = org_string.substr(pos + token.size(), org_string.size());
        if (delete_token) org_string.erase(pos, org_string.size());
	}
    return true;
}

/**
 * @brief	org_string ���� token �� �˻��ؼ� ���ڿ��� �߶󳽴�. 
			(org_string �� �ڿ������� token �� �˻�)

			ABCDEFG.HIJ.KLMN	: org_string
					   .		: token
			ABCDEFG.HIJ			: out_string if forward = TRUE
					    KLMN	: out_string if forward = FALSE
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
std::wstring
extract_last_tokenExW(
	_In_ const wchar_t* org,
	_In_ const wchar_t* token,	
	_In_ bool forward
	)
{
	_ASSERTE(NULL != org);
	_ASSERTE(NULL != token);
	if (NULL == org || NULL == token) return _null_stringw;

	std::wstring org_string = org;
	std::wstring out_string;
	if (true != extract_last_tokenW(org_string, token, out_string, forward, false))
		return _null_stringw;
	else
		return out_string;
}

/**
 * @brief	org_string ���� token �� �˻��ؼ� ���ڿ��� �߶󳽴�. 
			(org_string �� �ڿ������� token �� �˻�)
			
			ABCDEFG.HIJ.KLMN	: org_string
			           .		: token
		    ABCDEFG.HIJ			: out_string if forward = TRUE
						KLMN	: out_string if forward = FALSE

            delete_token �� True �� ��� org_string ���� out_string + token �� ����
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
bool 
extract_last_tokenA(
	_In_ std::string& org_string,
	_In_ const std::string& token,
	_Out_ std::string& out_string, 
	_In_ bool forward,
	_In_ bool delete_token
    )
{
    if (TRUE == delete_token)
    {
        if (&org_string == &out_string) 
        {
#ifndef TEST_EXPORTS
            _ASSERTE(!"prarameters conflict! ");
#endif
            return false;
        }
    }

	size_t pos = org_string.rfind(token);
	if (std::string::npos == pos)
	{
        out_string = org_string;
        return true;
	}

	if (true == forward)
	{
		out_string = org_string.substr(0, pos);
        if (delete_token) org_string.erase(0, pos + token.size());
	}
	else
	{
        out_string = org_string.substr(pos + token.size(), org_string.size());
        if (delete_token) org_string.erase(pos, org_string.size());
	}
    return true;
}

/**
 * @brief	
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
std::string
extract_last_tokenExA(
	_In_ const char* org,
	_In_ const char* token,
	_In_ bool forward
    )
{
	_ASSERTE(NULL != org);
	_ASSERTE(NULL != token);
	if (NULL == org || NULL == token) return _null_stringa;

	std::string org_string = org;
	std::string out_string;
	if (true != extract_last_tokenA(org_string, token, out_string, forward, false))
		return _null_stringa;
	else
		return out_string;

}



/**
* @brief	trim �Լ���

            string szbuf="__12345_____";  
            cout<<"Before trim: "<<szbuf<<endl;  
            cout<<"After  trim: "<<trim(szbuf,"_")<<endl;
            cout<<"After rtrim: "<<rtrim(szbuf,"_")<<endl;
            cout<<"After ltrim: "<<ltrim(szbuf,"_")<<endl;
            ----------------------------------------------
            Before trim: __12345_____
            After  trim: 12345
            After rtrim: __12345
            After ltrim: 12345
* @param	
* @see		
* @remarks	
* @code		
* @endcode	
* @return	
*/
std::string trima(std::string& s, const std::string& drop)
{
    std::string r=s.erase(s.find_last_not_of(drop)+1);
    return r.erase(0,r.find_first_not_of(drop));
}
std::string rtrima(std::string& s, const std::string& drop)
{
  return s.erase(s.find_last_not_of(drop)+1); 
}
std::string ltrima(std::string& s, const std::string& drop)
{ 
  return s.erase(0,s.find_first_not_of(drop)); 
}
std::wstring  trimw(std::wstring& s, const std::wstring& drop)
{
    std::wstring r=s.erase(s.find_last_not_of(drop)+1);
    return r.erase(0,r.find_first_not_of(drop));
}
std::wstring rtrimw(std::wstring& s, const std::wstring& drop)
{
    return s.erase(s.find_last_not_of(drop)+1); 
}
std::wstring ltrimw(std::wstring& s, const std::wstring& drop)
{
    return s.erase(0,s.find_first_not_of(drop)); 
}



/**
 * @brief	ASCII(Multibyte) --> WIDE CHAR �� ��ȯ, caller �� ���ϵǴ� �����͸� �Ҹ�����־�� ��
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
wchar_t* MbsToWcs(_In_ const char* mbs)
{
    _ASSERTE(NULL!=mbs);
    if(NULL==mbs) return NULL;

    int outLen=MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, mbs, -1, NULL, 0);
    if(0==outLen) return NULL;

    wchar_t* outWchar=(wchar_t*) malloc(outLen * (sizeof(wchar_t)));  // outLen contains NULL char 
    if(NULL==outWchar) return NULL;
    RtlZeroMemory(outWchar, outLen);

    if(0==MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, mbs, -1, outWchar, outLen))
    {
        log_err "MultiByteToWideChar() failed, errcode=0x%08x", GetLastError() log_end

        free(outWchar);
        return NULL;
    }

    return outWchar;
}

/**
 * @brief	WIDE CHAR --> ASCII(Multibyte) �� ��ȯ, caller �� ���ϵǴ� �����͸� �Ҹ�����־�� ��
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
char* WcsToMbs(_In_ const wchar_t* wcs)
{
    _ASSERTE(NULL!=wcs);
    if(NULL==wcs) return NULL;

    int outLen=WideCharToMultiByte(CP_ACP, 0, wcs, -1, NULL, 0, NULL, NULL);
    if(0==outLen) return NULL;

    char* outChar=(char*) malloc(outLen * sizeof(char));
    if(NULL==outChar) return NULL;
    RtlZeroMemory(outChar, outLen);

    if(0==WideCharToMultiByte(CP_ACP, 0, wcs, -1, outChar, outLen, NULL, NULL))
    {
        log_err "WideCharToMultiByte() failed, errcode=0x%08x", GetLastError() log_end
        free(outChar);
        return NULL;
    }

    return outChar;
}
    
/**
 * @brief	wide char -> utf8 ��ȯ, caller �� ���ϵǴ� �����͸� �Ҹ�����־�� �� 
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
char* WcsToMbsUTF8(_In_ const wchar_t* wcs)
{
    _ASSERTE(NULL!=wcs);
    if(NULL==wcs) return NULL;

    int outLen=WideCharToMultiByte(CP_UTF8, 0, wcs, -1, NULL, 0, NULL, NULL);
    if(0==outLen) return NULL;

    char* outChar=(char*) malloc(outLen * sizeof(char));
    if(NULL==outChar) return NULL;
    RtlZeroMemory(outChar, outLen);

    if(0==WideCharToMultiByte(CP_UTF8, 0, wcs, -1, outChar, outLen, NULL, NULL))
    {
        log_err "WideCharToMultiByte() failed, errcode=0x%08x", GetLastError() log_end

        free(outChar);
        return NULL;
    }

    return outChar;
}

/**
 * @brief	
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
wchar_t* Utf8MbsToWcs(_In_ const char* utf8)
{
    _ASSERTE(NULL!=utf8);
    if(NULL==utf8) return NULL;

    int outLen=MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8, -1, NULL, 0);
    if(0==outLen) return NULL;

    wchar_t* outWchar=(wchar_t*) malloc(outLen * (sizeof(wchar_t)));  // outLen contains NULL char 
    if(NULL==outWchar) return NULL;
    RtlZeroMemory(outWchar, outLen);

    if(0==MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8, -1, outWchar, outLen))
    {
        log_err "MultiByteToWideChar() failed, errcode=0x%08x", GetLastError() log_end

        free(outWchar);
        return NULL;
    }

    return outWchar;
}

/**
* @brief	
* @param	
* @see		
* @remarks	
* @code		
* @endcode	
* @return	
*/
std::wstring MbsToWcsEx(_In_ const char *mbs)
{
    wchar_t* tmp = MbsToWcs(mbs);
    if (NULL == tmp)
    {
        return _null_stringw;
    }
    else
    {
        SafePtr ptr((void*)tmp);
        return std::wstring(tmp);
    }
}

/**
* @brief	
* @param	
* @see		
* @remarks	
* @code		
* @endcode	
* @return	
*/
std::string WcsToMbsEx(_In_ const wchar_t *wcs)
{
    char* tmp = WcsToMbs(wcs);
    if (NULL == tmp)
    {
        return _null_stringa;
    }
    else
    {
        SafePtr ptr((void*)tmp);
        return std::string(tmp);
    }
}


/// @brief	http://support.microsoft.com/kb/131065/EN-US/
bool set_privilege(_In_z_ const wchar_t* privilege, _In_ bool enable)
{
    if (IsWindowsXPOrGreater())
	{
		HANDLE hToken;
		if ( TRUE != OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken) )
		{
			if (GetLastError() == ERROR_NO_TOKEN)
			{
				if (ImpersonateSelf(SecurityImpersonation)	!= TRUE ) 
				{
					log_err "ImpersonateSelf( ) failed. gle=0x%08x", GetLastError() log_end
					return false;
				}

				if (TRUE != OpenThreadToken(GetCurrentThread(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,FALSE,&hToken))
				{
					log_err "OpenThreadToken() failed. gle=0x%08x", GetLastError() log_end
					return false;
				}
			}
			else
			{
				log_err "OpenThread() failed. gle=0x%08x", GetLastError() log_end
				return false;
			}
		}  

		TOKEN_PRIVILEGES tp = { 0 };		
		LUID luid = {0};
		DWORD cb = sizeof(TOKEN_PRIVILEGES);
		if(!LookupPrivilegeValue( NULL, privilege, &luid ))
		{		
			log_err "LookupPrivilegeValue() failed. gle=0x%08x", GetLastError() log_end
			CloseHandle(hToken);
			return false;
		}
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		if(enable) 
		{
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		} 
		else 
		{
			tp.Privileges[0].Attributes = 0;
		}
		
        if (FALSE == AdjustTokenPrivileges( hToken, FALSE, &tp, cb, NULL, NULL ))
        {
            DWORD gle=GetLastError();
		    if (gle != ERROR_SUCCESS)
		    {		
				log_err "AdjustTokenPrivileges() failed. gle=0x%08x", GetLastError() log_end
				CloseHandle(hToken);				
			    return false;
		    }		
        }        

		CloseHandle(hToken);
	}

	return true;
}
