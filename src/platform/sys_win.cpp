#ifdef _WIN32

#include "sys_win.h"

namespace Sysinfo {
	int Uptime() {
		int sec = GetTickCount() / 1000;
		return sec;
	}
	namespace Mem {
		double GetUsedVirtual() {
			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);

			return memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
		}
		double GetTotalVirtual() {
			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);

			return memInfo.ullTotalPageFile;
		}

		double GetUsedPhysical() {
			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);

			return memInfo.ullTotalPhys - memInfo.ullAvailPhys;
		}
		double GetTotalPhysical() {
			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);

			return memInfo.ullTotalPhys;
		}
	}
	namespace CPU {
		static PDH_HQUERY cpuQuery;
		static PDH_HCOUNTER cpuTotal;

		void Init() {
			PdhOpenQuery(NULL, NULL, &cpuQuery);
			PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
			PdhCollectQueryData(cpuQuery);
		}
		void Destroy() {
			PdhCloseQuery(&cpuQuery);
		}
		double GetUsage() {
			PDH_FMT_COUNTERVALUE counterVal;

			PdhCollectQueryData(cpuQuery);
			PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);

			return counterVal.doubleValue;
		}
		int* GetLoadAverages() {
			return NULL;
		}
	}
	namespace Net {

	}
}

#endif