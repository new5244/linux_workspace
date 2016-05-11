/*
 * crunchfc.h
 *
 * $Id: crunchfc.h,v 1.3 2008/03/29 12:20:55 andrzej Exp $
 *
 * Redboot Flash Configuration parser. 
 * Configuration parsing routines - header. 
 *
 * Copyright (C) 2006 Ekiert sp z o.o.
 * Author: Andrzej Ekiert <a.ekiert@ekiert.com>
 *
 * Changes:
 *   2008/03/29 - 'offset' option added by wimpunk
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version. 
 */

#ifndef CRUNCHFC_H
#define CRUNCHFC_H

#include <stdint.h>

struct config_data {
	int fd;
	int offset;
	uint32_t maxlen;
	uint32_t reallen;
	uint8_t swab;
	uint8_t *buf;
};

int8_t verify_fconfig(struct config_data *data);
int8_t get_key_value(struct config_data *data, uint8_t *nickname);
int8_t set_key_value(struct config_data *data, uint8_t *nickname, void *value);
int8_t list_keys(struct config_data *data);
void recalculate_crc(struct config_data *data);

#endif //CRUNCHFC_H

