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
#include "EEPROM/config_storage.h"
#include "Time/Time.h"
#include "stdio.h"
#include "UI/ui.h"

time_t current_time;

 
uint8_t getPOTvalue(){
    int signal = ADCC_GetSingleConversion(RA0)*7/1023;
    uint8_t value = (uint8_t) signal;
    return value;
}


void Timer1_counter(void) {
    // Simple debouncing
    __delay_ms(5);
    
    // Increment time variable
    Time_increment(&current_time);
    IO_RA7_D5_Toggle();
    
    // Clear interrupt flag
    PIR4bits.TMR1IF = 0;
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

void getData(ui_display_data_t *data){
    uint8_t c = (uint8_t) readTC74();
    int lum = ADCC_GetSingleConversion(RA0)*7/1023;
    
    data->temperature = c;
    data->luminosity = (uint8_t) lum;
    data->current_time.second = current_time.second;
    data->current_time.hour = current_time.hour;
    data->current_time.minute = current_time.minute;
}

void main(void) {    
    //char buf[17];
    ui_display_data_t data;

    // initialize the device
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
    
    // Initialize memory
    memory_flag_t m_flag = memory_init();
    if (m_flag == MEM_CORRUPTED) {
        memory_reset();
    }
    
    // Setting Timer 1 interrupt handler
    TMR1_SetInterruptHandler(Timer1_counter);
    
    // Stop Timer 1
    TMR1_StopTimer();
    
    // Reload Timer value
    TMR1_WriteTimer((uint16_t)0x00);
    
    // Reset time variable
    Time_reset(&current_time);
    
    // Start Timer 1
    TMR1_StartTimer();

    uint8_t S1_pressed;
    uint8_t S2_pressed;
    int time_ms = 0;

    while (1) {
        S1_pressed = 0;
        S2_pressed = 0;
        time_ms = time_ms + 1000;
        
        getData(&data);

        if(!IO_RB4_S1_GetValue()) { S1_pressed = 1; }
        if(!IO_RC5_S2_GetValue()) { S2_pressed = 1; }

        ui_update(&data,S1_pressed,S2_pressed, time_ms);
        __delay_ms(1000);
        
    }
}
