/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.6
        Device            :  PIC16F18875
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "I2C/i2c.h"
#include "LCD/lcd.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/tmr2.h"


/* HEADER STUFF */

#define PMON_DEFAULT 5
#define TALA_DEFAULT 3
#define ALAF_DEFAULT 0
#define ALAH_DEFAULT 12
#define ALAM_DEFAULT 0
#define ALAS_DEFAULT 0
#define ALAT_DEFAULT 25
#define ALAL_DEFAULT 3
#define CLKH_DEFAULT 0
#define CLKM_DEFAULT 0

typedef enum {
    UI_NORMAL,
    UI_SHOW_THRESHOLDS,
    UI_SHOW_RECORDS_TEMP,
    UI_SHOW_RECORDS_LUM,
    UI_CONFIG_CLKH,
    UI_CONFIG_CLKM,
    UI_CONFIG_CLKS,
    UI_CONFIG_C,
    UI_CONFIG_C_ALAH,
    UI_CONFIG_C_ALAM,
    UI_CONFIG_C_ALAS,
    UI_CONFIG_T,
    UI_CONFIG_T_ALAT,
    UI_CONFIG_L,
    UI_CONFIG_L_ALAL,
    UI_CONFIG_A_ALAF,
    UI_RECORDS_RESET
} UI_State;

UI_State ifS1pressed[] = {
    UI_SHOW_THRESHOLDS,
    UI_CONFIG_CLKH, 
    UI_SHOW_RECORDS_TEMP, 
    UI_SHOW_RECORDS_LUM, 
    UI_CONFIG_CLKM,
    UI_CONFIG_CLKS,
    UI_CONFIG_C,
    UI_CONFIG_T,
    UI_CONFIG_C_ALAM,
    UI_CONFIG_C_ALAS,
    UI_CONFIG_T,
    UI_CONFIG_L,
    UI_CONFIG_L,
    UI_CONFIG_A_ALAF,
    UI_CONFIG_A_ALAF,
    UI_RECORDS_RESET,
    UI_NORMAL
};

typedef struct {
    uint8_t hh;
    uint8_t mm;
    uint8_t ss;
    uint8_t temperature;
    uint8_t luminosity;
} ui_record_t;

uint8_t PMON;
uint8_t TALA;
uint8_t ALAF = 0;
uint8_t ALAH;
uint8_t ALAM;
uint8_t ALAS;
uint8_t ALAT;
uint8_t ALAL;
uint8_t CLKH; // when last time changed
uint8_t CLKM; // when last time changed

uint8_t hh = 0;
uint8_t mm = 0;
uint8_t ss = 0;
uint8_t luminosity = 0;
uint8_t temperature = 0;

uint8_t FCLK = 0; // flag for clock alarm
uint8_t FLUM = 0; // flag for luminosity alarm
uint8_t FTEM = 0; // flag for temperature alarm

ui_record_t luminosity_max;
ui_record_t luminosity_min;
ui_record_t temperature_max;
ui_record_t temperature_min;

UI_State state = UI_NORMAL;

uint8_t TMMR1_INT = 0;
uint8_t S1_INT = 0;
uint8_t S2_INT = 0;


/* MEASURE STUFF */

uint8_t getPOTvalue(){
    int signal = ADCC_GetSingleConversion(RA0)*7/1023;
    uint8_t value = (uint8_t) signal;
    return value;
}

unsigned char readTC74 (void) {
	unsigned char value;
    do {
        IdleI2C();
        StartI2C(); IdleI2C();

        WriteI2C(0x9a | 0x00); IdleI2C();
        WriteI2C(0x01); IdleI2C();
        RestartI2C(); IdleI2C();
        WriteI2C(0x9a | 0x01); IdleI2C();
        value = ReadI2C(); IdleI2C();
        NotAckI2C(); IdleI2C();
        StopI2C();
    } while (!(value & 0x40));

	IdleI2C();
	StartI2C(); IdleI2C();
	WriteI2C(0x9a | 0x00); IdleI2C();
	WriteI2C(0x00); IdleI2C();
	RestartI2C(); IdleI2C();
	WriteI2C(0x9a | 0x01); IdleI2C();
	value = ReadI2C(); IdleI2C();
	NotAckI2C(); IdleI2C();
	StopI2C();

	return value;
}

