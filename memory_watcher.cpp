/******************************************************************************
 * @file    MemoryWatcher.cpp 
 * @author  Rémi Pincent - INRIA
 * @date    29 janv. 2014   
 *
 * @brief Implementation of MemoryWatcher class
 * 
 * Project : memory_watcher library
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/

#include "memory_watcher.h"
#include "assert.h"
#include "app_util.h"
#include "app_error.h"
#include "stdio.h"

/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/** Pattern to fill memory - can be defined in startup file
 */
extern const uint32_t FREE_MEM_PATTERN;

#ifndef RAM_WARN_LEVEL
#define RAM_WARN_LEVEL 150
#endif

/**************************************************************************
 * Type Definitions
 **************************************************************************/

/**************************************************************************
 * Variables
 **************************************************************************/

/** Gain access to linker symbol for base of the stack.
 * This symbol is defined to be the address at the bottom of the stack.
 * Stack/Heap checked according to limits given in startup files
 */
/** stack start */
extern uint32_t __StackTop;
/** stack end */
extern uint32_t __StackLimit;

/** Heap base */
extern uint32_t __HeapBase;
/** heap end */
extern uint32_t __HeapLimit;
/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Local Functions
 **************************************************************************/
int32_t MemoryWatcher::getRemainingRAM(void)
{
	return getRemainingStack() + getRemainingHeap();
}

int32_t MemoryWatcher::getRemainingStack(void)
{
	return (uint8_t*)GET_SP() - (uint8_t*)&__StackLimit;
}


int32_t MemoryWatcher::getRemainingHeap(void)
{
	/** RP - 28/01/2015 - for now mapped to min remaining heap... */
	return getMinRemainingHeap();
}

void MemoryWatcher::checkRAM(const char * arg_as8_file, unsigned arg_u_line)
{
	int32_t loc_s32_ram = getRemainingRAM();

	/** only check stack */
	if(loc_s32_ram < RAM_WARN_LEVEL)
	{
		/** put here code to call some error handler or continue execution... */
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}
}

void MemoryWatcher::checkRAMHistory(void)
{
	int32_t loc_s32_minRam = getMinRemainingStack() + getMinRemainingHeap();
	if(loc_s32_minRam < RAM_WARN_LEVEL)
	{
		/** put here code to call some error handler or continue execution... */
		APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
	}
}


void MemoryWatcher::paintStackNow(void)
{
    uint32_t *p = (uint32_t *) GET_SP();

    while(p >= &__StackLimit)
    {
        *p = FREE_MEM_PATTERN;
        p--;
    }
}

int32_t MemoryWatcher::getMinRemainingStack(void)
{
    const uint32_t *p = &__StackLimit;
    uint32_t loc_u32_stackSpace = 0;

    while(*p == FREE_MEM_PATTERN && p < &__StackTop)
    {
        p++;
        loc_u32_stackSpace++;
    }
    return loc_u32_stackSpace*sizeof(*p);
}

int32_t MemoryWatcher::getMinRemainingHeap(void)
{
    const uint32_t *p = &__HeapLimit - 1;
    uint32_t loc_u32_heapSpace = 0;

    while(*p == FREE_MEM_PATTERN && p >= &__HeapBase)
    {
        p--;
        loc_u32_heapSpace++;
    }
    return loc_u32_heapSpace*sizeof(*p);
}
