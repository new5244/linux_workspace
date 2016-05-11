/*
 * fconfig.c
 *
 * $Id: fconfig.c,v 1.3 2008/03/29 12:20:55 andrzej Exp $
 *
 * Redboot Flash Configuration parser. 
 * Main file. 
 *
 * Copyright (C) 2006 Ekiert sp z o.o.
 * Author: Andrzej Ekiert <a.ekiert@ekiert.com>
 *
 * Changes:
 *   2007/10/21 - 'list' option added by Hamish Moffatt
 *   2008/03/29 - 'offset' option added by wimpunk
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version. 
 *
 * Usage hints:
 * The 'device' is probably going to be an MTD device. You may also operate
 * on an ordinary file. However, if you're working with MTD, it's highly 
 * recommended to open it as a character device, not as a block device 
 * (eg. open /dev/mtd[n], not /dev/mtdblock[n]). If an MTD device is emulating
 * a block device, then the OS will do heavy caching (a lot of unneccessary 
 * reads), and will do block writes, while in character device mode it's
 * possible to actually write exactly these bytes, that have been changed
 * (see my 24xx I2C EEPROM driver - ee24.c). In case of operation with slow
 * I2C EEPROMs this allows to speed up things and to save on EEPROM wear-out. 
 *
 * The above should explain why mmap() is not used in this application ;-)
 * (initially it was, but the performance was poor, when we're forced to work
 *  with slow block devices). 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>

#include "debug.h"
#include "ftypes.h"
#include "crunchfc.h"

/*
 * Parse type name, return type ID. 
 * Type ID is the type number in the type table. 
 */
/*
static int8_t parse_type(uint8_t *type)
{
	uint8_t i, ret = -1;

	MESSAGE(VERB_HIGH, "Parsing type: %s\n", type);
	for (i = 0; i < NUM_TYPES; i++) {
		if (strncmp(TYPE_NAME(i), type, MAX_TYPE_NAME)==0) {
			MESSAGE(VERB_HIGH, "Found type: ID = %d\n", i);
			ret = i;
			break;
		}
	}
	return ret;
}
*/

/*
 * Print usage information.
 */
static void usage(void)
{
	uint8_t i;
	if (verbosity == 0) {
		return;
	}

	fputs("Read, write or list Redboot configuration\n", stdout);
	fputs("usage: fconfig [-r|-w|-l]"
	      " -d dev -o offset -n nickname -x value\n", stdout);
	fputs("'dev' may be a char device, block device or a file\n", stdout);
	fputs("'offset' is the offset in bytes on the device\n", stdout);
	fputs("Supported types: \n", stdout);
	for (i = 0; i < NUM_TYPES; i++) {
		printf(" - %s\n", TYPE_NAME(i));
	}
	fputs("Additional switches: \n", stdout);
	fputs(" -v:\tVerbose mode, use more to increase verbosity\n", stdout);
	fputs(" -s:\tSilent mode, absolutely no messages printed\n", stdout);
}

/*
 * Open the file or device containing configuration and copy the configuration 
 * data to a buffer. We could mmap() here and operate directly on mmaped data 
 * (it'd make life easier later), but that would force us to work with MTD in 
 * block device emulation mode (see comments at the top of this file). 
 *
 * Remember, that the MTD partition is likely to be "forced read-only", 
 * if the partition size is not equal to erase block size.
 */
static uint8_t buffer[MAX_CONFIG_DATA];
struct config_data *get_fconfig_handle(struct config_data *data, 
	uint8_t *dev, int offset, mode_t mode)
{
	uint16_t count;

	if ((data->fd = open(dev, mode)) < 0) {
		MESSAGE(VERB_LOW, "Failed to open device or file %s!\n", dev);
		return NULL;
	}

	data->offset = offset;
	if (data->offset > 0) {
		lseek(data->fd, data->offset, SEEK_SET);
	}
	count = read(data->fd, buffer, MAX_CONFIG_DATA);
	if (count <= 0) {
		MESSAGE(VERB_LOW, "Nothing read!\n");
		close(data->fd);
		return NULL;
	}
	MESSAGE(VERB_NORMAL, "Read %d bytes\n", count);

	data->buf = buffer;
	data->maxlen = count;
	return data;
}

/*
 * Synchronize the data back and close the file or device containing 
 * the configuration data. 
 */
void close_fconfig_handle(struct config_data *data)
{
	close(data->fd);
}

