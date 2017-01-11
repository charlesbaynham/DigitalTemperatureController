#pragma once

#include <limits.h>

namespace YbCtrl {

	/**
	 * Error codes for ErrorChannel operation
	 */
	enum class ErrorChannelReturn {
		NO_ERROR = 0,				/*!< No error */
		NOT_IMPLEMENTED,			/*!< Feature not implemented by derived class */
		WRITING_TO_REG_FAILED,		/*!< ADC comms failed */
		TIMEOUT,					/*!< Reading timeout */
		OUT_OF_RANGE,				/*!< Reading out of PGA range */
		PGA_ERROR,					/*!< Undefined PGA error */
		NO_SUCH_CHANNEL				/*!< Channel does not exist */
	};

	/**
	 * @defgroup   ErrorInput Error signal input
	 *
	 * @brief      Manage input of an error signal
	 *
	 *             The classes in this group are collectivly responsible for
	 *             managing the input of error signals from the real world.
	 *             Their template base class is ErrorChannel.
	 */

	 /**
	  * @brief      Abstract object to allow measuring an error signal
	  *
	  *             This object is a tempate for class that reads an input from
	  *             the real world. Child classes will implement specifics, e.g.
	  *             reading a voltage from an ADC.
	  *
	  *             As with the Algorithm class, this class is abstract and so
	  *             derived classes must override many of its methods.
	  *
	  *             For an example, see V4_ADC_ChannelPair.
	  *
	  * @ingroup    ErrorInput
	  */
	class ErrorChannel
	{
	public:

		ErrorChannel() {}

		virtual ~ErrorChannel() {}

		/* ***************************************************************
		  These functions must be defined by child implementation classes
		*****************************************************************/

		/**
		 * @brief      Recall error signal
		 *
		 *             Read out the last measured error. Values from -1 to 1.
		 *
		 * @deprecated use recallError(double&) instead
		 *
		 * @return     Last measured error signal from -1 to +1
		 */
		virtual double recallError() = 0;

		// These functions are the same as above, but instead of returning doubles they return
		// error statuses and copy the double return value into the passed reference `output`

		/**
		 * @brief      Recall error signal
		 *
		 *             Read out the last measured error into output. Values from
		 *             -1 to 1.
		 *
		 * @param[out] output  Target for the error signal
		 *
		 * @return     Error code
		 */
		virtual ErrorChannelReturn recallError(double& output) = 0;

		/**
		 * @brief      Check if there's a read in progress by this channel
		 *
		 *             Returns true if a reading has been started with
		 *             startReading, has not been aborted with abortReading and
		 *             has not been returned by getReading BY THIS ErrorChannel
		 *
		 *             Otherwise returns false
		 *
		 *             Before it is overridden, this function always returns
		 *             true. This is so that simple ErrorChannels that do not
		 *             require this start / check / read method can instead just
		 *             return a value from getReading each time.
		 *
		 *             Child objects should either override all of
		 *             readInProgress, readingReady, readingTimeout and
		 *             startReading or none of them.
		 *
		 * @return     true / false
		 */
		virtual bool readInProgress() {
			return true;
		}
		
		/**
		 * @brief      Check if there's a read in progress globally
		 *
		 *             Returns true if a reading has been started with
		 *             startReading, has not been aborted with abortReading and
		 *             has not been returned by getReading BY ANY ErrorChannel.
		 *
		 *             Otherwise returns false
		 *
		 *             Before it is overridden, this function always returns
		 *             true. This is so that simple ErrorChannels that do not
		 *             require this start / check / read method can instead just
		 *             return a value from getReading each time.
		 *
		 *             Child objects should either override all of
		 *             readInProgress, readingReady, readingTimeout and
		 *             startReading or none of them.
		 *
		 * @return     true / false
		 */
		virtual bool globalReadInProgress() {
			return true;
		}

		/**
		 * @brief      Check if the current reading is ready
		 *
		 *             Returns true if a reading has been started with
		 *             startReading, has not been aborted with abortReading, has
		 *             not been returned by getReading and has successfully
		 *             completed
		 *
		 *             Otherwise returns false
		 *
		 *             Before it is overridden, this function always returns
		 *             true. This is so that simple ErrorChannels that do not
		 *             require this start / check / read method can instead just
		 *             return a value from getReading each time.
		 *
		 *             Child objects should either override all of
		 *             readInProgress, readingReady, readingTimeout and
		 *             startReading or none of them.
		 *
		 * @return     true / false
		 */
		virtual bool readingReady() {
			return true;
		}

		/**
		 * @brief      Check if the current reading is has timed out
		 *
		 *             Returns true if a reading has been started with
		 *             startReading, has not been aborted with abortReading, has
		 *             not been returned by getReading, has not successfully
		 *             completed and has exceeded a maximum timeout.
		 *
		 *             Otherwise returns false
		 *
		 *             Before it is overridden, this function always returns
		 *             false. This is so that simple ErrorChannels that do not
		 *             require this start / check / read method can instead just
		 *             return a value from getReading each time.
		 *
		 *             Child objects should either override all of
		 *             readInProgress, readingReady, readingTimeout and
		 *             startReading or none of them.
		 *
		 * @return     true / false
		 */
		virtual bool readingTimeout() {
			return false;
		}

		/**
		 * @brief      Aborts a reading currently in progress
		 *
		 *             If a reading is in progress, abort it, If not, do
		 *             nothing
		 *
		 *             Child objects should either override all of
		 *             readInProgress, readingReady, readingTimeout and
		 *             startReading or none of them.
		 */
		virtual void abortReading() { }

		/**
		 * @brief      Start reading
		 *
		 *             Start a new reading if none in progress (by this channel
		 *             or globally).
		 *
		 *             If a reading is already in progress, abort it and start
		 *             another one.
		 *
		 *             Child objects should either override all of
		 *             readInProgress, readingReady, readingTimeout and
		 *             startReading or none of them.
		 *
		 * @return     ErrorChannelReturn error code.
		 */
		virtual ErrorChannelReturn startReading() { return ErrorChannelReturn::NO_ERROR; }

		/**
		 * @brief      Get the latest reading
		 *
		 *             Gets the completed reading and stores it in
		 *             readingOutput. The reading uses the format -1 -> +1,
		 *             where -1 corresponds to the minimum value the
		 *             ErrorChannel can read and vice versa.
		 *
		 *             If no conversion is in progress / the conversion failed,
		 *             return an error message.
		 *
		 *             Otherwise return ErrorChannelReturn::NO_ERROR.
		 *
		 *             This method must be overridden by child objects.
		 *
		 * @param[out] readingOutput  Output variable for the reading.
		 *
		 * @return     ErrorChannelReturn error code.
		 */
		virtual ErrorChannelReturn getReading(double &readingOutput) = 0;

		/**
		 * @brief      Is this object valid?
		 *
		 * Return true if this object has been constucted with valid parameters
		 */
		virtual explicit operator bool() const = 0;

		/* ***************************************************************
		*****************************************************************/
	};


}