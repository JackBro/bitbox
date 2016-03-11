/**
 * @file    file_engine.h
 * @brief   
 * @author  Yonhgwhan, Noh (fixbrain@gmail.com)
 * @date    2016.03.06 14:36 created.
 * @copyright All rights reserved by Yonghwan, Noh.
**/
#pragma once
#include "conf.h"

typedef class BCFile : public IBCEngine
{
public:
    BCFile()
    {
    
    }
    virtual ~BCFile()
    {
    
    }

	bool initialize() { return true; }
	void finalize() { return; }
	BC_ENGINE_TYPE get_engine_type() { return FILE_ENGINE; }

    /// @brief
	bool is_exists(const char* values, ...)
    {
        _ASSERTE(NULL != values);
        if (NULL == values) return false;

        WIN32_FILE_ATTRIBUTE_DATA info = { 0 };

        //
        // CreateFile()�� �ƴ� GetFileAttributesEx()�� �̿��ϸ� ������ �ٸ� process�� ���� lock�Ǿ� �־
        // ���� ���翩�θ� ��Ȯ�� üũ�� �� �ִ�.
        //
        if (GetFileAttributesExA(values, GetFileExInfoStandard, &info) == 0)
            return false;
        else
            return true;
    }

	void dump(const char* values)
	{
		log_info "[Detected File] = %s", values log_end;
	}

private:


} *PBCFile;

