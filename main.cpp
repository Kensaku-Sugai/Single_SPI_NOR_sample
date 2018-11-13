#include "mbed.h"
#include "SPI_NOR.h"

Serial pc(USBTX, USBRX);
SPI_NOR spinor(D11, D12, D13, D10, D8);

void dump(uint8_t *dt) {
    uint16_t ca = 0;

    for(uint16_t i=0; i<16; i++) {
        pc.printf("%04X: ", ca);
        for(uint16_t j=0; j<16; j++) {
            pc.printf("%02X ", dt[i*16+j]);
        }
        pc.printf("\n");
        ca = ca + 16;
    }
}

//Block erase
void block_erase(uint32_t addr) {
    pc.printf("==================Block Erase Start==================\n");
    spinor.SPI_NOR_WriteEnable();
    spinor.SPI_NOR_Block_Erase(addr);
    pc.printf("Block Erase (D8h) : done\n");
}

// Read data
void read_data(uint32_t addr) {
    uint8_t  rdata[PAGE_SIZE]={};    // Read data buffer
    uint16_t num;                    // Number of read data

    pc.printf("=====================Read Start======================\n");

    num = spinor.SPI_NOR_Read(addr, rdata);
    pc.printf("Read (03h) : num = %d\n", num);
    dump(rdata);
}

// Program data
void program_data(uint32_t addr, uint8_t mode) {
    uint8_t  pdata[PAGE_SIZE]={};    // Program data buffer
    uint16_t num;                    // Number of read data

    pc.printf("====================Program Start======================\n");

    if (mode==0) {    // Increment program mode
        for(uint16_t i=0; i<PAGE_SIZE; i++) {
            pdata[i] = pdata[i] + i;
        }
    } else if(mode==1) {    // AAh 55h program mode
        for(uint16_t i=0; i<PAGE_SIZE; i++) {
            if(i%2==0) {
                pdata[i] = 0xAA;
            } else {
                pdata[i] = 0x55;
            }
        }
    } else if(mode==2) {    // FFh 00h program mode
        for(uint16_t i=0; i<PAGE_SIZE; i++) {
            if(i%2==0) {
                pdata[i] = 0xFF;
            } else {
                pdata[i] = 0x00;
            }
        }
    } else if(mode==3) {    // 00h program mode
        for(uint16_t i=0; i<PAGE_SIZE; i++) {
            pdata[i] = 0x00;
        }
    }

    spinor.SPI_NOR_WriteEnable();
    num = spinor.SPI_NOR_Page_Program(addr, pdata);
    if(num) {
        pc.printf("Page Program (02h) : num = %d\n", num);
    }
}

int main() {
    uint8_t temp[32];    // Temporary data buffer
    uint8_t menu;
    uint32_t address;

    pc.printf("\n");

    while(1) {
        pc.printf("=====================================================\n");
        pc.printf("=          mbed Serial NOR Sample Program           =\n");
        pc.printf("=          Winbond Electronics Corporation          =\n");
        pc.printf("=====================================================\n");
        // Get Manufacture and Device ID
        spinor.SPI_NOR_ReadID(temp);
        pc.printf("MFR ID : ");
        for(uint8_t i=0; i<1; i++) {
            pc.printf("%02X", temp[i]);
            pc.printf(" ");
        }
        pc.printf("\n");
        pc.printf("DEV ID : ");
        for(uint8_t i=1; i<3; i++) {
            pc.printf("%02X", temp[i]);
            pc.printf(" ");
        }
        pc.printf("\n");

        pc.printf("=====================================================\n");
        pc.printf("Menu :\n");
        pc.printf("  1. Read\n");
        pc.printf("  2. Block Erase\n");
        pc.printf("  3. Program Increment Data\n");
        pc.printf("  4. Program AAh 55h\n");
        pc.printf("  5. Program FFh 00h\n");
        pc.printf("  6. Program 00h\n");
        pc.printf("Please input menu number: ");
        pc.scanf("%d", &menu);
        
        switch(menu) {
            case 1:
                pc.printf(">Read\n");
                pc.printf(">Please input address: ");
                pc.scanf("%x", &address);
                read_data(address);
                break;
            case 2:
                pc.printf(">Block Erase\n");
                pc.printf(">Please input address: ");
                pc.scanf("%x", &address);
                block_erase(address);
                break;
            case 3:
                pc.printf(">Program Increment Data\n");
                pc.printf(">Please input address: ");
                pc.scanf("%x", &address);
                program_data(address, 0);
                break;
            case 4:
                pc.printf(">Program AAh 55h\n");
                pc.printf(">Please input address: ");
                pc.scanf("%x", &address);
                program_data(address, 1);
                break;
            case 5:
                pc.printf(">Program FFh 00h\n");
                pc.printf(">Please input address: ");
                pc.scanf("%x", &address);
                program_data(address, 2);
                break;
            case 6:
                pc.printf(">Program 00h\n");
                pc.printf(">Please input address: ");
                pc.scanf("%x", &address);
                program_data(address, 3);
                break;
            default:
                pc.printf("Invalid menu number\n");
                break;
        }
    }
}
