/*
 *
 *  Wireless daemon for Linux
 *
 *  Copyright (C) 2016-2019  Intel Corporation. All rights reserved.
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

enum security;
struct network_info;
struct scan_freq_set;

typedef bool (*known_networks_foreach_func_t)(const struct network_info *info,
						void *user_data);

struct known_frequency {
	uint32_t frequency;
};

int known_network_offset(const struct network_info *target);
bool known_networks_foreach(known_networks_foreach_func_t function,
				void *user_data);
bool known_networks_has_hidden(void);
struct network_info *known_networks_find(const char *ssid,
						enum security security);

const char *known_network_get_path(const struct network_info *network);

struct scan_freq_set *known_networks_get_recent_frequencies(
						uint8_t num_networks_tosearch);
int known_network_add_frequency(struct network_info *info, uint32_t frequency);
