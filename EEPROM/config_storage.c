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

memory_flag_t write_record(parameter_t record, record_t buffer) {
    // Validate parameters
    if ((record<=record_START) ||
        (record>=record_END)) {
        return MEM_API_ERROR;
    }

    DATAEE_WriteByte((uint16_t)record_START | 0xF000, MW);

    uint16_t add = (uint16_t)(5*(record-record_START) - 4);
    add = ((uint16_t)record_START + add) | 0xF000;

    DATAEE_WriteByte(add, buffer.timestamp.hour);
    DATAEE_WriteByte(add+1, buffer.timestamp.minute);
    DATAEE_WriteByte(add+2, buffer.timestamp.second);
    DATAEE_WriteByte(add+3, buffer.temperature);
    DATAEE_WriteByte(add+4, buffer.luminosity);
    return MEM_OK;
}

memory_flag_t read_record(parameter_t record, record_t* buffer) {
    // Validate parameters
    if ((record<=record_START) ||
        (record>=record_END)   || !buffer ||
        !records_exist()) {
        return MEM_API_ERROR;
    }

    uint16_t add = (uint16_t)(5*(record-record_START) - 4);
    add = ((uint16_t)record_START + add) | 0xF000;

    buffer->timestamp.hour = DATAEE_ReadByte(add);
    buffer->timestamp.minute = DATAEE_ReadByte(add+1);
    buffer->timestamp.second = DATAEE_ReadByte(add+2);
    buffer->temperature = DATAEE_ReadByte(add+3);
    buffer->luminosity = DATAEE_ReadByte(add+4);

    return MEM_OK;
}

bool records_exist(void) {
    return DATAEE_ReadByte((uint16_t)record_START | 0xF000) == MW;
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
