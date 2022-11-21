#ifndef _FT_COMMON_H
#define _FT_COMMON_H

#include <scshm.h>
#include <scsem.h>
#include <scutil.h>

#include <common_memory.h>

#ifdef __cplusplus
extern "C" {
#endif

COMMON_SHM*	mdb;

void	Log(char* fmt, ...);
void	SetProgramName(char* param);
int		InitCommonMemory();

int 	RegistProcess(int process_type);
void	RemoveProcess(int process_type);

#ifdef __cplusplus
}
#endif

#endif
