
#include "mbed.h"

InterruptIn mybutton(USER_BUTTON);
DigitalOut myled(LED1);
//Ticker ti;

float delay = 1.0; // 1 sec

/*void tick()
{
    if (delay == 1.0)
        delay = 0.2; // 200 ms
    else
        delay = 1.0; // 1 sec
}
*/
void pressed()
{
    if (delay == 1.0)
        delay = 0.2; // 200 ms
    else
        delay = 1.0; // 1 sec
}

int main()
{
//	ti.attach(tick, 2);

    mybutton.fall(&pressed);
    while (1) {
        myled = !myled;
        wait(delay);
    }
}
