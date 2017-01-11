#pragma once
#include "Algorithm.h"
#include "Arduino.h"

#include "Microprocessor_debugging\debugging_disable.h"

namespace YbCtrl {

	/**
	 * @brief      Implementation of an Algorithm to perform a PID lock
	 *
	 *             An implementation of Algorithm that performs a PID lock based
	 *             on the parameters passed during its constuction.
	 *
	 *             The lock performed by this object implements the following
	 *             transfer function:
	 *
	 *             @f[ G(s) = -K \left( 1 + \frac{1}{sT_I} + \frac{sT_D}{1 +
	 *             \frac{sT_D}{N}} \right)
	 * @f]
	 * 
	 * When created without parameters passed to the consructor, this object will be
	 * marked invalid. This allows the user to reserve space on the stack, while still
	 * recording which PIDAlgorithms are properly setup.
	 * 
	 * As usual, all inputs / outputs are to be given in -1 to +1 format. 
	 * 
	 * @ingroup Algos
	 */
	class PIDAlgorithm :
		public Algorithm
	{
	public:
				
		/**
		 * @brief      Constuctor
		 *
		 * @param[in]  K                    Proportional gain
		 * @param[in]  Ti                   Integral time constant
		 * @param[in]  Td                   Differential time constant
		 * @param[in]  initialOutput        The initial output
		 * @param[in]  target               The target error signal
		 * @param[in]  N                    High frequency damping coefficient.
		 *                                  ~10 is a good value.
		 * @param[in]  disableProportional  Disable proportional gain
		 * @param[in]  timingCycles         To speed up execution, the PID
		 *                                  parameters are recalulated according
		 *                                  to the measured loop speed every
		 *                                  timingCycles loops.
		 * @param[in]  debug                Enable debug output
		 */
		PIDAlgorithm(double K, double Ti, double Td, double initialOutput, double target = 0, double N = 10, bool disableProportional = false);

		
		/**
		 * @brief      Alternative constuctor
		 *
		 *             A PIDAlgorithm constructed without parameters is marked
		 *             invalid. This can be useful for reserving stack space for
		 *             these objects, allowing them to be declared in advance.
		 *             
		 *             E.g.
		 *             
		 *             		PIDAlgorithm algos[3];
		 *             		
		 *             		bool valid = algos[1]; // returns false
		 *             		
		 *             		algos[1] = PIDALgorithm(1,1,0,0,0);
		 *             		
		 *             		bool valid = algos[1]; // returns true
		 */
		PIDAlgorithm() :
			_isValid(false), _K(0), _Ti(0), _Td(0), _N(0)
		{}

		virtual ~PIDAlgorithm();

		// This algorithm DOES lock
		inline bool lockingAlgo() override { return true; }

		/**
		 * @brief      Returns true if this object has been initialised with valid parameters
		 */
		explicit operator bool() override {
			CONSOLE_LOG_LN(F("PIDAlgorithm::operator(bool)"));
			return _isValid;
		}

		// Do the PID calculation
		
		/**
		 * @brief      Do the PID calculation
		 *
		 *             See p242 of
		 *             http://www.cds.caltech.edu/~murray/courses/cds101/fa02/caltech/astrom-ch6.pdf
		 *             for an in depth explaination of how this works
		 *
		 * @param[in]  input  The current error signal input. Values from -1 to +1
		 *
		 * @return     The calculated new ctrl signal. Values from -1 to +1
		 */
		double output(double input) override;

		// Set the output manually
		void setOutput(double output) override;

		// Change the setpoint
		void setSetpoint(double setpoint) override;

		// Set limits on output
		void setLimits(double min, double max) override;

		// Return the setpoint
		double getSetpoint() override { return _target; }

	protected:
		// Calculate the combined constants based on the number of us per cycle
		int recalculateConsts(unsigned long microseconds);

		// Local variables for the calculation
		double _output, _deltaDold;
		double _old, _old2;
		bool _debug;

		// Limits on Algo output
		double _min, _max;

		// Counter: the cycle is timed every 100 loops
		int _timingCount;
		// Start and end of loop
		unsigned long _timerStart, _timerEnd;

		// Is this the first run?
		bool _firstRun;

		// Calculated, combined values of the constants
		// These are calculated in recalculateConsts() and depend on the time for one cycle
		double _Kp, _Ki, _Kd1, _Kd2;

		// These are the constants input by the user to use for the PID calc:
		double _K, _Ti, _Td, _N;
		double _target;
		bool _disableProportional;

		// Report state. Send a string that identifies the currently running algorithm
		char* reportState(char* ptr) override { return strncpy(ptr, "Algo:PID", 128); }

		// Has this algorithm been constucted properly?
		bool _isValid;
	};


}
