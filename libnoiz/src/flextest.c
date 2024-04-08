#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "libflex.h"

int main()
{
	uint8_t buff[COS_PKT_MAX_SIZE] = {
		COS_ACT_NOTIFY,
		COS_WATCH_REM,
		0x0C,
		'h',
		'e',
		'l',
		'l',
		'o',
		'\0',
		'h',
		'e',
		'l',
		'l',
		'o',
		'\0',
	};

	// char *testData[2] = {
	// 	"first string",
	// 	"second string",
	// };

	// uint8_t cereal[1024] = {0};

	struct cos_packet *packet = cos_packet_create();

	if (packet == NULL)
		return 1;

	int deserialize = cos_deserialize(buff, packet);
	union cos_response *deserialize_res = (union cos_response *)&deserialize;

	printf("deserialize 0x%x =>%i\n", deserialize_res->value, deserialize);

	if (deserialize_res->code != COS_REPLY_VALID)
		fprintf(stderr, "GOT %d\n", deserialize_res->code);

	cos_packet_read(packet);

	// int serialize = cos_serialize(cereal, packet);
	// union cos_response *serialize_res = (union cos_response *)&deserialize;

	// printf("serialize 0x%x =>%i\n", serialize_res->value, serialize);

	// for (int i = 0; i < serialize_res->length; i++)
	// 	printf("%x\t", cereal[i]);
	// printf("\n");

	// packet->data = testData;
	// packet->head.bytes.data_length = 2;

	// serialize(cereal, packet, result);
	// if (result->reply != COS_REPLY_VALID)
	// {
	// 	fprintf(stderr, "GOT %d\n", result->reply);
	// 	return 1;
	//
	// for (int i = 0; i < 3 + cereal[3]; i++)
	// {
	// 	printf("%x ", cereal[i]);
	// }
	// printf("\n");
	cos_packet_delete(packet);

	return 0;
}
