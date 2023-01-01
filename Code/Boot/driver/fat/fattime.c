#include "ff.h"

DWORD get_fattime (void)
{
	DWORD res;
	//RTC_t rtc;

	//rtc_gettime( &rt	);
	
	int year = 2023;
	int month = 5;
	int mday = 4;
	int hour = 3;
	int min = 22;
	int sec = 11;
	
	res =  (((DWORD)year - 1980) << 25)
			| ((DWORD)month << 21)
			| ((DWORD)mday << 16)
			| (WORD)(hour << 11)
			| (WORD)(min << 5)
			| (WORD)(sec >> 1);

	return res;
}