/*
 *
 *  Wireless daemon for Linux
 *
 *  Copyright (C) 2013-2014  Intel Corporation. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <stdbool.h>

typedef void (*netdev_command_func_t) (bool result, void *user_data);

enum netdev_state {
	NETDEV_STATE_DISCONNECTED = 0,	/* Disconnected, no auto-connect */
	NETDEV_STATE_AUTOCONNECT,	/* Disconnected, try auto-connect */
	NETDEV_STATE_CONNECTING,	/* Connecting */
	NETDEV_STATE_CONNECTED,
	NETDEV_STATE_DISCONNECTING,
};

void netdev_set_linkmode_and_operstate(uint32_t ifindex,
				uint8_t linkmode, uint8_t operstate,
				netdev_command_func_t cb, void *user_data);

bool netdev_init(void);
bool netdev_exit(void);
