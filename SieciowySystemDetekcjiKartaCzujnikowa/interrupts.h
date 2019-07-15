#ifndef INTERRUPTS_H

    void WylaczPrzerwania(void);

    void InterruptHandlerLow(void);//void interrupt_at_low_vector(void);
    void InterruptHandlerHigh(void);//void interrupt_at_high_vector(void);

#endif