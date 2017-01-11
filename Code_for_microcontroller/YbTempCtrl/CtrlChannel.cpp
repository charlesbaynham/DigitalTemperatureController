#include "CtrlChannel.h"

namespace YbCtrl {

	// Set the control signal. Calls writeCtrl and manages updating the linked
	// Controller if it exists
	//
	// See the documentation for this method in CtrlChannel.h for an
	// explaination of why it's in a .cpp file
	//
	void CtrlChannel::setCtrl(double val) {
		if (_containingController) {
			_containingController->getAlgorithm()->setOutput(val);
		}
		writeCtrl(val);
	}

	// Set limits on the control signal. Calls writeLimits and manages updating the linked
	// Controller if it exists
	//
	// See the documentation for this method in CtrlChannel.h for an
	// explaination of why it's in a .cpp file
	//
	CtrlChannelReturn CtrlChannel::setLimits(double minimum, double maximum) {
		if (_containingController) {
			_containingController->getAlgorithm()->setLimits(minimum, maximum);
		}
		writeLimits(minimum, maximum);
	}

	// Closes conflicting controllers. 
	CtrlChannelReturn CtrlChannel::closeConflictingControllers() {

		CONSOLE_LOG(F("CtrlChannel::closeConflictingControllers ("));
		CONSOLE_LOG((int)this);
		CONSOLE_LOG_LN(")");

		// Length of array
		const size_t arrayLen = sizeof(_conflictingChannels) / sizeof(*_conflictingChannels);

		// Buffer vars
		Controller * manager;
		CtrlChannelReturn out;

		// If there's a controller managing this channel, close it
		out = getContainingController(manager);
		if (out == CtrlChannelReturn::NO_ERROR) {
			
			CONSOLE_LOG(F("Closing this channel's @ "));
			CONSOLE_LOG_LN((int)manager);

			manager->reset();
		}

		// Loop over all potential conflicts and close these too if they exist
		for (size_t i = 0; i < arrayLen; i++) {

			// If there's a channel stored and it's valid
			if (_conflictingChannels[i] && _conflictingChannels[i]) {

				// Get manager of this conflicting channel
				out = _conflictingChannels[i]->getContainingController(manager);

				// Close it if it exists
				if (out == CtrlChannelReturn::NO_ERROR) {

					CONSOLE_LOG(F("Closing conflicting channel ("));
					CONSOLE_LOG((int)_conflictingChannels[i]);
					CONSOLE_LOG(F(")'s @ "));
					CONSOLE_LOG_LN((int)manager);

					manager->reset();
				}
			}

		}

		// Done
		return CtrlChannelReturn::NO_ERROR;
	}

}