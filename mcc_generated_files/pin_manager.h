/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.6
        Device            :  PIC16F18875
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.30 and above
        MPLAB 	          :  MPLAB X 5.40	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set IO_RA0_POT aliases
#define IO_RA0_POT_TRIS                 TRISAbits.TRISA0
#define IO_RA0_POT_LAT                  LATAbits.LATA0
#define IO_RA0_POT_PORT                 PORTAbits.RA0
#define IO_RA0_POT_WPU                  WPUAbits.WPUA0
#define IO_RA0_POT_OD                   ODCONAbits.ODCA0
#define IO_RA0_POT_ANS                  ANSELAbits.ANSA0
#define IO_RA0_POT_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define IO_RA0_POT_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define IO_RA0_POT_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define IO_RA0_POT_GetValue()           PORTAbits.RA0
#define IO_RA0_POT_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define IO_RA0_POT_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define IO_RA0_POT_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define IO_RA0_POT_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define IO_RA0_POT_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define IO_RA0_POT_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define IO_RA0_POT_SetAnalogMode()      do { ANSELAbits.ANSA0 = 1; } while(0)
#define IO_RA0_POT_SetDigitalMode()     do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set IO_RA4_D2 aliases
#define IO_RA4_D2_TRIS                 TRISAbits.TRISA4
#define IO_RA4_D2_LAT                  LATAbits.LATA4
#define IO_RA4_D2_PORT                 PORTAbits.RA4
#define IO_RA4_D2_WPU                  WPUAbits.WPUA4
#define IO_RA4_D2_OD                   ODCONAbits.ODCA4
#define IO_RA4_D2_ANS                  ANSELAbits.ANSA4
#define IO_RA4_D2_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define IO_RA4_D2_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define IO_RA4_D2_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define IO_RA4_D2_GetValue()           PORTAbits.RA4
#define IO_RA4_D2_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define IO_RA4_D2_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define IO_RA4_D2_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define IO_RA4_D2_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define IO_RA4_D2_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define IO_RA4_D2_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define IO_RA4_D2_SetAnalogMode()      do { ANSELAbits.ANSA4 = 1; } while(0)
#define IO_RA4_D2_SetDigitalMode()     do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set IO_RA5_D3 aliases
#define IO_RA5_D3_TRIS                 TRISAbits.TRISA5
#define IO_RA5_D3_LAT                  LATAbits.LATA5
#define IO_RA5_D3_PORT                 PORTAbits.RA5
#define IO_RA5_D3_WPU                  WPUAbits.WPUA5
#define IO_RA5_D3_OD                   ODCONAbits.ODCA5
#define IO_RA5_D3_ANS                  ANSELAbits.ANSA5
#define IO_RA5_D3_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define IO_RA5_D3_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define IO_RA5_D3_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define IO_RA5_D3_GetValue()           PORTAbits.RA5
#define IO_RA5_D3_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define IO_RA5_D3_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define IO_RA5_D3_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define IO_RA5_D3_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define IO_RA5_D3_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define IO_RA5_D3_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define IO_RA5_D3_SetAnalogMode()      do { ANSELAbits.ANSA5 = 1; } while(0)
#define IO_RA5_D3_SetDigitalMode()     do { ANSELAbits.ANSA5 = 0; } while(0)

// get/set IO_RA6_D4 aliases
#define IO_RA6_D4_TRIS                 TRISAbits.TRISA6
#define IO_RA6_D4_LAT                  LATAbits.LATA6
#define IO_RA6_D4_PORT                 PORTAbits.RA6
#define IO_RA6_D4_WPU                  WPUAbits.WPUA6
#define IO_RA6_D4_OD                   ODCONAbits.ODCA6
#define IO_RA6_D4_ANS                  ANSELAbits.ANSA6
#define IO_RA6_D4_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define IO_RA6_D4_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define IO_RA6_D4_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define IO_RA6_D4_GetValue()           PORTAbits.RA6
#define IO_RA6_D4_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define IO_RA6_D4_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)
#define IO_RA6_D4_SetPullup()          do { WPUAbits.WPUA6 = 1; } while(0)
#define IO_RA6_D4_ResetPullup()        do { WPUAbits.WPUA6 = 0; } while(0)
#define IO_RA6_D4_SetPushPull()        do { ODCONAbits.ODCA6 = 0; } while(0)
#define IO_RA6_D4_SetOpenDrain()       do { ODCONAbits.ODCA6 = 1; } while(0)
#define IO_RA6_D4_SetAnalogMode()      do { ANSELAbits.ANSA6 = 1; } while(0)
#define IO_RA6_D4_SetDigitalMode()     do { ANSELAbits.ANSA6 = 0; } while(0)

