#include "mbed.h"

DigitalOut myled(PC_8);
DigitalOut led2(PC_9);

Ticker tick;
Serial device(PA_9, PA_10);  // tx, rx
//RawSerial device(PA_9, PA_10);  // tx, rx
InterruptIn button(PA_0);
SPI spi(PA_7,PA_6,PA_5);
//I2C i2c(PB_9,PB_8);
I2CSlave i2cSlave(PB_9,PB_8);

void flip()
{
    //led2 = 0; // Turn green LED off
	myled = !myled;
}

void click()
{
    //button press event
	device.printf("test message....\r\n");
}

int main() {

		//flash_test();
		tick.attach(&flip, 2); // setup ticker to call flip led2 after 0.7 seconds
		button.rise(&click);
		device.baud(115200);
		i2cSlave.frequency(100000);

	 while (1) {

		  wait(0.5);
		 led2 = !led2;
	 }
}
