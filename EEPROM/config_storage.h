/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   config_storage.h
 * Author: Tomás Seabra
 * Comments:
 * Revision history: 
 */
 
#ifndef CONFIG_STORAGE_H
#define	CONFIG_STORAGE_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "..\mcc_generated_files/memory.h"

#define MW 0xA5

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
    parameter_START,
            
    PMON,  // monitoring period
    TALA,  // duration of alarm signal
    ALAF,  // alarm fag
    ALAH,  // alarm clock hours
    ALAM,  // alarm clock minutes
    ALAS,  // alarm clock seconds
    ALAT,  // threshold for temperature alarm
    ALAL,  // threshold for luminosity level alarm
    CLKH,  // clock hours
    CLKM,   // clock minutes
            
    parameter_END
} parameter_t;

typedef enum {
    MEM_OK,         // no error
         
    MEM_CORRUPTED,  // data corrupted
    MEM_API_ERROR   // api error
} memory_flag_t;

memory_flag_t memory_init(void);

memory_flag_t write_parameter(parameter_t parameter, uint8_t buffer);

memory_flag_t read_parameter(parameter_t parameter, uint8_t* buffer);

void memory_reset(void);

#endif	

