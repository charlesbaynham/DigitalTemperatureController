#pragma once

// N.B. "Controller.h" is included at the bottom of this file.
// This is because the definition of Controller depends on this file, so this class must
// be defined first

#include "Microprocessor_debugging\debugging_disable.h"

namespace YbCtrl {

	/**
	 * Error codes for CtrlChannel operation
	 *
	 * @todo Change to enum class
	 */
	enum class CtrlChannelReturn {
		NO_ERROR = 0, 			/*!< No error */
		NOT_IMPLEMENTED, 		/*!< Feature not implemented by derived class */
		NO_SUCH_CHANNEL, 		/*!< This channel does not exist */
		CHANNEL_NOT_MANAGED,	/*!< This channel has no managing Controller */
		INVALID_PARAMETER,		/*!< Parameter passed was invalid */
		OUT_OF_MEMORY			/*!< Out of memory */
	};

	// Forward declaration of Controller so that we can use pointers to it
	class Controller;

	/**
	 * @defgroup   CtrlOutput Control signal output
	 *
	 * @brief      Manage output of a control signal
	 *
	 *             The classes in this group are collectivly responsible for
	 *             managing the output of control signals to the real world.
	 *             Their template base class is CtrlChannel.
	 */

	 /**
	  * @brief      Abstract object to allow outputting a control signal
	  *
	  *             This object is a tempate for class that writes an output to
	  *             the real world. Child classes will implement specifics, e.g.
	  *             writing a voltage to an OPA.
	  *
	  *             As with the Algorithm class, this class is abstract and so
	  *             derived classes must override many of its methods.
	  *
	  *             For an example, see V4_OPA_OutputChannel.
	  *
	  * @ingroup    CtrlOutput
	  */
	class CtrlChannel
	{
	public:

		// Default constructor
		CtrlChannel() : 
			_containingController(0), _conflictingChannels{ 0 }
		{
			CONSOLE_LOG(F("CtrlChannel::Constuctor of "));
			CONSOLE_LOG_LN((int)this);
		}

		virtual ~CtrlChannel() {
			CONSOLE_LOG(F("CtrlChannel::destructor of "));
			CONSOLE_LOG_LN((int)this);
		}

		/* ***************************************************************
		These functions must be defined by derived implementation classes
		*****************************************************************/
	protected:

		// Writes to the ctrl signal
		virtual void writeCtrl(double val) = 0;

		// Sets the limits
		virtual CtrlChannelReturn writeLimits(double minimum, double maximum) = 0;

	public:

		/**
		 * @brief      Read out the current output.
		 *
		 *             Read out the current output. Values from -1 to 1.
		 *
		 * @return     Current ctrl level
		 */
		virtual double recallCtrl() = 0;

		/**
		 * @brief      Set software limits on the max/min ctrl signal
		 *
		 *             Also, if this channel is managed by a Controller, inform
		 *             that Controller's Algorithm of the new limits
		 *
		 * @note       This method is defined in an external object file. This
		 *             is necessary because, at the point of including this
		 *             header file, the Controller object has not yet been fully
		 *             defined. Since setCtrl() uses methods in Controller, it
		 *             cannot be compiled until Controller is fully defined.
		 *             Therefore the code must be in a .cpp file, to be compiled
		 *             later after all the headers are resolved.
		 *
		 * @note       Similarily to setCtrl() vs writeCtrl(), this is the
		 *             public interface which is defined by the base class. The
		 *             derived classes will overwrite the private pure virtual
		 *             member writeLimits() which is called by this function and
		 *             which will actually set the limits.
		 *
		 * @param[in]  minimum  The new minimum
		 * @param[in]  maximum  The new maximum
		 *
		 * @return     Error status
		 */
		virtual CtrlChannelReturn setLimits(double minimum, double maximum) final;
		// By declaring this method "virtual final" we prevent child classes
		// from erroneously attempting to override this method when they should
		// actually be overriding writeLimits()

		/**
		 * @brief      Is this object valid?
		 *
		 * Return true if this object has been constucted with valid parameters
		 */
		virtual explicit operator bool() const = 0;

		/* ***************************************************************
		These functions can optionally be defined by derived implementation classes
		*****************************************************************/


		/**
		 * @brief      Limits the current
		 *
		 *             If implemented, limit the current to the given value.
		 *
		 *             N.B. this method DOES NOT use -1 -> +1 notation; the
		 *             input is in amps.
		 *
		 * @param[in]  val   The current limit in Amps
		 *
		 * @return     Error code
		 */
		virtual CtrlChannelReturn setCurrentLimit(double val) {
			CONSOLE_LOG_LN(F("CtrlChannel::setCurrentLimit"));
			return CtrlChannelReturn::NOT_IMPLEMENTED;
		}

		/**
		 * @brief      Gets the current limit.
		 *
		 *             Gets the current limit. Value in amps
		 *
		 * @deprecated Use getCurrentLimit(double&) instead
		 *
		 * @return     The current limit in amps. -999 if not implemented
		 */
		virtual double getCurrentLimit() {
			CONSOLE_LOG_LN(F("CtrlChannel::getCurrentLimit"));
			return -999;
		}

		/**
		 * @brief      Check for overheated
		 *
		 *             Check if this channel is overheated
		 *
		 * @param[out] state  True if overheated, false otherwise
		 *
		 * @return     Error return
		 */
		virtual CtrlChannelReturn isOverheated(bool& state) {
			CONSOLE_LOG_LN(F("CtrlChannel::isOverheated"));
			return CtrlChannelReturn::NOT_IMPLEMENTED;
		}

