/** @file
 * Contains all the code for the arduino-pin-toggler library
 */

/** @brief      The prescaler for TIMER1
 *
 *             This value is used to calculate the correct value to start TIMER1
 *             on, given this prescaler factor.
 *
 *             This does not need to be changed unless you need particularly
 *             fast flash rates.
 *
 *             Specifically, if your application requires `16000000 / PRESCALER
 *             / FLASH_FREQ_HZ >= 65536` you will need to alter the prescaler
 *             value both here and where it is set in the private
 *             setupSingleton() method.
 */
#define PRESCALER 1024

#define CONSOLE_LOG(...)
#define CONSOLE_LOG_LN(...)

/** @brief      How often the ISR will trigger.
 *
 *             This defines the max toggle rate. In combination with the
 *             #FLASHRATE chosen with pinToggler#setFlashRate, this determines
 *             the rate at which the pins will toggle.
 */
#define FLASH_FREQ_HZ 8

/**
 * @brief      Flash rate options.
 *
 *             These rates are used as parameters to the setFlashRate method.
 *
 *             In combination with FLASH_FREQ_HZ, these set the flash rate of an
 *             LED.
 *
 *             OFF and ON allow you to disable flashing, leaving the LED either
 *             on or off.
 */
enum FLASHRATE {
	ON = -1,		/*!< Always on */
	OFF = 0,		/*!< Always off */
	SLOW = 1,		/*!< Flash at FLASH_FREQ_HZ / 8 */
	MEDIUM = 2,		/*!< Flash at FLASH_FREQ_HZ / 2 */
	FAST = 4,		/*!< Flash at FLASH_FREQ_HZ / 4 */
	MAX = 8			/*!< Flash at FLASH_FREQ_HZ */
};

// Declare the TIMER1 ISR routine
extern "C" void TIMER1_OVF_vect(void) __attribute__((signal));

class pinTogglerBase {

	// The global, shared instance
	static pinTogglerBase * _instance;

	// Define a value for timer1_counter such that our ISR is called at the right rate
	static const int timer1_counter = 65536 - 16000000 / PRESCALER / FLASH_FREQ_HZ;

	// Allow the ISR to access this
	friend void TIMER1_OVF_vect();

	// Do the loop. This is implemented by each templated pinToggler version
	virtual void doLoop() = 0;

	// Get the global instance
	inline static pinTogglerBase * instance() { return _instance; }

public:
	// Return the number of pins defined by the templated derived class
	virtual int numManagedPins() = 0;

};

/**
 * @brief      Class for toggling pins.
 *
 *             This class maintains a list of pins that it controls as outputs.
 *             It initiates TIMER1 and uses this to trigger an interrupt
 *             routine, allowing arbitary numbers of pins to be toggled at
 *             selectable rates.
 *
 *             This class implements a singleton and all access to it is via
 *             static methods. The first usage of init() will define the value
 *             of numPins. Attempts to use init() with another value of numPins
 *             after this will fail with an error code.
 *
 *             For example usage see the Arduino sketches located in the
 *             `examples` directory.
 *
 * @tparam     numPins  The number of pins that this object will handle.
 */
template <int numPins>
class pinToggler : public pinTogglerBase {

private:
	inline int numManagedPins() override { return numPins; }

	// Pin assignments
	uint8_t _LED_Pins[numPins] = { 0 };

	// Counters for each LED: these count up to 8
	volatile uint8_t _LED_count[numPins] = { 0 };

	// Increment values for each LED: these define how much to increment the counters
	// on each loop cycle
	volatile uint8_t _LED_add[numPins] = { 0 };

	// N.B. the constuctor is private so only access is via instance()
	pinToggler() {}

	// This method takes ownership of the base class's pointer, pointing
	// it at this templated instance
	void setupSingleton(const uint8_t LED_Pins[numPins]) {

		// Store the pin numbers and set as outputs, low
		for (size_t i = 0; i < numPins; i++) {

			CONSOLE_LOG(F("setupSingleton::storing pin "));
			CONSOLE_LOG(LED_Pins[i]);
			CONSOLE_LOG(F(" in _LED_Pins["));
			CONSOLE_LOG(i);
			CONSOLE_LOG_LN(']');

			_LED_Pins[i] = LED_Pins[i];

			pinMode(LED_Pins[i], OUTPUT);
			digitalWrite(LED_Pins[i], LOW);
		}

		// Setup the timer

		CONSOLE_LOG_LN(F("Disabling interrupts"));

		// Wipe any existing config for TIMER1
		noInterrupts();
		TCCR1A = 0;
		TCCR1B = 0;

		// Start the timer at the calculated value to allow for 1/FLASH_FREQ_HZ s
		// until trigger
		TCNT1 = timer1_counter;

		// Set the prescaler to 1024
		// Alter the code here if you change the value of PRESCALER
		TCCR1B |= (1 << CS12);
		TCCR1B |= (1 << CS10);

		// Enable the TIMER1 overflow interrupt
		TIMSK1 |= (1 << TOIE1);

		// Reenable global interrupts
		interrupts();

		CONSOLE_LOG_LN(F("Timer setup and interrupts reenabled"));
	}

