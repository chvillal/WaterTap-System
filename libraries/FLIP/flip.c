#include "flip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BITMASK_DST_SIZE_2	0x08
#define BITMASK_DST_SIZE_4	0x10
#define BITMASK_DST_SIZE_16	0x18
#define BITMASK_VER			0x20
#define BITMASK_SRC_SIZE_2	0x20
#define BITMASK_SRC_SIZE_4	0x40
#define BITMASK_SRC_SIZE_16 0x60
#define BITMASK_LEN			0x04
#define BITMASK_TTL			0x02
#define BITMASK_FLOW		0x01
#define BITMASK_PROTOCOL	0x10
#define BITMASK_CRC			0x08
#define BITMASK_ESP			0x40
#define BITMASK_CONT		0x80
#define BITMAP_MAX_LEN		2

#define MAX_STR_SIZE 128 //temporary, not sure what max lora string len is...

meta_header metaHdr;
flip_header flipHdr;
meta_header rcv_bitmap;
flip_header rcv_header_values;

char* FLIP_construct_bitmap (void)
{
	/* We want to send a one byte bitmap */
	if (metaHdr.continuation1 == 0){
		
		// The first continuationo bit is off, initialize a bitmap of one byte
		uint8_t bitmap = 0;
		
		// Construct the bitmap based on the meta header fields
		if (metaHdr.esp){
			bitmap = bitmap | 1 << 6;
		}
		if (metaHdr.version){
			bitmap = bitmap | 1 << 5;
		}
		if (metaHdr.destination1){
			bitmap = bitmap | 1 << 4;
		}
		if (metaHdr.destination2){
			bitmap = bitmap | 1 << 3;
		}
		if (metaHdr.length){
			bitmap = bitmap | 1 << 2;
		}
		if (metaHdr.ttl){
			bitmap = bitmap | 1 << 1;
		}
		if (metaHdr.flow){
			bitmap = bitmap | 1;
		}
		
		printf("Bitmap is: %u\n", bitmap);
		
		char *bitmap_string = (char *) malloc(sizeof(char) * (8+1));
		
		unsigned char byte1 = *((unsigned char *)&bitmap);

		sprintf(bitmap_string, "%c", byte1);
		printf("Unsigned: %u\n", byte1);
		printf("Bytes: %c\n", byte1);
	
		printf("The string is: %s\n", bitmap_string);
		
		uint8_t b1 = bitmap_string[0];
		printf("Unsigned int: %u\n", b1);
		
		return bitmap_string;
		
	/* We want to send a two byte bitmap */	
	}else if (metaHdr.continuation1){
		// The first continuationo bit is on, initialize a bitmap of two bytes
		uint16_t bitmap = 32768;
		//printf("Bitmap is: %u\n", bitmap);
		
		// Construct the bitmap based on the meta header fields
		if (metaHdr.esp){
			bitmap = bitmap | 1 << 14;
			//printf("Bitmap is: %u\n", bitmap);
		}
		if (metaHdr.version){
			bitmap = bitmap | 1 << 13;
			//printf("Bitmap is: %u\n", bitmap);
		}
		if (metaHdr.destination1){
			bitmap = bitmap | 1 << 12;
			//printf("Bitmap is: %u\n", bitmap);
		}
		if (metaHdr.destination2){
			bitmap = bitmap | 1 << 11;
			//printf("Bitmap is: %u\n", bitmap);
		}
		if (metaHdr.length){
			bitmap = bitmap | 1 << 10;
			//printf("Bitmap is: %u\n", bitmap);
		}
		if (metaHdr.ttl){
			bitmap = bitmap | 1 << 9;
			//printf("Bitmap is: %u\n", bitmap);
		}
		if (metaHdr.flow){
			bitmap = bitmap | 1 << 8;
			//printf("Bitmap is: %u\n", bitmap);
		}
		if (metaHdr.continuation2){
			bitmap = bitmap | 1 << 7;
			//printf("Bitmap is: %u\n", bitmap);
		}
		if (metaHdr.source1){
			bitmap = bitmap | 1 << 6;
			//printf("Bitmap is: %u\n", bitmap);
		}
		if (metaHdr.source2){
			bitmap = bitmap | 1 << 5;
			//printf("Bitmap is: %u\n", bitmap);
		}
		if (metaHdr.protocol){
			bitmap = bitmap | 1 << 4;
			//printf("Bitmap is: %u\n", bitmap);
		}
		if (metaHdr.checksum){
			bitmap = bitmap | 1 << 3;
			//printf("Bitmap is: %u\n", bitmap);
		}
		
		printf("Bitmap is: %u\n", bitmap);
		
		char *bitmap_string = (char *) malloc(sizeof(char) * (16+1));
		
		unsigned char byte1 = *((unsigned char *)&bitmap + 1);
		unsigned char byte2 = *((unsigned char *)&bitmap);

		sprintf(bitmap_string, "%c%c", byte1, byte2);
		printf("Unsigned: %u %u\n", byte1, byte2);
		printf("Bytes: %c %c\n", byte1, byte2);
	
		printf("The string is: {%s}\n", bitmap_string);
		
		uint16_t b1 = (uint8_t)bitmap_string[0] << 8 | (uint8_t)bitmap_string[1];
		printf("Unsigned int: %u\n", b1);
		
		return bitmap_string;
	}
}