		/* ***************************************************************
		These functions are the same as the ones above, but return CtrlChannelReturn
		error messages instead, copying any output data into the passed ref
		*****************************************************************/

		// Read out the current output. Values from -1 to 1.
		virtual CtrlChannelReturn recallCtrl(double& ctrl) = 0;


		/**
		 * @brief      Gets the output limits.
		 *
		 *             Store the output limits in the passed references.
		 *
		 * @param[out] min   The minimum
		 * @param[out] max   The maximum
		 *
		 * @return     Error code
		 */
		virtual CtrlChannelReturn getLimits(double& min, double& max) = 0;

		/**
		 * @brief      Gets the current limit.
		 *
		 * Gets the current limit and stores it in out. Value in amps
		 *
		 * @param[out]      out   The current limit in amps
		 *
		 * @return     Error code
		 */
		virtual CtrlChannelReturn getCurrentLimit(double& out) {
			CONSOLE_LOG_LN(F("CtrlChannel::getCurrentLimit"));
			return CtrlChannelReturn::NOT_IMPLEMENTED;
		}

		/* ***************************************************************
		*****************************************************************/

		/**
		 * @brief      Sets the control signal.
		 *
		 *             Set the control signal to the given value. Also, if this
		 *             channel is managed by a Controller, inform that
		 *             Controller's Algorithm of the updated output level
		 *
		 * @note       This method is declared in an external object file. This
		 *             is necessary because, at the point of including this
		 *             header file, the Controller object has not yet been fully
		 *             defined. Since setCtrl() uses methods in Controller, it
		 *             cannot be compiled until Controller is fully defined.
		 *             Therefore the code must be in a .cpp file, to be compiled
		 *             later after all the headers are resolved.
		 *
		 *             See the documentation for #setLimits for an explaination
		 *             as to why this structure is needed
		 *
		 * @param      val   The value, from -1 to +1
		 */
		virtual void setCtrl(double val) final;
		// By declaring this method "virtual final" we prevent child classes
		// from erroneously attempting to override this method when they should
		// actually be overriding writeCtrl()


	private:

		// This pointer points to the Controller that manages this Channel, if
		// there is one. If this channel is managed by a Controller, it cannot
		// be added to another Controller until its managing Controller is
		// deleted. Changes to the channel's output will also update the
		// Controller's Algorithm's output.
		Controller * _containingController;

		// This array stores pointers to conflicting channels whose Controllers
		// can be closed with closeConflictingControllers if they exist
		CtrlChannel * _conflictingChannels[2];
	public:

		/**
		 * @brief      Sets the containing Controller.
		 *
		 * Set this object's managing Controller to the given target
		 *
		 * @param      newController  The new managing Controller
		 */
		inline void setContainingController(Controller * newController) { _containingController = newController; }

		/**
		 * @brief      Gets the containing Controller.
		 *
		 * @deprecated Use getContainingController(Controller*&) instead
		 *
		 * @return     The containing controller. NULL if none present.
		 */
		inline Controller* getContainingController() { return _containingController; }


		/**
		 * @brief      Gets the containing controller.
		 *
		 * @param[out]      out   The containing Controller
		 *
		 * @return     Error code
		 */
		CtrlChannelReturn getContainingController(Controller*& out) {

			out = _containingController;

			if (_containingController)
			{
				return CtrlChannelReturn::NO_ERROR;
			}
			else
			{
				return CtrlChannelReturn::CHANNEL_NOT_MANAGED;
			}
		}

		/**
		 * @brief      Adds a conflicting channel.
		 *
		 *             Adds a channel that is considered to be conflicting with
		 *             this one. I.e. this channel and the conflicting one
		 *             should not be simultaneously controlled.
		 *
		 *             This is useful e.g. for bipolar vs. single-sided channels
		 *             which use the same outputs.
		 *
		 *             Up to 2 channels can be added using this function. Any
		 *             Controllers associated with these channels can be closed
		 *             with closeConflictingControllers
		 *
		 * @return     Error code
		 */
		CtrlChannelReturn addConflictingChannel(CtrlChannel * newConflict) {

			CONSOLE_LOG(F("addConflictingChannel: adding "));
			CONSOLE_LOG_LN((int)newConflict);
			
			// Length of array
			const size_t arrayLen = sizeof(_conflictingChannels) / sizeof(*_conflictingChannels);

			// Loop through array and store this pointer in the first empty slot
			for (size_t i = 0; i < arrayLen; i++) {
				if (_conflictingChannels[i] == 0) {
					CONSOLE_LOG(F("Storing in slot "));
					CONSOLE_LOG_LN(i);

					_conflictingChannels[i] = newConflict;
					return CtrlChannelReturn::NO_ERROR;
				}
			}

			CONSOLE_LOG_LN(F("Storing failed!"));

			// If we get here, we didn't store it
			return CtrlChannelReturn::OUT_OF_MEMORY;
		}


		/**
		 * @brief      Closes conflicting controllers.
		 *
		 *             Close any Controllers that are managing either this
		 *             channel or any conflicting ones. Conflicting channels are
		 *             identified by adding pointers to them with
		 *             addConflictingChannel.
		 *
		 * @note       Similarily to setCtrl() vs writeCtrl(), this is the
		 *             public interface which is defined by the base class. The
		 *             derived classes will overwrite the private pure virtual
		 *             member writeLimits() which is called by this function and
		 *             which will actually set the limits.
		 *
		 * @return     Error code
		 */
		CtrlChannelReturn closeConflictingControllers();

	};
}

#include "Controller.h"