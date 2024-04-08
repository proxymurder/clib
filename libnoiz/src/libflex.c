/*  libflex.c - netflex protocol implementation helper functions
	Copyright (C) 2024 mdhoff

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libflex.h"

const uint8_t act_dlen_arr[5] = {
	COS_WATCH_DLEN,
	COS_QUIT_DLEN,
	COS_NOTIFY_DLEN,
	COS_REPLY_DLEN,
	COS_STATUS_DLEN,
};

const uint8_t act_opt_size_arr[5] = {
	COS_WATCH_REM,
	COS_QUIT_ERROR,
	COS_NOTIFY_MOVE,
	COS_REPLY_INVALID_DATA,
	COS_STATUS_ERR_READ_INOTIFY,
};

void cos_packet_read(struct cos_packet *packet)
{
	printf("--PACKET--\n");
	printf("head:%x\n", packet->cos);

	printf("command: %x\noption: %x\nsize: %x\ni: %x\n",
		   packet->command, packet->option, packet->size, packet->length);

	for (int i = 0; i < packet->length; i++)
		printf("%d: %s\n", i, packet->data[i]);
}

struct cos_packet *cos_packet_create(void)
{
	struct cos_packet *packet = (struct cos_packet *)malloc(sizeof(struct cos_packet));

	if (packet == NULL)
		return NULL;

	packet->command = COS_ACT_UNSET;
	packet->option = COS_UNSET_UNSET;
	packet->size = COS_UNSET_UNSET;
	packet->length = 0x00;

	packet->data = NULL;

	return packet;
}

void cos_packet_delete(struct cos_packet *packet)
{
	if (packet->data != NULL)
	{
		for (int i = 0; i < packet->length; i++)
			if (packet->data[i] != NULL)
				free(packet->data[i]);
		free(packet->data);
	}

	return;
}

union cos_response cos_res_init(void)
{
	union cos_response response;

	response.code = COS_REPLY_UNSET;
	response.length = COS_REPLY_UNSET;

	return response;
}

uint16_t cos_serialize(uint8_t buffer[COS_PKT_MAX_SIZE], struct cos_packet *packet)
{
	union cos_response response = cos_res_init();

	uint8_t act_data_len = COS_UNSET_UNSET;

	if (!(packet->command < COS_ACT_SIZE))
		response.code = COS_REPLY_BAD_ACTION;

	if (packet->option > act_opt_size_arr[packet->command])
		response.code = COS_REPLY_BAD_OPTION;

	act_data_len = act_dlen_arr[packet->command];

	if (packet->length != act_data_len)
		response.code = COS_REPLY_BAD_SIZE;

	if ((packet->data == NULL && act_data_len != 0) ||
		(packet->data != NULL && act_data_len == 0))
		response.code = COS_REPLY_INVALID_DATA;

	if (response.code != COS_REPLY_UNSET)
		return response.value;

	int size = 0;
	for (int i = 0; i < packet->length; i++)
	{
		if (packet->data[i] == NULL)
		{
			response.value = COS_REPLY_INVALID_DATA;
			return response.value;
		}

		int current = 0;
		for (int j = 0; j < COS_PKT_MAX_SIZE - COS_PKT_MIN_SIZE - size; j++)
		{
			buffer[COS_PKT_MIN_SIZE + size + j] = packet->data[i][j];

			if (packet->data[i][j] == '\0')
			{
				current = j + 1;
				break;
			}
		}

		size += current;
	}
	buffer[0] = packet->command;
	buffer[1] = packet->option;
	buffer[2] = size;

	response.code = COS_REPLY_VALID;
	response.length = COS_PKT_MIN_SIZE + size;

	return response.value;
}

signed char cos_deserialize(unsigned char buffer[COS_PKT_MAX_SIZE], struct cos_packet *packet)
{
	signed char res = -1;

	cos_packet_delete(packet);

	if (!(buffer[0] < COS_ACT_SIZE))
		return res ^= COS_REPLY_BAD_ACTION;
	else if (!(buffer[1] < act_opt_size_arr[buffer[0]]))
		return res ^= COS_REPLY_BAD_OPTION;
	else if ((!buffer[2] && !!act_dlen_arr[buffer[0]]) ||
			 (!!buffer[2] && !act_dlen_arr[buffer[0]]))
		return res ^= COS_REPLY_BAD_SIZE;

	int *chunk = NULL;

	int size = 0;
	int length = 0;

	if ((chunk = malloc(sizeof(int) * act_dlen_arr[buffer[0]])) == NULL)
		return res;

	for (int i = COS_PKT_MIN_SIZE; i < COS_PKT_MAX_SIZE; i++)
	{
		int r = length == act_dlen_arr[buffer[0]];
		if (r)
			break;
		else if ((r && !!size) || !(i < buffer[2] + COS_PKT_MIN_SIZE))
		{
			free(chunk);
			return res ^= COS_REPLY_BAD_SIZE;
		}

		size++;

		if (buffer[i] == '\0')
		{
			chunk[length++] = size;
			size = 0;
			continue;
		}
		else if (buffer[i] != ' ' && (buffer[i] < '!' || buffer[i] > '~'))
		{
			free(chunk);
			return res ^= COS_REPLY_INVALID_DATA;
		}
	}

	packet->command = buffer[0];
	packet->option = buffer[1];
	packet->size = buffer[2];
	packet->length = length;

	if ((packet->data = malloc(sizeof(char *) * packet->length)) == NULL)
		return res;

	for (int i = 0; i < packet->length; i++)
	{
		if ((packet->data[i] = malloc(sizeof(char) * chunk[i])) == NULL)
			return res;

		for (int j = 0; j < chunk[i]; j++)
			packet->data[i][j] = buffer[COS_PKT_MIN_SIZE + chunk[i] + j];
	}

	free(chunk);

	return res &= COS_REPLY_VALID ^ (COS_PKT_MIN_SIZE | packet->size);
}
