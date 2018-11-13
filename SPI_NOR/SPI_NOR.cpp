#include "SPI_NOR.h"

SPI_NOR::SPI_NOR(PinName mosi, PinName miso, PinName sclk, PinName cs, PinName hold)
    :
    _spi(mosi, miso, sclk),
    _cs(cs),
    _hold(hold)
{
    SPI_NOR_Begin();
}

SPI_NOR::~SPI_NOR() {
}

void SPI_NOR::SPI_NOR_Begin() {
    _spi.format(8, 0);
    _spi.frequency(20000000);  //SPI Clock = 20MHz
    SPI_NOR_Deselect();
}

void SPI_NOR::SPI_NOR_Select() {
    _cs = 0;
}

void SPI_NOR::SPI_NOR_Deselect() {
    _cs = 1;
}

void SPI_NOR::SPI_NOR_Hold() {
    _hold = 0;
}

void SPI_NOR::SPI_NOR_Unhold() {
    _hold = 1;
}

void SPI_NOR::SPI_NOR_WriteEnable() {
    SPI_NOR_Select();
    SPI_NOR_Unhold();
    _spi.write(CMD_WRITE_ENABLE);
    SPI_NOR_Deselect();
}

void SPI_NOR::SPI_NOR_WriteDisable() {
    SPI_NOR_Select();
    SPI_NOR_Unhold();
    _spi.write(CMD_WRITE_DISABLE);
    SPI_NOR_Deselect();
}

uint8_t SPI_NOR::SPI_NOR_ReadStatusReg1() {
    uint8_t rc;
    SPI_NOR_Select();
    SPI_NOR_Unhold();
    _spi.write(CMD_READ_STATUS_REG1);
    rc = _spi.write(0xFF);
    SPI_NOR_Deselect();
    return rc;
}

void SPI_NOR::SPI_NOR_WriteStatusReg1(uint8_t rc) {
    SPI_NOR_Select();
    SPI_NOR_Unhold();
    _spi.write(CMD_WRITE_STATUS_REG1);
    _spi.write(rc);
    SPI_NOR_Deselect();
}

bool SPI_NOR::SPI_NOR_IsBusy() {
    uint8_t r1;
    SPI_NOR_Select();
    SPI_NOR_Unhold();
    _spi.write(CMD_READ_STATUS_REG1);
    r1 = _spi.write(0xFF);
    SPI_NOR_Deselect();
    if(r1 & EWIP_MASK)
        return true;
    return false;
}

void SPI_NOR::SPI_NOR_ReadID(uint8_t* d) {
    SPI_NOR_Select();
    SPI_NOR_Unhold();
    _spi.write(CMD_READ_JEDEC_ID);
    for (uint8_t i=0; i<3; i++) {
        d[i] = _spi.write(0x00);
    }
    SPI_NOR_Deselect();
}

uint16_t SPI_NOR::SPI_NOR_Read(uint32_t addr, uint8_t *buf) { 
    SPI_NOR_Select();
    SPI_NOR_Unhold();
    _spi.write(CMD_READ);
    _spi.write(addr>>16);       // A23-A16
    _spi.write(addr>>8);        // A15-A08
    _spi.write(addr & 0xFF);    // A07-A00
    uint16_t i;
    for(i=0; i<PAGE_SIZE; i++) {
        buf[i] = _spi.write(0x00);
    }
    SPI_NOR_Deselect();
    wait(0.1);
    return i;
}

void SPI_NOR::SPI_NOR_Block_Erase(uint32_t addr) {
    SPI_NOR_Select();
    SPI_NOR_Unhold();
    _spi.write(CMD_BLOCK_ERASE);
    _spi.write(addr>>16);       // A23-A16
    _spi.write(addr>>8);        // A15-A08
    _spi.write(addr & 0xFF);    // A07-A00
    SPI_NOR_Deselect();
    wait(2);
}

uint16_t SPI_NOR::SPI_NOR_Page_Program(uint32_t addr, uint8_t *buf) {
    SPI_NOR_Select();
    SPI_NOR_Unhold();
    _spi.write(CMD_PAGE_PROGRAM);
    _spi.write(addr>>16);       // A23-A16
    _spi.write(addr>>8);        // A15-A08
    _spi.write(addr & 0xFF);    // A07-A00
    uint16_t i;
    for(i=0; i<PAGE_SIZE; i++) {
        _spi.write(buf[i]);
    }
    SPI_NOR_Deselect();
    wait(0.1);
    return i;
}
