#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#define GPIO_PINS_ALL GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7

void playSound(uint32_t frequency, uint32_t duration, float vol)
{
    //konfiguracja zegara dla PWM
    uint32_t PWMclock = ROM_SysCtlClockGet() / 4 ;
    uint32_t period = (PWMclock/frequency) - 1;
    float numOfCyc = ROM_SysCtlClockGet()/1000.0; // obliczanie liczby cykli na ms
    uint32_t counter = duration * numOfCyc / 3; // obliczanie licznika dla funkcji SysCtlDelay
    ROM_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, period); // ustawienie okresu sygnalu PWM
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, vol * period); // ustawienie wspolczynnika wypelnienia

    // wlaczenie wyjsc generatora
    ROM_PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT, true);
    // wlaczenie generatora
    ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_1);
    ROM_SysCtlDelay(counter); // dodanie opoznienia
    // wylaczenie generatora
    ROM_PWMGenDisable(PWM1_BASE, PWM_GEN_1);
}

void noteC(int freq, int duration,float vol){playSound(523+freq, duration, vol);}
void noteD(int freq, int duration,float vol){playSound(587+freq, duration, vol);}
void noteE(int freq, int duration,float vol){playSound(659+freq, duration, vol);}
void noteF(int freq, int duration,float vol){playSound(698+freq, duration, vol);}
void noteG(int freq, int duration,float vol){playSound(784+freq, duration, vol);}
void noteA(int freq, int duration,float vol){playSound(880+freq, duration, vol);}
void noteB(int freq, int duration,float vol){playSound(988+freq, duration, vol);}

void Melody()
{
    //noteC(500,300,0.5);noteD(300);noteE(300);
    //noteF(500,300,0.5);noteG(300);noteA(300);
    //noteB(500,300,0.5);
}

int main(void)
{
    float vol = 0.9;
    uint32_t freq = 50;

    ROM_SysCtlClockSet (SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

    // ustawianie pwm
    ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_4);
    ROM_SysCtlDelay(ROM_SysCtlClockGet() / 2);

    // wlaczenie portu a i pwm1
    ROM_SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

    // ustawienie pierwszego generatora pwm i tryb zliczania w dol
    ROM_PWMGenConfigure(PWM1_BASE, PWM_GEN_1,PWM_GEN_MODE_DOWN);

    // konfiguracja linii - brzeczyk jest na pa6 (laczymy linie PA6 z wyjsciem nr 2 pierwszego generatora pwm)
    ROM_GPIOPinConfigure(GPIO_PA6_M1PWM2);
    // ustawienie typu linii pa6 na pwm
    ROM_GPIOPinTypePWM(GPIO_PORTA_BASE,GPIO_PIN_6);


    //sterujace H i J do dzwiekow
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOJ);
    // sterujace E do glosnosci
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOE);


    // zezwolenie na input z pinow portu H, J i E
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PINS_ALL);
    GPIOPinTypeGPIOInput(GPIO_PORTH_BASE, GPIO_PINS_ALL);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PINS_ALL);

    for (;;)
    {
        // PORT E
        if(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_7) == GPIO_PIN_7){
            vol += 0.01;
            if(vol >= 1){
                vol = 0.99;
            }
        }
        if(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_6) == GPIO_PIN_6){
            vol -= 0.01;
            if(vol >= 1){
                vol = 0.99;
            }
        }

        if(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_4) == GPIO_PIN_4){
            freq = 0;
        }

        if(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_3) == GPIO_PIN_3){
            freq = 25;
        }

        if(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_2) == GPIO_PIN_2){
            freq = 75;
        }

        if(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_2) == GPIO_PIN_2){
            freq = 50;
        }

        // PORT J
        if(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_7) == GPIO_PIN_7){
            noteC(freq, 300, vol);
        }
        if(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_6) == GPIO_PIN_6){
            noteD(freq, 300, vol);
        }
        if(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_5) == GPIO_PIN_5){
            noteE(freq, 300, vol);
        }

        // PORT H
        if(GPIOPinRead(GPIO_PORTH_BASE,GPIO_PIN_7) == GPIO_PIN_7){
            noteF(freq, 300, vol);
        }
        if(GPIOPinRead(GPIO_PORTH_BASE,GPIO_PIN_6) == GPIO_PIN_6){
            noteG(freq, 300, vol);
        }
        if(GPIOPinRead(GPIO_PORTH_BASE,GPIO_PIN_5) == GPIO_PIN_5){
            noteA(freq, 300, vol);
        }

        ROM_SysCtlDelay(ROM_SysCtlClockGet() / 2);
    }
    return 0;
}
