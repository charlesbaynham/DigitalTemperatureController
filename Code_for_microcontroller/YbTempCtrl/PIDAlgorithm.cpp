#include "PIDAlgorithm.h"
#include "Arduino.h"

#include "Microprocessor_debugging\debugging_disable.h"

namespace YbCtrl {

	PIDAlgorithm::PIDAlgorithm(double K, double Ti, double Td, double initialOutput, double target, double N, bool disableProportional) :
		_K(K), _Ti(Ti), _Td(Td), _N(N), _target(target), _disableProportional(disableProportional),
		_isValid(true),
		_min(-1), _max(1)
	{
		CONSOLE_LOG(F("PIDAlgorithm::constuctor at "));
		CONSOLE_LOG_LN((int)this);

		// Reset the loop counter that will time the cycles to determine the precalculated constants
		_timingCount = 0;
		_timerStart = micros();

		// Calculate the constants assuming that the cycle time is 50ms: 
		//	this will be wrong, but get corrected on the first recalulation. 
		recalculateConsts(50000);

		// _Output should start as whatever it currently is
		setOutput(initialOutput);

		// Mark the PID as not having been run yet
		_firstRun = true;
	}


	PIDAlgorithm::~PIDAlgorithm()
	{
		CONSOLE_LOG(F("~PIDAlgorithm::at "));
		CONSOLE_LOG_LN((int)this);
	}

	// Set the output level manually
	void PIDAlgorithm::setOutput(double output) {

		CONSOLE_LOG(F("PIDAlgorithm::setOutput to "));
		CONSOLE_LOG_LN(output);

		// Check the bounds are OK
		if (output > _max)
			output = _max;
		else if (output < _min)
			output = _min;

		_output = output;

	}

	// Set limits on output
	void PIDAlgorithm::setLimits(double min, double max) {

		_min = (min <= -1 ? -1 : min);
		_max = (max >= 1 ? 1 : max);
		_min = (_min <= _max ? _min : _max);

		CONSOLE_LOG(F("PIDAlgorithm::setLimits "));
		CONSOLE_LOG(_min);
		CONSOLE_LOG(F(", "));
		CONSOLE_LOG_LN(_max);

	}

	// Change the PID's setpoint
	void PIDAlgorithm::setSetpoint(double setpoint) {

		// Spoof a steady state at previous value
		_old2 = _old;
		_deltaDold = 0;

		// Check the bounds are OK
		if (setpoint > 1)
			setpoint = 1;
		else if (setpoint < -1)
			setpoint = -1;

		// Update the setpoint
		_target = setpoint;
	}


	// Do the incremental PID calculation
	// See p242 of http://www.cds.caltech.edu/~murray/courses/cds101/fa02/caltech/astrom-ch6.pdf
	double PIDAlgorithm::output(double input)
	{

		// If it's the first run, spoof a steady state by setting the old value to the current error
		if (_firstRun) {
			_old = input;
			_old2 = input;
			_deltaDold = 0;
			_firstRun = false;
		}
		
		// Calculate the cycle time
		_timerEnd = micros();

		// Recalculate the constants. 
		// micros() will roll over to zero every 70 mins or so, so catch this
		if (_timerEnd > _timerStart)
		{
			long cycleTime = _timerEnd - _timerStart;

			recalculateConsts(cycleTime);
		}

		_timerStart = _timerEnd;

		double deltaD = _Kd1 * _deltaDold - _Kd2 * (input - 2 * _old + _old2);

		CONSOLE_LOG(F("Error: "));
		CONSOLE_LOG((_target - input));
		CONSOLE_LOG(F(", integral bit: "));
		CONSOLE_LOG(_Ki * (_target - input));
		CONSOLE_LOG(F(", deltaD: "));
		CONSOLE_LOG(deltaD);
		CONSOLE_LOG(F(", ctrl sig: "));
		CONSOLE_LOG(_output);
		CONSOLE_LOG(F(", input: "));
		CONSOLE_LOG_LN(input);

		if (_disableProportional)
			_output += _Ki * (_target - input) + deltaD;
		else
			_output += _Kp * (_old - input) + _Ki * (_target - input) + deltaD;

		_old2 = _old;
		_old = input;
		_deltaDold = deltaD;

		// Respect limits
		if (_output > _max)
			_output = _max;
		else if (_output < _min)
			_output = _min;

		return _output;
	}

	int PIDAlgorithm::recalculateConsts(const unsigned long microseconds)
	{
		const double cycletime = (float)microseconds / 1000000.0;
		// All calculations in seconds

		CONSOLE_LOG_LN(F("PIDAlgorithm::recalculateConsts. inputs are:"));
		CONSOLE_LOG_LN(_K);
		CONSOLE_LOG_LN(cycletime);
		CONSOLE_LOG_LN(_Ti);
		CONSOLE_LOG_LN(_Td);
		CONSOLE_LOG_LN(_N);

		_Kp = _K;
		_Ki = _K * cycletime / _Ti;
		_Kd1 = _Td / (_Td + _N * cycletime);
		_Kd2 = _Kd1 * _K * _N;

		CONSOLE_LOG_LN(F("PIDAlgorithm::Derived constants:"));
		CONSOLE_LOG_LN(_Kp);
		CONSOLE_LOG_LN(_Ki);
		CONSOLE_LOG_LN(_Kd1);
		CONSOLE_LOG_LN(_Kd2);

	}

}