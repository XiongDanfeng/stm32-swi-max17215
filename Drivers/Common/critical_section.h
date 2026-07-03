#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void CriticalSection_Enter(void);
void CriticalSection_Exit(void);

#define EXECUTOR_ENTER_CRITICAL() CriticalSection_Enter()
#define EXECUTOR_EXIT_CRITICAL()  CriticalSection_Exit()

#ifdef __cplusplus
}
#endif

#endif