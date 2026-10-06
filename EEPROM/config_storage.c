/*
 * File:   config_storage.c
 * Author: Tomás Seabra
 * 
 */

#include <xc.h>
#include "config_storage.h"

static bool memory_initialized = false;

memory_flag_t memory_init(void) {
    
    memory_initialized = true;
    
    if (DATAEE_ReadByte(0x00) != MW) {
        // No data has been written yet
        memory_reset();
    } else {
        // Validate checksum
        uint8_t checksum = 0;
        for (int i=1; i<=8; i++) {
            checksum += DATAEE_ReadByte((uint16_t)i);
        }
        if (checksum != DATAEE_ReadByte(0x09)) {
            // Data is corrupted
            return MEM_CORRUPTED;
        }
    }
    return MEM_OK;
}

memory_flag_t write_parameter(parameter_t parameter, uint8_t buffer) {
    // Validate parameters
    if ((parameter<=parameter_START) || 
        (parameter>=parameter_END)   || 
        !memory_initialized) {
        return MEM_API_ERROR;
    }
    
    // Validate checksum
    uint8_t checksum = 0;
    for (int i=1; i<=8; i++) {
        checksum += DATAEE_ReadByte((uint16_t)i);
    }
    if (checksum != DATAEE_ReadByte(0x09)) {
        // Data is corrupted
        return MEM_CORRUPTED;
    }
    
    DATAEE_WriteByte((uint16_t)parameter, buffer);
    
    // Re-compute checksum
    checksum = 0;
    for (int i=1; i<=8; i++) {
        checksum += DATAEE_ReadByte((uint16_t)i);
    }
    DATAEE_WriteByte(0x09, checksum);
    
    return MEM_OK;
}

memory_flag_t read_parameter(parameter_t parameter, uint8_t* buffer) {
    // Validate parameters
    if ((parameter<=parameter_START) ||
        (parameter>=parameter_END)   || 
        !buffer                      || 
        !memory_initialized) {
        return MEM_API_ERROR;
    }
    
    // Validate checksum
    uint8_t checksum = 0;
    for (int i=1; i<=8; i++) {
        checksum += DATAEE_ReadByte((uint16_t)i);
    }
    if (checksum != DATAEE_ReadByte(0x09)) {
        // Data is corrupted
        return MEM_CORRUPTED;
    }
    
    *buffer = DATAEE_ReadByte((uint16_t)parameter);
    
    return MEM_OK;
}

void memory_reset(void) {
    DATAEE_WriteByte(0x00, MW);
    DATAEE_WriteByte((uint16_t)PMON, PMON_DEFAULT);
    DATAEE_WriteByte((uint16_t)TALA, TALA_DEFAULT);
    DATAEE_WriteByte((uint16_t)ALAF, ALAF_DEFAULT);
    DATAEE_WriteByte((uint16_t)ALAH, ALAH_DEFAULT);
    DATAEE_WriteByte((uint16_t)ALAM, ALAM_DEFAULT);
    DATAEE_WriteByte((uint16_t)ALAS, ALAS_DEFAULT);
    DATAEE_WriteByte((uint16_t)ALAT, ALAT_DEFAULT);
    DATAEE_WriteByte((uint16_t)ALAL, ALAL_DEFAULT);
    DATAEE_WriteByte((uint16_t)CLKH, CLKH_DEFAULT);
    DATAEE_WriteByte((uint16_t)CLKM, CLKM_DEFAULT);
    uint8_t checksum = 0;
    for (int i=1; i<=8; i++) {
        checksum += DATAEE_ReadByte((uint16_t)i);
    }
    DATAEE_WriteByte(0x09, checksum);
}
