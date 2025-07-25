#include "Screen.h"

#if DEBUG
#define FIRST_SCREEN_DURATION 3000
#define SECOND_SCREEN_DURATION 3000
#else
#define FIRST_SCREEN_DURATION 10000
#define SECOND_SCREEN_DURATION 3000
#endif
#define TEXT_SCREEN_DURATION 5000

#pragma region "Background images"

const unsigned char BOOT_PAGE_IMAGE[] PROGMEM =
	{
		0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xff, 0xf8, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x07, 0x00, 0x07, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x18, 0x00, 0x00, 0xc4, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x60, 0x00, 0x00, 0x20, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x80, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00, 0x08, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x08, 0x00, 0x00, 0x00, 0x01, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x10, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x10, 0x00, 0x78, 0x00, 0x00, 0x80, 0x7f, 0x00, 0x00, 0xf4, 0x1e, 0x8f, 0x8f, 0x87, 0x47, 0x40,
		0x20, 0x01, 0x86, 0x00, 0x00, 0x83, 0x80, 0x80, 0x01, 0x8c, 0x31, 0x84, 0xc4, 0xcc, 0xcc, 0xc0,
		0x20, 0x02, 0x3f, 0x00, 0x00, 0x4c, 0x00, 0x40, 0x01, 0x04, 0x20, 0x84, 0x44, 0x48, 0x48, 0x40,
		0x40, 0x04, 0xff, 0x80, 0x00, 0x70, 0x00, 0x20, 0x02, 0x04, 0x40, 0x84, 0x44, 0x4c, 0x0c, 0x00,
		0x40, 0x04, 0xe7, 0x80, 0x00, 0x40, 0x00, 0x20, 0x02, 0x00, 0x40, 0x04, 0xc4, 0xc7, 0x07, 0x00,
		0x40, 0x09, 0xe3, 0xc0, 0x01, 0x80, 0x07, 0xa0, 0x02, 0x1e, 0x43, 0xc7, 0x87, 0x81, 0x81, 0x80,
		0x40, 0x09, 0xe3, 0xc0, 0x06, 0x00, 0x1f, 0xe0, 0x02, 0x04, 0x40, 0x84, 0x04, 0x00, 0x40, 0x40,
		0x40, 0x09, 0xff, 0xc0, 0x18, 0x00, 0x3f, 0xc0, 0x03, 0x04, 0x60, 0x84, 0x04, 0x08, 0x48, 0x40,
		0x00, 0x09, 0xff, 0xc0, 0x20, 0x00, 0x7f, 0xc0, 0x01, 0x8c, 0x31, 0x84, 0x04, 0x0c, 0xcc, 0xc0,
		0x00, 0x2c, 0xff, 0x80, 0x00, 0x00, 0xff, 0x80, 0x00, 0xf8, 0x1f, 0x0e, 0x0e, 0x0b, 0x8b, 0x80,
		0x00, 0x54, 0xff, 0x80, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0xaa, 0x3f, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x55, 0xff, 0x00, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0xaa, 0xc0, 0xc0, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x55, 0x00, 0x20, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00,
		0x00, 0xaa, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00,
		0x00, 0x54, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x04, 0x90, 0x00, 0x00, 0x00,
		0x00, 0x28, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x09, 0x48, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x07, 0x8f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x09, 0x48, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x04, 0x00, 0x1e, 0x03, 0x5f, 0x80, 0x00, 0x00, 0x00, 0x08, 0x88, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x02, 0x00, 0x7c, 0x00, 0xea, 0xe0, 0x00, 0x00, 0x00, 0x04, 0x10, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x01, 0x83, 0xfc, 0x00, 0x35, 0x70, 0x00, 0x00, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x0a, 0xf0, 0x00, 0x00, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x0d, 0x58, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x10, 0x84, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x10, 0x84, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x4e, 0x39, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x71, 0xc7, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x01, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x02, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x01, 0x7f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0xbf, 0xe8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x55, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x2a, 0xac, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x15, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x0a, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x05, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char WAITING_PAGE_IMAGE[] PROGMEM =
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x40, 0x10, 0x81, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x10,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x40, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0c,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x4c, 0x31, 0xe3, 0x16, 0x38, 0x00, 0x00, 0x08, 0x00, 0x03,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x42, 0x10, 0x81, 0x1a, 0x48, 0x00, 0x00, 0x08, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x4e, 0x10, 0x81, 0x12, 0x48, 0x00, 0x00, 0x10, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xd2, 0x10, 0x81, 0x12, 0x48, 0x00, 0x00, 0x10, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x92, 0x10, 0x81, 0x12, 0x78, 0x00, 0x00, 0x1a, 0x80, 0x3c,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x8e, 0x38, 0x63, 0x92, 0x08, 0x00, 0x01, 0xf5, 0x50, 0x3b,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x06, 0x0f, 0xe8, 0x1f,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x08, 0x00, 0x3c, 0x0f,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x07, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x27, 0x80, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x28, 0x70, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x30, 0x0c, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x0c, 0x31, 0x65, 0x8c, 0x31, 0xe3, 0x0c, 0x58, 0x10, 0x03, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x12, 0x49, 0xa6, 0x92, 0x48, 0x81, 0x12, 0x68, 0x08, 0x00, 0xc0, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x10, 0x49, 0x24, 0x9e, 0x40, 0x81, 0x12, 0x48, 0x04, 0x00, 0x38, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x10, 0x49, 0x24, 0x90, 0x40, 0x81, 0x12, 0x48, 0x02, 0x00, 0x07, 0xc2,
		0x00, 0x00, 0x00, 0x00, 0x10, 0x49, 0x24, 0x90, 0x40, 0x81, 0x12, 0x48, 0x01, 0x80, 0x00, 0x1c,
		0x00, 0x00, 0x00, 0x00, 0x0e, 0x31, 0x24, 0x8e, 0x38, 0x63, 0x8c, 0x48, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x80, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x54, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xaa, 0x80,
		0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfd, 0x4c,
		0x00, 0x07, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xe0,
		0x00, 0x08, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xc0,
		0x00, 0x30, 0x00, 0x00, 0x00, 0x80, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3a, 0xff, 0x00,
		0x00, 0xc0, 0x00, 0x00, 0x00, 0x40, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xd5, 0x7a, 0x00,
		0x03, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x21, 0xe0, 0x00, 0x00, 0x00, 0x03, 0x2a, 0x14, 0x00,
		0x0c, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x31, 0xb0, 0x00, 0x00, 0x00, 0x7c, 0x01, 0x40, 0x00,
		0x30, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x19, 0x98, 0x00, 0x00, 0x1f, 0x80, 0x02, 0x08, 0x00,
		0xc0, 0x00, 0x00, 0x00, 0x04, 0x01, 0x00, 0x0d, 0x8c, 0x00, 0x0f, 0xe0, 0x00, 0x01, 0x00, 0x00,
		0x00, 0x07, 0xe0, 0x02, 0x8f, 0x80, 0x80, 0x07, 0x98, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x2a, 0xf8, 0x01, 0x3f, 0xe0, 0x40, 0x03, 0xb0, 0x80, 0x00, 0x00, 0x00, 0x04, 0x40, 0x00,
		0x01, 0x51, 0xdc, 0x00, 0xff, 0xf8, 0x20, 0x01, 0xe0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x22, 0xbe, 0x00, 0xff, 0xfc, 0x10, 0x01, 0xc0, 0x40, 0x00, 0x00, 0x00, 0x05, 0x10, 0x00,
		0x00, 0x45, 0x7e, 0x00, 0x4f, 0xff, 0x08, 0x01, 0xe0, 0x20, 0x18, 0x00, 0x00, 0x02, 0x00, 0x00,
		0x00, 0x0a, 0xe1, 0x00, 0x3f, 0xfc, 0x86, 0x03, 0xb0, 0x18, 0x70, 0x00, 0x00, 0x05, 0x40, 0x00,
		0x00, 0x55, 0x80, 0x80, 0x0f, 0xfc, 0x61, 0x87, 0x98, 0x04, 0x20, 0x00, 0x00, 0x02, 0x80, 0x00,
		0x02, 0xab, 0x00, 0x40, 0x03, 0x7e, 0x10, 0x8d, 0x8c, 0x02, 0x40, 0x30, 0x00, 0x05, 0x50, 0x00,
		0x00, 0x56, 0x00, 0x3e, 0x01, 0x3f, 0x0f, 0x19, 0x98, 0x02, 0x41, 0xfa, 0xaa, 0xaa, 0x80, 0x00,
		0x0a, 0xac, 0x00, 0x01, 0x80, 0x9f, 0x00, 0x31, 0xb0, 0x01, 0x3f, 0x7d, 0x55, 0x55, 0x50, 0x00,
		0x00, 0x58, 0x00, 0x00, 0x60, 0x5f, 0x00, 0x21, 0xe0, 0x01, 0x0a, 0xff, 0xfa, 0xaf, 0xa0, 0x00,
		0x2a, 0xb0, 0x00, 0x00, 0x18, 0x4f, 0x80, 0x01, 0xc0, 0x00, 0x87, 0x80, 0x7f, 0xff, 0x54, 0x00,
		0x01, 0x60, 0x00, 0x00, 0x1e, 0x4f, 0x80, 0x01, 0x80, 0x00, 0x7c, 0x00, 0x00, 0x03, 0xa0, 0x00,
		0xaa, 0xc0, 0x00, 0x00, 0x1d, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xd4, 0x00,
		0x05, 0x80, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x00,
		0xab, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd4, 0x00,
		0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x00,
		0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x00,
		0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0x00,
		0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x40,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xa0,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd0,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68};