// get/set IO_RA7_D5 aliases
#define IO_RA7_D5_TRIS                 TRISAbits.TRISA7
#define IO_RA7_D5_LAT                  LATAbits.LATA7
#define IO_RA7_D5_PORT                 PORTAbits.RA7
#define IO_RA7_D5_WPU                  WPUAbits.WPUA7
#define IO_RA7_D5_OD                   ODCONAbits.ODCA7
#define IO_RA7_D5_ANS                  ANSELAbits.ANSA7
#define IO_RA7_D5_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define IO_RA7_D5_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define IO_RA7_D5_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define IO_RA7_D5_GetValue()           PORTAbits.RA7
#define IO_RA7_D5_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define IO_RA7_D5_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)
#define IO_RA7_D5_SetPullup()          do { WPUAbits.WPUA7 = 1; } while(0)
#define IO_RA7_D5_ResetPullup()        do { WPUAbits.WPUA7 = 0; } while(0)
#define IO_RA7_D5_SetPushPull()        do { ODCONAbits.ODCA7 = 0; } while(0)
#define IO_RA7_D5_SetOpenDrain()       do { ODCONAbits.ODCA7 = 1; } while(0)
#define IO_RA7_D5_SetAnalogMode()      do { ANSELAbits.ANSA7 = 1; } while(0)
#define IO_RA7_D5_SetDigitalMode()     do { ANSELAbits.ANSA7 = 0; } while(0)

// get/set IO_RB4_S1 aliases
#define IO_RB4_S1_TRIS                 TRISBbits.TRISB4
#define IO_RB4_S1_LAT                  LATBbits.LATB4
#define IO_RB4_S1_PORT                 PORTBbits.RB4
#define IO_RB4_S1_WPU                  WPUBbits.WPUB4
#define IO_RB4_S1_OD                   ODCONBbits.ODCB4
#define IO_RB4_S1_ANS                  ANSELBbits.ANSB4
#define IO_RB4_S1_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define IO_RB4_S1_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define IO_RB4_S1_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define IO_RB4_S1_GetValue()           PORTBbits.RB4
#define IO_RB4_S1_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define IO_RB4_S1_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define IO_RB4_S1_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define IO_RB4_S1_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define IO_RB4_S1_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define IO_RB4_S1_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define IO_RB4_S1_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define IO_RB4_S1_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set RC3 procedures
#define RC3_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define RC3_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define RC3_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define RC3_GetValue()              PORTCbits.RC3
#define RC3_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define RC3_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define RC3_SetPullup()             do { WPUCbits.WPUC3 = 1; } while(0)
#define RC3_ResetPullup()           do { WPUCbits.WPUC3 = 0; } while(0)
#define RC3_SetAnalogMode()         do { ANSELCbits.ANSC3 = 1; } while(0)
#define RC3_SetDigitalMode()        do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set RC4 procedures
#define RC4_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define RC4_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define RC4_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define RC4_GetValue()              PORTCbits.RC4
#define RC4_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define RC4_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define RC4_SetPullup()             do { WPUCbits.WPUC4 = 1; } while(0)
#define RC4_ResetPullup()           do { WPUCbits.WPUC4 = 0; } while(0)
#define RC4_SetAnalogMode()         do { ANSELCbits.ANSC4 = 1; } while(0)
#define RC4_SetDigitalMode()        do { ANSELCbits.ANSC4 = 0; } while(0)

// get/set IO_RC5_S2 aliases
#define IO_RC5_S2_TRIS                 TRISCbits.TRISC5
#define IO_RC5_S2_LAT                  LATCbits.LATC5
#define IO_RC5_S2_PORT                 PORTCbits.RC5
#define IO_RC5_S2_WPU                  WPUCbits.WPUC5
#define IO_RC5_S2_OD                   ODCONCbits.ODCC5
#define IO_RC5_S2_ANS                  ANSELCbits.ANSC5
#define IO_RC5_S2_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define IO_RC5_S2_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define IO_RC5_S2_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define IO_RC5_S2_GetValue()           PORTCbits.RC5
#define IO_RC5_S2_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define IO_RC5_S2_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define IO_RC5_S2_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define IO_RC5_S2_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define IO_RC5_S2_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define IO_RC5_S2_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define IO_RC5_S2_SetAnalogMode()      do { ANSELCbits.ANSC5 = 1; } while(0)
#define IO_RC5_S2_SetDigitalMode()     do { ANSELCbits.ANSC5 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/