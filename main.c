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
#include "UI/ui.h"

#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/tmr2.h"
#include "stdbool.h"

/**************** Global Variables ********************/
time_t current_time;
uint8_t temperature;
int luminosity;
ui_record_t maximum_temperature;
ui_record_t minimum_temperature;
ui_record_t maximum_luminosity;
ui_record_t minimum_luminosity;

volatile uint8_t alarm_notification_mask, read_sensors = 1, buzzer;

volatile bool interrupt_occured;
volatile bool S1_press, S2_press, S1_pressed, S2_pressed;
/******************************************************/
 
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


void buttonS1(void){
    S1_pressed = true;
    interrupt_occured = true;
    
    // clear interrupt flag
    PIR0bits.IOCIF = 0;
}

void buttonS2(void){
    S2_pressed = true;
    interrupt_occured = true;
    
    // clear interrupt flag
    PIR0bits.IOCIF = 0;
}


/**
 * Timer 1 ISR
 */
void Timer1_counter(void) {
    
    // Increment time variable
    Time_increment(&current_time);
    IO_RA7_D5_Toggle();
    memory_flag_t flag;
    flag = write_parameter(CLKH, current_time.hour);
    flag = write_parameter(CLKM, current_time.minute);
    if (flag == MEM_CORRUPTED) {
        memory_reset();
    }
    
    // Check thresholds
    uint8_t alarm_flag,
            alarm_h,
            alarm_m,
            alarm_s,
            alarm_t,
            alarm_l;
    flag = read_parameter(ALAF, &alarm_flag);
    flag = read_parameter(ALAH, &alarm_h);
    flag = read_parameter(ALAM, &alarm_m);
    flag = read_parameter(ALAS, &alarm_s);
    if (flag == MEM_CORRUPTED) {
        memory_reset();
    } else if (alarm_flag) {
        if (alarm_h == current_time.hour &&
            alarm_m == current_time.minute &&
            alarm_s == current_time.second) {
            
            alarm_notification_mask |= UI_ALARM_CLOCK;

            // Start buzzer
	    TMR2_Start();
	    buzzer = TALA_DEFAULT;
        }
    }
    
    // Read sensors
    if (!(read_sensors++ % PMON_DEFAULT)) {
        temperature = (uint8_t) readTC74();
        luminosity = ADCC_GetSingleConversion(RA0)*7/1023;
        
        // Update maximum and minimum values
        if (temperature > maximum_temperature.temperature) {
            maximum_temperature.temperature = temperature;
            maximum_temperature.luminosity = luminosity;
            maximum_temperature.timestamp = current_time;
        } else if (temperature < minimum_temperature.temperature) {
            minimum_temperature.temperature = temperature;
            minimum_temperature.luminosity = luminosity;
            minimum_temperature.timestamp = current_time;
        }
        
        if (luminosity > maximum_luminosity.luminosity) {
            maximum_luminosity.temperature = temperature;
            maximum_luminosity.luminosity = luminosity;
            maximum_luminosity.timestamp = current_time;
        } else if (luminosity < minimum_luminosity.luminosity) {
            minimum_luminosity.temperature = temperature;
            minimum_luminosity.luminosity = luminosity;
            minimum_luminosity.timestamp = current_time;
        }
        
        // Check thresholds
        flag = read_parameter(ALAT, &alarm_t);
        flag = read_parameter(ALAL, &alarm_l);
        if (flag == MEM_CORRUPTED) {
            memory_reset();
        } else if (alarm_flag) {
            if (alarm_t <= temperature) {
                alarm_notification_mask |= UI_ALARM_TEMP;
		
                // Start buzzer
		if (!IO_RA6_D4_GetValue()) {
		    TMR2_Start();
	    	    buzzer = TALA_DEFAULT;	
		}

		// Turn on LED
		IO_RA6_D4_SetHigh();

            } else {
		IO_RA6_D4_SetLow();
	    }
            if (alarm_l >= luminosity) {
                alarm_notification_mask |= UI_ALARM_LUM;
		
            	if (!IO_RA5_D3_GetValue()) {
		    TMR2_Start();
	    	    buzzer = TALA_DEFAULT;	
		}
		
		// Turn on LED
		IO_RA5_D3_SetHigh();

	    } else {
		IO_RA5_D3_SetLow();
	    }
        }
        read_sensors = 1;
    }

    // Decrement buzzer counter
    if ((buzzer--)==0) {
	TMR2_Stop();
    }
    
    interrupt_occured = true;
    
    // Clear interrupt flag
    PIR4bits.TMR1IF = 0;
}