const unsigned char FIRST_PAGE_IMAGE[] PROGMEM =
	{
		0xf9, 0xff, 0xff, 0xc3, 0x0f, 0xff, 0xcf, 0xff, 0xff, 0x0c, 0x3f, 0xff, 0xff, 0xc3, 0xfc, 0xdf,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0xf3, 0xfd, 0xff, 0xc7, 0xff, 0xff, 0xf8, 0x61, 0xff, 0xff, 0xe7, 0xff, 0xe1, 0x87, 0xff, 0xbf};

const unsigned char SECOND_PAGE_IMAGE[] PROGMEM =
	{
		0xf9, 0xff, 0xff, 0xc3, 0x0f, 0xff, 0xcf, 0xff, 0xff, 0x0c, 0x3f, 0xff, 0xff, 0xc3, 0xfc, 0xdf,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x29, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x41, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x61, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x81, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x81, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0xe1, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x80, 0x80, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x80, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x60, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x40, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x00, 0x29, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x19, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x81, 0x80, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x81, 0x40, 0xac, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x81, 0x31, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x81, 0x0e, 0x20, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x81, 0x04, 0x20, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x81, 0x04, 0x20, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x81, 0x74, 0x20, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x81, 0x34, 0x20, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x81, 0x54, 0x20, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x81, 0x84, 0x20, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x81, 0x04, 0x20, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x81, 0x84, 0x50, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x44, 0x8c, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x35, 0x02, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x0e, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x80, 0x14, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x17, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x0b, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x05, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x00, 0x02, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x02, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x04, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x09, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x13, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x17, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x80, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0xf3, 0xfd, 0xff, 0xc7, 0xff, 0xff, 0xf8, 0x61, 0xff, 0xff, 0xe7, 0xff, 0xe1, 0x87, 0xff, 0xbf};

