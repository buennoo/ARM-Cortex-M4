    #include <stdint.h>
    #include <stdbool.h>
    #include <math.h>
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
    uint32_t *color = 10092441;
    char *text = "Choinka";
    char *font = "g_psFontCm20b";

    for(i = 0; i < 100; i++){
        GrContextForegroundSet(&sContext, color);
        GrLineDraw(&sContext, (GrContextDpyWidthGet(&sContext) / 2), (GrContextDpyHeightGet(&sContext) / 2) - 100, (GrContextDpyWidthGet(&sContext) / 2) - 50 + i, (GrContextDpyHeightGet(&sContext) / 2) - 40);
    }

    for(i = 0; i < 140; i++){
        GrContextForegroundSet(&sContext, color);
        GrLineDraw(&sContext, (GrContextDpyWidthGet(&sContext) / 2), (GrContextDpyHeightGet(&sContext) / 2) - 40, (GrContextDpyWidthGet(&sContext) / 2) - 70 + i, (GrContextDpyHeightGet(&sContext) / 2) + 20);
    }

    for(i = 0; i < 180; i++){
        GrContextForegroundSet(&sContext, color);
        GrLineDraw(&sContext, (GrContextDpyWidthGet(&sContext) / 2), (GrContextDpyHeightGet(&sContext) / 2) + 20, (GrContextDpyWidthGet(&sContext) / 2) - 90 + i, (GrContextDpyHeightGet(&sContext) / 2) + 80);
    }

    GrFlush(&sContext);

    uint32_t *colors[] = {16751103, 10079487, 10066431, 16751001, 16777164};
    unsigned int j = 0, k = 1, l = 2;
    while(1)
    {
    if(j < 0 || j > 4){
        j = 0;
    }
    if(k < 0 || k > 4){
        k = 0;
    }
    if(l < 0 || l > 4){
        l = 0;
    }
    GrContextForegroundSet(&sContext, colors[j]);
    GrCircleFill(&sContext, (GrContextDpyWidthGet(&sContext) / 2) - 23, (GrContextDpyWidthGet(&sContext) / 2)+5, 6);
    GrCircleFill(&sContext, (GrContextDpyWidthGet(&sContext) / 2) + 24, (GrContextDpyWidthGet(&sContext) / 2)-95, 6);
    GrCircleFill(&sContext, (GrContextDpyWidthGet(&sContext) / 2) - 22, (GrContextDpyWidthGet(&sContext) / 2)-100, 6);

    GrContextForegroundSet(&sContext, colors[k]);
    GrCircleFill(&sContext, (GrContextDpyWidthGet(&sContext) / 2) + 21, (GrContextDpyWidthGet(&sContext) / 2)-40, 6);
    GrCircleFill(&sContext, (GrContextDpyWidthGet(&sContext) / 2) - 21, (GrContextDpyWidthGet(&sContext) / 2)-42, 6);
    GrCircleFill(&sContext, (GrContextDpyWidthGet(&sContext) / 2) - 40, (GrContextDpyWidthGet(&sContext) / 2) + 25, 6);

    GrContextForegroundSet(&sContext, colors[l]);
    GrCircleFill(&sContext, (GrContextDpyWidthGet(&sContext) / 2) - 5, (GrContextDpyWidthGet(&sContext) / 2)-120, 6);
    GrCircleFill(&sContext, (GrContextDpyWidthGet(&sContext) / 2) + 10, (GrContextDpyWidthGet(&sContext) / 2)+25, 6);

    j++;
    k++;
    l++;

    ROM_SysCtlDelay(ROM_SysCtlClockGet()/2);
    }
}