void increment_time(){
    ss++;

    if (ss >= 60) {
        ss = 0;
        mm++;

        if (mm >= 60) {
            mm = 0;
            hh++;

            if (hh >= 24) {
                hh = 0;
            }
        }
    }
    IO_RA7_D5_Toggle();
}

uint8_t increment_variable(uint8_t atual, uint8_t max){
    
    uint8_t valor = atual + 1;
    if (valor > max){
        valor = 0;
    }
    
    return valor;
}

void save_record(ui_record_t* record){
    record->hh = hh;
    record->mm = mm;
    record->ss = ss;
    record->luminosity = luminosity;
    record->temperature = temperature;
}

void measure_values(){
    luminosity = getPOTvalue();
    temperature = (uint8_t) readTC74();
}

void check_values(){
    if (temperature >= temperature_max.temperature){
        save_record(&temperature_max);
    }
    
    if(temperature <= temperature_min.temperature){
        save_record(&temperature_min);
    }
    
    if (luminosity >= luminosity_max.luminosity){
        save_record(&luminosity_max);
    }
    
    if(luminosity <= luminosity_min.luminosity){
        save_record(&luminosity_min);
    }
    
    // se os alarmes nao estao ligados
    if(!ALAF) return;
    
    if(temperature > ALAT){
        FTEM = 1;
        IO_RA6_D4_SetHigh();
    }
    
    if(luminosity < ALAL){
        FLUM = 1;
        IO_RA5_D3_SetHigh();
    }
        
}

/* TIMER AND INTERRUPT STUFF */

void TMR1_interrupt(){
    TMMR1_INT = 1;
}

void S1_interrupt(){
    S1_INT = 1;
}

void S2_interrupt(){
    S2_INT = 1;
}

/* UI STUFF */


void ui_write(uint8_t config, uint8_t row, uint8_t column){
    char line1[17] = {0};
    char line2[17] = {0};
    char cc = 'C';
    char tt = 'T';
    char ll = 'L'; 
    char aa = ALAF ? 'A' : 'a';
    char rr = 'R';
        
    if(!config){
        cc = FCLK ? 'C' : ' ';
        tt = FLUM ? 'T' : ' ';
        ll = FTEM ? 'L' : ' ';
        rr = ' ';
    }
    
    sprintf(line1,"%02u:%02u:%02u  %c%c%c %c%c", hh, mm, ss, cc, tt, ll, aa, rr);
    sprintf(line2,"%02u C         L %1u", temperature, luminosity);

    LCDcmd(0x80);       //first line, first column
    while (LCDbusy());
    LCDstr(line1);

    LCDcmd(0xc0);       //second line, first column
    while (LCDbusy());
    LCDstr(line2);
    
    LCDpos(row, column);
    LCDcmd(0x0f);
}

void ui_write_thresholds(){
    char line1[17] = {0};
    char line2[17] = {0};

    char aa = ALAF ? 'A' : 'a';

    sprintf(line1,"%02u:%02u:%02u      %c*", ALAH, ALAM, ALAS, aa);
    sprintf(line2,"%02u C         L %1u", ALAT, ALAL);

    LCDcmd(0x80);       //first line, first column
    while (LCDbusy());
    LCDstr(line1);

    LCDcmd(0xc0);       //second line, first column
    while (LCDbusy());
    LCDstr(line2);
}

// if type is 0 then it's temperature, if 1 is luminosity
void ui_write_records(uint8_t type){
    char line1[17] = {0};
    char line2[17] = {0};

    uint8_t hours1, hours2, mins1, mins2, secs1, secs2, temp1, temp2, lumi1, lumi2;
    
    // luminosity
    if (type){
        hours1 = luminosity_max.hh;
        mins1 = luminosity_max.mm;
        secs1 = luminosity_max.ss;
        temp1 = luminosity_max.temperature;
        lumi1 = luminosity_max.luminosity;

        hours2 = luminosity_min.hh;
        mins2 = luminosity_min.mm;
        secs2 = luminosity_min.ss;
        temp2 = luminosity_min.temperature;
        lumi2 = luminosity_min.luminosity;
    } else { // temperature
        hours1 = temperature_max.hh;
        mins1 = temperature_max.mm;
        secs1 = temperature_max.ss;
        temp1 = temperature_max.temperature;
        lumi1 = temperature_max.luminosity;

        hours2 = temperature_min.hh;
        mins2 = temperature_min.mm;
        secs2 = temperature_min.ss;
        temp2 = temperature_min.temperature;
        lumi2 = temperature_min.luminosity;
    }

    sprintf(line1,"%02u:%02u:%02u  %02uC L %1u", hours1, mins1, secs1, temp1, lumi1);
    sprintf(line2,"%02u:%02u:%02u  %02uC L %1u", hours2, mins2, secs2, temp2, lumi2);
    
    LCDcmd(0x80);       //first line, first column
    while (LCDbusy());
    LCDstr(line1);

    LCDcmd(0xc0);       //second line, first column
    while (LCDbusy());
    LCDstr(line2);
}

