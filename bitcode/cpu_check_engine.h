/**
* @file    driver_engine.h
* @brief
* @author  Yu-hyeon, Cha (xargo707@hotmail.com)
* @date    2016.03.14 14:10 created.
* @copyright All rights reserved by team bixbox
**/
#pragma once
#include "conf.h"
#include "stdafx.h"
#include <intrin.h>

typedef class BCcpu : public IBCEngine
{
private:
	HANDLE _driver_hendle;

public:
	BCcpu()
	{

	}
	virtual ~BCcpu()
	{

	}

	bool initialize() { return true; }
	void finalize() { return; }
	BC_ENGINE_TYPE get_engine_type() { return CPU_ENGINE; }

	/// @brief
	bool is_exists(const char* values, ...)
	{
		char cpu_vendor[13] = { 0, };

		_ASSERTE(NULL != values);
		if (NULL == values) return false;
		
		cpu_write_vendor(cpu_vendor);

		if (!memcmp(cpu_vendor, values, 12)) return TRUE;

		return FALSE;
	}

	void dump(const char* values)
	{
		log_info "[Detected string] = %s", values log_end;
	}
	
	void cpu_write_vendor(char * vendor) 
	{	
#if _M_x64
		cpuid_vendor_00_x64(vendor);
#else
		cpuid_vendor_00_x86(vendor);
#endif
	}

	static inline void cpuid_vendor_00_x64(char *vendor)
	{
		// ������
	}

	static inline void cpuid_vendor_00_x86(char * vendor) 
	{
		int cpuInfo[4] = { 0, }; // [0] = ebx, [1] = edx, [2] = ecx, [3] = eax
		uint32_t function_id = 0; // A code that specifies the information to retrieve, passed in EAX.
								/*
									EAX = 0 : ������ü ID
									EAX=1: ���μ��� ���� �� ��� ��Ʈ
									EAX=2: ĳ�� �� TLB ������ ����
									EAX=3: ���μ��� �Ϸ� ��ȣ
									EAX=80000000h: ���� ���� Ȯ�� �Լ� ��������
									EAX=80000001h: Ȯ�� ���μ��� ���� �� ��� ��Ʈ
									EAX=80000002h,80000003h,80000004h: ���μ��� �귣�� ���ڿ�
									// �󼼴� Intel 64 �� IA-32 ��Ű��ó ����Ʈ���� ������ ���� 2��: ��� ���� ����
								*/

		__cpuid(cpuInfo, function_id); // function_id : EAX �������ͷ� ����Ǵ� ���̴�.
										// ���� ���� �����ϴ� CPU ������ �޶���.
		for (int i = 0; i < 4; i++)
			vendor[i] = cpuInfo[1] >> (i * 8);

		for (int i = 0; i < 4; i++)
			vendor[i + 4] = cpuInfo[3] >> (i * 8);

		for (int i = 0; i < 4; i++)
			vendor[i + 8] = cpuInfo[2] >> (i * 8);

		vendor[12] = 0x00;
	}	
} *PBCcpu;