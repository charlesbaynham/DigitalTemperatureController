#pragma once

#include "CtrlChannel.h"
#include "ErrorChannel.h"
#include "Algorithm.h"

// #include "TemporaryLooper.h"

/**
 * Namespace to hold all the temperature controller classes
 *
 * All the classes contained in the YbCtrl namespace are involved with the input
 * of error signals, output of control signals or calculation of the appropriate
 * ctrl signal from the corresponding error signal.
 */
namespace YbCtrl {

	/**
	 * @brief      Object to manage the locking loop.
	 *
	 *             This object manages pointers to an ErrorChannel, a
	 *             CtrlChannel and an Algorithm.
	 *
	 *             It uses these objects to get the error signal, calculate a
	 *             new output level and send that output every time doLoop() is
	 *             called.
	 *
	 *             This object can be marked as valid or invalid. It is
	 *             considered invalid if any of its three contained objects are
	 *             a) not present or b) themselves marked invalid.
	 */
	class Controller
	{
	public:

		/**
		 * @brief      Constuct an empty Controller
		 *
		 *             This empty controller is marked invalid.
		 */
		Controller();

		/**
		 * @brief      Constuct a Controller
		 *
		 *             Constuct a controller using pointers to its substituents.
		 *             This is the recommended constructor form.
		 *
		 *             N.B. a CtrlChannel may only be assigned to a single
		 *             Controller at a time. To ensure this, a pointer is placed
		 *             within the CtrlChannel to its controlling Controller,
		 *             if it exists. This constuctor checks for the presence of
		 *             this pointer and, if it points to another rival
		 *             Controller, it refuses to use the passed CtrlChannel
		 *             and this Controller is marked invalid.
		 *
		 * @param[in]  errorInt   The error interface
		 * @param[in]  ctrlInt    The control interface
		 * @param[in]  algorithm  The algorithm
		 */
		Controller(ErrorChannel* errorInt, CtrlChannel* ctrlInt, Algorithm* algorithm);

		
		/**
		 * @brief      Is this Controller valid?
		 *
		 *             Return true if the ErrorInterface, CtrlChannel and
		 *             Algorithm are all present and valid. Otherwise return
		 *             false.
		 */
		explicit operator bool() const;
		
		~Controller();		

		/**
		 * @brief      Wipe this Controller
		 *
		 *             Relinquishes control of the current CtrlChannel and
		 *             wipes all parameters.
		 */
		void reset();

		/**
		 * @brief      Do a locking cycle
		 *
		 *             Does one cycle of the loop. Reads error, performs
		 *             calculation and outputs result
		 *
		 * @return     Returns 0 (error code not implemented)
		 */
		int doLoop();

		/**
		 * @brief      Gets the control channel.
		 *
		 * @return     The control channel.
		 */
		inline CtrlChannel * getCtrlChannel() { return _ctrlChannel; }

		/**
		 * @brief      Gets the error interface.
		 *
		 * @return     The error interface.
		 */
		inline ErrorChannel * getErrorInterface() { return _errorChannel; }

		/**
		 * @brief      Gets the algorithm.
		 *
		 * @return     The algorithm.
		 */
		inline Algorithm * getAlgorithm() { return _algorithm; }

		
		/**
		 * @brief      Remove the current CtrlChannel
		 *
		 *             Calls replaceCtrlChannel() with a null pointer.
		 *
		 * @param[in]  <unnamed>  Any bool. Value is ignored
		 *
		 * @return     Error codes
		 *
		 * @retval     0          Success
		 */
		int replaceCtrlChannel(bool) {
			replaceCtrlChannel( (CtrlChannel*) 0 );
		}
			
