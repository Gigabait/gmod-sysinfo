#include "GarrysMod/Lua/Interface.h"
#include <stdio.h>

#ifdef _WIN32
#include "platform/sys_win.h"
#endif
#ifdef __linux__
#include "platform/sys_linux.h"
#endif

using namespace GarrysMod::Lua;

namespace Sysinfo {
	namespace Mem {
		double GetUsedVirtual();
		double GetTotalVirtual();

		double GetUsedPhysical();
		double GetTotalPhysical();
	}
	namespace CPU {
		void Init();
		void Destroy();

		double GetUsage();
	}
	namespace Net {

	}
}

int VMem( lua_State* state )
{
	LUA->PushNumber(Sysinfo::Mem::GetUsedVirtual());
	LUA->PushNumber(Sysinfo::Mem::GetTotalVirtual());
	return 2;
}
int PMem(lua_State* state)
{
	LUA->PushNumber(Sysinfo::Mem::GetUsedPhysical());
	LUA->PushNumber(Sysinfo::Mem::GetTotalPhysical());
	return 2;
}
int CPU(lua_State* state)
{
	LUA->PushNumber(Sysinfo::CPU::GetUsage());
	return 1;
}
int Net(lua_State* state)
{
	LUA->CreateTable();
		LUA->PushString("Test");
		LUA->PushNumber(42);
		LUA->SetTable(-3);
	return 1;
}

GMOD_MODULE_OPEN()
{
	Sysinfo::CPU::Init();

	LUA->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );	// Push global table

		LUA->PushString("sysinfo");
		LUA->CreateTable();

			LUA->PushString( "VMem" );
			LUA->PushCFunction( VMem );
			LUA->SetTable(-3);

			LUA->PushString("PMem");
			LUA->PushCFunction(PMem);
			LUA->SetTable(-3);

			LUA->PushString("CPU");
			LUA->PushCFunction(CPU);
			LUA->SetTable(-3);

			LUA->PushString("Net");
			LUA->PushCFunction(Net);
			LUA->SetTable(-3);

		LUA->SetTable(-3);

	return 0;
}

//
// Called when your module is closed
//
GMOD_MODULE_CLOSE()
{
	Sysinfo::CPU::Destroy();

	return 0;
}