char* FLIP_construct_header (void)
{
	char *header_string = (char *) malloc(sizeof(char) * 20);
	memset(header_string, '\0', sizeof(header_string));
	
	int index = 0;
	
	printf("\n\nConstructing header\n");
	
	if (metaHdr.version){
		char version_string[10];
		
		unsigned char byte1 = *((unsigned char *)&flipHdr.version);

		sprintf(version_string, "%c", byte1);
		printf("Unsigned: %u\n", byte1);
		printf("Bytes: %c\n", byte1);
	
		printf("The version string is: %s\n", version_string);
		
		uint8_t b1 = (uint8_t)version_string[0];
		printf("Unsigned int: %u\n\n", b1);
		
		
		mod_strncat(header_string, version_string, 1, &index);
		printf("Printing header string in hex:\n");
		for (int i = 0; i < sizeof(header_string); i ++) {
			printf(" %02x", header_string[i]);
		}
		printf("\n");

	}
	
	if (metaHdr.destination1 && !metaHdr.destination2){
		char destination_string[100];
		
		unsigned char byte1 = *((unsigned char *)&flipHdr.destination_addr + 3);
		unsigned char byte2 = *((unsigned char *)&flipHdr.destination_addr + 2);
		unsigned char byte3 = *((unsigned char *)&flipHdr.destination_addr + 1);
		unsigned char byte4 = *((unsigned char *)&flipHdr.destination_addr);

		sprintf(destination_string, "%c%c%c%c", byte1, byte2, byte3, byte4);
		printf("Unsigned: %u %u %u %u\n", byte1, byte2, byte3, byte4);
		printf("Bytes: %c %c %c %c\n", byte1, byte2, byte3, byte4);
	
		printf("The destination string is: %s\n", destination_string);
		
		uint32_t b1 = (uint8_t)destination_string[0] <<  24 | (uint8_t)destination_string[1] << 16 | (uint8_t)destination_string[2] << 8 | (uint8_t)destination_string[3];
		printf("Unsigned int: %u\n\n", b1);
		
		
		mod_strncat(header_string, destination_string, 4, &index);
		printf("Printing header string in hex:\n");
		for (int i = 0; i < sizeof(header_string); i ++) {
			printf(" %02x", header_string[i]);
		}
		printf("\n");
	}
	
	if (metaHdr.destination2 && !metaHdr.destination1){
		
		uint16_t dest_addr = (uint16_t) flipHdr.destination_addr;
		char destination_string[16];
		
		unsigned char byte1 = *((unsigned char *)&dest_addr + 1);
		unsigned char byte2 = *((unsigned char *)&dest_addr);

		sprintf(destination_string, "%c%c", byte1, byte2);
		printf("Unsigned: %u %u\n", byte1, byte2);
		printf("Bytes: %c %c\n", byte1, byte2);
	
		printf("The destination string is: %s\n", destination_string);
		
		uint16_t b1 = (uint8_t)destination_string[0] << 8 | (uint8_t)destination_string[1];
		printf("Unsigned int: %u\n\n", b1);
		
		
		mod_strncat(header_string, destination_string, 2, &index);
		printf("Printing header string in hex:\n");
		
	}
	
	if (metaHdr.length){
		char length_string[18];
		
		unsigned char byte1 = *((unsigned char *)&flipHdr.length + 1);
		unsigned char byte2 = *((unsigned char *)&flipHdr.length);

		sprintf(length_string, "%c%c", byte1, byte2);
		printf("Unsigned: %u %u\n", byte1, byte2);
		printf("Bytes: %c %c\n", byte1, byte2);
	
		printf("The length string is: %s\n\n", length_string);
		
		uint16_t b1 = (uint8_t)length_string[0] << 8 | (uint8_t)length_string[1];
		printf("Unsigned int: %u\n\n", b1);
		printf("Just the length string in hex:\n");
		for (int i = 0; i < 2; i ++) {
			printf(" %02x", length_string[i]);
		}
		printf("\n");
		
		mod_strncat(header_string, length_string, 2, &index);
		printf("Printing header string in hex:\n");
		for (int i = 0; i < sizeof(header_string); i ++) {
			printf(" %02x", header_string[i]);
		}
		printf("\n");
	}
	
	if (metaHdr.ttl){
		char ttl_string[10];
		
		unsigned char byte1 = *((unsigned char *)&flipHdr.ttl);

		sprintf(ttl_string, "%c", byte1);
		printf("Unsigned: %u\n", byte1);
		printf("Bytes: %c\n", byte1);
	
		printf("The ttl string is: %s\n", ttl_string);
		
		uint8_t b1 = (uint8_t)ttl_string[0];
		printf("Unsigned int: %u\n\n", b1);
		
		
		mod_strncat(header_string, ttl_string, 1, &index);
		for (int i = 0; i < sizeof(header_string); i ++) {
			printf(" %02x", header_string[i]);
		}
		printf("\n");
	}
	
	if (metaHdr.flow){
		char flow_string[33] = {0};
		
		unsigned char byte1 = *((unsigned char *)&flipHdr.flow + 3);
		unsigned char byte2 = *((unsigned char *)&flipHdr.flow + 2);
		unsigned char byte3 = *((unsigned char *)&flipHdr.flow + 1);
		unsigned char byte4 = *((unsigned char *)&flipHdr.flow);

		
		sprintf(flow_string, "%c%c%c%c", byte1, byte2, byte3, byte4);
		printf("Unsigned: %u %u %u %u\n", byte1, byte2, byte3, byte4);
		printf("Bytes: %c %c %c %c\n", byte1, byte2, byte3, byte4);
		
		printf("The flow string is: %s\n", flow_string);
		
		uint32_t b1 = (uint8_t)flow_string[0] <<  24 | (uint8_t)flow_string[1] << 16 | (uint8_t)flow_string[2] << 8 | (uint8_t)flow_string[3];
		printf("Unsigned int: %u\n\n", b1);
		
		mod_strncat(header_string, flow_string, 4, &index);
		for (int i = 0; i < sizeof(header_string); i ++) {
			printf(" %02x", header_string[i]);
		}
		printf("\n");
	}
	
	if (metaHdr.source1){
		char source_string[33];
		
		unsigned char byte1 = *((unsigned char *)&flipHdr.source_addr + 3);
		unsigned char byte2 = *((unsigned char *)&flipHdr.source_addr + 2);
		unsigned char byte3 = *((unsigned char *)&flipHdr.source_addr + 1);
		unsigned char byte4 = *((unsigned char *)&flipHdr.source_addr);

		sprintf(source_string, "%c%c%c%c", byte1, byte2, byte3, byte4);
		printf("Unsigned: %u %u %u %u\n", byte1, byte2, byte3, byte4);
		printf("Bytes: %c %c %c %c\n", byte1, byte2, byte3, byte4);
	
		printf("The source string is: %s\n", source_string);
		
		uint32_t b1 = (uint8_t)source_string[0] <<  24 | (uint8_t)source_string[1] << 16 | (uint8_t)source_string[2] << 8 | (uint8_t)source_string[3];
		printf("Unsigned int: %u\n\n", b1);
		
		mod_strncat(header_string, source_string, 4, &index);
		for (int i = 0; i < sizeof(header_string); i ++) {
			printf(" %02x", header_string[i]);
		}
		printf("\n");
	}
	
	if (metaHdr.source2){
		char source_string[18];
		
		unsigned char byte1 = *((unsigned char *)&flipHdr.source_addr + 1);
		unsigned char byte2 = *((unsigned char *)&flipHdr.source_addr);

		sprintf(source_string, "%c%c", byte1, byte2);
		printf("Unsigned: %u %u\n", byte1, byte2);
		printf("Bytes: %c %c\n", byte1, byte2);
	
		printf("The source string is: %s\n", source_string);
		
		uint16_t b1 = (uint8_t)source_string[0] << 8 | (uint8_t)source_string[1];
		printf("Unsigned int: %u\n\n", b1);
		
		mod_strncat(header_string, source_string, 2, &index);
		for (int i = 0; i < sizeof(header_string); i ++) {
			printf(" %02x", header_string[i]);
		}
		printf("\n");
	}
	
	if (metaHdr.protocol){
		char protocol_string[10];
		
		unsigned char byte1 = *((unsigned char *)&flipHdr.protocol);

		sprintf(protocol_string, "%c", byte1);
		printf("Unsigned: %u\n", byte1);
		printf("Bytes: %c\n", byte1);
	
		printf("The protocol string is: %s\n", protocol_string);
		
		uint8_t b1 = (uint8_t)protocol_string[0];
		printf("Unsigned int: %u\n\n", b1);
		
		mod_strncat(header_string, protocol_string, 1, &index);
		for (int i = 0; i < sizeof(header_string); i ++) {
			printf(" %02x", header_string[i]);
		}
		printf("\n");
	}
	
	if (metaHdr.checksum){
		char checksum_string[18];
		
		unsigned char byte1 = *((unsigned char *)&flipHdr.checksum + 1);
		unsigned char byte2 = *((unsigned char *)&flipHdr.checksum);

		sprintf(checksum_string, "%c%c", byte1, byte2);
		printf("Unsigned: %u %u\n", byte1, byte2);
		printf("Bytes: %c %c\n", byte1, byte2);
	
		printf("The checksum string is: %s\n", checksum_string);
		
		uint16_t b1 = (uint8_t)checksum_string[0] << 8 | (uint8_t)checksum_string[1];
		printf("Unsigned int: %u\n\n", b1);
		
		mod_strncat(header_string, checksum_string, 2, &index);
		for (int i = 0; i < sizeof(header_string); i ++) {
			printf(" %02x", header_string[i]);
		}
		printf("\n");
	}
	
	printf("\n");
	printf("Header: %s\n", header_string);
	for (int i = 0; i < sizeof(header_string); i ++) {
		printf(" %02x", header_string[i]);
	}
	printf("\n");
	
	
	flipHdr.header_len = index;
	
	
	return header_string;
}

