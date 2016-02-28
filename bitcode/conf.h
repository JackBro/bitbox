/**
 * @file    conf.h
 * @brief   
 *
 * @author  Yonhgwhan, Noh (fixbrain@gmail.com)
 * @date    2016.02.28 14:36 created.
 * @copyright All rights reserved by Yonghwan, Noh.
**/
#pragma once

#include <sal.h>
#include <string>
#include <list>
#include "log.h"

typedef class RegInfo
{
public:
    RegInfo(_In_ std::string& root_key, _In_ std::string& key_name, _In_ std::string& val_name, _In_ std::string& val_data)
        : _root_key(root_key),
        _key_name(key_name),
        _val_name(val_name),
        _val_data(val_data)
    {
    }

    std::string _root_key;
    std::string _key_name;
    std::string _val_name;
    std::string _val_data;

    void dump()
    {
        log_info 
            "root key = %s, key_name = %s, val_name = %s, val_data = %s", 
            _root_key.c_str(),
            _key_name.c_str(),
            _val_name.c_str(),
            _val_data.c_str()
        log_end
    }
} *PRegInfo;

typedef class BCConf
{
public:
    bool load_config(_In_ const wchar_t* config_path);
private:
    std::list<std::string>      _file_names;
    std::list<RegInfo>          _regs;
    std::list<std::string>      _svcs;

} *PBCConf;