/*
 * Write mode of operation: set parameter values in the configuration. 
 */
static int write_mode(uint8_t *device, int offset, 
	uint8_t *nickname, uint8_t *value)
{
	struct config_data data;
	if (value == NULL) {
		MESSAGE(VERB_LOW, "You must provide a value in WRITE mode\n");
		return 1;
	}

	if (get_fconfig_handle(&data, device, offset, O_RDWR) == NULL) {
		MESSAGE(VERB_LOW, "Could not get a config data handle!\n");
		return 1;
	}
	if (verify_fconfig(&data)) {
		MESSAGE(VERB_LOW, "Config verification failed!\n");
		goto exit_fail;
	}

	if (set_key_value(&data, nickname, value)) {
		goto exit_fail;
	}

	recalculate_crc(&data);

	close_fconfig_handle(&data);
	return 0;

exit_fail: 
	close_fconfig_handle(&data);
	return 1;
}

/*
 * Read mode of operation: get parameter values from the configuration. 
 */
static int read_mode(uint8_t *device, int offset, uint8_t *nickname)
{
	struct config_data data;
	if (get_fconfig_handle(&data, device, offset, O_RDONLY) == NULL) {
		MESSAGE(VERB_LOW, "Could not get a config data handle!\n");
		return 1;
	}
	if (verify_fconfig(&data)) {
		MESSAGE(VERB_LOW, "Config verification failed!\n");
		goto exit_fail;
	}

	if (get_key_value(&data, nickname)) {
		goto exit_fail;
	}

	close_fconfig_handle(&data);
	return 0;

exit_fail:
	close_fconfig_handle(&data);
	return 1;
}

/*
 * List mode of operation: list parameter values from the configuration.
 */
static int list_mode(uint8_t *device, int offset)
{
	struct config_data data;
	if (get_fconfig_handle(&data, device, offset, O_RDONLY) == NULL) {
		MESSAGE(VERB_LOW, "Could not get a config data handle!\n");
		return 1;
	}
	if (verify_fconfig(&data)) {
		MESSAGE(VERB_LOW, "Config verification failed!\n");
		goto exit_fail;
	}

	if (list_keys(&data)) {
		goto exit_fail;
	}

	close_fconfig_handle(&data);
	return 0;

exit_fail:
	close_fconfig_handle(&data);
	return 1;
}

#define MODE_NONE 0
#define MODE_WRITE 1
#define MODE_READ 2
#define MODE_LIST 3

/*
 * main(). ...nuff said.
 */
int main(int argc, char **argv)
{
	int c, ret;
	uint8_t mode = MODE_NONE;
	uint8_t *nickname = NULL;
	uint8_t *value = NULL;
	uint8_t *device = NULL;
	int offset = 0;

	while ((c = getopt(argc, argv, "hrwlvsd:n:o:x:")) != -1) {
		switch (c) {
		case 'r':
			mode = MODE_READ;
			break;
		case 'w':
			mode = MODE_WRITE;
			break;
		case 'l':
			mode = MODE_LIST;
			break;
		case 'n':
			nickname = optarg;
			break;
		case 'o':
			sscanf(optarg, "%i", &(offset));
			break;
		case 'x':
			value = optarg;
			break;
		case 'v':
			verbosity++;
			break;
		case 's':
			verbosity = 0;
			break;
		case 'd':
			device = optarg;
			break;
		case 'h':
			usage();
			break;
		case '?':
		default:
			usage();
			exit(1);
			break;
		}
	}

	MESSAGE(VERB_LOW, "Low verbosity messages are printed.\n");
	MESSAGE(VERB_NORMAL, "Normal verbosity messages are printed.\n");
	MESSAGE(VERB_HIGH, "High verbosity messages are printed.\n");

	if ((nickname == NULL) && (mode != MODE_LIST)) {
		usage();
		exit(1);
	}

	if (device == NULL) {
		MESSAGE(VERB_LOW, "You must provide a device name.\n");
		exit(1);
	}

	switch (mode) {
		case MODE_WRITE :
			ret = write_mode(device, offset, nickname, value);
			break;
		case MODE_READ :
			ret = read_mode(device, offset, nickname);
			break;
		case MODE_LIST :
			ret = list_mode(device, offset);
			break;
		default :
			MESSAGE(VERB_LOW, "Unknown mode of operation\n");
			usage();
			ret = 1;
	} //switch

	return ret;
}