		/**
		 * @brief      Replace the current CtrlChannel
		 *
		 *             Replaces the current CtrlChannel with a new one,
		 *             releasing control of the current CtrlChannel.
		 *
		 * @param[in]  newInterface  The new CtrlChannel
		 *
		 * @return     Error codes
		 *
		 * @retval     0             Success
		 * @retval     1             Error: new CtrlChannel is already assigned.
		 */
		int replaceCtrlChannel(CtrlChannel * newInterface) {

			// If the interface is an invalid pointer, this function is asking the Controller to disable itself
			// Delete the output channel and deassign this Controller from it
			if (!newInterface) {
				if (_ctrlChannel) {
					_ctrlChannel->setContainingController(0);
				}
				_ctrlChannel = 0;
				return 0;
			}

			if (newInterface->getContainingController()) {
				// If the new interface is already assigned to a controller, fail
				return 1;
			}

			// Otherwise, deassign the current interface and assign the new one
			if (_ctrlChannel) {
				_ctrlChannel->setContainingController(0);
			}
			_ctrlChannel = newInterface;
			_ctrlChannel->setContainingController(this);

			return 0;
		}

		/* Replace the ErrorInterface reference.
		 *
		 * @param[in]  interface  The new interface
		 *
		 * @return     Error codes
		 *
		 * @retval     0          Success
		 */
		int replaceErrorChannel(ErrorChannel * interface) {
			_errorChannel = interface;
			return 0;
		}

		/* Replace the Algorithm reference.
		 *
		 * @param[in]  interface  The new Algorithm
		 *
		 * @return     Error codes
		 *
		 * @retval     0          Success
		 */
		int replaceAlgorithm(Algorithm * algorithm) {
			_algorithm = algorithm;
			return 0;
		}

		// assign operator
		Controller& operator=(const Controller& rhs);

		//// Add a temporaryLooper object to this controller so that it will intercept the next loop runthrough
		//TemporaryLooper * addTemporaryLooper(TemporaryLooper * newLooper) {
		//	return _tempLooper = newLooper;
		//}

		//// Is there a temporaryLooper present?
		//bool temporaryLooperPresent() { return (bool)(_tempLooper); }

		//// Has the looper finished?
		//bool temporaryLooperFinished() { return _tempLooper->finished(); }

		//// Delete the looper
		//void deleteTemporaryLooper() { delete(_tempLooper); _tempLooper = 0; }

		//// Do a loop of the TemporaryLooper
		//void runTemporaryLooper() { _tempLooper->doLoop(); }

		//// Return a pointer to the temporary looper
		//TemporaryLooper * getTemporaryLooper() { return _tempLooper; }

		// Report state. Write into ptr a string that identifies the currently running loop. Returns -1 for an error. 
		// This is delegated to whichever object is currently 
		//		running: the temporary looper or the algorithm
		// int reportState(char* ptr) { return (temporaryLooperPresent() ? _tempLooper->reportState(ptr) : _algorithm->reportState(ptr)); }
		

		/**
		 * @brief      Reports the state.
		 *
		 *
		 *             Write into ptr a string that identifies the currently
		 *             running loop. This is delegated to the Algorithm
		 *
		 * @param[out]      ptr   Pointer to a buffer. Should hold at least 128 chars. 
		 *
		 * @return     Returns ptr
		 */
		char* reportState(char* ptr) { return _algorithm->reportState(ptr); }

		// Set the output manually
		bool setOutput(double output) {
			_algorithm->setOutput(output);
			_ctrlChannel->setCtrl(output);
		}

		// Change the setpoint
		bool setSetpoint(double setpoint) { _algorithm->setSetpoint(setpoint); }

	protected:
		
		// Temporary looper, to be given by user if needed
		// Initialised to the null pointer
		//TemporaryLooper * _tempLooper;

		// Interfaces for reading and writing to the world
		ErrorChannel * _errorChannel;
		mutable CtrlChannel * _ctrlChannel;

		// Algorithm object to handle the calculation
		Algorithm * _algorithm;

		// If channel is not already assigned, take ownership of it
		// Else, return an error code
		int attemptOwnershipOfCtrlChannel(CtrlChannel * channel);

	private:
		void debugPrintHeader() const;
		void debugPrintContents() const;

	};

}
