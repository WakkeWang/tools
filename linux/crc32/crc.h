/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2009
 * Marvell Semiconductor <www.marvell.com>
 * Written-by: Prafulla Wadaskar <prafulla@marvell.com>
 */

#ifndef _UBOOT_CRC_H
#define _UBOOT_CRC_H

/* borrowed from compiler.h*/
#define uswap_32(x) \
     ((((x) & 0xff000000) >> 24) | \
      (((x) & 0x00ff0000) >>  8) | \
      (((x) & 0x0000ff00) <<  8) | \
      (((x) & 0x000000ff) << 24))

#if __BYTE_ORDER == __LITTLE_ENDIAN
# define cpu_to_le32(x)     (x)
# define le32_to_cpu(x)     (x)
#else
# define cpu_to_le32(x)     uswap_32(x)
# define le32_to_cpu(x)     uswap_32(x)
#endif


#if 0
#include <compiler.h> /* 'uint*' definitions */
#else
#define USE_HOSTCC
//#include <compiler.h> /* 'uint*' definitions */
typedef signed char             int8_t;
typedef short int               int16_t;
typedef int                     int32_t;

typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;

typedef uint8_t __u8;
typedef uint16_t __u16;
typedef uint32_t __u32;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef uint32_t uint;
typedef uint32_t uInt;
typedef uint32_t uLong;
typedef unsigned char Byte;
typedef unsigned char Bytef;

#endif 

/**
 * crc8() - Calculate and return CRC-8 of the data
 *
 * This uses an x^8 + x^2 + x + 1 polynomial.  A table-based algorithm would
 * be faster, but for only a few bytes it isn't worth the code size
 *
 * lib/crc8.c
 *
 * @crc_start: CRC8 start value
 * @vptr: Buffer to checksum
 * @len: Length of buffer in bytes
 * @return CRC8 checksum
 */
unsigned int crc8(unsigned int crc_start, const unsigned char *vptr, int len);

/* lib/crc16.c - 16 bit CRC with polynomial x^16+x^12+x^5+1 (CRC-CCITT) */
uint16_t crc16_ccitt(uint16_t crc_start, const unsigned char *s, int len);
/**
 * crc16_ccitt_wd_buf - Perform CRC16-CCIT on an input buffer and return the
 *                      16-bit result (network byte-order) in an output buffer
 *
 * @in:	input buffer
 * @len: input buffer length
 * @out: output buffer (at least 2 bytes)
 * @chunk_sz: ignored
 */
void crc16_ccitt_wd_buf(const uint8_t *in, uint len,
			uint8_t *out, uint chunk_sz);

/* lib/crc32.c */

/**
 * crc32 - Calculate the CRC32 for a block of data
 *
 * @crc: Input crc to chain from a previous calculution (use 0 to start a new
 *	calculation)
 * @buf: Bytes to checksum
 * @len: Number of bytes to checksum
 * @return checksum value
 */
uint32_t crc32(uint32_t crc, const unsigned char *buf, uint len);

/**
 * crc32_wd - Calculate the CRC32 for a block of data (watchdog version)
 *
 * This checksums the data @chunk_sz bytes at a time, calling WATCHDOG_RESET()
 * after each chunk, to prevent the watchdog from firing.
 *
 * @crc: Input crc to chain from a previous calculution (use 0 to start a new
 *	calculation)
 * @buf: Bytes to checksum
 * @len: Number of bytes to checksum
 * @chunk_sz: Chunk size to use between watchdog resets
 * @return checksum
 */
uint32_t crc32_wd(uint32_t crc, const unsigned char *buf, uint len,
		  uint chunk_sz);

/**
 * crc32_no_comp - Calculate the CRC32 for a block of data (no one's compliment)
 *
 * This version uses a different algorithm which doesn't use one's compliment.
 * JFFS2 (and other things?) use this.
 *
 * @crc: Input crc to chain from a previous calculution (use 0 to start a new
 *	calculation)
 * @buf: Bytes to checksum
 * @len: Number of bytes to checksum
 * @return checksum value
 */
uint32_t crc32_no_comp(uint32_t crc, const unsigned char *buf, uint len);

/**
 * crc32_wd_buf - Perform CRC32 on a buffer and return result in buffer
 *
 * @input:	Input buffer
 * @ilen:	Input buffer length
 * @output:	Place to put checksum result (4 bytes)
 * @chunk_sz:	Trigger watchdog after processing this many bytes
 */
void crc32_wd_buf(const uint8_t *input, uint ilen, uint8_t *output,
		  uint chunk_sz);

/* lib/crc32c.c */

/**
 * crc32c_init() - Set up a the CRC32 table
 *
 * This sets up 256-item table to aid in CRC32 calculation
 *
 * @crc32c_table: Place to put table
 * @pol: polynomial to use
 */
void crc32c_init(uint32_t *crc32c_table, uint32_t pol);

/**
 * crc32c_cal() - Perform CRC32 on a buffer given a table
 *
 * This algorithm uses the table (set up by crc32c_init() to speed up
 * processing.
 *
 * @crc: Previous crc (use 0 at start)
 * @data: Data bytes to checksum
 * @length: Number of bytes to process
 * @crc32c_table:: CRC table
 * @return checksum value
 */
uint32_t crc32c_cal(uint32_t crc, const char *data, int length,
		    uint32_t *crc32c_table);

#endif /* _UBOOT_CRC_H */