char* FLIP_construct_packet (char *bitmap, char *header, char *payload)
{
	char *packet_string = (char *) malloc(sizeof(char) * 500);
	
	memset(packet_string, '\0', sizeof(packet_string));
	
	int index = 0;
	
	mod_strncat(packet_string, bitmap, strlen(bitmap), &index);
	mod_strncat(packet_string, header, flipHdr.header_len, &index);
	mod_strncat(packet_string, payload, strlen(payload), &index);
	
	printf("Packet is: %s\n", packet_string);
	printf("Index is: %d\n", index);
	
	return packet_string;
}


int setsockopt(int optname, uint32_t optval, int optlen)
{
	int ret;
	
	switch (optname) {

		case FLIPO_ESP:
			printf("Not implemented yet\n");
			break; 
		
		case FLIPO_VERSION:
			ret = add_version (optval);
			break; 
		
		case FLIPO_DESTINATION:
			ret = add_destination (optval, optlen);
			break;
		
		case FLIPO_LENGTH:
			ret = add_length (optval);
			break;
		
		case FLIPO_TTL:
			ret = add_ttl (optval);
			break; 
		
		case FLIPO_FLOW:
			ret = add_flow (optval);
			break; 
		
		case FLIPO_SOURCE:
			ret = add_source (optval, optlen);
			break;
		
		case FLIPO_PROTOCOL:
			ret = add_protocol (optval);
			break; 
		
		case FLIPO_CHECKSUM:
			ret = add_checksum (optval);
			break; 
			
	  
		default:
			printf("Not a valid option\n");
			return -1;
	}
	return ret;
}