#pragma endregion

/// @brief Constructor of the class.
Display::Display()
	: ChangeScreen(false),
	  _justDrawn(false),
	  _display(U8G2_R0, U8X8_PIN_NONE),
	  _previousMillis(0),
	  _currentScreen(EMPTY_SCREEN) {}

/// @brief Initializes display module showing boot screen.
void Display::Initialize()
{
	if (_display.begin())
	{
		_display.enableUTF8Print();

		// Display boot screen then wait 3 seconds
		this->DisplayScreen(BOOT_PAGE_SCREEN, BOOT_PAGE_IMAGE, true);
	}
	else
	{
#if DEBUG
		Serial.println(F("Display initializing... FAILED!"));
#endif
		// Stop program flow
		while (true)
		{
		}
	}
};

/// @brief Displayes a screen.
/// @param screenToDisplay Screen to be displayed.
/// @param imageToDisplay Background image to be displayed.
/// @param refreshDisplay [OPTIONAL] True to refresh display content immediatly.
void Display::DisplayScreen(Screens screenToDisplay, const unsigned char *imageToDisplay, bool refreshDisplay)
{
	if (_currentScreen != screenToDisplay)
	{
		// Clear display
		_display.clearBuffer();
		_display.drawBitmap(0, 0, 128 / 8, 64, imageToDisplay);

		// Set to true in order to avoid check about same data
		_justDrawn = true;

		// Send image to display if set
		if (refreshDisplay)
		{
			_display.sendBuffer();
		}

		_currentScreen = screenToDisplay;
	}
}

