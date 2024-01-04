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

    int
    main(void)
    {
    tContext sContext;
    tRectangle sRect;
    // Enable lazy stacking for interrupt handlers. This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    ROM_FPULazyStackingEnable();

    // Set the clocking to run directly from the crystal.

    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
SYSCTL_OSC_MAIN);


    // Initialize the display driver.
    ILI9341_240x320x262K_Init();

    // Initialize the graphics context.
    GrContextInit(&sContext, &g_sILI9341_240x320x262K);

    // Flush any cached drawing operations
    GrFlush(&sContext);

    // Proper code
    unsigned int addX = 0;
    unsigned int addY = 0;
    unsigned int i;

    // Colors: red, green, blue, yellow
    uint32_t *color[] = { 16711680, 3394611, 255, 16776960};
    char *text[] = { "Pole A", "Pole B", "Pole C", "Pole D"};
    char *font[] = { "g_psFontCm20b", "g_sFontCmss12", "g_sFontCm14i", "g_sFontCmtt14"};
    unsigned int j = 3;

    for(i = 0; i < 4; i++){
        sRect.i16XMin = 40 + addX;
        sRect.i16XMax = 140 + addX;
        sRect.i16YMin = 20 + addY;
        sRect.i16YMax = 100 + addY;

        GrContextForegroundSet(&sContext, color[i]);
        GrRectFill(&sContext, &sRect);

        GrContextForegroundSet(&sContext, color[j]);
        GrContextFontSet(&sContext, font[i]);
        GrStringDrawCentered(&sContext, text[i], -1,
        (10 + GrContextDpyWidthGet(&sContext) / 4) + addX,
        (GrContextDpyHeightGet(&sContext) / 4) + addY,
        0);

        addX += 140;
        j--;
        if(i >= 1){
            addY = 120;
            if(i == 1){
                addX = 0;
            }
        }
    }
    GrFlush(&sContext);

    while(1)
    {
    // Draw RGB rectangles
    /*
    sRect.i16XMin = 40;
    sRect.i16XMax = 140;
    sRect.i16YMin = 20;
    sRect.i16YMax = 100;

    GrContextForegroundSet(&sContext, ClrRed);
    GrRectFill(&sContext, &sRect);
    GrFlush(&sContext);
    */
    ROM_SysCtlDelay(ROM_SysCtlClockGet()/2);

    }
}