int add_destination(uint32_t optval, int optlen)
{
	if (optlen == 16){
		printf("Destination field should be two bytes, it is %d bits", optlen);
		metaHdr.destination1 = 0;
		metaHdr.destination2 = 1;
	}else if (optlen == 32){
		printf("Destination field should be four bytes, it is %d bits", optlen);
		metaHdr.destination1 = 1;
		metaHdr.destination2 = 0;
	}else if (optlen == 64){
		printf("Destination field should be sixteen bytes, it is %d bits", optlen);
		metaHdr.destination1 = 1;
		metaHdr.destination2 = 1;
	}else{
		printf("Not a valid destination address length\n");
		return -1;
	}
		
	flipHdr.destination_addr = optval;
	printf("Destination address is: %u\n", flipHdr.destination_addr);
	
	return 0;
}

int add_length (uint32_t optval)
{
	flipHdr.length = (uint16_t) optval;
	printf("length is %u\n", flipHdr.length);
	
	return 0;
}

int add_ttl (uint32_t optval)
{
	flipHdr.ttl =  (uint8_t) optval;
	printf("Time to live is %u\n", flipHdr.ttl);
	
	return 0;
}

int add_protocol (uint32_t optval)
{
	flipHdr.protocol = (uint8_t) optval;
	printf("Protocol is %u\n", flipHdr.protocol);
	
	return 0;
}

