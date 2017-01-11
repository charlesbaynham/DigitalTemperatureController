#pragma once

namespace YbCtrl {

	/**
	 * @defgroup   Algos Algorithms
	 *
	 * @brief Locking algorithms
	 * 
	 * The classes in this module are responsible for transforming error signals into control signals. They are based on the template base class Algorithm which defines various methods, most important of which is Algorithm::output(double). This method takes an error signal as a parameter (using the -1 -> +1 format that is universal in this codebase) and outputs a desired control signal (also using the same format).
	 */

	/**
	 * @brief      Abstract class for algorithms.
	 * This abstract class defines a template for a lock algorithm. The main method is output() which
	 * is responsible for taking an error signal and calculating a subsequent output from it. output() 
	 * obeys the convention followed everywhere in this code that both input and output signals can range
	 * from -1 to +1. 
	 * 
	 * As with all abstract classes, the pure virtual methods in this class MUST be overridden by derived
	 * classes. The virtual methods SHOULD be overridden. 
	 * 
	 * For an example implementation, see PIDAlgorithm. 
	 * 
	 * @ingroup Algos
	 */
	class Algorithm
	{
	public:

		Algorithm() {}

		virtual ~Algorithm() {}

		////////////////////////////////////////////////////////////////////////////////////////////////
		// The following methods MUST be define by children of this class (i.e. locking algorithms)	  //
		////////////////////////////////////////////////////////////////////////////////////////////////
		
		/**
		 * @brief      Do the locking calculation.
		 *
		 * @param[in]  input  The error signal, ranging from -1 to +1
		 *
		 * @return     The new ctrl level, ranging from -1 to +1
		 */
		virtual double output(double input) = 0;
		
		/** Report state.
		 *
		 * Send a string that identifies the currently running algorithm.
		 *
		 * @param[out] ptr   A buffer to contain the output. Should be at least
		 *                   128 chars long.
		 *
		 * @return     Returns ptr
		 */
		virtual char* reportState(char* ptr) = 0;

		
		/**
		 * @brief      Sets the output.
		 *
		 *             Set the output to a specified level, smoothly if
		 *             possible.
		 *
		 * @param[in]  output  The new output, from -1 to +1.
		 */
		virtual void setOutput(double output) = 0;

		
		/**
		 * @brief      Sets the setpoint.
		 * 
		 * Change setpoint, smoothly if possible.
		 *
		 * @param[in]  setpoint  The new setpoint, from -1 to +1
		 */
		virtual void setSetpoint(double setpoint) = 0;

		
		/**
		 * @brief      Gets the setpoint.
		 *
		 * @return     The current setpoint.
		 */
		virtual double getSetpoint() = 0;

		
		/**
		 * @brief      Sets output limits.
		 * 
		 * Set limits on the algorithm's output. Values are from -1 to +1. 
		 *
		 * @param[in]  min   The new minimum
		 * @param[in]  max   The new maximum
		 */
		virtual void setLimits(double min, double max) = 0;

		
		/**
		 * @brief      Does this algorithm lock?
		 *
		 *             Returns a boolean describing whether this Algorithm
		 *             implementation locks or not. E.g. the PIDAlgorithm, a
		 *             derived implementation of this class, does lock so it
		 *             overrides this method with
		 *
		 *             		return true;
		 *
		 *             Currently there are no examples of Algorithms that do not
		 *             lock.
		 *
		 * @return     true / false
		 */
		virtual bool lockingAlgo() { return false; }

		
		/**
		 * @brief      Is this algorithm valid?
		 *
		 *             Return true if this Algorithm is valid for use in calculations. 
		 *             E.g. see PIDAlgorithm for an example of why this may be useful. 
		 */
		virtual explicit operator bool () { return false; }

		////////////////////////////////////////////////////////////////////////////////////////////////
		// 											end												  //
		////////////////////////////////////////////////////////////////////////////////////////////////

	};

}
