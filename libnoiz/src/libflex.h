/*  libflex.h - netflex protocol implementation base definitions
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

#include <stdint.h>
// #include <stdbool.h>

#define COS_ACT_WATCH 0x00
#define COS_ACT_QUIT 0x01
#define COS_ACT_NOTIFY 0x02
#define COS_ACT_REPLY 0x03
#define COS_ACT_STATUS 0x04

#define COS_ACT_SIZE 0x05
#define COS_ACT_UNSET 0xFF

#define COS_WATCH_ADD 0x00
#define COS_WATCH_REM 0x01

#define COS_QUIT_USER 0x00
#define COS_QUIT_ERROR 0x01

#define COS_NOTIFY_CREATE 0x00
#define COS_NOTIFY_DELETE 0x01
#define COS_NOTIFY_ACCESS 0x02
#define COS_NOTIFY_CLOSE 0x03
#define COS_NOTIFY_MODIFY 0x04
#define COS_NOTIFY_MOVE 0x05

#define COS_REPLY_VALID 0x00
#define COS_REPLY_BAD_SIZE 0x01
#define COS_REPLY_BAD_ACTION 0x02
#define COS_REPLY_BAD_OPTION 0x03
#define COS_REPLY_BAD_PATH 0x04
#define COS_REPLY_INVALID_DATA 0x05
#define COS_REPLY_UNSET 0xFF

#define COS_STATUS_SUCCESS 0x00
#define COS_STATUS_ERR_INIT_INOTIFY 0x01
#define COS_STATUS_ERR_ADD_WATCH 0x02
#define COS_STATUS_ERR_READ_INOTIFY 0x03

#define COS_UNSET_UNSET 0xFF

#define COS_PKT_MIN_SIZE 0x03
#define COS_PKT_MAX_SIZE 0xFF

#define COS_WATCH_DLEN 0x01
#define COS_QUIT_DLEN 0x00
#define COS_NOTIFY_DLEN 0x02
#define COS_REPLY_DLEN 0x00
#define COS_STATUS_DLEN 0x00
#define COS_UNSET_DLEN 0x00

struct cos_packet
{
    char **data;

    union
    {
        uint32_t cos;

        struct
        {
            uint8_t command;
            uint8_t option;
            uint8_t size;
            uint8_t length;
        };
    };
};

union cos_response
{
    uint16_t value;

    struct
    {
        uint8_t code;
        uint8_t length;
    };
};

void cos_packet_read(struct cos_packet *packet);

struct cos_packet *cos_packet_create(void);

void cos_packet_delete(struct cos_packet *packet);

union cos_response cos_res_init(void);

signed char cos_deserialize(unsigned char buffer[COS_PKT_MAX_SIZE], struct cos_packet *packet);

uint16_t cos_serialize(uint8_t buffer[COS_PKT_MAX_SIZE], struct cos_packet *packet);