void read_parameters(ui_display_data_t *data){
    memory_flag_t flag;
    do {
        flag = read_parameter(ALAH, &data->alarm_time.hour);
        flag = read_parameter(ALAM, &data->alarm_time.minute);
        flag = read_parameter(ALAS, &data->alarm_time.second);
        flag = read_parameter(ALAT, &data->temperature_threshold);
        flag = read_parameter(ALAL, &data->luminosity_threshold);
        flag = read_parameter(ALAF, &data->alarms_enabled);
        if (flag == MEM_CORRUPTED) {
            memory_reset();
        }
    } while (flag == MEM_CORRUPTED);
}

void getData(ui_display_data_t *data){
    
    // Fetch memory stored data
    read_parameters(data);
    
    data->temperature = temperature;
    data->luminosity = (uint8_t)luminosity;
    data->current_time = current_time;
    data->alarm_notifications = alarm_notification_mask;
    data->maximum_temperature = maximum_temperature;
    data->minimum_temperature = minimum_temperature;
    data->maximum_luminosity = maximum_luminosity;
    data->minimum_luminosity = minimum_luminosity;
}

void write_parameters(ui_display_data_t* data){
    // Disable Timer 1 interrupt
    PIE4bits.TMR1IE = 0b0;

    memory_flag_t flag;
    flag = write_parameter(ALAH, data->alarm_time.hour);
    flag = write_parameter(ALAM, data->alarm_time.minute);
    flag = write_parameter(ALAS, data->alarm_time.second);
    flag = write_parameter(ALAT, data->temperature_threshold);
    flag = write_parameter(ALAL, data->luminosity_threshold);
    flag = write_parameter(ALAF, data->alarms_enabled);
    if (flag == MEM_CORRUPTED) {
        memory_reset();
    }
    current_time = data->current_time;
    
    // Enable Timer 1 interrupt
    PIE4bits.TMR1IE = 0b1;
}

void first_reading(void){
    temperature = (uint8_t) readTC74();
    luminosity = ADCC_GetSingleConversion(RA0)*7/1023;
    maximum_temperature.temperature = temperature;
    maximum_temperature.luminosity = luminosity;
    maximum_temperature.timestamp = current_time;
    minimum_temperature.temperature = temperature;
    minimum_temperature.luminosity = luminosity;
    minimum_temperature.timestamp = current_time;
    maximum_luminosity.temperature = temperature;
    maximum_luminosity.luminosity = luminosity;
    maximum_luminosity.timestamp = current_time;
    minimum_luminosity.temperature = temperature;
    minimum_luminosity.luminosity = luminosity;
    minimum_luminosity.timestamp = current_time;
}

void main(void) {    
    ui_display_data_t data;

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

    // Initialize UI
    ui_init();
    
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
    TMR1_Reload();
    // Reset time variable
    Time_reset(&current_time);
    // Start Timer 1
    TMR1_StartTimer();
    
    // Setting button press interrupt handler
    IOCBF4_SetInterruptHandler(buttonS1);
    IOCCF5_SetInterruptHandler(buttonS2);
    
    // First reading
    first_reading();

    while (1) {
        
        SLEEP(); // wait for interrupt
        NOP(); // ensure instruction pre-fetched while sleeping is safe to execute on awakening
        
        if (interrupt_occured || ui_timeout_pending) {
            
            // Fetch data
            getData(&data);
            
            // Update UI
            ui_update(&data, S1_pressed, S2_pressed);
            
            // Reset flags
            interrupt_occured = false;
            S1_pressed = false;
            S2_pressed = false;
            
            // Update memory
            write_parameters(&data);

        }
    }
}
