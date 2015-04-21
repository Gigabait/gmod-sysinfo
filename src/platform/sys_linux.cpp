#ifdef __linux__

#include "sys_linux.h"

namespace Sysinfo {
	int Uptime() {
		struct sysinfo memInfo;
		sysinfo(&memInfo);
		
		return memInfo.uptime;
	}
	int[] LoadAverages() {
		struct sysinfo memInfo;
		sysinfo(&memInfo);

		return memInfo.loads;
	}
	namespace Mem {
		double GetUsedVirtual() {
			struct sysinfo memInfo;

			sysinfo(&memInfo);

			long long virtualMemUsed = memInfo.totalram - memInfo.freeram;
			//Add other values in next statement to avoid int overflow on right hand side...
			virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
			virtualMemUsed *= memInfo.mem_unit;

			return virtualMemUsed;
		}
		double GetTotalVirtual() {
			struct sysinfo memInfo;

			sysinfo(&memInfo);
			long long totalVirtualMem = memInfo.totalram;
			//Add other values in next statement to avoid int overflow on right hand side...
			totalVirtualMem += memInfo.totalswap;
			totalVirtualMem *= memInfo.mem_unit;

			return totalVirtualMem;
		}

		double GetUsedPhysical() {
			struct sysinfo memInfo;

			sysinfo(&memInfo);

			long long physMemUsed = memInfo.totalram - memInfo.freeram;
			//Multiply in next statement to avoid int overflow on right hand side...
			physMemUsed *= memInfo.mem_unit;

			return physMemUsed;
		}
		double GetTotalPhysical() {
			struct sysinfo memInfo;

			sysinfo(&memInfo);

			long long totalPhysMem = memInfo.totalram;
			//Multiply in next statement to avoid int overflow on right hand side...
			totalPhysMem *= memInfo.mem_unit;

			return totalPhysMem;
		}
	}
	namespace CPU {
		static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

		void Init() {
			FILE* file = fopen("/proc/stat", "r");
			fscanf(file, "cpu %Ld %Ld %Ld %Ld", &lastTotalUser, &lastTotalUserLow,
				&lastTotalSys, &lastTotalIdle);
			fclose(file);
		}
		void Destroy() {}

		double GetUsage() {
			double percent;
			FILE* file;
			unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

			file = fopen("/proc/stat", "r");
			fscanf(file, "cpu %Ld %Ld %Ld %Ld", &totalUser, &totalUserLow,
				&totalSys, &totalIdle);
			fclose(file);

			if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
				totalSys < lastTotalSys || totalIdle < lastTotalIdle){
				//Overflow detection. Just skip this value.
				percent = -1.0;
			}
			else{
				total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
					(totalSys - lastTotalSys);
				percent = total;
				total += (totalIdle - lastTotalIdle);
				percent /= total;
				percent *= 100;
			}

			lastTotalUser = totalUser;
			lastTotalUserLow = totalUserLow;
			lastTotalSys = totalSys;
			lastTotalIdle = totalIdle;

			return percent;
		}
	}
}

#endif