int add_checksum (uint32_t optval)
{
	flipHdr.checksum = (uint16_t) optval;
	printf("Checksum is %u\n", flipHdr.checksum);
	
	return 0;
}

int add_version (uint32_t optval)
{
	flipHdr.version = (uint8_t) optval;
	printf("Version is %u\n", flipHdr.version);
	
	return 0;
}

int add_flow (uint32_t optval)
{
	flipHdr.flow = optval;
	printf("Flow is %u\n", flipHdr.flow);
	
	return 0;
}

int add_source(uint32_t optval, int optlen)
{
	if (optlen == 16){
		printf("Source field should be two bytes, it is %d bits", optlen);
		metaHdr.source1 = 0;
		metaHdr.source2 = 1;
	}else if (optlen == 32){
		printf("Source field should be four bytes, it is %d bits", optlen);
		metaHdr.source1 = 1;
		metaHdr.source2 = 0;
	}else if (optlen == 64){
		printf("Source field should be sixteen bytes, it is %d bits", optlen);
		metaHdr.source1 = 1;
		metaHdr.source2 = 1;
	}else{
		printf("Not a valid source address length\n");
		return -1;
	}
		
	flipHdr.source_addr = optval;
	printf("Source address is: %u\n", flipHdr.source_addr);
	
	return 0;
}

uint32_t test_function(uint32_t num)
{
	char str[10];
	
	unsigned char byte1 = *((unsigned char *)&num + 3);
	unsigned char byte2 = *((unsigned char *)&num + 2);
	unsigned char byte3 = *((unsigned char *)&num + 1);
	unsigned char byte4 = *((unsigned char *)&num);

	
	sprintf(str, "%c%c%c%c", byte1, byte2, byte3, byte4);
	
	uint32_t b1 = str[0] <<  24 | str[1] << 16 | str[2] << 8 | str[3];
	
	return b1;
}

/** GET SOCKET OPTIONS - RECEIVING END FUNCTIONS **/

