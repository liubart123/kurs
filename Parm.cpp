#include "Parm.h"
#include <tchar.h>
#include "Error.h"

namespace Parm
{

	PARM getparm(int argc, _TCHAR* argv[]) {
		int i = 1;
		PARM p;
		p.in[0] = '\0';
		p.log[0] = '\0';
		p.out[0] = '\0';
		while (i != argc) {
			if (wcsstr((wchar_t*)(argv[i]), PARM_IN) == (argv[i])) {
				if (p.in[0] != '\0') {
					continue;
				}
				else {
					if (wcslen((wchar_t*)(argv[i])) >= PARM_MAX_SIZE) {
						throw ERROR_THROW(104);
					}
					wchar_t s[PARM_MAX_SIZE];
					int cas = 0;
					for (cas = 0; cas < PARM_MAX_SIZE - 5 && (wchar_t*)(argv[i])[cas + 4] != L'\0'; cas++) {
						s[cas] = (argv[i])[cas + 4];
					}
					s[cas] = L'\0';
					wcscpy_s(p.in, s);
				}
			}
			else if (wcsstr((wchar_t*)(argv[i]), PARM_OUT) == (argv[i])) {
				if (p.out[0] != '\0') {
					continue;
				}
				else {
					if (wcslen((wchar_t*)(argv[i])) >= PARM_MAX_SIZE) {
						throw ERROR_THROW(104);
					}
					wchar_t s[PARM_MAX_SIZE];
					int cas = 0;
					for (cas = 0; cas < PARM_MAX_SIZE - 6 && (wchar_t*)(argv[i])[cas + 5] != L'\0'; cas++) {
						s[cas] = (argv[i])[cas + 5];
					}
					s[cas] = L'\0';
					wcscpy_s(p.out, s);
				}
			}
			else if (wcsstr((wchar_t*)(argv[i]), PARM_LOG) == (argv[i])) {
				if (p.log[0] != '\0') {
					continue;
				}
				else {
					if (wcslen((wchar_t*)(argv[i])) >= PARM_MAX_SIZE) {
						throw ERROR_THROW(104);
					}
					wchar_t s[PARM_MAX_SIZE];
					int cas = 0;
					for (cas = 0; cas < PARM_MAX_SIZE - 6 && (wchar_t*)(argv[i])[cas + 5] != L'\0'; cas++) {
						s[cas] = (argv[i])[cas + 5];
					}
					s[cas] = L'\0';
					wcscpy_s(p.log, s);
				}
			}
			else if (wcsstr((wchar_t*)(argv[i]), PARM_ERR) == (argv[i])) {
				if (wcslen((wchar_t*)(argv[i])) >= PARM_MAX_SIZE) {
					throw ERROR_THROW(104);
				}
				wchar_t s[PARM_MAX_SIZE];
				int cas = 0;
				for (cas = 0; cas < PARM_MAX_SIZE - 5 && (wchar_t*)(argv[i])[cas + 4] != L'\0'; cas++) {
					s[cas] = (argv[i])[cas + 4];
				}
				s[cas] = L'\0';
				int count = _wtoi(s);
				if (count <= 0) {
					throw ERROR_THROW(101);
				}
				p.ErrorsMaxCount = count;
			}
			else if (wcsstr((wchar_t*)(argv[i]), PARM_SHOW_SYNTAX_DETAILS) == (argv[i])) {
				if (wcslen((wchar_t*)(argv[i])) >= PARM_MAX_SIZE) {
					throw ERROR_THROW(104);
				}
				wchar_t s[PARM_MAX_SIZE];
				int cas = 0;
				for (cas = 0; cas < PARM_MAX_SIZE - 6 && (wchar_t*)(argv[i])[cas + 5] != L'\0'; cas++) {
					s[cas] = (argv[i])[cas + 5];
				}
				s[cas] = L'\0';
				int count = _wtoi(s);
				if (count < 0) {
					throw ERROR_THROW(101);
				}
				else if (count == 0) {
					p.showDetailsSyntax = false;
				} else { p.showDetailsSyntax =true;}
			}
			else if (wcsstr((wchar_t*)(argv[i]), PARM_SHOW_POLAND_DETAILS) == (argv[i])) {
				if (wcslen((wchar_t*)(argv[i])) >= PARM_MAX_SIZE) {
					throw ERROR_THROW(104);
				}
				wchar_t s[PARM_MAX_SIZE];
				int cas = 0;
				for (cas = 0; cas < PARM_MAX_SIZE - 6 && (wchar_t*)(argv[i])[cas + 5] != L'\0'; cas++) {
					s[cas] = (argv[i])[cas + 5];
				}
				s[cas] = L'\0';
				int count = _wtoi(s);
				if (count == 0) {
					p.polandDetails = false;
				}
				else { p.polandDetails = true; }
			}
			i++;
		}
		if (p.in[0] == '\0') {
			throw ERROR_THROW(100);
		}
		if (p.out[0] == '\0') {
			wcscpy_s(p.out, p.in);
			wcscat_s(p.out, PARM_OUT_DEFAULT_EXT);
		}
		if (p.log[0] == '\0') {
			wcscpy_s(p.log, p.in);
			wcscat_s(p.log, PARM_LOG_DEFAULT_EXT);
		}
		wcscpy_s(p.source, p.in);
		wcscat_s(p.source, PARM_OUT_SOURCE_DEFAULT_EXT);
		return p;
	}
}