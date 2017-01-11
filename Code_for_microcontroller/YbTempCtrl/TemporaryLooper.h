#pragma once

#include "IOInterface.h"
namespace YbCtrl {

	
	/**
	 * @brief      Temporarily divert the lock, before returning to normal
	 *
	 *             Class for defining actions that need to take place instead of
	 *             the loop, for a short time, after which the controller
	 *             reverts to locking as normal.
	 *
	 * @todo This class is currently unused: the code in Controller exists but
	 * is commented out. It could be used to implement e.g. an autotuning lock
	 * or a relocking routine
	 */
	class TemporaryLooper
	{

	public:
		// Initiate the temporary loop with the IOInterface and LabInterface
		// N.B. this syntax is deprecated
		TemporaryLooper() : 
		{}

		// Does one loop of this temporary function
		virtual int doLoop() =0;

		// Returns true once the temporary loop has finished & control should return to the normal locking loop
		virtual bool finished() =0;

		// Report state. Send a string that identifies the currently running algorithm
		virtual int reportState(char* ptr) = 0;

		// Print any relevant data to the output
		virtual void reportData() {}

		virtual ~TemporaryLooper()
		{}

	protected:

		// IO interface to deal with the ADC / DAC
		// N.B. this syntax is deprecated
		IOInterface * _ControlInterface;

	};

}
