/******************************************************************************
 * @file    memory_watcher.h
 * @author  Rémi Pincent - INRIA
 * @date    29 janv. 2014   
 *
 * @brief description of classes getting memory information from underlying platform
 * 
 * Project : memory_watcher library
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/

#ifndef MEMORYWATCHER_H_
#define MEMORYWATCHER_H_

#include "Arduino.h"

/**
 * @class MemoryWatcher
 * @brief Class getting memory information from underlying plaform
 *
 * 
 */
class MemoryWatcher {
public:
	MemoryWatcher();
	~MemoryWatcher();

	/**
	 * Get remaining RAM in bytes
	 * @return size in bytes
	 */
	static long getRemainingRAM(void);

	/**
	 * Check ram level
	 * @param arg_as8_file
	 * @param arg_u16_line
	 */
	static void checkRAM(const __FlashStringHelper * arg_as8_file, int arg_u16_line);

	/**
	 * Check ram level history to detect low level during execution
	 * @param arg_as8_file
	 * @param arg_u16_line
	 */
	static void checkRAMHistory(void);

	/**
	 * Paint stack during program execution. Useful when malloc/free sequences
	 * are done. In this case if stack not painted, getMinRemainingStackSize()
	 * will return 0
	 */
	static void paintStackNow(void);

	/**
	 * @return size in bytes of minimum remaining stack space
	 * since program size startup
	 * Refer : http://www.avrfreaks.net/forum/soft-c-avrgcc-monitoring-stack-usage
	 *
	 * WARNING !!!!!!!!! invalid results when heap decreases - in this case
     * repaint stack calling paintStackNow()
	 */
	static long getMinRemainingStackSize(void);
};

#endif /* MEMORYWATCHER_H_ */