/// @brief Displayes waiting for connection screen.
void Display::DisplayWaitingScreen()
{
	// Display waiting for connection screen
	this->DisplayScreen(WAITING_PAGE_SCREEN, WAITING_PAGE_IMAGE, true);
}

/// @brief Handles timer to change screen to be displayed.
void Display::CheckScreenToDisplay()
{
	// Get current millis
	unsigned long currentMillis = millis();

	switch (_currentScreen)
	{
	case FIRST_PAGE_SCREEN:
		if (currentMillis - _previousMillis >= FIRST_SCREEN_DURATION)
		{
			// Change to second screen
			this->DisplayScreen(SECOND_PAGE_SCREEN, SECOND_PAGE_IMAGE);
			_previousMillis = currentMillis;
		}
		break;

	case SECOND_PAGE_SCREEN:
		if (currentMillis - _previousMillis >= SECOND_SCREEN_DURATION)
		{
			// Change to first screen
			this->DisplayScreen(FIRST_PAGE_SCREEN, FIRST_PAGE_IMAGE);
			_previousMillis = currentMillis;
		}
		break;

	case TEXT_PAGE_SCREEN:
		if (currentMillis - _previousMillis >= TEXT_SCREEN_DURATION)
		{
			// Change to first screen
			this->DisplayScreen(FIRST_PAGE_SCREEN, FIRST_PAGE_IMAGE);
			_previousMillis = currentMillis;
		}
		break;

	case WAITING_PAGE_SCREEN:
		if (this->ChangeScreen)
		{
			this->ChangeScreen = false;

			// Change to first screen
			this->DisplayScreen(FIRST_PAGE_SCREEN, FIRST_PAGE_IMAGE);
			_previousMillis = currentMillis;
		}
		break;

	default:
		// Should never happen
		break;
	}
}

