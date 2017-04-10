// Main program to test UART communication from BBB to VESC
// Last modified on 3/7/2017 by: Ryan Owens
#include <stdio.h>
#include "bldc.h"
#include "motortypes.h"
#include <unistd.h> // for usleep
#include "timers.h" // for receiving data. must compile with -lrt flag.

static enum {RequestMotor1, ReadMotor1, RequestMotor2, ReadMotor2} readMode = RequestMotor1;

int main(void) {
	// constants 
	const int timerSP = 17; // timer set-point in milliseconds.
	                        // adjust for frequency of read data.
	                        // setting too low may cause packets to be dropped.
	// variables
	int command = 0;
	int sleep = 3000;
	bool decrement = false;
	bool loop = true;
	bool read = true;
	
	float val = 0;
	float brake = 0;
	float pos = 0;
	
	RxData testData = {};
	
	// Initialize the Serial interface
	BLDC::init((char*)"/dev/ttyO1");
	
	// Initialize motor objects
	// Parameters are defined in motortypes.h
	BLDC leftMotor(VESC1, motor1);
	BLDC rightMotor(VESC2, motor2);
	
	// Create new timer for reading data
	timer_t* timerid = new_timer();
    if(timerid == NULL) {
        fprintf(stderr, "Timer creation failed\n");
        return 1;
    }
	// Main loop 
	while(loop) {
		printf("Choose a command\n");
		printf("    1 : Set speed\n");
		printf("    2 : Set current\n");
		printf("    3 : Apply brake\n");
		printf("    4 : Set duty cycle\n");
		printf("    5 : Set position\n");
		printf("    6 : Sweep position 0-360 degrees\n");
		printf("    7 : Get values\n");
		printf("    8 : Send alive\n");
		printf("Other : End\n");
		printf("Enter a number: ");
		scanf("%d", &command);
		switch(command) {
			case 1:
				printf("Enter desired speed in RPM: ");
				scanf("%f", &val);
				leftMotor.set_Speed(val);
				rightMotor.set_Speed(val);
				printf("Speed set to %f RPM\n\n", val);
				break;
			case 2:
				printf("Enter desired current in Amps: ");
				scanf("%f", &val);
				leftMotor.set_Current(val);
				rightMotor.set_Current(val);
				printf("Current set to %f Amps\n\n", val);
				break;
			case 3:
				printf("Enter desired brake current in Amps: ");
				scanf("%f", &brake);
				leftMotor.apply_Brake(brake);
				rightMotor.apply_Brake(brake);
				printf("Brake current set to %f Amps\n\n", brake);
				break;
			case 4:
				printf("Enter desired duty cycle -1.0 to 1.0: ");
				scanf("%f", &val);
				leftMotor.set_Duty(val);
				rightMotor.set_Duty(val);
				printf("Duty cycle set to %f\n\n", val);
				break;
			case 5:
				printf("Enter desired position 0-360 degrees: ");
				scanf("%f", &pos);
				leftMotor.set_Pos(pos);
				rightMotor.set_Pos(pos);
				printf("Position set to %f\n\n", pos);
				break;
			case 6:
				printf("Sweeping position from 0-360 degrees\n\n");
				while (true) {
					leftMotor.set_Pos(pos);
					rightMotor.set_Pos(pos);
					if (pos == 360)
						decrement = true;
					else if (pos == 0)
						decrement = false;

					if (decrement == true)
						pos -= 1;
					else
						pos += 1;
					usleep(sleep);
				}
				break;
			case 7:
				// Loop through state machine until all reads are finished.
				// This loop can also be implemented as a thread to read continuously
				while (read){
				switch(readMode){
					case RequestMotor1:
						leftMotor.request_Values();
						start_timer(timerid, timerSP);
						readMode = ReadMotor1;
						break;
					case ReadMotor1:
						if (check_timer(timerid)){
							leftMotor.read_Data();
							// Return values for use by application
							//testData = leftMotor.get_Values();
							leftMotor.print_Data();
							readMode = RequestMotor2;
						}
						break;
					case RequestMotor2:
						rightMotor.request_Values();
						start_timer(timerid, timerSP);
						readMode = ReadMotor2;
						break;
					case ReadMotor2:
						if (check_timer(timerid)){
							rightMotor.read_Data();
							// Return values for use by application
							//testData = rightMotor.get_Values();
							rightMotor.print_Data();
							readMode = RequestMotor1; // make state-machine circular
							read = false; // break read loop
						}
						break;
					default:
						break;
				}
				}
				break;
			case 8:
				leftMotor.send_Alive();
				rightMotor.send_Alive();
				printf("Alive sent\n\n");
				break;
			default:
				loop = false;
				break;
		}
		command = 0;
		val = 0;
		brake = 0;
		read = 1;
		}
	leftMotor.apply_Brake(3);
	rightMotor.apply_Brake(3);
	delete_timer(timerid);
	BLDC::close();
}
