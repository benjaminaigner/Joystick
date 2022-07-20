# Joystick library

## Methods

### `Joystick.begin()`

Must be called before starting using the Joystick emulation. To end control, use `Joystick.end()`.

#### Syntax 

```
Joystick.begin()
```

#### Parameters

None.

#### Returns

None.

#### Example

```
#include <Joystick.h>

void setup() {
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  // Initialize the Joystick library when button is pressed
  if (digitalRead(2) == LOW) {
    Joystick.begin();
  }
}
```

#### See also


* [Joystick.button()](#joystickbutton)
* [Joystick.end()](#joystickend)
* [Joystick.send_now()](#joysticksendnow)
* [Joystick.position()](#joystickposition)
* [Joystick.X()](#joystickx)
* [Joystick.hat()](#joystickhat)
* [Joystick.use8bit()](#joystickuse8bit)
* [Joystick.useManualSend()](#joystickuseManualSend)

### `Joystick.button()`

Updates a button of the USB joystick.

#### Syntax 

```
Joystick.button(1,true);
delay(250);
Joystick.button(1,false);
```

#### Parameters

* `button`: number of Joystick button, which status should be changed
* `val`: state of button, `true` for pressed, `false` for released
  
#### Returns

None.

#### Example

```
#include <Joystick.h>

void setup() {
  pinMode(2, INPUT_PULLUP);
  Joystick.begin();
}

void loop() {
  uint8_t i = 1; //counter for the button number 1-32
  if (digitalRead(2) == LOW) { //if button is pressed
    Joystick.button(i,true); //"press" the Joystick button
	delay(250); //wait for 0.25s
	Joystick.button(i,false); //"release" the Joystick button
	i = i + 1; //increment & use next Joystick button number
	if(i > 32) i = 1; //we have 32 buttons available, wrap-around
  }
}
```

#### Notes

* Up to 32 buttons are available, numbered as button 1 to button 32.
* If manual_send is active, call `Joystick.send_now()` to send an update to the host.

#### See also

* [Joystick.send_now()](#joysticksendnow)
* [Joystick.useManualSend()](#joystickuseManualSend)
* [Joystick.X()](#joystickx)
* [Joystick.hat()](#joystickhat)

  
### `Joystick.end()`

Stops emulating the Joystick connected to a computer. To start control, use `Joystick.begin()`.

#### Syntax 

```
Joystick.end()
```

#### Parameters

None.

#### Returns

None.

#### Example

```
#include <Joystick.h>

void setup() {
  pinMode(2, INPUT_PULLUP);
  // Initiate the Joystick library
  Joystick.begin();
}

void loop() {
  // If the button is pressed, send a button 1 press / release
  if (digitalRead(2) == LOW) {
    Joystick.button(1,true);
	delay(250);
	Joystick.button(1,false);
    // Then end the Joystick emulation
    Joystick.end();
  }
}
```

#### See also

* [Joystick.begin()](#joystickbegin)

### `Joystick.use8bit()`

Switch axis value range between 10bit and 8bit.
* Default: 10bit, range for an axis from 0 to 1023
* 8bit mode: range from -127 to 127.

__Note:__ due to the gamepad descriptor of tinyUSB, the maximum range is -127/127. 10bit mode enables mapping, not a higher resolution.


#### Syntax 

```
Joystick.use8bit(true)
```

#### Parameters

* `mode`: true, if values from -127/127 are used. False to use a range from 0 to 1023.

#### Returns

None.

#### Example

```
#include <Joystick.h>

void setup() {
  pinMode(2, INPUT_PULLUP);
  Joystick.begin();
}

void loop() {
  //send middle position in default 10bit mode
  Joystick.position(512,512);
  delay(250);
  //enable 8bit mode
  Joystick.use8bit(true);
  //send middle position in 8bit mode
  Joystick.position(0,0);
  delay(250);
  
  //send maximum left in 10bit mode
  Joystick.use8bit(false);
  Joystick.position(0,0);
  delay(250);
  //enable 8bit mode
  Joystick.use8bit(true);
  //send left position in 8bit mode
  Joystick.position(-127,-127);
}
```

#### See also

* [Joystick.position()](#joystickposition)
* [Joystick.X()](#joystickx)
* [Joystick.Y()](#joysticky)
* [Joystick.Z()](#joystickz)
* [Joystick.Zrotate()](#joystickrotate)
* [Joystick.slider()](#joystickslider)
* [Joystick.sliderLeft()](#joysticksliderleft)
* [Joystick.sliderRight()](#joysticksliderright)


### `Joystick.useManualSend()`

To fully control transmitting the USB-HID reports, enable manual sending.
If disabled, each call to a function updating the Joystick status (buttons, all axis, hat)
will send a HID report. If you update in a loop, the time between updates (at least 1ms) is too short and something might be not transmitted correctly.
If enabled, update all your axis values, buttons, hat and then send one report via `Joystick.send_now()`.

#### Syntax 

```
Joystick.useManualSend(true)
```

#### Parameters

* `mode`: false is sending report each Joystick update, true enables manual sending via send_now().

#### Returns

None.

#### Example

```
#include <Joystick.h>

void setup() {
  pinMode(2, INPUT_PULLUP);
  Joystick.begin();
}

void loop() {
  if (digitalRead(2) == LOW) {
      // send data in 4 different reports
	  Joystick.button(1,true);
	  Joystick.button(2,true);
	  Joystick.button(3,true);
	  Joystick.button(4,true);

	  //enable manual send
	  Joystick.useManualSend(true);

	  //send same data in one report
	  Joystick.button(1,false);
	  Joystick.button(2,false);
	  Joystick.button(3,false);
	  Joystick.button(4,false);
	  Joystick.send_now();
  }
}
```

#### See also

* [Joystick.send_now()](#joysticksendnow)



### `Joystick.send_now()`

Send a HID report now. Used together with manual sending, see `Joystick.useManualSend()`.

#### Syntax 

```
Joystick.send_now()
```

#### Parameters

None.

#### Returns

None.

#### Example

```
#include <Joystick.h>

void setup() {
  pinMode(2, INPUT_PULLUP);
  Joystick.begin();
  //enable manual sending in setup
  Joystick.useManualSend(true);
}

void loop() {
  if (digitalRead(2) == LOW) {
      // update all buttons, but nothing is sent to the host
	  for(uint8_t i = 1; i<=32; i++) Joystick.button(i,true);
	  Joystick.X(256);
	  Joystick.sliderLeft(0);
	  
	  //now send in one HID report
	  Joystick.send_now();
  }
}
```

#### See also

* [Joystick.useManualSend()](#joystickusemanualsend)


### `Mouse.move()`

Moves the cursor on a connected computer. The motion onscreen is always relative to the cursor’s current location. Before using `Mouse.move()` you must call `Mouse.begin()`.

#### Syntax 

```
Mouse.move(xVal, yVal, wheel)
```

#### Parameters

* `xVal`: amount to move along the x-axis. Allowed data types: signed char.
* `yVal`: amount to move along the y-axis. Allowed data types: signed char.
* `wheel`: amount to move scroll wheel. Allowed data types: signed char.

#### Returns

None.

#### Example

```
#include <Mouse.h>

const int xAxis = A1;         // Analog sensor for X axis
const int yAxis = A2;         // Analog sensor for Y axis

int range = 12;               // Output range of X or Y movement
int responseDelay = 2;        // Response delay of the mouse, in ms
int threshold = range / 4;    // Resting threshold
int center = range / 2;       // Resting position value
int minima[] = {1023, 1023};  // Actual analogRead minima for (x, y)
int maxima[] = {0, 0};        // Actual analogRead maxima for (x, y)
int axis[] = {xAxis, yAxis};  // Pin numbers for (x, y)
int mouseReading[2];          // Final mouse readings for (x, y)

void setup() {
  // Initialize the Mouse library
  Mouse.begin();
}

void loop() {
  // Read and scale the two axes
  int xReading = readAxis(0);
  int yReading = readAxis(1);

  // Move the mouse
  Mouse.move(xReading, yReading, 0);
  delay(responseDelay);
}

/*
  Reads an axis (0 or 1 for x or y) and scales the
  analog input range to a range from 0 to <range>
*/
int readAxis(int axisNumber) {
  int distance = 0; // Distance from center of the output range

  // Read the analog input
  int reading = analogRead(axis[axisNumber]);

  // Of the current reading exceeds the max or min for this axis, reset the max or min
  if (reading < minima[axisNumber]) {
    minima[axisNumber] = reading;
  }
  if (reading > maxima[axisNumber]) {
    maxima[axisNumber] = reading;
  }

  // Map the reading from the analog input range to the output range
  reading = map(reading, minima[axisNumber], maxima[axisNumber], 0, range);

  // If the output reading is outside from the rest position threshold,  use it
  if (abs(reading - center) > threshold) {
    distance = (reading - center);
  }

  // The Y axis needs to be inverted in order to map the movement correctly
  if (axisNumber == 1) {
    distance = -distance;
  }

  // Return the distance for this axis
  return distance;
}
```

#### Notes and warnings

When you use the `Mouse.move()` command, the Arduino takes over your mouse! Make sure you have control before you use the command. A pushbutton to toggle the mouse control state is effective.

#### See also

* [Mouse.begin()](#mousebegin)
* [Mouse.click()](#mouseclick)
* [Mouse.end()](#mouseend)
* [Mouse.press()](#mousepress)
* [Mouse.release()](#mouserelease)
* [Mouse.isPressed()](#mouseispressed)

# TBD: 

hat
x y z
position
zrotate slider + left right 