	/**
	 @brief      Do the loop.
	
	             This method is called by the ISR and controls the toggling of
	             the controlled pins. It should not be called manually (hence private). 
	*/
	void doLoop() override {
		
		CONSOLE_LOG(F("pinToggler<"));
		CONSOLE_LOG(numManagedPins());
		CONSOLE_LOG_LN(F(">::doLoop"));

		// Loop over LEDs
		for (int i = 0; i < numPins; i++) {
			// Add to the counter val
			_LED_count[i] += _LED_add[i];

			// If it's greater than 8, reset and toggle
			if (_LED_count[i] >= 8) {
				_LED_count[i] = 0;
				digitalWrite(_LED_Pins[i], digitalRead(_LED_Pins[i]) ^ 1);
			}
		}
	}

	static int checkValidity(const int LED) {
	  // Confirm that a) there is a static instance
	  if (!_instance)
	    return -1;

	  // b) it's of the same template type as this method call (i.e. same num pins)
	  if (_instance->numManagedPins() != numPins)
	    return -2;

	  // and c) the user has requested a valid pin
	  if (LED >= numPins)
	    return -3;

	  return 0;
	}

public:

	/**
	@brief      Initiate the pins and TIMER1
	
	            Setup and start the ISR triggered by TIMER1, and set TIMER1
	            running. Also, set all input pins as outputs and set their
	            output to LOW.
	
	@param[in]  LED_Pins  Pointer to a <numPins> dimention array of pins to be
	                      controlled. The values in this array will be copied so
	                      it can be deleted from memory after this function call
	                      is complete.
	
	@retval     0         No error
	@retval     -1        init() has been called already
	@retval     -2        Stack assignment failed: out of memory
	
	@return     Error code
	*/
	static int init(const uint8_t * LED_Pins) {

		CONSOLE_LOG_LN(F("pinToggler::init started"));

		if (NULL != _instance)
			return -1;

		CONSOLE_LOG_LN(F("pinToggler::no pre-existing static instance"));

		// Create the new object
		pinToggler * singletonInstance = new pinToggler;

		CONSOLE_LOG(F("pinToggler::instance defined at 0x"));
		CONSOLE_LOG_LN((unsigned int)singletonInstance, HEX);

		if (0 == singletonInstance) {
			CONSOLE_LOG_LN(F("pinToggler::Error, no space for assignment"));
			return -2;
		}

		// Setup its pins
		singletonInstance->setupSingleton(LED_Pins);

		CONSOLE_LOG_LN(F("pinToggler::pins setup"));

		// Store it in the base class
		_instance = singletonInstance;

		CONSOLE_LOG_LN(F("pinToggler::setup complete"));

		return 0;
	}

	/** Change the flash rate of an LED managed by this routine.
	 *
	 * This method sets the flash rate of one of the pins controlled by the
	 * pinToggler class. Pins are placed under control by passing them to
	 * init(), after which they are refered to by their zero-indexed position in
	 * the array passed to init().
	 *
	 * @param      LED   The zero reference LED to set the flash rate of. The
	 *                   LED number should correspond to the index of this pin
	 *                   in the array passed to init().
	 * @param      rate  The rate at which to flash this LED. Options are
	 *                   enumerated in #FLASHRATE.
	 *
	 * @retval     0     No error
	 * @retval     -1    init() has not been called.
	 * @retval     -2    init() was called with a different value of numPins
	 * @retval     -3    The given pin does not exist
	 *
	 * @return     Error code
	 */
	static int setFlashRate(const size_t LED, const FLASHRATE rate) {

	  // Check that this call is valid
	  int e = checkValidity(LED);
	  if (e)
	    return e;

	  // Cast to the derived type
	  pinToggler * ref = (pinToggler*)_instance;

	  // If the user has asked for a value < 0, change it to zero
	  uint8_t newRate = (rate < 0 ? 0 : rate);

	  // Change the rate
	  ref->_LED_add[LED] = newRate;

	  // If we disabled the LED, turn it off or on depending on user choice
	  if (rate <= 0) {
	    if (rate == OFF)
	      digitalWrite(ref->_LED_Pins[LED], LOW);
	    else // rate == ON
	      digitalWrite(ref->_LED_Pins[LED], HIGH);
	  }

	  return 0;
	}

	/**
	@brief      Return the pin corresponding to a given LED
	
	@param      LED   The zero reference LED to get the pin of. The LED number
	                  should correspond to the index of this pin in the array
	                  passed to init().
	
	@return     Pin number. N.B. If an error occurs the return value will be
	            negative, according to the error codes in #setFlashRate.
	*/
	static int getPin(const size_t LED) {
	  
	  // Check that this call is valid
	  int e = checkValidity(LED);
	  if (e)
	    return e;
	
	  // Cast to the derived type
	  pinToggler * ref = (pinToggler*)_instance;

	  // Return the pin number
	  return ref->_LED_Pins[LED];
	}

};

// Define the static instance
pinTogglerBase * pinTogglerBase::_instance = 0;

// Interrupt service routine
// This is called by TIMER1 at a rate of FLASH_FREQ_HZ
ISR(TIMER1_OVF_vect)
{
	// Reset timer
	TCNT1 = pinTogglerBase::timer1_counter;

	// Call the derived loop function
	pinTogglerBase * inst = pinTogglerBase::instance();
	if (inst) inst->doLoop();

}