/*
 * Receives raw string BUFF from radio. reads FLIP bitmap and then parses 
 * corresponding values. Saves ptr where payload starts in string.
 *
 * returns 0 if parsed successfully, 1 if esp packet,
 * or -1 if error.
 *
 */
int flip_read_packet(char *buff, int buff_len, char *payload ){
	int i = 0;
	int dst_size = 0;
	int src_size = 0;	
	
	//reset bitmap and header values in buffer struct
	reset_bitmap(&rcv_bitmap);
	reset_header(&rcv_header_values);
	
	//start reading bitmap
	if (read_bitmap(buff, &i, &dst_size, &src_size) > 0)
		return 1; // ESP packet, done parsing.
	
	//start reading header values
	read_header_values(buff, &i, dst_size, src_size);
	
	//store starting address of they payload
	payload = buff + i;
	
	return 0; // no errors.
}


/*
 * reads very first bytes of a passed string. translates bitmap into
 * corresponding boolean flags. 
 * returns 1 if esp packet, 0 if no errors, -1 on error
 */
int read_bitmap(char *buff, int *i, int *dst_size, int *src_size) {
		
	uint8_t bitmap = (uint8_t) buff[*i];
	
	if (bitmap & BITMASK_ESP){
		rcv_bitmap.esp = true;
		// ESP detected, notify caller
		return 1; 
	}
	
	if (bitmap & BITMASK_VER){
		rcv_bitmap.version = true;
	}
	
	if (bitmap & BITMASK_DST_SIZE_16){		
			
		if ((bitmap & BITMASK_DST_SIZE_2) == BITMASK_DST_SIZE_2){
			*dst_size = 2;
			rcv_bitmap.destination2 = true;	
		}
		if ((bitmap & BITMASK_DST_SIZE_4) == BITMASK_DST_SIZE_4){
			*dst_size = 4;
			rcv_bitmap.destination1 = true;
		}
		if ((bitmap & BITMASK_DST_SIZE_16) == BITMASK_DST_SIZE_16){
			*dst_size = 16;
			rcv_bitmap.destination1 = true;
			rcv_bitmap.destination2 = true;
		}
	}
	
	if (bitmap & BITMASK_LEN){
		rcv_bitmap.length = true;
	}
	
	if (bitmap & BITMASK_TTL){
		rcv_bitmap.ttl = true;
	}
	
	if (bitmap & BITMASK_FLOW){
		rcv_bitmap.flow = true;
	}
	
	if (bitmap & BITMASK_CONT){
		rcv_bitmap.continuation1 = true;
		*i += 1;
		bitmap = (uint8_t) buff[*i];
		
		if (bitmap & BITMASK_SRC_SIZE_16){

			if ((bitmap & BITMASK_SRC_SIZE_2) == BITMASK_SRC_SIZE_2){
				*src_size = 2;
				rcv_bitmap.source2 = true;
			}
			if ((bitmap & BITMASK_SRC_SIZE_4) == BITMASK_SRC_SIZE_4){
				*src_size = 4;
				rcv_bitmap.source1 = true;
			}
			if ((bitmap & BITMASK_SRC_SIZE_16) == BITMASK_SRC_SIZE_16){
				*src_size = 16;
				rcv_bitmap.source1 = true;
				rcv_bitmap.source2 = true;
			}
		}
		
		if (bitmap & BITMASK_PROTOCOL){
			rcv_bitmap.protocol = true;
		}
		
		if (bitmap & BITMASK_CRC){
			rcv_bitmap.checksum = true;
		}
	}
	
	// move index to point to next byte
	*i += 1;
	
	return 0;
}


/*
 * reads header values flagged by rcv_bitmap struct in sequential oder,
 * stores values in rcv_header_values struct. 
 *
 * index i should point to the first byte of header values.
 *
 * returns 0 if successful, or -1 if error
 */
