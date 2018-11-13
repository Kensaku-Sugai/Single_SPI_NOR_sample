#include <mbed.h>

#define PAGE_SIZE                 256
#define CMD_READ_STATUS_REG1      0x05
#define CMD_WRITE_STATUS_REG1     0x01
#define CMD_WRITE_ENABLE          0x06
#define CMD_WRITE_DISABLE         0x04
#define CMD_READ                  0x03
#define CMD_PAGE_PROGRAM          0x02
#define CMD_BLOCK_ERASE           0xD8
#define CMD_READ_JEDEC_ID         0x9F
#define EWIP_MASK                 0x01

class SPI_NOR
{
public:
    // Create an SPI_NOR instance
    SPI_NOR(PinName mosi, PinName miso, PinName sclk, PinName cs, PinName hold);

    // Destructor of SPI_NOR
    virtual ~SPI_NOR();

    void SPI_NOR_Begin();
    void SPI_NOR_Select();
    void SPI_NOR_Deselect();
    void SPI_NOR_Hold();
    void SPI_NOR_Unhold();
    void SPI_NOR_WriteEnable();
    void SPI_NOR_WriteDisable();
    uint8_t SPI_NOR_ReadStatusReg1();
    void SPI_NOR_WriteStatusReg1(uint8_t rc);
    bool SPI_NOR_IsBusy();
    void SPI_NOR_ReadID(uint8_t* d);
    uint16_t SPI_NOR_Read(uint32_t addr, uint8_t *buf);
    void SPI_NOR_Block_Erase(uint32_t addr);
    uint16_t SPI_NOR_Page_Program(uint32_t addr, uint8_t *buf);

private:
    SPI _spi;
    DigitalOut _cs;
    DigitalOut _hold;
};
