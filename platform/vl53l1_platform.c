
/* 
* This file is part of VL53L1 Platform 
* 
* Copyright (c) 2016, STMicroelectronics - All Rights Reserved 
* 
* License terms: BSD 3-clause "New" or "Revised" License. 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met: 
* 
* 1. Redistributions of source code must retain the above copyright notice, this 
* list of conditions and the following disclaimer. 
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution. 
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software 
* without specific prior written permission. 
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
* 
*/

#include "vl53l1_platform.h"
#include <string.h>
#include <time.h>
#include <math.h>

int8_t VL53L1_WriteMulti( I2C_Handle i2c, uint16_t index, uint8_t *pdata, uint32_t count) {
	return 0; // to be implemented
}

int8_t VL53L1_ReadMulti(I2C_Handle i2c, uint16_t index, uint8_t *pdata, uint32_t count){
	return 0; // to be implemented
}

int8_t VL53L1_WrByte(I2C_Handle i2c, uint16_t index, uint8_t data) {

    bool status;

    writeBuffer[0]=(index >> 8) & 0xFF;
    writeBuffer[1]=index  & 0xFF;
    writeBuffer[2]=data  & 0xFF;

    i2cTransaction.slaveAddress = 0x29;
    i2cTransaction.writeBuf = writeBuffer;  /* Buffer to be written */
    i2cTransaction.writeCount = 3;          /* Number of bytes to be written */
    i2cTransaction.readBuf = NULL;          /* Buffer to be read */
    i2cTransaction.readCount = 0;           /* Number of bytes to be read */

    status = I2C_transfer(i2c, &i2cTransaction); /* Perform I2C transfer */
	return status;
}

int8_t VL53L1_WrWord(I2C_Handle i2c, uint16_t index, uint16_t data) {

    bool status;

    writeBuffer[0]=(index >> 8) & 0xFF;
    writeBuffer[1]=index  & 0xFF;
    writeBuffer[2]=(data >> 8) & 0xFF;
    writeBuffer[3]=data  & 0xFF;

    i2cTransaction.slaveAddress = 0x29;
    i2cTransaction.writeBuf = writeBuffer;  /* Buffer to be written */
    i2cTransaction.writeCount = 4;          /* Number of bytes to be written */
    i2cTransaction.readBuf = NULL;    /* Buffer to be read */
    i2cTransaction.readCount = 0;           /* Number of bytes to be read */

    status = I2C_transfer(i2c, &i2cTransaction); /* Perform I2C transfer */

    return status;

}

int8_t VL53L1_WrDWord(I2C_Handle i2c, uint16_t index, uint32_t data) {

    bool status;

    writeBuffer[0]=(index >> 8) & 0xFF;
    writeBuffer[1]=index  & 0xFF;
    writeBuffer[2]=(data >> 24) & 0xFF;
    writeBuffer[3]=(data >> 16) & 0xFF;
    writeBuffer[4]=(data >> 8) & 0xFF;
    writeBuffer[5]=data & 0xFF;

    i2cTransaction.slaveAddress = 0x29;
    i2cTransaction.writeBuf = writeBuffer;  /* Buffer to be written */
    i2cTransaction.writeCount = 6;          /* Number of bytes to be written */
    i2cTransaction.readBuf = NULL;    /* Buffer to be read */
    i2cTransaction.readCount = 0;           /* Number of bytes to be read */

    status = I2C_transfer(i2c, &i2cTransaction); /* Perform I2C transfer */

    return status;
}

int8_t VL53L1_RdByte(I2C_Handle i2c, uint16_t index, uint8_t *data) {

    bool retVal;

    writeBuffer[0]=(index >> 8) & 0xFF;
    writeBuffer[1]=index  & 0xFF;

    i2cTransaction.slaveAddress = 0x29;
    i2cTransaction.writeBuf = writeBuffer;  /* Buffer to be written */
    i2cTransaction.writeCount = 2;          /* Number of bytes to be written */
    i2cTransaction.readBuf = readBuffer;    /* Buffer to be read */
    i2cTransaction.readCount = 1;           /* Number of bytes to be read */

    /* Re-try writing to slave till I2C_transfer returns true */
    do {
         retVal = I2C_transfer(i2c, &i2cTransaction);
    } while(!retVal);
    *data = readBuffer[0];
    //retVal = I2C_transfer(i2c, &i2cTransaction);
    return retVal;
}

int8_t VL53L1_RdWord(I2C_Handle i2c, uint16_t index, uint16_t *data) {

    bool retVal;

    writeBuffer[0]=(index >> 8) & 0xFF;
    writeBuffer[1]=index  & 0xFF;

    i2cTransaction.slaveAddress = 0x29;
    i2cTransaction.writeBuf = writeBuffer;  /* Buffer to be written */
    i2cTransaction.writeCount = 2;          /* Number of bytes to be written */
    i2cTransaction.readBuf = readBuffer;    /* Buffer to be read */
    i2cTransaction.readCount = 2;           /* Number of bytes to be read */

    /* Re-try writing to slave till I2C_transfer returns true */
    do {
         retVal = I2C_transfer(i2c, &i2cTransaction);
    } while(!retVal);
    *data = (readBuffer[0] << 8);
    *data |= readBuffer[1] ;
    //retVal = I2C_transfer(i2c, &i2cTransaction);
    return retVal;
}

int8_t VL53L1_RdDWord(I2C_Handle i2c, uint16_t index, uint32_t *data) {

    bool retVal;

    writeBuffer[0]=(index >> 8) & 0xFF;
    writeBuffer[1]=index  & 0xFF;

    i2cTransaction.slaveAddress = 0x29;
    i2cTransaction.writeBuf = writeBuffer;  /* Buffer to be written */
    i2cTransaction.writeCount = 2;          /* Number of bytes to be written */
    i2cTransaction.readBuf = readBuffer;    /* Buffer to be read */
    i2cTransaction.readCount = 4;           /* Number of bytes to be read */

    /* Re-try writing to slave till I2C_transfer returns true */
    do {
         retVal = I2C_transfer(i2c, &i2cTransaction);
    } while(!retVal);

    *data = (readBuffer[0] << 24);
    *data |= (readBuffer[1] << 16);
    *data |= (readBuffer[2] << 8);
    *data |= (readBuffer[3] << 0);

    //retVal = I2C_transfer(i2c, &i2cTransaction);
    return retVal;
}

int8_t VL53L1_WaitMs(I2C_Handle i2c, int32_t wait_ms){
	return 0; // to be implemented
}