int read_header_values(char *buff, int *i, int dst_size, int src_size){
	int j = *i;
	
	if ( rcv_bitmap.version){
		//1 byte long
		rcv_header_values.version = (uint8_t) buff[j];
		printf("index: %d, value: %u\n", j, rcv_header_values.version);
		j++;
		
	}
	
	if ( rcv_bitmap.destination1 || rcv_bitmap.destination2){		
		//2-16 bytes long
		if ( dst_size == 2) {
			rcv_header_values.destination_addr = (uint8_t)buff[j] << 8 | (uint8_t)buff[j+1] ;
			j +=2;
		} 
		else if ( dst_size == 4) {
			rcv_header_values.destination_addr = (uint8_t)buff[j] << 24 | (uint8_t)buff[j+1] << 16 | (uint8_t)buff[j+2] << 8 | (uint8_t)buff[j+3] ;
			printf("index: %d, value: %u\n", j, rcv_header_values.destination_addr);
			j +=4;
		}
		else if ( dst_size == 16) {
		//	rcv_header_values.destination_addr = buff[j] << 24 | buff[j+1] << 16 | buff[j+2] << 8 | buff[j+3]
		//										 buff[j+4] << 24 | buff[j+5] << 16 | buff[j+6] << 8 | buff[j+7]
		//										 buff[j+8] << 24 | buff[j+9] << 16 | buff[j+10] << 8 | buff[j+11]
		//										 buff[j+12] << 24 | buff[j+13] << 16 | buff[j+14] << 8 | buff[j+15];
			j +=16;
		}
		
		
	}
	
	if ( rcv_bitmap.length){
		// 2 bytes long
		rcv_header_values.length = (uint8_t)buff[j] << 8 | (uint8_t)buff[j+1];
		printf("index: %d, value: %u\n", j, rcv_header_values.length);
		j +=2;
		
	}
	
	if ( rcv_bitmap.ttl){
		//1 byte long
		rcv_header_values.ttl = (uint8_t)buff[j];
		printf("index: %d, value: %u\n", j, rcv_header_values.ttl);
		j++;
		
	}
	
	if ( rcv_bitmap.flow){ 
		// 4 bytes long
		rcv_header_values.flow = (uint8_t)buff[j] << 24 | (uint8_t)buff[j+1] <<16 | (uint8_t)buff[j+2] << 8 | (uint8_t)buff[j+3] ;
		printf("index: %d, value: %u\n", j, rcv_header_values.flow);
		j +=4;
		
	}
	
	if ( rcv_bitmap.source1 || rcv_bitmap.source2){ 
		// 2-16 bytes long
		if ( src_size == 2) {
			rcv_header_values.source_addr = (uint8_t)buff[j] << 8 | (uint8_t)buff[j+1] ;
			j +=2;
		}
		else if ( src_size == 4) {
			rcv_header_values.source_addr = (uint8_t)buff[j] << 24 | (uint8_t)buff[j+1] << 16 | (uint8_t)buff[j+2] << 8 | (uint8_t)buff[j+3] ;
			printf("index: %d, value: %u\n", j, rcv_header_values.source_addr);
			j +=4;
		}
		else if ( src_size == 16) {
			//	rcv_header_values.source_addr = buff[j] << 24 | buff[j+1] << 16 | buff[j+2] << 8 | buff[j+3]
			//										 buff[j+4] << 24 | buff[j+5] << 16 | buff[j+6] << 8 | buff[j+7]
			//										 buff[j+8] << 24 | buff[j+9] << 16 | buff[j+10] << 8 | buff[j+11]
			//										 buff[j+12] << 24 | buff[j+13] << 16 | buff[j+14] << 8 | buff[j+15];
			j +=16;
		}
		
	}
	
	if ( rcv_bitmap.protocol){
		// 1 byte long
		rcv_header_values.protocol = (uint8_t)buff[j];
		
		printf("index: %d, value: %u\n", j, rcv_header_values.protocol);
		j++;
	}
	
	if ( rcv_bitmap.checksum){
		// 2 bytes long
		rcv_header_values.checksum =  (uint8_t)buff[j] << 8 | (uint8_t)buff[j+1];
		
		printf("index: %d, value: %u\n", j, rcv_header_values.checksum);
		j +=2;
	}
	
	*i = j;
	return 0;
}

