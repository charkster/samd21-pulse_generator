# samd21-pulse_generator
A software defined pattern is given by serial input and then driven on a gpio pin. Fastest pattern has 2us intervals, but any interval may be specified.
The pattern is defined with hex values (example: "A51" is 1010_0101_0001 where each value is driven for the constant interval, starting with the right-most bit).
