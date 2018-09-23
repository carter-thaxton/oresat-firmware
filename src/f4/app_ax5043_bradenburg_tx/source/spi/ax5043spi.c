// Copyright (c) 2018 Brandenburg Tech, LLC
// All right reserved.
//
// THIS SOFTWARE IS PROVIDED BY BRANDENBURG TECH, LLC AND CONTRIBUTORS
// ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BRANDENBURT TECH, LLC
// AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "ax5043spi.h"

//#include <process.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#define MAX_SPI_WRITE_SIZE (512)

int spiChannel = -1;
int spiSpeed = -1;


void ax5043WriteReg(SPIDriver * spip, uint16_t reg, uint8_t val) {
    uint8_t buf[3];
    uint8_t ret_value[3]={0,0,0};
    int result;

    if (spiChannel < 0) {
        chprintf(DEBUG_CHP, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0xF0;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = val & 0xff;

    spiSelect(spip);
    result = spiStartExchange(spip, sizeof(buf), buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);

    if (result < 0) {
        chprintf(DEBUG_CHP,
                "Failed to write the register with result %d and error %s\n",
                result, strerror(result));
        exit(EXIT_FAILURE);
    }
}

void ax5043WriteReg2(SPIDriver * spip, uint16_t reg, uint16_t val) {
    uint8_t buf[4];
    uint8_t ret_value[4]={0,0,0,0};
    int result;

    if (spiChannel < 0) {
        chprintf(DEBUG_CHP, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0xF0;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = (val >> 8) & 0xff;
    buf[3] = val & 0xff;

    spiSelect(spip);
    result = spiStartExchange(spip, sizeof(buf), buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);

    if (result < 0) {
        chprintf(DEBUG_CHP,
                "Failed to write the register with result %d and error %s\n",
                result, strerror(result));
        exit(EXIT_FAILURE);
    }
}

void ax5043WriteReg3(SPIDriver * spip, uint16_t reg, uint32_t val) {
    uint8_t buf[5];
    uint8_t ret_value[5]={0,0,0,0,0};
    int result;

    if (spiChannel < 0) {
        chprintf(DEBUG_CHP, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0xF0;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = (val >> 16)& 0xff;
    buf[3] = (val >> 8) & 0xff;
    buf[4] = val & 0xff;

    spiSelect(spip);
    result = spiStartExchange(spip, sizeof(buf), buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);

    if (result < 0) {
        chprintf(DEBUG_CHP,
                "Failed to write the register with result %d and error %s\n",
                result, strerror(result));
        exit(EXIT_FAILURE);
    }
}

void ax5043WriteReg4(SPIDriver * spip, uint16_t reg, uint32_t val) {
    uint8_t buf[6];
    uint8_t ret_value[6]={0,0,0,0,0,0};
    int result;

    if (spiChannel < 0) {
        chprintf(DEBUG_CHP, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0xF0;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = (val >> 24) & 0xff;
    buf[3] = (val >> 16) & 0xff;
    buf[4] = (val >> 8) & 0xff;
    buf[5] = val & 0xff;

    spiSelect(spip);
    result = spiStartExchange(spip, sizeof(buf), buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);

    if (result < 0) {
        chprintf(DEBUG_CHP,
                "Failed to write the register with result %d and error %s\n",
                result, strerror(result));
        exit(EXIT_FAILURE);
    }
}

void ax5043WriteRegN(SPIDriver * spip, uint16_t reg, const uint8_t *in, uint32_t len) {
    uint8_t buf[MAX_SPI_WRITE_SIZE + 2];
    uint8_t ret_value[MAX_SPI_WRITE_SIZE + 2]={0,0,0};
    int result;

    if (spiChannel < 0) {
        chprintf(DEBUG_CHP, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }
    if (len > MAX_SPI_WRITE_SIZE) {
        chprintf(DEBUG_CHP,
                "ERROR: attempting to write too much data to SPI channel (max of %d): %d\n",
                MAX_SPI_WRITE_SIZE, len);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0xF0;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    for (uint32_t i = 0; i < len; ++i) {
        buf[i + 2] = *(in + i);
    }


    spiSelect(spip);
    result = spiStartExchange(spip, len + 2, buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);

    if (result < 0) {
        chprintf(DEBUG_CHP,
                "Failed to write the register with result %d and error %s\n",
                result, strerror(result));
        exit(EXIT_FAILURE);
    }
}

uint8_t ax5043ReadReg(SPIDriver * spip, uint16_t reg) {
    uint8_t buf[3];
    uint8_t ret_value[3]={0,0,0};
    int result;

    if (spiChannel < 0) {
        chprintf(DEBUG_CHP, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0x70;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = 0x0000;


    spiSelect(spip);
    result = spiStartExchange(spip, sizeof(buf), buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);


    if (result < 0) {
        chprintf(DEBUG_CHP,
                "Failed to read register with result = %d and error %s\n",
                result, strerror(errno));
        exit(EXIT_FAILURE);
    }

    //printf("DEBUG: read value: %d\n", (int)buf[2]);
    return (ret_value[2]);
}

uint16_t ax5043ReadReg2(SPIDriver * spip, uint16_t reg) {
    uint8_t buf[4];
    uint8_t ret_value[3]={0,0,0};
    int result;

    if (spiChannel < 0) {
        chprintf(DEBUG_CHP, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0x70;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = 0x0000;
    buf[3] = 0x0000;

    spiSelect(spip);
    result = spiStartExchange(spip, sizeof(buf), buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);


    if (result < 0) {
        chprintf(DEBUG_CHP,
                "Failed to read register with result = %d and error %s\n",
                result, strerror(errno));
        exit(EXIT_FAILURE);
    }

    //printf("DEBUG: read value: %d\n", (int)buf[2]);
    return (ret_value[3]) | (ret_value[2] << 8);
}

uint32_t ax5043ReadReg3(SPIDriver * spip, uint16_t reg) {
    uint8_t buf[5];
    uint8_t ret_value[3]={0,0,0};
    int result;

    if (spiChannel < 0) {
        chprintf(DEBUG_CHP, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0x70;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = 0x0000;
    buf[3] = 0x0000;
    buf[4] = 0x0000;

    spiSelect(spip);
    result = spiStartExchange(spip, sizeof(buf), buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);


    if (result < 0) {
        chprintf(DEBUG_CHP,
                "Failed to read register with result = %d and error %s\n",
                result, strerror(errno));
        exit(EXIT_FAILURE);
    }

    //printf("DEBUG: read value: %d\n", (int)buf[2]);
    return (ret_value[4]) | (ret_value[3] << 8) | (ret_value[2] << 16);
}

uint32_t ax5043ReadReg4(SPIDriver * spip, uint16_t reg) {
    uint8_t buf[6];
    uint8_t ret_value[6]={0,0,0};
    int result;

    if (spiChannel < 0) {
        chprintf(DEBUG_CHP, "ERROR: invalid SPI channel %d\n", spiChannel);
        exit(EXIT_FAILURE);
    }

    uint8_t mask = 0x70;

    buf[0] = mask | (~mask & (reg >> 8));
    buf[1] = (reg & 0xff);
    buf[2] = 0x0000;
    buf[3] = 0x0000;
    buf[4] = 0x0000;
    buf[5] = 0x0000;


    spiSelect(spip);
    result = spiStartExchange(spip, sizeof(buf), buf, ret_value);
    while((*spip).state != SPI_READY) { }
    spiUnselect(spip);


    if (result < 0) {
        chprintf(DEBUG_CHP,
                "Failed to read register with result = %d and error %s\n",
                result, strerror(errno));
        exit(EXIT_FAILURE);
    }

    //printf("DEBUG: read value: %d\n", (int)buf[2]);
    return (ret_value[5]) | (ret_value[4] << 8) | (ret_value[3] << 16) | (ret_value[2] << 24);
}
