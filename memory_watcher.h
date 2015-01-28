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

#include <stdint.h>

/**
 * @class MemoryWatcher
 * @brief Class getting memory information from underlying plaform
 *
 * 
 */
class MemoryWatcher {
public:
	MemoryWatcher(){};
	~MemoryWatcher(){};

	/**
	 * Get remaining RAM in bytes
	 * @return size in bytes
	 */
	static int32_t getRemainingRAM(void);

	/**
	 * Get remaining Stack in bytes
	 * @return size in bytes
	 */
	static int32_t getRemainingStack(void);

	/**
	 * Get remaining Stack in bytes
	 * @return size in bytes
	 */
	static int32_t getRemainingHeap(void);

	/**
	 * Check ram level
	 * @param arg_as8_file
	 * @param arg_u16_line
	 */
	static void checkRAM(const char* arg_as8_file, unsigned arg_u_line);

	/**
	 * Check ram level history to detect low level during execution
	 * @param arg_as8_file
	 * @param arg_u16_line
	 */
	static void checkRAMHistory(void);

	/**
	 * Paint stack during program execution in order to check dynamically stack
	 */
	static void paintStackNow(void);

	/**
	 * @return size in bytes of minimum remaining stack space
	 */
	static long getMinRemainingStack(void);

	/**
	 * @return size in bytes of minimum remaining heap space
	 */
	static long getMinRemainingHeap(void);
};

#endif /* MEMORYWATCHER_H_ */
