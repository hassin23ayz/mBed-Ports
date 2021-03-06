#include "SoftSerial.h"

int SoftSerial::_getc( void ) {
    while(!readable());
    out_valid = false;
    return out_buffer;
}

int SoftSerial::readable(void) {
    return out_valid;
}

//Start receiving byte
void SoftSerial::rx_gpio_irq_handler(void) {
    rxticker.prime();
    rxticker.setNext(bit_period + (bit_period >> 1));
    rx->fall(NULL);
    rx_bit = 0;
    rx_error = false;
};    

void SoftSerial::rx_handler(void) {
    rxticker.setNext(bit_period);
    rx_bit++;
    
    //Receive data
    int val = rx->read();
    if (rx_bit <= _bits) {
        read_buffer |= val << (rx_bit - 1);
        return;
    }
    
    //Receive parity
    bool parity_count;
    if (rx_bit == _bits + 1) {
        switch (_parity) {
            case Forced1:
                if (val == 0)
                    rx_error = true;
                return;
            case Forced0:
                if (val == 1)
                    rx_error = true;
                return;
            case Even:
            case Odd:
                parity_count = val;
                for (int i = 0; i<_bits; i++) {
                    if (((read_buffer >> i) & 0x01) == 1)
                        parity_count = !parity_count;
                }
                if ((parity_count) && (_parity == Even))
                    rx_error = true;
                if ((!parity_count) && (_parity == Odd))
                    rx_error = true;
                return;
        }
    }
    
    //Receive stop
    if (rx_bit < _bits + (bool)_parity + _stop_bits) {
        if (!val)
            rx_error = true;
        return;
    }    
    
    //The last stop bit
    if (!val)
        rx_error = true;
    
    if (!rx_error) {
        out_valid = true;
        out_buffer = read_buffer;
        fpointer[RxIrq].call();
    }
    read_buffer = 0;
    rxticker.detach(); 
    rx->fall(this, &SoftSerial::rx_gpio_irq_handler);
}

