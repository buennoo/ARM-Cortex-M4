    #include <stdint.h>
    #include <stdbool.h>
    #include "inc/hw_memmap.h"
    #include "driverlib/fpu.h"
    #include "driverlib/sysctl.h"
    #include "driverlib/rom.h"
    #include "driverlib/pin_map.h"
    #include "driverlib/uart.h"
    #include "grlib/grlib.h"
    #include "drivers/ili9341_240x320x262K.h"
    #include "utils/uartstdio.h"
    #include "driverlib/gpio.h"
    #define GPIO_PINS_ALL GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7

    bool collisionTop(int y){
        int topBorderY = 2;
        if(y < topBorderY + 6){
            return true;
        }
        return false;
    }

    bool collisionBottom(int y){
        int bottomBorderY = 238;
        if(y > bottomBorderY - 6){
            return true;
        }
        return false;
    }

    bool collisionRight(int x){
        int rightBorderX = 318;
        if(x > rightBorderX - 6){
            return true;
        }
        return false;
    }

    bool collisionLeft(int x){
        int leftBorderX = 2;
        if(x < leftBorderX + 6){
            return true;
        }
        return false;
    }

    int main(void) {
    tContext sContext;
    tRectangle sRect;

    ROM_FPULazyStackingEnable();

    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    ILI9341_240x320x262K_Init();
    GrContextInit(&sContext, &g_sILI9341_240x320x262K);


    SysCtlPeripheralReset(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralReset(SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralReset(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralReset(SYSCTL_PERIPH_GPIOD);

    SysCtlGPIOAHBDisable(SYSCTL_PERIPH_GPIOH);
    SysCtlGPIOAHBDisable(SYSCTL_PERIPH_GPIOB);
    SysCtlGPIOAHBDisable(SYSCTL_PERIPH_GPIOE);

    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOK);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_7);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinTypeGPIOInput(GPIO_PORTH_BASE, GPIO_PIN_2);


    GrContextFontSet(&sContext,g_psFontCm16b);
    GrContextForegroundSet(&sContext, ClrBlue);

    bool move = false;

    //kolko
    GrContextForegroundSet(&sContext, ClrRed);
    GrCircleFill(&sContext, (GrContextDpyWidthGet(&sContext) / 2) - 23, (GrContextDpyWidthGet(&sContext) / 2)+5, 6);
    
    //tlo
    sRect.i16XMin = 0;
    sRect.i16XMax = 320;
    sRect.i16YMin = 0;
    sRect.i16YMax = 240;
    GrContextForegroundSet(&sContext, 16751052);
    GrRectFill(&sContext, &sRect);

    //pozycja gracza
    int x = 20;
    int y = 20;
    GrContextForegroundSet(&sContext, ClrWhite);
    GrCircleFill(&sContext, x, y, 6);

    GrFlush(&sContext);

    while(1){
        //gora
        if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_0))
        {
            if(!collisionTop(y)){
                y--;
                GrContextForegroundSet(&sContext, ClrWhite);
                GrCircleFill(&sContext, x, y, 6);
                move = true;
            }
        }

        //dol
        if(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_5))
        {
            if(!collisionBottom(y)){
                y++;
                GrContextForegroundSet(&sContext, ClrWhite);
                GrCircleFill(&sContext, x, y, 6);
                move = true;
            }
        }

        //prawo
        if(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_4)){
            if(!collisionRight(x)){
                x++;
                GrContextForegroundSet(&sContext, ClrWhite);
                GrCircleFill(&sContext, x, y, 6);
                move = true;
            }
        }

        //lewo
        if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_7)){
            if(!collisionLeft(x)){
                x--;
                GrContextForegroundSet(&sContext, ClrWhite);
                GrCircleFill(&sContext, x, y, 6);
                move = true;
            }
        }

        if(move){
            GrContextForegroundSet(&sContext, 16751052);
            GrRectFill(&sContext, &sRect);

            GrContextForegroundSet(&sContext, ClrWhite);
            GrCircleFill(&sContext, x, y, 6);
            move = false;
        }

        ROM_SysCtlDelay(ROM_SysCtlClockGet()/(75));
    }
}

