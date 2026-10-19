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
#include <string.h>

/**************** Global Variables ********************/
time_t current_time;
uint8_t temperature;
uint8_t luminosity;
ui_record_t maximum_temperature;
ui_record_t minimum_temperature;
ui_record_t maximum_luminosity;
ui_record_t minimum_luminosity;

volatile uint8_t alarm_notification_mask, buzzer, read_sensors = 1;

volatile bool interrupt_occured, tick = false;
volatile bool S1_press, S2_press, S1_pressed, S2_pressed;
/******************************************************/


/**
 * Reads potentiometer value and converts it to a scale of 0 to 7.
 * 
 * @return Luminosity value. 
 */
uint8_t getPOTvalue(){
    int signal = ADCC_GetSingleConversion(RA0)*7/1023;
    uint8_t value = (uint8_t) signal;
    return value;
}


/**
 * Reads the temperature sensor.
 * 
 * @return Temperature reading in 2's complement format.
 */
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


/**
 * Button S1 falling edge ISR.
 */
void buttonS1(void){
    
    // TEMPORARY////
    __delay_ms(50);
    ////////////////
    
    S1_pressed = true;
    interrupt_occured = true;
    
    // clear interrupt flag
    PIR0bits.IOCIF = 0;
}


/**
 * Button S2 falling edge ISR.
 */
void buttonS2(void){
    
    // TEMPORARY////
    __delay_ms(50);
    ////////////////
    
    S2_pressed = true;
    interrupt_occured = true;
    
    // clear interrupt flag
    PIR0bits.IOCIF = 0;
}


/**
 * Timer 1 ISR
 */
void Timer1_counter(void) {
    interrupt_occured = true;
    tick = true;
    
    // Clear interrupt flag
    PIR4bits.TMR1IF = 0;
}


/**
 * Tasks to be executed every tick of Timer 1.
 * 
 * 1. Increments global time variable
 * 2. Verifies the Time, Temperature and Luminosity thresholds
 * 3. Reads the sensors
 * 4. Executes the alarm in case of any threshold breach
 */
void timer_tick(void) {
    
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
            alarm_l;
    int8_t alarm_t;
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
        int8_t temp_compare;
        memcpy(&temp_compare, &temperature, sizeof(uint8_t));
        luminosity = getPOTvalue();
        
        // Update maximum and minimum values
        if (temp_compare > maximum_temperature.temperature) {
            maximum_temperature.temperature = temp_compare;
            maximum_temperature.luminosity = luminosity;
            maximum_temperature.timestamp = current_time;
        } else if (temp_compare < minimum_temperature.temperature) {
            minimum_temperature.temperature = temp_compare;
            minimum_temperature.luminosity = luminosity;
            minimum_temperature.timestamp = current_time;
        }
        
        if (luminosity > maximum_luminosity.luminosity) {
            maximum_luminosity.temperature = temp_compare;
            maximum_luminosity.luminosity = luminosity;
            maximum_luminosity.timestamp = current_time;
        } else if (luminosity < minimum_luminosity.luminosity) {
            minimum_luminosity.temperature = temp_compare;
            minimum_luminosity.luminosity = luminosity;
            minimum_luminosity.timestamp = current_time;
        }
        
        // Check thresholds
        uint8_t buffer;
        flag = read_parameter(ALAT, &buffer);
        memcpy(&alarm_t, &buffer, sizeof(uint8_t));
        flag = read_parameter(ALAL, &alarm_l);
        if (flag == MEM_CORRUPTED) {
            memory_reset();
        } else if (alarm_flag) {
            if (alarm_t <= temp_compare) {
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
}


/**
 * Fetches local data to be passed to the User Interface API.
 * 
 * @param data pointer to the data structure where local data will be copied to.
 */
void getData(ui_display_data_t *data){
    
    // Fetch memory stored data
    memory_flag_t flag;
    do {
        flag = read_parameter(ALAH, &data->alarm_time.hour);
        flag = read_parameter(ALAM, &data->alarm_time.minute);
        flag = read_parameter(ALAS, &data->alarm_time.second);
        uint8_t buffer;
        flag = read_parameter(ALAT, &buffer);
        memcpy(&data->temperature_threshold, &buffer, sizeof(uint8_t));
        flag = read_parameter(ALAL, &data->luminosity_threshold);
        flag = read_parameter(ALAF, &data->alarms_enabled);
        if (flag == MEM_CORRUPTED) {
            memory_reset();
        }
    } while (flag == MEM_CORRUPTED);
    
    memcpy(&data->temperature, &temperature, sizeof(uint8_t));
    data->luminosity = luminosity;
    data->current_time = current_time;
    data->alarm_notifications = alarm_notification_mask;
    data->maximum_temperature = maximum_temperature;
    data->minimum_temperature = minimum_temperature;
    data->maximum_luminosity = maximum_luminosity;
    data->minimum_luminosity = minimum_luminosity;
}


/**
 * Updates local data with the values updated by the User Interface.
 * 
 * @param data pointer to the data structure from which local data will be 
 * updated.
 */
void write_parameters(ui_display_data_t* data){
    memory_flag_t flag;
    flag = write_parameter(ALAH, data->alarm_time.hour);
    flag = write_parameter(ALAM, data->alarm_time.minute);
    flag = write_parameter(ALAS, data->alarm_time.second);
    uint8_t buffer;
    memcpy(&buffer, &data->temperature_threshold, sizeof(uint8_t));
    flag = write_parameter(ALAT, buffer);
    flag = write_parameter(ALAL, data->luminosity_threshold);
    flag = write_parameter(ALAF, data->alarms_enabled);
    if (flag == MEM_CORRUPTED) {
        memory_reset();
    }
    current_time = data->current_time;
}


/**
 * Initializes Temperature and Luminosity records.
 */
void first_reading(void){
    maximum_temperature.temperature = ALAT_DEFAULT;
    maximum_temperature.luminosity = ALAL_DEFAULT;
    maximum_temperature.timestamp = current_time;
    minimum_temperature.temperature = ALAT_DEFAULT;
    minimum_temperature.luminosity = ALAL_DEFAULT;
    minimum_temperature.timestamp = current_time;
    maximum_luminosity.temperature = ALAT_DEFAULT;
    maximum_luminosity.luminosity = ALAL_DEFAULT;
    maximum_luminosity.timestamp = current_time;
    minimum_luminosity.temperature = ALAT_DEFAULT;
    minimum_luminosity.luminosity = ALAL_DEFAULT;
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
    
    // Stop Timer 1
    TMR1_StopTimer();
    // Setting Timer 1 interrupt handler
    TMR1_SetInterruptHandler(Timer1_counter);
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
        
        if (tick) {
            timer_tick();
            tick = false;
        }
        
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
