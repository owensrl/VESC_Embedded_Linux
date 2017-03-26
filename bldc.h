/*
	Copyright 2017 Ryan Owens

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

/*
 * bldc.h
 *
 *  Created on: 19 mar 2017
 *      Author: Ryan
 */
#ifndef BLDC_H
#define BLDC_H
#include "datatypes.h" // to receive data from BLDC:: scope
#include "motortypes.h" // motor constants
#include <string>
using std::string;

// Struct to store realtime data from VESC
struct RxData {
    float voltageIn;
    float tempPCB;
    float tempMOS1;
    float tempMOS2;
    float tempMOS3;
    float tempMOS4;
    float tempMOS5;
    float tempMOS6;
    float currentMotor;
    float currentIn;
    float rpm;
    float duty;
    float ampHours;
    float ampHoursCharged;
    float wattHours;
    float wattHoursCharged;
    int tachometer;
    int tachometerAbs;
    string faultCode;
};

// Rx values and position callback functions
// BLDC interface will call these functions implicitly
// whenever the appropriate packet is received
void bldc_val_received(mc_values *val);
void bldc_pos_received(float pos); // should work after VESC firmware mod

class BLDC {
public:
    /* Static member functions */
    // Function to initialize serial port for communication with VESC
    // Call this once in main function before declaring any
    // motor objects. Only needs to be called once.
    static void init(char* serialPort);
    // Function to close fd for read/write at end of program
    static void close(void);

    // Constructor
    BLDC(VescID vescID, Motor_Config motorConfig);
    // Destructor
    ~BLDC();
    
    /*  Setters */
    // Calling any one of these will cause the motor to spin
    // using the corresponding control method until VESC timeout occurs.
    // VESC timeout is currently set to 15sec. Call them at regular
    // intervals or send alive before the timeout occurs.
    void send_Alive(void); // reset VESC timeout
    void set_Speed(int rpm); // rpm control
    void set_Current(float amps); // torque control
    void set_Duty(float duty); // duty cycle control
    void set_Pos(float pos); // 360 degree position control
    void apply_Brake(float brake); // Stops motor using given brake current
    
    // Use the Unscaled setter functions to set speed and current
    // with raw Analog/Digital input values that have not been mapped.
    void set_Speed_Unscaled(float val); // rpm control with unmapped ADC values
    void set_Current_Unscaled(float val); // torque control with unmapped ADC values
    void set_Duty_Unscaled(float val); // duty control with unmapped ADC values
    
    // Getters
    void get_Values(void); // rpm, current, voltage, etc.
    void get_Pos(void); // Won't work without VESC firmware change.
    // VESC can send position continuously to BLDC tool
    // but does not respond to request from serial packet.
    
    // Print Rx data to console (for debugging)
    void print_Data(void);

private:
    int id; // CAN Address
    RxData rxData; // realtime data received from VESC
    Motor_Config config; // Individual motor parameters
    
    // Function to read data from UART1
    bool read_Data(void);
    
    // Scaling functions
    float scale_To_Float(float val, float min, float max);
    int scale_To_Int(float val, int min, int max);
};

#endif
