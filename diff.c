#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#define GPIO_PINS_ALL GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
int main(void)
{
    uint8_t val1 = 0;
    uint8_t val2 = 0;
    uint8_t diff = 0;

    // ustawienie zegara
    ROM_SysCtlClockSet (SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

    // wlacznie GPIOA (pinow)
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOE);

    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOJ);

    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOG);

    // ustawienie pinow
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PINS_ALL);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PINS_ALL);
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PINS_ALL);

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

        SysCtlDelay(SysCtlClockGet() / 2);
    }
    return 0;
}
