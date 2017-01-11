#include "Controller.h"

#include "Microprocessor_debugging\debugging_disable.h"

namespace YbCtrl {

	Controller::Controller() :
		_errorChannel(NULL), _algorithm(NULL), _ctrlChannel(NULL)
	{
		debugPrintHeader();
		CONSOLE_LOG_LN(F("Default constuctor"));
	}

	// Constuctor using normal pointers
	Controller::Controller(ErrorChannel * errorInt, CtrlChannel * ctrlInt, Algorithm * algorithm) :
		// Store the pointers
		_errorChannel(errorInt), _algorithm(algorithm), _ctrlChannel(NULL)
	{
		// Store CtrlChannel if we're allowed
		attemptOwnershipOfCtrlChannel(ctrlInt);

		debugPrintHeader();
		CONSOLE_LOG_LN(F("Ptr constuctor"));
	}

	// Destructor
	Controller::~Controller() {

		debugPrintHeader();
		CONSOLE_LOG(F("Destructor for "));
		CONSOLE_LOG_LN((int)this);

		reset();
	}

	// Is this Controller valid?
	Controller::operator bool() const {

		debugPrintHeader();
		CONSOLE_LOG_LN(F("operator bool()"));

		bool ctrlValid	= _ctrlChannel 		&& *_ctrlChannel;
		bool algValid  	=  _algorithm 		&& *_algorithm;
		bool errValid 	= _errorChannel 	&& *_errorChannel;

		debugPrintHeader();
		CONSOLE_LOG(ctrlValid); CONSOLE_LOG(',');
		CONSOLE_LOG(algValid); CONSOLE_LOG(',');
		CONSOLE_LOG_LN(errValid);

		return ctrlValid && algValid && errValid;
	}

	// Wipe this Controller
	void Controller::reset() {

		debugPrintHeader();
		CONSOLE_LOG_LN(F("reset()"));

		// Print contents
		debugPrintHeader();
		debugPrintContents();

		// Remove reference to this Controller from the output channel
		if (_ctrlChannel) {
			debugPrintHeader();
			CONSOLE_LOG(F("Freeing "));
			CONSOLE_LOG_LN((int)_ctrlChannel);

			_ctrlChannel->setContainingController(0);
		}

		// Abort any measurement in progress
		if (0 != _errorChannel && _errorChannel->readInProgress()) {
			
			debugPrintHeader();
			CONSOLE_LOG_LN(F("Reading aborted"));

			_errorChannel->abortReading();
		}

		_errorChannel = 0;
		_ctrlChannel = 0;
		_algorithm = 0;
	}

	// Do a loop cycle
	int Controller::doLoop() {

		//// Is there a temporaryLooper object waiting to be executed?
		//// If so, run it
		//if (temporaryLooperPresent()) {

		//	// Has it finished? If so, delete it
		//	if (temporaryLooperFinished())
		//	{
		//		deleteTemporaryLooper();
		//		return 2;
		//	}
		//	else
		//	{
		//		runTemporaryLooper();
		//		return 1;
		//	}

		//}
		//else { // If not, run the normal loop:

		// Is a read already in progress?
		if (_errorChannel->globalReadInProgress()) {

			// Is it ours? (i.e. does it belong to our ErrorChannel?)
			if (!_errorChannel->readInProgress()) {

				// Nope. Give up and try again next time
				
				debugPrintHeader();
				CONSOLE_LOG_LN(F("ADC busy"));
				return -1;
			}
			
			debugPrintHeader();
			CONSOLE_LOG_LN(F("Reading already in progress"));

			// See if it's done
			if (_errorChannel->readingReady()) {

				debugPrintHeader();
				CONSOLE_LOG_LN(F("Reading done"));

				// Get the error signal
				double errorSig;
				ErrorChannelReturn result = _errorChannel->getReading(errorSig);

				// Check there wasn't an error
				if (ErrorChannelReturn::NO_ERROR != result) {
					// Oh no, there was
					CONSOLE_LOG_LN(F("Error returned by getReading()"));

					return -2;
				}

				debugPrintHeader();
				CONSOLE_LOG(F("Error is "));
				CONSOLE_LOG_LN(errorSig);

				// Calculate the new control signal
				double signal = _algorithm->output(errorSig);

				debugPrintHeader();
				CONSOLE_LOG(F("New Ctrl calculated as "));
				CONSOLE_LOG_LN(signal);

				// Output the new value of the control signal
				_ctrlChannel->setCtrl(signal);

				debugPrintHeader();
				CONSOLE_LOG_LN(F("New Ctrl set"));

			}
			else
			// Read in progress but not done yet
			{
				// Check for timeout
				if (_errorChannel->readingTimeout()) {
					// Timed out! Abort reading and retry next cycle
					debugPrintHeader();
					CONSOLE_LOG_LN(F("Timeout!"));

					_errorChannel->abortReading();
				}
			}
		}
		else {
			// No read in progress yet. Start one
			_errorChannel->startReading();

			// If the reading was started successfully, call this method again to
			// check if the reading is ready already
			if (_errorChannel->readInProgress())
			{
				doLoop();
			}
		}
		
		return 0;
		
	}

	void Controller::debugPrintHeader() const {
		CONSOLE_LOG(F("Controller("));
		CONSOLE_LOG((int)this);
		CONSOLE_LOG(F(")::"));
	}

	void Controller::debugPrintContents() const {
		CONSOLE_LOG_LN(F("reset. err, ctrl & algo @ :"));
		CONSOLE_LOG_LN((int)_errorChannel);
		CONSOLE_LOG_LN((int)_ctrlChannel);
		CONSOLE_LOG_LN((int)_algorithm);
	}

	// assign operator
	Controller& Controller::operator=(const Controller& rhs) {

		debugPrintHeader();
		CONSOLE_LOG(F("operator= @ "));
		CONSOLE_LOG_LN((int)&rhs);

		// Reset this Controller
		reset();

		_errorChannel = rhs._errorChannel;
		_algorithm = rhs._algorithm;

		// If a CtrlChannel exists, steal it
		if (rhs._ctrlChannel) {
			
			// Nick the pointer
			_ctrlChannel = rhs._ctrlChannel;

			// Remove it from the rhs
			rhs._ctrlChannel = 0;

			// Inform the CtrlChannel of the change of ownership
			_ctrlChannel->setContainingController(this);

			debugPrintHeader(); 
			CONSOLE_LOG(F("Stealing CtrlChannel "));
			CONSOLE_LOG_LN((int)_ctrlChannel);
		}
		else {
			debugPrintHeader(); 
			CONSOLE_LOG_LN(F("No CtrlChannel to steal"));
		}
		
	}

	int Controller::attemptOwnershipOfCtrlChannel(CtrlChannel * channel) {

		debugPrintHeader();
		CONSOLE_LOG_LN(F("attemptOwnershipOfCtrlChannel"));

		// Check that the ctrlInterface isn't already assigned to another Controller
		if (channel) {
			if (channel->getContainingController()) {
				// If so, don't store it
				return -1;
			}
			else {
				// Else, store it and assign it to this one
				_ctrlChannel = channel;
				channel->setContainingController(this);

				debugPrintHeader();
				CONSOLE_LOG(F("Taking ownership of "));
				CONSOLE_LOG_LN((int)channel);

				return 0;
			}
		}

		// return error: object was null
		return -2;
	}

}