/*
 * Resets all flags of BITMAP to false.
 */
void reset_bitmap(meta_header *bitmap){
	
	bitmap->checksum = false;
	bitmap->destination1 = false;
	bitmap->destination2 = false;
	bitmap->esp = false;
	bitmap->flow = false;
	bitmap->length = false;
	bitmap->protocol = false;
	bitmap->source1 = false;
	bitmap->source2 = false;
	bitmap->ttl = false;
	bitmap->version = false;
}

/*
 * Resets all values of the header HEADER_VALUES to 0.
 */
void reset_header(flip_header *header_values){
	
	header_values->checksum = 0;
	header_values->destination_addr = 0;
	header_values->flow = 0;
	header_values->length = 0;
	header_values->protocol = 0;
	header_values->source_addr = 0;
	header_values->ttl = 0;
	header_values->version = 0;
}


int get_bitmap_str(char *packet, char *str, int str_len){
	uint8_t bitmap;
	int i,j;
	
	memset( str, '\0', str_len);
	
	//needs at least 16 bytes + 1 (null)
	if (str_len < 17)
		return -1;
	
	//iterated for number of bytes in bitmap (1-2)	
	for (i=0; i< BITMAP_MAX_LEN; i++){
		bitmap = (uint8_t) packet[i];
		//printf("Inside bitmapRead: %u\n", bitmap);
		
		//iterate 8 times, store bit value in string
		for (j=0; j< 8; j++){
			//strcat(str, " ");
			
			if (( bitmap & (1<< 7-j)) > 0)
				strcat(str, "1");				
			else
				strcat(str, "0");
		}
		
		//if no more continuation bytes, done, exit
		if (( bitmap & (1<<7)) == 0)
			break;
	}
	
	return 0;
}

//int get_headervals_str(char *str, int str_len){
	//char vals[64];
	//
	//// 109 in max val characters + 5*8 definitions + 1 null
	//if (str_len < 200 )
	//return -1;
	//
	//if (rcv_header_values.version > 0 ){
		//sprintf(vals, "VER: %u ", rcv_header_values.version)
		//strcat(str, vals);
	//}
	//
//}

void read_rcv_values(void){
		
	printf("\n** PRINTING RCV VALUES STRUCT **\n");
	printf("VER: %u\n", rcv_header_values.version);
	printf("LEN: %u\n", rcv_header_values.length);
	printf("TTL: %u\n", rcv_header_values.ttl);
	printf("FLW: %u\n", rcv_header_values.flow);
	printf("SRC: %u\n", rcv_header_values.source_addr);
	printf("DST: %u\n", rcv_header_values.destination_addr);
	printf("PRO: %u\n", rcv_header_values.protocol);
	printf("CRC: %u\n\n", rcv_header_values.checksum);

}


void read_rcv_flags(void){
	
	printf("\n** PRINTING RCV FLAGS **\n");
	printf("CONT1: %d\n", rcv_bitmap.continuation1);
	printf("ESP: %d\n", rcv_bitmap.esp);
	printf("VER: %d\n", rcv_bitmap.version);
	printf("DEST1: %d\n", rcv_bitmap.destination1);
	printf("DEST2: %d\n", rcv_bitmap.destination2);
	printf("LEN: %d\n", rcv_bitmap.length);
	printf("TTL: %d\n", rcv_bitmap.ttl);
	printf("FLOW: %d\n", rcv_bitmap.flow);
	printf("CONT2: %d\n", rcv_bitmap.continuation2);
	printf("SRC1: %d\n", rcv_bitmap.source1);
	printf("SRC2: %d\n", rcv_bitmap.source2);
	printf("PROTO: %d\n", rcv_bitmap.protocol);
	printf("CRC: %d\n\n", rcv_bitmap.checksum);

}

char* mod_strncat(char *dest, const char *src, size_t n, int *index)
{
    //size_t dest_len = strlen(dest);
    size_t i;

	for (i = 0; i < n; i++){
        dest[*index + i] = src[i];
	}
   dest[*index + i] = '\0';

   *index += n;
   return dest;
}