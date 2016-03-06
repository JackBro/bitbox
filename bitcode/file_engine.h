/**
 * @file    file_engine.h
 * @brief   
 * @author  Yonhgwhan, Noh (fixbrain@gmail.com)
 * @date    2016.03.06 14:36 created.
 * @copyright All rights reserved by Yonghwan, Noh.
**/
#pragma once
#include "conf.h"

typedef class BCFile
{
public:
    BCFile()
    {
    
    }
    virtual ~BCFile()
    {
    
    }

    /// @brief
    bool is_file_exists(_In_ const char* file_path)
    {
        _ASSERTE(NULL != file_path);
        if (NULL == file_path) return false;

        WIN32_FILE_ATTRIBUTE_DATA info = { 0 };

        //
        // CreateFile()�� �ƴ� GetFileAttributesEx()�� �̿��ϸ� ������ �ٸ� process�� ���� lock�Ǿ� �־
        // ���� ���翩�θ� ��Ȯ�� üũ�� �� �ִ�.
        //
        if (GetFileAttributesExA(file_path, GetFileExInfoStandard, &info) == 0)
            return false;
        else
            return true;
    }

private:


} *PBCFile;

