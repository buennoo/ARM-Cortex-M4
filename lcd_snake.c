    #include <stdint.h>
    #include <stdlib.h>
    #include <time.h>
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

    void enemy(int xPos, int yPos, tContext sContext){
        GrContextForegroundSet(&sContext, ClrRed);
        GrCircleFill(&sContext, xPos, yPos, 4);
        ROM_SysCtlDelay(ROM_SysCtlClockGet()/(16));
    }

    int randomPosX(){
        srand(time(NULL));

        int minX = 8;
        int maxX = 310;

        int randomX = minX + rand() % (maxX - minX + 1);
        return randomX;
    }

    int randomPosY(){
        srand(time(NULL));

        int minY = 8;
        int maxY = 230;

        int randomY = minY + rand() % (maxY - minY + 1);
        return randomY;
    }

    struct Position {
        int x;
        int y;
    };

    int main(void) {
    tContext sContext;
    tRectangle sRect;
    tRectangle sRect2;

    FPULazyStackingEnable();

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    ILI9341_240x320x262K_Init();
    GrContextInit(&sContext, &g_sILI9341_240x320x262K);

    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOK);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_7);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinTypeGPIOInput(GPIO_PORTK_BASE, GPIO_PIN_7);


    GrContextFontSet(&sContext,g_psFontCm16b);
    GrContextForegroundSet(&sContext, ClrBlue);

    bool move = false;

    //tlo
    sRect.i16XMin = 0;
    sRect.i16XMax = 320;
    sRect.i16YMin = 0;
    sRect.i16YMax = 240;
    GrContextForegroundSet(&sContext, 12895484);
    GrRectFill(&sContext, &sRect);

    //pozycja gracza
    int x = 40;
    int y = 40;
    bool turn = true;
    GrContextForegroundSet(&sContext, ClrWhite);
    GrCircleFill(&sContext, x, y, 6);

    //przeciwnik
    int xPos = 20;
    int yPos = 20;
    bool spawn = true;

    //ogon
    int xTail = x;
    int yTail = y;

    //inne
    bool start = true;
    bool youLose = false;
    int points = 0;

    int moveDir = 0;

    GrFlush(&sContext);

    while(1){
        //gora
        if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_0))
        {
            if(!collisionTop(y) && !youLose){
                turn = false;
                y--;
                GrContextForegroundSet(&sContext, ClrWhite);
                GrCircleFill(&sContext, x, y, 6);

                if(points > 0){
                    GrContextForegroundSet(&sContext, 12895484);
                    GrCircleFill(&sContext, x, y+((points*(points+1))/2)+5, 8);
                }
                move = true;
            }
            else{
                youLose = true;
            }
            moveDir = 1;
            GrFlush(&sContext);
        }
        //dol
        else if(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_5))
        {
            if(!collisionBottom(y) && !youLose){
                turn = false;
                y++;
                GrContextForegroundSet(&sContext, ClrWhite);
                GrCircleFill(&sContext, x, y, 6);

                if(points > 0){
                    GrContextForegroundSet(&sContext, 12895484);
                    GrCircleFill(&sContext, x, y-((points*(points+1))/2)-5, 8);
                }

                move = true;
            }
            else{
                youLose = true;
            }
            moveDir = 2;
            GrFlush(&sContext);
        }
        //prawo
        else if(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_4)){
            if(!collisionRight(x) && !youLose){
                turn = false;
                x++;
                GrContextForegroundSet(&sContext, ClrWhite);
                GrCircleFill(&sContext, x, y, 6);

                if(points > 0){
                    GrContextForegroundSet(&sContext, 12895484);
                    GrCircleFill(&sContext, x-((points*(points+1))/2)-5, y, 8);
                }

                move = true;
            }
            else{
                youLose = true;
            }
            moveDir = 3;
            GrFlush(&sContext);
        }
        //lewo
        else if(GPIOPinRead(GPIO_PORTK_BASE,GPIO_PIN_7)){
            if(!collisionLeft(x) && !youLose){
                turn = false;
                x--;
                GrContextForegroundSet(&sContext, ClrWhite);
                GrCircleFill(&sContext, x, y, 6);

                if(points > 0){
                    GrContextForegroundSet(&sContext, 12895484);
                    GrCircleFill(&sContext, x+((points*(points+1))/2)+5, y, 8);
                }

                move = true;
            }
            else{
                youLose = true;
            }
            moveDir = 4;
            GrFlush(&sContext);
        }

        if(move){
            if(youLose){
                GrContextForegroundSet(&sContext, ClrRed);
                GrRectFill(&sContext, &sRect);
                youLose = true;
            }
            else{
                if(points == 0){
                    GrContextForegroundSet(&sContext, 12895484);
                    GrCircleFill(&sContext, x, y, 8);
                }

                //przeciwnik
                if(!spawn){
                    enemy(xPos, yPos, sContext);
                }
                else{
                    GrContextForegroundSet(&sContext, 12895484);
                    GrCircleFill(&sContext, xPos, yPos, 6);
                }

                //gracz
                GrContextForegroundSet(&sContext, ClrWhite);
                GrCircleFill(&sContext, x, y, 6);
                if(start){
                    GrContextForegroundSet(&sContext, 12895484);
                    GrCircleFill(&sContext, x, y, 8);
                    start = false;
                }
                move = false;
            }
        }

        if(youLose){
            GrContextForegroundSet(&sContext, ClrRed);
            GrRectFill(&sContext, &sRect);
        }

        if(spawn){
            xPos = randomPosX();
            yPos = randomPosY();
            enemy(xPos, yPos, sContext);
            ROM_SysCtlDelay(ROM_SysCtlClockGet()/(16));
            spawn = false;
        }

        if((x == xPos-8 && y > yPos-8 && y < yPos+8) || (x == xPos+8 && y > yPos-8 && y < yPos+8) || (y == yPos-8 && x > xPos-8 && x < xPos+8) || (y == yPos+8 && x > xPos-8 && x < xPos+8)){
            GrContextForegroundSet(&sContext, 12895484);
            GrCircleFill(&sContext, xPos, yPos, 6);
            xTail += 1;
            yTail += 1;
            points++;
            spawn = true;
            ROM_SysCtlDelay(ROM_SysCtlClockGet()/(2));
        }

        ROM_SysCtlDelay(ROM_SysCtlClockGet()/(75));
    }
}

