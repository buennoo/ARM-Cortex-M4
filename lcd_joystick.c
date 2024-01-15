    //*****************************************************************************
    // hello.c - Simple hello world example.
    //
    // Maciej Kucia July 2013
    //
    // This is part of revision 1.0 of the EK-LM4F232 Firmware Package.
    //*****************************************************************************

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


    //*****************************************************************************
    //
    // TI logo in array form (1 bit per pixel)
    //
    //*****************************************************************************
    const unsigned char TI_logo[(16*2)+5] = {
    IMAGE_FMT_1BPP_UNCOMP, 16,0, 16,0,
    0x0f,0xff,0x0f,0xff,0x0f,0xf3,0x0f,0xfa,0x0f,0x8f,0x0f,0x89,0x81,0x99,0x81,0x19,0x03,0x09,0x07,0x89,0x07,0xdf,0x3f,0xee,0x7f,0xf0,0xff,0xf8,0xff,0xf9,0xff,0xfb};
    //  1,1,1,1,0,0,0,0, 1,1,1,1,1,1,1,1,
    //  1,1,1,1,0,0,0,0, 1,1,1,1,1,1,1,1,
    //  1,1,1,1,0,0,0,0, 1,1,0,0,1,1,1,1,
    //  1,1,1,1,0,0,0,0, 0,1,0,1,1,1,1,1,
    //  1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,1,
    //  1,1,1,1,0,0,0,0, 1,0,0,1,0,0,0,1,
    //  1,0,0,0,0,0,0,1, 1,0,0,1,1,0,0,1,
    //  1,0,0,0,0,0,0,1, 1,0,0,1,1,0,0,0,
    //  1,1,0,0,0,0,0,0, 1,0,0,1,0,0,0,0,
    //  1,1,1,0,0,0,0,0, 1,0,0,1,0,0,0,1,
    //  1,1,1,0,0,0,0,0, 1,1,1,1,1,0,1,1,
    //  1,1,1,1,1,1,0,0, 0,1,1,1,0,1,1,1,
    //  1,1,1,1,1,1,1,0, 0,0,0,0,1,1,1,1,
    //  1,1,1,1,1,1,1,1, 0,0,0,1,1,1,1,1,
    //  1,1,1,1,1,1,1,1, 1,0,0,1,1,1,1,1,
    //  1,1,1,1,1,1,1,1, 1,1,0,1,1,1,1,1
    //*****************************************************************************
    //
    // Print some text to the display.
    //
    //***********************************************

    bool collisionTop(int y){
        int topBorderY = 2;
        if(y == topBorderY + 6){
            return true;
        }
        return false;
    }

    bool collisionBottom(int y){
        int bottomBorderY = 238;
        if(y == bottomBorderY - 6){
            return true;
        }
        return false;
    }

    bool collisionRight(int x){
        int rightBorderX = 318;
        if(x == rightBorderX - 6){
            return true;
        }
        return false;
    }

    bool collisionLeft(int x){
        int leftBorderX = 2;
        if(x == leftBorderX + 6){
            return true;
        }
        return false;
    }

    int main(void) {
    tContext sContext;
    tRectangle sRect;
    //
    // Enable lazy stacking for interrupt handlers. This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    ROM_FPULazyStackingEnable();
    //
    // Set the clocking to run directly from the crystal.
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    //
    // Initialize the display driver.
    //
    ILI9341_240x320x262K_Init();
    //
    // Initialize the graphics context.
    //
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

    //granice
    int leftBorderX = 2;
    int rightBorderX = 318;
    int topBorderY = 2;
    int bottomBorderY = 238;

    //rysowanie granic
    GrContextForegroundSet(&sContext, 11731033);
    GrLineDraw(&sContext, 0, 0, 2, 280);

    GrContextForegroundSet(&sContext, 11731033);
    GrLineDraw(&sContext, 318, 0, 319, 240);

    GrContextForegroundSet(&sContext, 11731033);
    GrLineDraw(&sContext, 0, 1, 320, 2);

    GrContextForegroundSet(&sContext, 11731033);
    GrLineDraw(&sContext, 0, 238, 320, 240);

    //tlo
    sRect.i16XMin = leftBorderX;
    sRect.i16XMax = rightBorderX;
    sRect.i16YMin = topBorderY;
    sRect.i16YMax = bottomBorderY;
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

