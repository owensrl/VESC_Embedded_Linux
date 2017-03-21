// Main program to test UART communication from BBB to VESC
// Last modified on 3/7/2017 by: Ryan Owens
#include <stdio.h>
#include <string.h>
#include "bldc_interface.h"
#include "comm_uart.h"
#include <unistd.h> // for usleep

void bldc_val_received(mc_values *val) {
	printf("\r\n");
	printf("Input voltage: %.2f V\r\n", val->v_in);
	printf("Temp:          %.2f degC\r\n", val->temp_pcb);
	printf("Temp MOSFET1:  %.2f degC\r\n", val->temp_mos1);
	printf("Temp MOSFET2:  %.2f degC\r\n", val->temp_mos2);
	printf("Temp MOSFET3:  %.2f degC\r\n", val->temp_mos3);
	printf("Temp MOSFET4:  %.2f degC\r\n", val->temp_mos4);
	printf("Temp MOSFET5:  %.2f degC\r\n", val->temp_mos5);
	printf("Temp MOSFET6:  %.2f degC\r\n", val->temp_mos6);
	printf("Current motor: %.2f A\r\n", val->current_motor);
	printf("Current in:    %.2f A\r\n", val->current_in);
	printf("RPM:           %.1f RPM\r\n", val->rpm);
	printf("Duty cycle:    %.1f %%\r\n", val->duty_now * 100.0);
	printf("Ah Drawn:      %.4f Ah\r\n", val->amp_hours);
	printf("Ah Regen:      %.4f Ah\r\n", val->amp_hours_charged);
	printf("Wh Drawn:      %.4f Wh\r\n", val->watt_hours);
	printf("Wh Regen:      %.4f Wh\r\n", val->watt_hours_charged);
	printf("Tacho:         %i counts\r\n", val->tachometer);
	printf("Tacho ABS:     %i counts\r\n", val->tachometer_abs);
	printf("Fault Code:    %s\r\n", bldc_interface_fault_to_string(val->fault_code));
	printf("\r\n");
}

int main(void) {

	// variables
	int command = 0;
	int id = -1; // disable CAN forwarding
	bool decrement = false;
	bool loop = true;
	
	float ratio = 0;
	float brake = 0;
	float pos = 0;
	
	// For the serial interface
	comm_uart_init("/dev/ttyO1");
	// Give bldc interface a callback function to handle received values
	bldc_interface_set_rx_value_func(bldc_val_received);
	
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
				scanf("%f", &ratio);
				bldc_interface_set_forward_can(id);
				bldc_interface_set_rpm(ratio);
				printf("Speed set to %f RPM\n\n", ratio);
				break;
			case 2:
				printf("Enter desired current in Amps: ");
				scanf("%f", &ratio);
				bldc_interface_set_forward_can(id);
				bldc_interface_set_current(ratio);
				printf("Current set to %f Amps\n\n", ratio);
				break;
			case 3:
				printf("Enter desired brake current in Amps: ");
				scanf("%f", &brake);
				bldc_interface_set_forward_can(id);
				bldc_interface_set_current_brake(brake);
				printf("Brake current set to %f Amps\n\n", brake);
				break;
			case 4:
				printf("Enter desired duty cycle -1.0 to 1.0: ");
				scanf("%f", &ratio);
				bldc_interface_set_forward_can(id);
				bldc_interface_set_duty_cycle(ratio);
				printf("Duty cycle set to %f\n\n", ratio);
				break;
			case 5:
				printf("Enter desired position 0-360 degrees: ");
				scanf("%f", &pos);
				bldc_interface_set_forward_can(id);
				bldc_interface_set_pos(pos);
				printf("Position set to %f\n\n", pos);
				break;
			case 6:
				printf("Sweeping position from 0-360 degrees\n\n");
				while (true) {
					bldc_interface_set_forward_can(id);
					bldc_interface_set_pos(pos);
					if (pos == 360)
						decrement = true;
					else if (pos == 0)
						decrement = false;

					if (decrement == true)
						pos -= 1;
					else
						pos += 1;
					usleep(3000);
				}
				break;
			case 7:
				bldc_interface_set_forward_can(id);
				bldc_interface_get_values();
				usleep(10000);
				receive_packet();
				break;
			case 8:
				bldc_interface_set_forward_can(id);
				bldc_interface_send_alive();
				printf("Alive sent\n\n");
				break;
			default:
				loop = false;
				break;
		}
		command = 0;
		ratio = 0;
		brake = 0;
		}
	bldc_interface_set_forward_can(id);
	bldc_interface_set_current_brake(3);
	comm_uart_close();
}