/**
 * ESTA BUE INCOMPLETA
 */
void ui_function(){

    switch (state) {
        case UI_NORMAL:
            ui_write(0,0,0);
            LCDcmd(0x0c); // no cursor 
            break;

        case UI_SHOW_THRESHOLDS:
            ui_write_thresholds();
            LCDcmd(0x0c); // no cursor 
            // TODO: COUNT TIME PASSED, SWITCH
            
            // TODO: "CLEAR ALARM INFORMATION"
            break;

        case UI_SHOW_RECORDS_TEMP:
            ui_write_records(0);
            LCDcmd(0x0c); // no cursor 
            // TODO: COUNT TIME PASSED, SWITCH TO UI_SHOW_RECORDS_LUM
            break;

        case UI_SHOW_RECORDS_LUM:
            ui_write_records(1);
            LCDcmd(0x0c); // no cursor 
            // TODO: COUNT TIME PASSED, SWITCH TO UI_NORMAL
            break;

        case UI_CONFIG_CLKH:
            ui_write(1,0,1);
            break;
            
            
        case UI_CONFIG_CLKM:
            ui_write(1,0,4);
            break;
            
        case UI_CONFIG_CLKS:
            ui_write(1,0,7);
            break;
            
        case UI_CONFIG_C:
            ui_write(1,0,10);
            break;
            
        case UI_CONFIG_C_ALAH:
        case UI_CONFIG_C_ALAM:
        case UI_CONFIG_C_ALAS:
        case UI_CONFIG_T:
            ui_write(1,0,11);
            break;
            
        case UI_CONFIG_T_ALAT:
        case UI_CONFIG_L:
            ui_write(1,0,12);
            break;
            
        case UI_CONFIG_L_ALAL:
        case UI_CONFIG_A_ALAF:
            ui_write(1,0,14);
            break;
            
        case UI_RECORDS_RESET:
            ui_write(1,0,15);
            break;
            
        default:
            ui_write(0,0,0);
            LCDcmd(0x0c); // no cursor 
            break;
    }
}


/* MAIN */

void main(void) {    
    // Initialize the device
    SYSTEM_Initialize();

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    // Initialize I2C
    OpenI2C();
    
    // Initialize LCD
    LCDinit();

    
    // Setting Timer 1 interrupt handler
    TMR1_SetInterruptHandler(TMR1_interrupt);
    // Stop Timer 1
    TMR1_StopTimer();
    // Reload Timer value
    TMR1_Reload();
    // Reset time variable
    // Time_reset(&current_time);
    // Start Timer 1
    TMR1_StartTimer();
    
    // Setting button press interrupt handler
    IOCBF4_SetInterruptHandler(S1_interrupt);
    IOCCF5_SetInterruptHandler(S2_interrupt);
    
    save_record(&luminosity_max);
    save_record(&luminosity_min);
    save_record(&temperature_min);
    save_record(&temperature_min);

    while (1) {
        
        SLEEP(); // wait for interrupt
        NOP(); // ensure instruction pre-fetched while sleeping is safe to execute on awakening
        
        if(TMMR1_INT){
            increment_time();
            // TODO: contar ate 5 
            measure_values();
            check_values();
            ui_function();
            TMMR1_INT = 0;
            
        }
        
        if(S1_INT){
            state = ifS1pressed[state];
            S1_INT = 0;
            ui_function();
        }
        
        if(S2_INT){
            // TODO: tudo
            state = UI_SHOW_RECORDS_LUM;
        }
    }
}
