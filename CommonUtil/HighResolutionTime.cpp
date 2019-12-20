#include "HighResolutionTime.h"
#if defined LINUX
#include <time.h>
#else
#include <chrono>
#endif

HighResolutionTime::HighResolutionTime(void)
{
	_start_time = 0;
	_end_time = 0;

#if 0
#if defined _WIN32
	_frequency.QuadPart = 0;
	_start_time.QuadPart = 0;
#endif
#endif
}


HighResolutionTime::~HighResolutionTime(void)
{
}
timespec HighResolutionTime::get_current_s(int unit_resolution)
{
	struct timespec ts;
#if defined _WIN32
	long long res;
	LARGE_INTEGER tickNow;
	LARGE_INTEGER tickFrequency;

	QueryPerformanceFrequency(&tickFrequency);
	QueryPerformanceCounter(&tickNow);

	res = (long long)(tickNow.QuadPart * unit_resolution / tickFrequency.QuadPart);
	ts.tv_nsec = res % 1000000000;
	ts.tv_sec = res / 1000000000;
#else

	if (clock_gettime(CLOCK_MONOTONIC_RAW, &ts) < 0)
	{
		ts.tv_nsec = 0;
		ts.tv_sec = 0;
	}
#endif
	return ts;
}

long long HighResolutionTime::get_current(int unit_resolution)
{
	long long res = 0;
#if defined _WIN32
	LARGE_INTEGER tickNow;
	static LARGE_INTEGER tickFrequency;
	static bool is_first = true;
	if (is_first == true)
	{
		is_first = false;
		QueryPerformanceFrequency(&tickFrequency);
	}
	QueryPerformanceCounter(&tickNow);
	
	res = (long long) (tickNow.QuadPart * unit_resolution / tickFrequency.QuadPart);

	//res = (long long)(std::chrono::steady_clock::now().time_since_epoch().count()) / (1000000000 / unit_resolution);
#else
	struct timespec ts;

	if (clock_gettime(CLOCK_MONOTONIC_RAW, &ts) < 0)
	//if (clock_gettime(CLOCK_REALTIME, &ts) < 0)
		return -1;
	res = ts.tv_sec*unit_resolution;
	//res += (long long)ts.tv_nsec*unit_resolution/1000000000;
	res += (long long)ts.tv_nsec/(1000000000/unit_resolution);
#endif
	return res;
}

long long HighResolutionTime::get_current_system_time(int unit_resolution)
{
	long long res = -1;
#if defined _WIN32
	
	//res = (long long)(std::chrono::system_clock::now().time_since_epoch().count()) / (1000000000 / unit_resolution);
	res = (long long)(std::chrono::system_clock::now().time_since_epoch().count()) / (10000000 / unit_resolution);

#else
	struct timespec ts;

	//if (clock_gettime(CLOCK_MONOTONIC_RAW, &ts) < 0)
	if (clock_gettime(CLOCK_REALTIME, &ts) >= 0)
	{
		res = ts.tv_sec*unit_resolution;
		//res += (long long)ts.tv_nsec*unit_resolution / 1000000000;
		res += (long long)ts.tv_nsec / (1000000000 / unit_resolution);
	}
	
#endif
	return res;
}

bool HighResolutionTime::start(void)
{
#if 1
	_start_time = get_current_nano_sec();
#else
#if defined _WIN32
	QueryPerformanceFrequency(&_frequency);
	QueryPerformanceCounter(&_start_time);
#else
	if (clock_gettime(CLOCK_MONOTONIC_RAW, &_start_time) < 0)
		return false;
#endif
#endif
	return true;
}

long long HighResolutionTime::get_elapsed_time(int unit_resolution)
{
#if 1
	_end_time = get_current_nano_sec();
	long long elapsed_time = _end_time - _start_time;
	long long res = elapsed_time * ((double)unit_resolution / 1000000000.0);
	return res;
#else
#if defined _WIN32
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter( &currentTime );

	return (currentTime.QuadPart - _start_time.QuadPart)*unit_resolution/_frequency.QuadPart;
#else
	struct timespec currentTime;
	clock_gettime(CLOCK_MONOTONIC_RAW, &currentTime);
	long long res = 0;
	double tmp_unit = (double)unit_resolution / 1000000000.0;
	
	res = ((long long)currentTime.tv_sec-_start_time.tv_sec)*unit_resolution;
	res += (double)currentTime.tv_nsec*tmp_unit;
	res -= (double)_start_time.tv_nsec*tmp_unit;

	return res;
#endif
#endif
}

double HighResolutionTime::get_day_time()
{
	double res = 0.0;
#if defined _WIN32
	LARGE_INTEGER tickNow;
	LARGE_INTEGER tickFrequency;

	QueryPerformanceFrequency(&tickFrequency);
	QueryPerformanceCounter(&tickNow);

	res = (double)tickNow.QuadPart / tickFrequency.QuadPart;
#else
	struct timespec ts;
	struct tm tm_now;
	if (clock_gettime(CLOCK_REALTIME, &ts) < 0)
		return -1;
	localtime_r((time_t*)&ts.tv_sec, &tm_now);
	res = tm_now.tm_hour;
	res = res * 60 + tm_now.tm_min;
	res = res * 60 + tm_now.tm_sec;
	res = res + (double)ts.tv_nsec / 1000000000.0;
#endif
	return res;
}