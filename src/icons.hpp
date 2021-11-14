#include <Arduino.h>

const unsigned char startIcon[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00,
	0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x07, 0xc7, 0xf9, 0xf8, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
	0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0f, 0xf7, 0xfb, 0xfc, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x40, 0x00, 0x00, 0x0f, 0x9f, 0xf7, 0xfb, 0xfc, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
	0x00, 0x01, 0x80, 0x00, 0x00, 0x1f, 0xdf, 0xf7, 0xfb, 0xfc, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
	0x00, 0x02, 0x00, 0x00, 0x00, 0x1f, 0xdf, 0xf7, 0xfb, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00,
	0x00, 0x0c, 0x00, 0x00, 0x00, 0x1f, 0xdf, 0xf7, 0xfb, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00,
	0x00, 0x10, 0x00, 0x00, 0x00, 0x1f, 0xdf, 0xf7, 0xf0, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x08, 0x00,
	0x00, 0x20, 0x00, 0x00, 0x00, 0x1f, 0xdf, 0xf1, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x04, 0x00,
	0x00, 0x40, 0x00, 0x00, 0x00, 0x1f, 0xdf, 0xf0, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x02, 0x00,
	0x00, 0x80, 0x00, 0x00, 0x00, 0x5f, 0xdf, 0xf6, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x01, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0xdf, 0xdf, 0xf7, 0xc1, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x80,
	0x02, 0x00, 0x00, 0x00, 0x00, 0xdf, 0xcf, 0xe3, 0xf3, 0xcf, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xc0,
	0x06, 0x00, 0x00, 0x00, 0x00, 0x5f, 0xc7, 0xdf, 0xfd, 0xef, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x40,
	0x04, 0x00, 0x00, 0x00, 0x00, 0x6f, 0xbf, 0xff, 0xff, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x20,
	0x08, 0x00, 0x00, 0x00, 0x00, 0x7d, 0xff, 0xff, 0xfc, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x10,
	0x08, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xe3, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x10,
	0x10, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xcf, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x10, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xbf, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x08,
	0x10, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x08,
	0x20, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x08,
	0x20, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x04,
	0x20, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x04,
	0x20, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
	0x20, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
	0x20, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
	0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
	0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
	0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
	0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
	0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
	0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
	0x01, 0x00, 0x3f, 0xf1, 0xc0, 0x38, 0xff, 0xe0, 0x07, 0xfe, 0x3f, 0xfc, 0xe0, 0x0c, 0x01, 0x80,
	0x00, 0x80, 0x30, 0x01, 0xe0, 0x79, 0x80, 0x00, 0x0c, 0x06, 0x30, 0x0c, 0xf0, 0x3c, 0x03, 0x00,
	0x00, 0x40, 0x37, 0xe1, 0xf0, 0xf9, 0x8f, 0xe0, 0x18, 0x06, 0x30, 0x18, 0xf8, 0x7c, 0x06, 0x00,
	0x00, 0x20, 0x30, 0x01, 0x99, 0x99, 0x80, 0x60, 0x1f, 0xfe, 0x33, 0xe0, 0xcc, 0xcc, 0x08, 0x00,
	0x00, 0x08, 0x30, 0x01, 0x8f, 0x18, 0xc0, 0x60, 0x18, 0x06, 0x30, 0x70, 0xc7, 0x8c, 0x10, 0x00,
	0x00, 0x04, 0x0f, 0xf1, 0x84, 0x10, 0x7f, 0xe0, 0x08, 0x02, 0x30, 0x18, 0x41, 0x04, 0x20, 0x00,
	0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00,
	0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
	0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char fist[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x30, 0x00,
	0x03, 0x33, 0x30, 0x00, 0x03, 0x33, 0x00, 0x00, 0x03, 0x32, 0x20, 0x00, 0x03, 0x33, 0x3e, 0x00,
	0x03, 0x33, 0x1f, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x73, 0x00, 0x03, 0xfc, 0x03, 0x00,
	0x03, 0xf0, 0x07, 0x00, 0x03, 0xe3, 0xff, 0x00, 0x03, 0xe7, 0xff, 0x00, 0x03, 0xff, 0xff, 0x00,
	0x03, 0xff, 0xfe, 0x00, 0x01, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xe0, 0x00,
	0x00, 0x7f, 0xe0, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x7f, 0xe0, 0x00,
	0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char hand[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00,
	0x00, 0x26, 0x60, 0x00, 0x00, 0x76, 0x70, 0x00, 0x00, 0x76, 0x60, 0x00, 0x02, 0x76, 0x60, 0x00,
	0x02, 0x36, 0x60, 0x00, 0x02, 0x76, 0x60, 0x00, 0x02, 0x76, 0x60, 0x00, 0x02, 0x76, 0x60, 0x00,
	0x02, 0x77, 0x60, 0x00, 0x03, 0xff, 0xe0, 0x80, 0x03, 0xff, 0xe1, 0xc0, 0x03, 0xff, 0xe1, 0x80,
	0x03, 0xff, 0xe3, 0x80, 0x03, 0xf8, 0x0f, 0x00, 0x03, 0xe0, 0x7e, 0x00, 0x03, 0xc7, 0xfc, 0x00,
	0x03, 0xcf, 0xf8, 0x00, 0x03, 0xff, 0xf0, 0x00, 0x03, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xc0, 0x00,
	0x00, 0xff, 0x80, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
const unsigned char peace[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x10, 0x00, 0x00, 0x18, 0x38, 0x00,
	0x00, 0x1c, 0x30, 0x00, 0x00, 0x1c, 0x30, 0x00, 0x00, 0x0c, 0x70, 0x00, 0x00, 0x0c, 0x70, 0x00,
	0x00, 0x0e, 0x60, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x66, 0x00, 0x00, 0x02, 0x64, 0x00, 0x00,
	0x03, 0x30, 0xf8, 0x00, 0x01, 0x12, 0x18, 0x00, 0x01, 0x87, 0x8c, 0x00, 0x00, 0x1c, 0x0c, 0x00,
	0x03, 0xe0, 0x1c, 0x00, 0x03, 0xc7, 0xfc, 0x00, 0x03, 0xdf, 0xfc, 0x00, 0x03, 0xff, 0xf8, 0x00,
	0x01, 0xff, 0xe0, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x7f, 0x80, 0x00,
	0x00, 0x7f, 0x80, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char ok[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x06, 0x60, 0x00,
	0x00, 0x06, 0x20, 0x00, 0x00, 0x06, 0x20, 0x00, 0x00, 0x03, 0x34, 0x00, 0x00, 0x03, 0x34, 0x00,
	0x00, 0x03, 0x3c, 0x00, 0x00, 0x13, 0xbc, 0x00, 0x00, 0x7f, 0xfc, 0x00, 0x01, 0xff, 0xfc, 0x00,
	0x03, 0x9f, 0xfc, 0x00, 0x06, 0x0f, 0xf8, 0x00, 0x0e, 0x07, 0xf8, 0x00, 0x0e, 0x07, 0xf8, 0x00,
	0x07, 0x87, 0xf0, 0x00, 0x03, 0xcf, 0xf0, 0x00, 0x01, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xf0, 0x00,
	0x00, 0x7f, 0xf8, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xf8, 0x00,
	0x00, 0x07, 0xf8, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x03, 0xf8, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char thumbsUp[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x38, 0x00,
	0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x03, 0xe0, 0x00,
	0x00, 0x07, 0xe0, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x1f, 0x9f, 0x80, 0x00, 0x3f, 0x9f, 0x80,
	0x07, 0xff, 0x80, 0x00, 0x07, 0xff, 0x9f, 0x00, 0x07, 0xff, 0x9f, 0x80, 0x07, 0xff, 0x8c, 0x00,
	0x07, 0xff, 0x40, 0x00, 0x07, 0xff, 0x5f, 0x80, 0x07, 0xfe, 0xdf, 0x80, 0x03, 0xff, 0xc0, 0x00,
	0x00, 0x1f, 0xdf, 0x00, 0x00, 0x0f, 0xdf, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char callMe[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
	0x07, 0x6d, 0x80, 0x00, 0x07, 0x6d, 0x80, 0x00, 0x07, 0x6d, 0x86, 0x00, 0x03, 0x6d, 0x86, 0x00,
	0x03, 0x6d, 0x8e, 0x00, 0x03, 0xa4, 0xce, 0x00, 0x03, 0xff, 0x5c, 0x00, 0x03, 0xfa, 0x7c, 0x00,
	0x01, 0xff, 0xf8, 0x00, 0x01, 0xff, 0xf8, 0x00, 0x01, 0xff, 0xf0, 0x00, 0x01, 0xff, 0xe0, 0x00,
	0x00, 0xff, 0xc0, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00,
	0x00, 0x7f, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char rock[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0e, 0x00, 0x60, 0x00, 0x0e, 0x00, 0xf0, 0x00, 0x0e, 0x00, 0xf0, 0x00, 0x0e, 0x00, 0xf0, 0x00,
	0x0e, 0x00, 0xf0, 0x00, 0x0e, 0x00, 0xf0, 0x00, 0x0e, 0x66, 0xf0, 0x00, 0x0e, 0xee, 0xf0, 0x00,
	0x0e, 0xee, 0xf0, 0x60, 0x0e, 0xee, 0xf0, 0xe0, 0x0e, 0x46, 0xf0, 0xe0, 0x0f, 0x01, 0xf1, 0xe0,
	0x0f, 0xf8, 0x03, 0xc0, 0x0f, 0xe0, 0x1f, 0x80, 0x0f, 0xcf, 0xff, 0x00, 0x0f, 0xff, 0xfe, 0x00,
	0x07, 0xff, 0xf8, 0x00, 0x03, 0xff, 0xf0, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x00, 0xff, 0xc0, 0x00,
	0x00, 0xff, 0x80, 0x00, 0x00, 0xff, 0x80, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char countdown[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x80, 0x00, 0x00, 0x19, 0x80, 0x00,
	0x00, 0x19, 0x8c, 0x00, 0x00, 0x19, 0x8c, 0x00, 0x01, 0x19, 0x8c, 0x00, 0x01, 0x99, 0x98, 0x00,
	0x01, 0x99, 0x98, 0x00, 0x01, 0x9b, 0x98, 0x00, 0x01, 0x9f, 0x98, 0x00, 0x01, 0xdf, 0xb8, 0x00,
	0x00, 0xdf, 0x10, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x78, 0x00,
	0x00, 0xff, 0x1c, 0x00, 0x00, 0x7f, 0x0c, 0x00, 0x00, 0x7c, 0x0c, 0x00, 0x00, 0x78, 0xfc, 0x00,
	0x00, 0x73, 0xfc, 0x00, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0x7f, 0xf8, 0x00, 0x00, 0x3f, 0xf0, 0x00,
	0x00, 0x3f, 0xe0, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x1f, 0xc0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};