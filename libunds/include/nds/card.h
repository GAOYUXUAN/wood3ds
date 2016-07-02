/*---------------------------------------------------------------------------------

	Copyright (C) 2005
		Michael Noland (joat)
		Jason Rogers (dovoto)
		Dave Murphy (WinterMute)

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you
		must not claim that you wrote the original software. If you use
		this software in a product, an acknowledgment in the product
		documentation would be appreciated but is not required.
	2.	Altered source versions must be plainly marked as such, and
		must not be misrepresented as being the original software.
	3.	This notice may not be removed or altered from any source
		distribution.

---------------------------------------------------------------------------------*/

#ifndef NDS_CARD_INCLUDE
#define NDS_CARD_INCLUDE


#include "ndstypes.h"

// Card bus
   
#define CARD_COMMAND   ((vu8*)0x040001A8)

#define REG_ROMCTRL		(*(vu32*)0x040001A4)
#define REG_AUXSPICNT	(*(vu16*)0x040001A0)
#define REG_AUXSPICNTH	(*(vu8*)0x040001A1)
#define REG_AUXSPIDATA	(*(vu8*)0x040001A2)

#define CARD_DATA_RD   (*(vu32*)0x04100010)

#define CARD_1B0       (*(vu32*)0x040001B0)
#define CARD_1B4       (*(vu32*)0x040001B4)
#define CARD_1B8       (*(vu16*)0x040001B8)
#define CARD_1BA       (*(vu16*)0x040001BA)


#define CARD_CR1_ENABLE  0x80  // in byte 1, i.e. 0x8000
#define CARD_CR1_IRQ     0x40  // in byte 1, i.e. 0x4000

// SPI EEPROM COMMANDS
#define EEPROM_WRSR	0x01
#define EEPROM_WRDI	0x04
#define EEPROM_RDSR	0x05
#define EEPROM_WREN	0x06
#define EEPROM_RDID 0x9f

// ROMCTRL register:

#define CARD_ACTIVATE     (1<<31)           // when writing, get the ball rolling
#define CARD_WR           (1<<30)           // Card write enable
#define CARD_nRESET       (1<<29)           // value on the /reset pin (1 = high out, not a reset state, 0 = low out = in reset)
#define CARD_SEC_LARGE    (1<<28)           // Use "other" secure area mode, which tranfers blocks of 0x1000 bytes at a time
#define CARD_CLK_SLOW     (1<<27)           // Transfer clock rate (0 = 6.7MHz, 1 = 4.2MHz)
#define CARD_BLK_SIZE(n)  (((n)&0x7)<<24)   // Transfer block size, (0 = None, 1..6 = (0x100 << n) bytes, 7 = 4 bytes)
#define CARD_SEC_CMD      (1<<22)           // The command transfer will be hardware encrypted (KEY2)
#define CARD_DELAY2(n)    (((n)&0x3F)<<16)  // Transfer delay length part 2
#define CARD_SEC_SEED     (1<<15)           // Apply encryption (KEY2) seed to hardware registers
#define CARD_SEC_EN       (1<<14)           // Security enable
#define CARD_SEC_DAT      (1<<13)           // The data transfer will be hardware encrypted (KEY2)
#define CARD_DELAY1(n)    ((n)&0x1FFF)      // Transfer delay length part 1

// 3 bits in b10..b8 indicate something
// read bits
#define CARD_BUSY         (1<<31)           // when reading, still expecting incomming data?
#define CARD_DATA_READY   (1<<23)           // when reading, CARD_DATA_RD or CARD_DATA has another word of data and is good to go

// Card commands
#define CARD_CMD_DUMMY          0x9F
#define CARD_CMD_HEADER_READ    0x00
#define CARD_CMD_HEADER_CHIPID  0x90
#define CARD_CMD_ACTIVATE_BF    0x3C  // Go into blowfish (KEY1) encryption mode
#define CARD_CMD_ACTIVATE_SEC   0x40  // Go into hardware (KEY2) encryption mode
#define CARD_CMD_SECURE_CHIPID  0x10
#define CARD_CMD_SECURE_READ    0x20
#define CARD_CMD_DISABLE_SEC    0x60  // Leave hardware (KEY2) encryption mode
#define CARD_CMD_DATA_MODE      0xA0
#define CARD_CMD_DATA_READ      0xB7
#define CARD_CMD_DATA_CHIPID    0xB8


#ifdef __cplusplus
extern "C" {
#endif


void cardWriteCommand(const u8 *command);
void cardPolledTransfer(u32 flags, u32 *destination, u32 length, const u8 *command);
void cardStartTransfer(const u8 *command, u32 *destination, int channel, u32 flags);
uint32 cardWriteAndRead(const u8 *command, u32 flags);
void cardParamCommand (u8 command, u32 parameter, u32 flags, u32 *destination, u32 length);

// These commands require the cart to not be initialized yet, which may mean the user
// needs to eject and reinsert the cart or they will return random data.
void cardReadHeader(u8 *header);
u32 cardReadID(u32 flags);

#ifdef __cplusplus
}
#endif


#endif

