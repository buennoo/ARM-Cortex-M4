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
    unsigned int i;

    // Color white
    uint32_t *color = 16777215;
    char *text = "Kwadrat";
    char *font = "g_psFontCm20b";

    sRect.i16XMin = GrContextDpyWidthGet(&sContext) / 4;
    sRect.i16XMax = (GrContextDpyWidthGet(&sContext) / 4) + 160;
    sRect.i16YMin = GrContextDpyHeightGet(&sContext) / 4;
    sRect.i16YMax = (GrContextDpyHeightGet(&sContext) / 4) + 120;

    GrContextForegroundSet(&sContext, color);
    GrRectFill(&sContext, &sRect);

    for(i = 0; i < 160; i++){
        GrContextForegroundSet(&sContext, 16751103 - i);
        GrLineDraw(&sContext, (GrContextDpyWidthGet(&sContext) / 4) + i, (GrContextDpyHeightGet(&sContext) / 4), (GrContextDpyWidthGet(&sContext) / 4) + i, (GrContextDpyHeightGet(&sContext) / 4) + 120);
    }

    unsigned int changeColor = 26112;
    for(i = 0; i < 4; i++){
        GrContextForegroundSet(&sContext, 222 + changeColor);
        GrContextFontSet(&sContext, font);
        GrStringDrawCentered(&sContext, text, -1, (GrContextDpyWidthGet(&sContext) / 2) - i, (GrContextDpyHeightGet(&sContext) / 2), 0);
        changeColor += 100000;
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
