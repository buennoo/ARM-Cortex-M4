#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#define GPIO_PINS_ALL GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7

uint8_t NWD(uint8_t a, uint8_t b) {
    while (b != 0) {
        uint8_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

uint8_t NWW(uint8_t a, uint8_t b) {
    uint8_t temp = NWD(a, b);
    if(temp <= 0){
        return a;
    }
    return (a * b) / temp;
}

int main(void)
{
    uint8_t val1 = 0;
    uint8_t val2 = 0;
    uint8_t diff = 0;

    uint8_t nww = 0;
    uint8_t nwd = 0;

    // ustawienie zegara
    ROM_SysCtlClockSet (SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

    // wlacznie GPIOA (pinow)
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOE);

    //sterujace
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOJ);

    //roznica
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOG);

    //nww i nwd - port b i f
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOF);

    // ustawienie pinow
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PINS_ALL);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PINS_ALL);
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PINS_ALL);

    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PINS_ALL);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PINS_ALL);


    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_7);
    GPIOPinTypeGPIOInput(GPIO_PORTH_BASE, GPIO_PIN_7);

    for (;;)
    {
        if(GPIOPinRead(GPIO_PORTH_BASE,GPIO_PIN_7) == GPIO_PIN_7){
            val1++;
            GPIOPinWrite (GPIO_PORTA_BASE, GPIO_PINS_ALL, val1);
        }


        if(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_7) == GPIO_PIN_7){
            val2++;
            GPIOPinWrite (GPIO_PORTE_BASE, GPIO_PINS_ALL, val2);
        }

        diff = fabs(val1 - val2);
        GPIOPinWrite (GPIO_PORTG_BASE, GPIO_PINS_ALL, diff);

        nwd = NWD(val1, val2);
        GPIOPinWrite (GPIO_PORTB_BASE, GPIO_PINS_ALL, nwd);

        nww = NWW(val1, val2);
        GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PINS_ALL, nww);

        SysCtlDelay(SysCtlClockGet() / 2);
    }
    return 0;
}
