#include "config_storage.h"

static bool memory_initialized = false;

memory_flag_t memory_init(void) {
    
    memory_initialized = true;
    
    if (DATAEE_ReadByte(0xF000) != MW) {
        // No data has been written yet
        memory_reset();
    } else {
        // Validate checksum
        uint8_t checksum = 0;
        for (uint16_t i=1; i<=10; i++) {
            checksum += DATAEE_ReadByte(i | 0xF000);
        }
        if (checksum != DATAEE_ReadByte(0xF00B)) {
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
    for (uint16_t i=1; i<=10; i++) {
        checksum += DATAEE_ReadByte(i | 0xF000);
    }
    if (checksum != DATAEE_ReadByte(0xF00B)) {
        // Data is corrupted
        return MEM_CORRUPTED;
    }
    
    DATAEE_WriteByte((uint16_t)parameter | 0xF000, buffer);
    
    // Re-compute checksum
    checksum = 0;
    for (uint16_t i=1; i<=10; i++) {
        checksum += DATAEE_ReadByte(i | 0xF000);
    }
    DATAEE_WriteByte(0xF00B, checksum);
    
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
    for (uint16_t i=1; i<=10; i++) {
        checksum += DATAEE_ReadByte(i | 0xF000);
    }
    if (checksum != DATAEE_ReadByte(0xF00B)) {
        // Data is corrupted
        return MEM_CORRUPTED;
    }
    
    *buffer = DATAEE_ReadByte((uint16_t)parameter | 0xF000);
    
    return MEM_OK;
}

void memory_reset(void) {
    DATAEE_WriteByte(0xF000, MW);
    DATAEE_WriteByte((uint16_t)PMON | 0xF000, PMON_DEFAULT);
    DATAEE_WriteByte((uint16_t)TALA | 0xF000, TALA_DEFAULT);
    DATAEE_WriteByte((uint16_t)ALAF | 0xF000, ALAF_DEFAULT);
    DATAEE_WriteByte((uint16_t)ALAH | 0xF000, ALAH_DEFAULT);
    DATAEE_WriteByte((uint16_t)ALAM | 0xF000, ALAM_DEFAULT);
    DATAEE_WriteByte((uint16_t)ALAS | 0xF000, ALAS_DEFAULT);
    DATAEE_WriteByte((uint16_t)ALAT | 0xF000, ALAT_DEFAULT);
    DATAEE_WriteByte((uint16_t)ALAL | 0xF000, ALAL_DEFAULT);
    DATAEE_WriteByte((uint16_t)CLKH | 0xF000, CLKH_DEFAULT);
    DATAEE_WriteByte((uint16_t)CLKM | 0xF000, CLKM_DEFAULT);
    uint8_t checksum = 0;
    for (uint16_t i=1; i<=10; i++) {
        checksum += DATAEE_ReadByte(i | 0xF000);
    }
    DATAEE_WriteByte(0xF00B, checksum);
}
