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
        if(y < topBorderY + 12){
            return true;
        }
        return false;
    }

    bool collisionBottom(int y){
        int bottomBorderY = 238;
        if(y > bottomBorderY - 12){
            return true;
        }
        return false;
    }

    bool collisionRight(int x){
        int rightBorderX = 318;
        if(x > rightBorderX - 12){
            return true;
        }
        return false;
    }

    bool collisionLeft(int x){
        int leftBorderX = 2;
        if(x < leftBorderX + 12){
            return true;
        }
        return false;
    }

    int main(void) {
    tContext sContext;
    tRectangle sRect;

    FPULazyStackingEnable();

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    ILI9341_240x320x262K_Init();
    GrContextInit(&sContext, &g_sILI9341_240x320x262K);

    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOK);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinTypeGPIOInput(GPIO_PORTK_BASE, GPIO_PIN_7);

    //tlo
    sRect.i16XMin = 0;
    sRect.i16XMax = 320;
    sRect.i16YMin = 0;
    sRect.i16YMax = 240;
    GrContextForegroundSet(&sContext, 16751052);
    GrRectFill(&sContext, &sRect);

    //pozycja gracza
    int x = 50;
    int y = 50;
    GrContextForegroundSet(&sContext, ClrWhite);
    GrCircleFill(&sContext, x, y, 6);

    bool move = false;

    GrFlush(&sContext);


    while(1){

        //gora
        if(!(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_0)))
        {
            if(!collisionTop(y)){
                y -= 10;
                GrContextForegroundSet(&sContext, ClrWhite);
                GrCircleFill(&sContext, x, y, 6);
                move = true;
            }
        }

        //dol
        if(!(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_5)))
        {
            if(!collisionBottom(y)){
                y += 10;
                GrContextForegroundSet(&sContext, ClrWhite);
                GrCircleFill(&sContext, x, y, 6);
                move = true;
            }
        }

        //prawo
        if(!(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_4))){
            if(!collisionRight(x)){
                x += 10;
                GrContextForegroundSet(&sContext, ClrWhite);
                GrCircleFill(&sContext, x, y, 6);
                move = true;
            }
        }

        //lewo
        if(!(GPIOPinRead(GPIO_PORTK_BASE,GPIO_PIN_7))){
            if(!collisionLeft(x)){
                x -= 10;
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

        SysCtlDelay(ROM_SysCtlClockGet()/(75));
    }
}