/// @brief Writes data of first screen to display.
void Display::WriteFirstScreen()
{
	// If screen is just drawn avoid to check about data otherwise screen could be blank
	if (!_justDrawn)
	{
		// Check if data to display are changed
		if (_currentNavigationData.nextDirection == _previousNavigationData.nextDirection || _currentNavigationData.nextDirectionDistance == _previousNavigationData.nextDirectionDistance)
		{
			return;
		}
	}

	_display.setFont(u8g2_font_6x10_tr);

	if (_currentNavigationData.nextDirection.length() > 0)
	{
		uint16_t textWidth = _display.getStrWidth(_currentNavigationData.nextDirection.c_str());

		if (textWidth >= 128)
		{
			int bestSplit = -1;
			int center = _currentNavigationData.nextDirection.length() / 2;
			int closestDistance = _currentNavigationData.nextDirection.length();

			for (int i = 1; i < _currentNavigationData.nextDirection.length() - 1; ++i)
			{
				if (_currentNavigationData.nextDirection[i] == ' ')
				{
					int distance = abs(i - center);
					if (distance < closestDistance)
					{
						closestDistance = distance;
						bestSplit = i;
					}
				}
			}

			if (bestSplit != -1)
			{
				String line1 = _currentNavigationData.nextDirection.substring(0, bestSplit);
				String line2 = _currentNavigationData.nextDirection.substring(bestSplit + 1);

				// Next direction first line
				_display.drawStr(8, 15, line1.c_str());

				// Next direction second line
				_display.drawStr(8, 29, line2.c_str());

				// Next direction distance
				_display.setFont(u8g2_font_10x20_tr);
				_display.drawStr(8, 52, _currentNavigationData.nextDirectionDistance.c_str());
			}
			else
			{
				_display.drawStr(3, 12, _currentNavigationData.nextDirection.c_str());
				_display.setFont(u8g2_font_7x14_tr);
				_display.drawStr(3, 45, _currentNavigationData.nextDirectionDistance.c_str());
			}
		}
		else
		{
			// Next direction line
			_display.setFont(u8g2_font_7x13_tr);
			_display.drawStr(8, 23, _currentNavigationData.nextDirection.c_str());

			// Next direction distance line
			_display.setFont(u8g2_font_10x20_tr);
			_display.drawStr(8, 52, _currentNavigationData.nextDirectionDistance.c_str());
		}
	}

	_justDrawn = false;
	_previousNavigationData.nextDirection = _currentNavigationData.nextDirection;
	_previousNavigationData.nextDirectionDistance = _currentNavigationData.nextDirectionDistance;
}

/// @brief Writes data of second screen to display.
void Display::WriteSecondScreen()
{
	// If screen is just drawn avoid to check about data otherwise screen could be blank
	if (!_justDrawn)
	{
		// Check if data to display are changed
		if (_currentNavigationData.arrivalTime == _previousNavigationData.arrivalTime || _currentNavigationData.distanceLeft == _previousNavigationData.distanceLeft || _currentNavigationData.timeLeft == _previousNavigationData.timeLeft)
		{
			return;
		}
	}

	_display.setFont(u8g2_font_9x15_tr);
	_display.drawStr(37, 17, _currentNavigationData.arrivalTime.c_str());
	_display.drawStr(37, 37, _currentNavigationData.distanceLeft.c_str());
	_display.drawStr(37, 56, _currentNavigationData.timeLeft.c_str());

	_justDrawn = false;
	_previousNavigationData.arrivalTime = _currentNavigationData.arrivalTime;
	_previousNavigationData.distanceLeft = _currentNavigationData.distanceLeft;
	_previousNavigationData.timeLeft = _currentNavigationData.timeLeft;
}

/// @brief Write notification data to display.
void Display::DisplayNavigationData()
{
	this->CheckScreenToDisplay();

	switch (_currentScreen)
	{
	case FIRST_PAGE_SCREEN:
		this->WriteFirstScreen();
		break;

	case SECOND_PAGE_SCREEN:
		this->WriteSecondScreen();
		break;

	default:
		break;
	}

	_display.sendBuffer();
}

/// @brief Updates navigation data to be displayed.
/// @param navigationData Updated navigation data.
void Display::UpdateNavigationData(NAVIGATION_DATA navigationData)
{
	_currentNavigationData = navigationData;
}

/// @brief Write text to display.
/// @param text Text to be displayed.
/// @param x X position of the text on screen.
/// @param y Y position of the text on screen.
/// @param direction Direction of the text.
void Display::WriteText(String text, int x, int y, Direction direction)
{
	// Refresh previous millis to make timer working.
	_previousMillis = millis();

	_display.clearBuffer();
	_display.setFontDirection(int(direction));
	_display.setFont(u8g2_font_5x7_tr);

	_display.drawStr(x, y, text.c_str());
	_display.sendBuffer();

	_currentScreen = TEXT_PAGE_SCREEN;
}
