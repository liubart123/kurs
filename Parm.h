#pragma once
#include <tchar.h>
constexpr auto PARM_IN = L"/in:";
#define PARM_OUT L"/out:"
#define PARM_LOG L"/log:"
#define PARM_MAX_SIZE 300
#define PARM_OUT_DEFAULT_EXT L".out.asm"
#define PARM_OUT_SOURCE_DEFAULT_EXT L".out.asm"
#define PARM_LOG_DEFAULT_EXT L".log.txt"


namespace Parm
{
	struct PARM
	{
		wchar_t in[PARM_MAX_SIZE];
		wchar_t out[PARM_MAX_SIZE];
		wchar_t log[PARM_MAX_SIZE];
		wchar_t source[PARM_MAX_SIZE];
		PARM() {

		}
	};

	PARM getparm(int argc, _TCHAR *argv[]);
}