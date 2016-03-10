/**
 * @file    reg_engine.h
 * @brief
 *
 * @author  Yonhgwhan, Noh (fixbrain@gmail.com)
 * @date    2016.02.28 14:36 created.
 * @copyright All rights reserved by Yonghwan, Noh.
**/
#pragma once
#include "conf.h"
#include "RegistryUtil.h"

typedef class BCReg : public IBCEngine
{
private:
	std::wstring _key_name;
	std::wstring _val_name;
	std::wstring _val_data;

public:
	BCReg() {}
	virtual ~BCReg() {}

	bool initialize() { return true; }
	void finalize() { return; }
	BC_ENGINE_TYPE get_engine_type() { return REG_ENGINE; }

	/// @brief  reg_info �� ���� �ش� Ű/���/�����Ͱ� �����ϸ�
	///         true �� �����Ѵ�. 
	bool is_exists(const char* values, ...)
	{
		_ASSERTE(NULL != values);
		if (NULL == values) return false;

		va_list list;
		va_start(list, values);

		_key_name = MbsToWcsEx(values);
		_val_name = MbsToWcsEx(va_arg(list, const char*));
		_val_data = MbsToWcsEx(va_arg(list, const char*));

		va_end(list);

		std::wstring data;
		if (true == RUReadString(HKEY_LOCAL_MACHINE, _key_name.c_str(), _val_name.c_str(), data))
		{
			//log_info "%s", data.c_str() log_end;

			if (std::wstring::npos != data.find(_val_data.c_str()))
			{
				return true;    // ok, we got!
			}
		}

		return false;
	}

	void dump(const char* values)
	{
		log_info "[Detected Registry] = %s", values log_end;
	}

} *PBCReg;