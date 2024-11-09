#include <avr/io.h>

int main(void) {
    PORTB |= (1 << PB1);
    return 1;
}
