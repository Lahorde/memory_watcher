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


/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/** Magic value for testing if the stack has overran the program variables.
 */
#define STACK_CANARY 0xc5

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
 * This symbol is defined to be the address at the bootom of the stack.
 */
/** stack end */
extern uint8_t __stack;
/** end of .bss section */
extern uint8_t __bss_end;
extern uint8_t __heap_start;
/** heap end */
extern uint8_t * __brkval;
/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Local Functions
 **************************************************************************/

MemoryWatcher::MemoryWatcher() {
	// TODO Auto-generated constructor stub

}

MemoryWatcher::~MemoryWatcher() {
	// TODO Auto-generated destructor stub
}

long MemoryWatcher::getRemainingRAM(void)
{
	long loc_s32FreeValue;
	if((int) __brkval == 0)
		loc_s32FreeValue = ((int)&loc_s32FreeValue) - ((int)&__bss_end);
	else
		loc_s32FreeValue = ((int)&loc_s32FreeValue) - ((int)__brkval);
	return loc_s32FreeValue;
}

void MemoryWatcher::checkRAM(const __FlashStringHelper * arg_as8_file, int arg_u16_line)
{
	long loc_f32_ram = getRemainingRAM();
	if(loc_f32_ram < RAM_WARN_LEVEL)
	{
		Serial.print(arg_as8_file);
		Serial.print(F(" - l"));
		Serial.print(arg_u16_line);
		Serial.print(F(" - LOW ram level "));
		Serial.println(loc_f32_ram);
	}
}

void MemoryWatcher::checkRAMHistory(void)
{
	long loc_f32_min_ram = getMinRemainingStackSize();
	if(loc_f32_min_ram < RAM_WARN_LEVEL)
	{
		Serial.print(F(" LOW ram level reached : "));
		Serial.println(loc_f32_min_ram);
	}
}

/**
 * Fill stack with a defined pattern.
 * After you can check maximum occupied space stack
 * using getMaxStackSize()
 * Refer : http://www.avrfreaks.net/forum/soft-c-avrgcc-monitoring-stack-usage
 */

__attribute__ ((naked,section (".init1")))
void stackPaint(void)
{
#if 0
    uint8_t *p = &_end;

    while(p <= &__stack)
    {
        *p = STACK_CANARY;
        p++;
    }
#else
    __asm volatile ("    ldi r30,lo8(_end)\n"
                    "    ldi r31,hi8(_end)\n"
                    "    ldi r24,lo8(0xc5)\n" /* STACK_CANARY = 0xc5 */
                    "    ldi r25,hi8(__stack)\n"
                    "    rjmp .cmp\n"
                    ".loop:\n"
                    "    st Z+,r24\n"
                    ".cmp:\n"
                    "    cpi r30,lo8(__stack)\n"
                    "    cpc r31,r25\n"
                    "    brlo .loop\n"
                    "    breq .loop"::);
#endif
}

void MemoryWatcher::paintStackNow(void)
{
    uint8_t *p = (uint8_t*) (__brkval == 0 ? &__heap_start : __brkval);;

    while(p < (uint8_t*)&p)
    {
        *p = STACK_CANARY;
        p++;
    }
}

/** WARNING !!!!!!!!! invalid results when heap decreases - in this case
 * repaint stack calling paintStackNow()
 */
long MemoryWatcher::getMinRemainingStackSize(void)
{
    const uint8_t *p = NULL;

    if(__brkval == 0){
    	p = &__bss_end;
    }
    else{
    	p = __brkval;
    }

    uint32_t c = 0;
    while(*p == STACK_CANARY && p <= &__stack)
    {
        p++;
        c++;
    }

    return c;
}
