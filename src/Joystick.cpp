/*
    Joystick.cpp

    Copyright (c) 2022, Benjamin Aigner <beni@asterics-foundation.org>
    Implementation loosely based on:
    Mouse library from https://github.com/earlephilhower/arduino-pico
    Joystick functions from Teensyduino https://github.com/PaulStoffregen

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Joystick.h"
#include "Arduino.h"
#include <RP2040USB.h>

#include "tusb.h"
#include "class/hid/hid_device.h"

// Weak function override to add our descriptor to the TinyUSB list
void __USBInstallJoystick() { /* noop */ }

//================================================================================
//================================================================================
//	Joystick/Gamepad

Joystick_::Joystick_(void) {
    _use8bit = false;
    _autosend = true;
    memset(&data, 0, sizeof(data));
    //_X_axis = _Y_axis = _Z_axis = _Zrotate = _sliderLeft = _sliderRight = _hat = data.buttons = 0;
}

/** define the mapping of axes values
    default: axes methods are accepting values from 0-1023 (compatibility to other Joystick libraries)
		and are mapped internally to int8_t
    if use8bit(true) is called, -127 to 127 values are used.*/
void Joystick_::use8bit(bool mode) {
    _use8bit = mode;
}

//if set, the gamepad report is not automatically sent after an update of axes/buttons; use send_now to update
void Joystick_::useManualSend(bool mode) {
    _autosend = !mode;
}

/** Maps values from 8bit signed or 10bit unsigned to report values

    Depending on the setting via use8bit(), either values from 0-1023 or -127 - 127
    are mapped.
*/
int Joystick_::map8or10bit(int const value) {
    if (_use8bit) {
        if (value < -127) {
            return -127;
        } else if (value >  127) {
            return 127;
        } else {
            return value;
        }
    } else {
        if (value < 0) {
            return 0;
        }
        if (value > 1023) {
            return 1023;
        }
        return map(value, 0, 1023, -127, 127);
    }
}

void Joystick_::begin(void) {
}

void Joystick_::end(void) {
}

void Joystick_::button(uint8_t button, bool val) {
    //I've no idea why, but without a second dword, it is not possible.
    //maybe something with the alignment when using bit set/clear?!?
    static uint32_t buttons_local = 0;

    if (button >= 1 && button <= 32) {
        if (val) {
            buttons_local |= (1UL << (button - 1));
        } else {
            buttons_local &= ~(1UL << (button - 1));
        }

        data.buttons = buttons_local;
        if (_autosend) {
            send_now();
        }
    }
}

void Joystick_::setButton(uint8_t btn, bool val) {
    //simply call button, but we setButton uses 0-31; button 1-32
    button(btn + 1, val);
}

void Joystick_::X(int val) {
    data.x = map8or10bit(val);
    if (_autosend) {
        send_now();
    }
}
void Joystick_::Y(int val) {
    data.y = map8or10bit(val);
    if (_autosend) {
        send_now();
    }
}
void Joystick_::Z(int val) {
    data.z = map8or10bit(val);
    if (_autosend) {
        send_now();
    }
}
void Joystick_::Zrotate(int val) {
    data.rz = map8or10bit(val);
    if (_autosend) {
        send_now();
    }
}
void Joystick_::sliderLeft(int val) {
    data.rx = map8or10bit(val);
    if (_autosend) {
        send_now();
    }
}
void Joystick_::sliderRight(int val) {
    data.ry = map8or10bit(val);
    if (_autosend) {
        send_now();
    }
}

void Joystick_::slider(int val) {
    data.rx = map8or10bit(val);
    if (_autosend) {
        send_now();
    }
}

void Joystick_::position(int X, int Y) {
    data.x = map8or10bit(X);
    data.y = map8or10bit(Y);
    if (_autosend) {
        send_now();
    }
}

//additional hat function to use the hat position instead of the angle
void Joystick_::hat(HatPosition position) {
    data.hat = (uint8_t) position;
    if (_autosend) {
        send_now();
    }
}

//compatibility: there is only one hat implemented, num parameter is ignored
void Joystick_::hat(unsigned int num, int angle) {
    (void) num;
    hat(angle);
}

//set the hat value, from 0-360. -1 is rest position
void Joystick_::hat(int angle) {
    if (angle < 0) {
        data.hat = 0;
    }
    if (angle >= 0 && angle <= 360) {
        data.hat = map(angle, 0, 360, 1, 9);
    }
    if (data.hat == 9) {
        data.hat = 0;
    }
    if (_autosend) {
        send_now();
    }
}

//send back the Joystick report
void Joystick_::getReport(hid_gamepad_report_t *report) {
    memcpy(report, &data, sizeof(data));
}

//immediately send an HID report
void Joystick_::send_now(void) {
    CoreMutex m(&__usb_mutex);
    tud_task();
    if (tud_hid_ready()) {
        tud_hid_n_report(0, __USBGetJoystickReportID(), &data, sizeof(data));
    }
    tud_task();
}

Joystick_ Joystick;
