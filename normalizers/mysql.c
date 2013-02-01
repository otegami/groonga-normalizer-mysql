/* -*- c-basic-offset: 2 -*- */
/*
  Copyright(C) 2013  Kouhei Sutou <kou@clear-code.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; version 2
  of the License.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
  MA 02110-1301, USA

  This file uses normalization table defined in
  MySQL-5.5.29/strings/ctype-utf8.c. The following is the header of
  the file:

    Copyright (c) 2000, 2012, Oracle and/or its affiliates. All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; version 2
    of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
    MA 02110-1301, USA

    UTF8 according RFC 2279
    Written by Alexander Barkov <bar@udm.net>
*/

#include <groonga/normalizer.h>
#include <groonga/tokenizer.h>

#include <stdint.h>

#ifdef __GNUC__
#  define GNUC_UNUSED __attribute__((__unused__))
#else
#  define GNUC_UNUSED
#endif

/* based on mysql-5.5.29/strings/ctype-utf8.c: start */
static uint32_t plane00[] = {
  0x0000,  0x0001,
  0x0002,  0x0003,
  0x0004,  0x0005,
  0x0006,  0x0007,
  0x0008,  0x0009,
  0x000A,  0x000B,
  0x000C,  0x000D,
  0x000E,  0x000F,
  0x0010,  0x0011,
  0x0012,  0x0013,
  0x0014,  0x0015,
  0x0016,  0x0017,
  0x0018,  0x0019,
  0x001A,  0x001B,
  0x001C,  0x001D,
  0x001E,  0x001F,
  0x0020,  0x0021,
  0x0022,  0x0023,
  0x0024,  0x0025,
  0x0026,  0x0027,
  0x0028,  0x0029,
  0x002A,  0x002B,
  0x002C,  0x002D,
  0x002E,  0x002F,
  0x0030,  0x0031,
  0x0032,  0x0033,
  0x0034,  0x0035,
  0x0036,  0x0037,
  0x0038,  0x0039,
  0x003A,  0x003B,
  0x003C,  0x003D,
  0x003E,  0x003F,
  0x0040,  0x0041,
  0x0042,  0x0043,
  0x0044,  0x0045,
  0x0046,  0x0047,
  0x0048,  0x0049,
  0x004A,  0x004B,
  0x004C,  0x004D,
  0x004E,  0x004F,
  0x0050,  0x0051,
  0x0052,  0x0053,
  0x0054,  0x0055,
  0x0056,  0x0057,
  0x0058,  0x0059,
  0x005A,  0x005B,
  0x005C,  0x005D,
  0x005E,  0x005F,
  0x0060,  0x0041,
  0x0042,  0x0043,
  0x0044,  0x0045,
  0x0046,  0x0047,
  0x0048,  0x0049,
  0x004A,  0x004B,
  0x004C,  0x004D,
  0x004E,  0x004F,
  0x0050,  0x0051,
  0x0052,  0x0053,
  0x0054,  0x0055,
  0x0056,  0x0057,
  0x0058,  0x0059,
  0x005A,  0x007B,
  0x007C,  0x007D,
  0x007E,  0x007F,
  0x0080,  0x0081,
  0x0082,  0x0083,
  0x0084,  0x0085,
  0x0086,  0x0087,
  0x0088,  0x0089,
  0x008A,  0x008B,
  0x008C,  0x008D,
  0x008E,  0x008F,
  0x0090,  0x0091,
  0x0092,  0x0093,
  0x0094,  0x0095,
  0x0096,  0x0097,
  0x0098,  0x0099,
  0x009A,  0x009B,
  0x009C,  0x009D,
  0x009E,  0x009F,
  0x00A0,  0x00A1,
  0x00A2,  0x00A3,
  0x00A4,  0x00A5,
  0x00A6,  0x00A7,
  0x00A8,  0x00A9,
  0x00AA,  0x00AB,
  0x00AC,  0x00AD,
  0x00AE,  0x00AF,
  0x00B0,  0x00B1,
  0x00B2,  0x00B3,
  0x00B4,  0x039C,
  0x00B6,  0x00B7,
  0x00B8,  0x00B9,
  0x00BA,  0x00BB,
  0x00BC,  0x00BD,
  0x00BE,  0x00BF,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x00C6,  0x0043,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0049,  0x0049,
  0x0049,  0x0049,
  0x00D0,  0x004E,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x00D7,
  0x00D8,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0059,
  0x00DE,  0x0053,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x00C6,  0x0043,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0049,  0x0049,
  0x0049,  0x0049,
  0x00D0,  0x004E,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x00F7,
  0x00D8,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0059,
  0x00DE,  0x0059
};

static uint32_t plane01[] = {
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0043,  0x0043,
  0x0043,  0x0043,
  0x0043,  0x0043,
  0x0043,  0x0043,
  0x0044,  0x0044,
  0x0110,  0x0110,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0047,  0x0047,
  0x0047,  0x0047,
  0x0047,  0x0047,
  0x0047,  0x0047,
  0x0048,  0x0048,
  0x0126,  0x0126,
  0x0049,  0x0049,
  0x0049,  0x0049,
  0x0049,  0x0049,
  0x0049,  0x0049,
  0x0049,  0x0049,
  0x0132,  0x0132,
  0x004A,  0x004A,
  0x004B,  0x004B,
  0x0138,  0x004C,
  0x004C,  0x004C,
  0x004C,  0x004C,
  0x004C,  0x013F,
  0x013F,  0x0141,
  0x0141,  0x004E,
  0x004E,  0x004E,
  0x004E,  0x004E,
  0x004E,  0x0149,
  0x014A,  0x014A,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x0152,  0x0152,
  0x0052,  0x0052,
  0x0052,  0x0052,
  0x0052,  0x0052,
  0x0053,  0x0053,
  0x0053,  0x0053,
  0x0053,  0x0053,
  0x0053,  0x0053,
  0x0054,  0x0054,
  0x0054,  0x0054,
  0x0166,  0x0166,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0057,  0x0057,
  0x0059,  0x0059,
  0x0059,  0x005A,
  0x005A,  0x005A,
  0x005A,  0x005A,
  0x005A,  0x0053,
  0x0180,  0x0181,
  0x0182,  0x0182,
  0x0184,  0x0184,
  0x0186,  0x0187,
  0x0187,  0x0189,
  0x018A,  0x018B,
  0x018B,  0x018D,
  0x018E,  0x018F,
  0x0190,  0x0191,
  0x0191,  0x0193,
  0x0194,  0x01F6,
  0x0196,  0x0197,
  0x0198,  0x0198,
  0x019A,  0x019B,
  0x019C,  0x019D,
  0x019E,  0x019F,
  0x004F,  0x004F,
  0x01A2,  0x01A2,
  0x01A4,  0x01A4,
  0x01A6,  0x01A7,
  0x01A7,  0x01A9,
  0x01AA,  0x01AB,
  0x01AC,  0x01AC,
  0x01AE,  0x0055,
  0x0055,  0x01B1,
  0x01B2,  0x01B3,
  0x01B3,  0x01B5,
  0x01B5,  0x01B7,
  0x01B8,  0x01B8,
  0x01BA,  0x01BB,
  0x01BC,  0x01BC,
  0x01BE,  0x01F7,
  0x01C0,  0x01C1,
  0x01C2,  0x01C3,
  0x01C4,  0x01C4,
  0x01C4,  0x01C7,
  0x01C7,  0x01C7,
  0x01CA,  0x01CA,
  0x01CA,  0x0041,
  0x0041,  0x0049,
  0x0049,  0x004F,
  0x004F,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x018E,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x00C6,  0x00C6,
  0x01E4,  0x01E4,
  0x0047,  0x0047,
  0x004B,  0x004B,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x01B7,  0x01B7,
  0x004A,  0x01F1,
  0x01F1,  0x01F1,
  0x0047,  0x0047,
  0x01F6,  0x01F7,
  0x004E,  0x004E,
  0x0041,  0x0041,
  0x00C6,  0x00C6,
  0x00D8,  0x00D8
};

static uint32_t plane02[] = {
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0049,  0x0049,
  0x0049,  0x0049,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x0052,  0x0052,
  0x0052,  0x0052,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0053,  0x0053,
  0x0054,  0x0054,
  0x021C,  0x021C,
  0x0048,  0x0048,
  0x0220,  0x0221,
  0x0222,  0x0222,
  0x0224,  0x0224,
  0x0041,  0x0041,
  0x0045,  0x0045,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x0059,  0x0059,
  0x0234,  0x0235,
  0x0236,  0x0237,
  0x0238,  0x0239,
  0x023A,  0x023B,
  0x023C,  0x023D,
  0x023E,  0x023F,
  0x0240,  0x0241,
  0x0242,  0x0243,
  0x0244,  0x0245,
  0x0246,  0x0247,
  0x0248,  0x0249,
  0x024A,  0x024B,
  0x024C,  0x024D,
  0x024E,  0x024F,
  0x0250,  0x0251,
  0x0252,  0x0181,
  0x0186,  0x0255,
  0x0189,  0x018A,
  0x0258,  0x018F,
  0x025A,  0x0190,
  0x025C,  0x025D,
  0x025E,  0x025F,
  0x0193,  0x0261,
  0x0262,  0x0194,
  0x0264,  0x0265,
  0x0266,  0x0267,
  0x0197,  0x0196,
  0x026A,  0x026B,
  0x026C,  0x026D,
  0x026E,  0x019C,
  0x0270,  0x0271,
  0x019D,  0x0273,
  0x0274,  0x019F,
  0x0276,  0x0277,
  0x0278,  0x0279,
  0x027A,  0x027B,
  0x027C,  0x027D,
  0x027E,  0x027F,
  0x01A6,  0x0281,
  0x0282,  0x01A9,
  0x0284,  0x0285,
  0x0286,  0x0287,
  0x01AE,  0x0289,
  0x01B1,  0x01B2,
  0x028C,  0x028D,
  0x028E,  0x028F,
  0x0290,  0x0291,
  0x01B7,  0x0293,
  0x0294,  0x0295,
  0x0296,  0x0297,
  0x0298,  0x0299,
  0x029A,  0x029B,
  0x029C,  0x029D,
  0x029E,  0x029F,
  0x02A0,  0x02A1,
  0x02A2,  0x02A3,
  0x02A4,  0x02A5,
  0x02A6,  0x02A7,
  0x02A8,  0x02A9,
  0x02AA,  0x02AB,
  0x02AC,  0x02AD,
  0x02AE,  0x02AF,
  0x02B0,  0x02B1,
  0x02B2,  0x02B3,
  0x02B4,  0x02B5,
  0x02B6,  0x02B7,
  0x02B8,  0x02B9,
  0x02BA,  0x02BB,
  0x02BC,  0x02BD,
  0x02BE,  0x02BF,
  0x02C0,  0x02C1,
  0x02C2,  0x02C3,
  0x02C4,  0x02C5,
  0x02C6,  0x02C7,
  0x02C8,  0x02C9,
  0x02CA,  0x02CB,
  0x02CC,  0x02CD,
  0x02CE,  0x02CF,
  0x02D0,  0x02D1,
  0x02D2,  0x02D3,
  0x02D4,  0x02D5,
  0x02D6,  0x02D7,
  0x02D8,  0x02D9,
  0x02DA,  0x02DB,
  0x02DC,  0x02DD,
  0x02DE,  0x02DF,
  0x02E0,  0x02E1,
  0x02E2,  0x02E3,
  0x02E4,  0x02E5,
  0x02E6,  0x02E7,
  0x02E8,  0x02E9,
  0x02EA,  0x02EB,
  0x02EC,  0x02ED,
  0x02EE,  0x02EF,
  0x02F0,  0x02F1,
  0x02F2,  0x02F3,
  0x02F4,  0x02F5,
  0x02F6,  0x02F7,
  0x02F8,  0x02F9,
  0x02FA,  0x02FB,
  0x02FC,  0x02FD,
  0x02FE,  0x02FF
};

static uint32_t plane03[] = {
  0x0300,  0x0301,
  0x0302,  0x0303,
  0x0304,  0x0305,
  0x0306,  0x0307,
  0x0308,  0x0309,
  0x030A,  0x030B,
  0x030C,  0x030D,
  0x030E,  0x030F,
  0x0310,  0x0311,
  0x0312,  0x0313,
  0x0314,  0x0315,
  0x0316,  0x0317,
  0x0318,  0x0319,
  0x031A,  0x031B,
  0x031C,  0x031D,
  0x031E,  0x031F,
  0x0320,  0x0321,
  0x0322,  0x0323,
  0x0324,  0x0325,
  0x0326,  0x0327,
  0x0328,  0x0329,
  0x032A,  0x032B,
  0x032C,  0x032D,
  0x032E,  0x032F,
  0x0330,  0x0331,
  0x0332,  0x0333,
  0x0334,  0x0335,
  0x0336,  0x0337,
  0x0338,  0x0339,
  0x033A,  0x033B,
  0x033C,  0x033D,
  0x033E,  0x033F,
  0x0340,  0x0341,
  0x0342,  0x0343,
  0x0344,  0x0399,
  0x0346,  0x0347,
  0x0348,  0x0349,
  0x034A,  0x034B,
  0x034C,  0x034D,
  0x034E,  0x034F,
  0x0350,  0x0351,
  0x0352,  0x0353,
  0x0354,  0x0355,
  0x0356,  0x0357,
  0x0358,  0x0359,
  0x035A,  0x035B,
  0x035C,  0x035D,
  0x035E,  0x035F,
  0x0360,  0x0361,
  0x0362,  0x0363,
  0x0364,  0x0365,
  0x0366,  0x0367,
  0x0368,  0x0369,
  0x036A,  0x036B,
  0x036C,  0x036D,
  0x036E,  0x036F,
  0x0370,  0x0371,
  0x0372,  0x0373,
  0x0374,  0x0375,
  0x0376,  0x0377,
  0x0378,  0x0379,
  0x037A,  0x037B,
  0x037C,  0x037D,
  0x037E,  0x037F,
  0x0380,  0x0381,
  0x0382,  0x0383,
  0x0384,  0x0385,
  0x0391,  0x0387,
  0x0395,  0x0397,
  0x0399,  0x038B,
  0x039F,  0x038D,
  0x03A5,  0x03A9,
  0x0399,  0x0391,
  0x0392,  0x0393,
  0x0394,  0x0395,
  0x0396,  0x0397,
  0x0398,  0x0399,
  0x039A,  0x039B,
  0x039C,  0x039D,
  0x039E,  0x039F,
  0x03A0,  0x03A1,
  0x03A2,  0x03A3,
  0x03A4,  0x03A5,
  0x03A6,  0x03A7,
  0x03A8,  0x03A9,
  0x0399,  0x03A5,
  0x0391,  0x0395,
  0x0397,  0x0399,
  0x03A5,  0x0391,
  0x0392,  0x0393,
  0x0394,  0x0395,
  0x0396,  0x0397,
  0x0398,  0x0399,
  0x039A,  0x039B,
  0x039C,  0x039D,
  0x039E,  0x039F,
  0x03A0,  0x03A1,
  0x03A3,  0x03A3,
  0x03A4,  0x03A5,
  0x03A6,  0x03A7,
  0x03A8,  0x03A9,
  0x0399,  0x03A5,
  0x039F,  0x03A5,
  0x03A9,  0x03CF,
  0x0392,  0x0398,
  0x03D2,  0x03D2,
  0x03D2,  0x03A6,
  0x03A0,  0x03D7,
  0x03D8,  0x03D9,
  0x03DA,  0x03DA,
  0x03DC,  0x03DC,
  0x03DE,  0x03DE,
  0x03E0,  0x03E0,
  0x03E2,  0x03E2,
  0x03E4,  0x03E4,
  0x03E6,  0x03E6,
  0x03E8,  0x03E8,
  0x03EA,  0x03EA,
  0x03EC,  0x03EC,
  0x03EE,  0x03EE,
  0x039A,  0x03A1,
  0x03A3,  0x03F3,
  0x03F4,  0x03F5,
  0x03F6,  0x03F7,
  0x03F8,  0x03F9,
  0x03FA,  0x03FB,
  0x03FC,  0x03FD,
  0x03FE,  0x03FF
};

static uint32_t plane04[] = {
  0x0415,  0x0415,
  0x0402,  0x0413,
  0x0404,  0x0405,
  0x0406,  0x0406,
  0x0408,  0x0409,
  0x040A,  0x040B,
  0x041A,  0x0418,
  0x0423,  0x040F,
  0x0410,  0x0411,
  0x0412,  0x0413,
  0x0414,  0x0415,
  0x0416,  0x0417,
  0x0418,  0x0419,
  0x041A,  0x041B,
  0x041C,  0x041D,
  0x041E,  0x041F,
  0x0420,  0x0421,
  0x0422,  0x0423,
  0x0424,  0x0425,
  0x0426,  0x0427,
  0x0428,  0x0429,
  0x042A,  0x042B,
  0x042C,  0x042D,
  0x042E,  0x042F,
  0x0410,  0x0411,
  0x0412,  0x0413,
  0x0414,  0x0415,
  0x0416,  0x0417,
  0x0418,  0x0419,
  0x041A,  0x041B,
  0x041C,  0x041D,
  0x041E,  0x041F,
  0x0420,  0x0421,
  0x0422,  0x0423,
  0x0424,  0x0425,
  0x0426,  0x0427,
  0x0428,  0x0429,
  0x042A,  0x042B,
  0x042C,  0x042D,
  0x042E,  0x042F,
  0x0415,  0x0415,
  0x0402,  0x0413,
  0x0404,  0x0405,
  0x0406,  0x0406,
  0x0408,  0x0409,
  0x040A,  0x040B,
  0x041A,  0x0418,
  0x0423,  0x040F,
  0x0460,  0x0460,
  0x0462,  0x0462,
  0x0464,  0x0464,
  0x0466,  0x0466,
  0x0468,  0x0468,
  0x046A,  0x046A,
  0x046C,  0x046C,
  0x046E,  0x046E,
  0x0470,  0x0470,
  0x0472,  0x0472,
  0x0474,  0x0474,
  0x0474,  0x0474,
  0x0478,  0x0478,
  0x047A,  0x047A,
  0x047C,  0x047C,
  0x047E,  0x047E,
  0x0480,  0x0480,
  0x0482,  0x0483,
  0x0484,  0x0485,
  0x0486,  0x0487,
  0x0488,  0x0489,
  0x048A,  0x048B,
  0x048C,  0x048C,
  0x048E,  0x048E,
  0x0490,  0x0490,
  0x0492,  0x0492,
  0x0494,  0x0494,
  0x0496,  0x0496,
  0x0498,  0x0498,
  0x049A,  0x049A,
  0x049C,  0x049C,
  0x049E,  0x049E,
  0x04A0,  0x04A0,
  0x04A2,  0x04A2,
  0x04A4,  0x04A4,
  0x04A6,  0x04A6,
  0x04A8,  0x04A8,
  0x04AA,  0x04AA,
  0x04AC,  0x04AC,
  0x04AE,  0x04AE,
  0x04B0,  0x04B0,
  0x04B2,  0x04B2,
  0x04B4,  0x04B4,
  0x04B6,  0x04B6,
  0x04B8,  0x04B8,
  0x04BA,  0x04BA,
  0x04BC,  0x04BC,
  0x04BE,  0x04BE,
  0x04C0,  0x0416,
  0x0416,  0x04C3,
  0x04C3,  0x04C5,
  0x04C6,  0x04C7,
  0x04C7,  0x04C9,
  0x04CA,  0x04CB,
  0x04CB,  0x04CD,
  0x04CE,  0x04CF,
  0x0410,  0x0410,
  0x0410,  0x0410,
  0x04D4,  0x04D4,
  0x0415,  0x0415,
  0x04D8,  0x04D8,
  0x04D8,  0x04D8,
  0x0416,  0x0416,
  0x0417,  0x0417,
  0x04E0,  0x04E0,
  0x0418,  0x0418,
  0x0418,  0x0418,
  0x041E,  0x041E,
  0x04E8,  0x04E8,
  0x04E8,  0x04E8,
  0x042D,  0x042D,
  0x0423,  0x0423,
  0x0423,  0x0423,
  0x0423,  0x0423,
  0x0427,  0x0427,
  0x04F6,  0x04F7,
  0x042B,  0x042B,
  0x04FA,  0x04FB,
  0x04FC,  0x04FD,
  0x04FE,  0x04FF
};

static uint32_t plane05[] = {
  0x0500,  0x0501,
  0x0502,  0x0503,
  0x0504,  0x0505,
  0x0506,  0x0507,
  0x0508,  0x0509,
  0x050A,  0x050B,
  0x050C,  0x050D,
  0x050E,  0x050F,
  0x0510,  0x0511,
  0x0512,  0x0513,
  0x0514,  0x0515,
  0x0516,  0x0517,
  0x0518,  0x0519,
  0x051A,  0x051B,
  0x051C,  0x051D,
  0x051E,  0x051F,
  0x0520,  0x0521,
  0x0522,  0x0523,
  0x0524,  0x0525,
  0x0526,  0x0527,
  0x0528,  0x0529,
  0x052A,  0x052B,
  0x052C,  0x052D,
  0x052E,  0x052F,
  0x0530,  0x0531,
  0x0532,  0x0533,
  0x0534,  0x0535,
  0x0536,  0x0537,
  0x0538,  0x0539,
  0x053A,  0x053B,
  0x053C,  0x053D,
  0x053E,  0x053F,
  0x0540,  0x0541,
  0x0542,  0x0543,
  0x0544,  0x0545,
  0x0546,  0x0547,
  0x0548,  0x0549,
  0x054A,  0x054B,
  0x054C,  0x054D,
  0x054E,  0x054F,
  0x0550,  0x0551,
  0x0552,  0x0553,
  0x0554,  0x0555,
  0x0556,  0x0557,
  0x0558,  0x0559,
  0x055A,  0x055B,
  0x055C,  0x055D,
  0x055E,  0x055F,
  0x0560,  0x0531,
  0x0532,  0x0533,
  0x0534,  0x0535,
  0x0536,  0x0537,
  0x0538,  0x0539,
  0x053A,  0x053B,
  0x053C,  0x053D,
  0x053E,  0x053F,
  0x0540,  0x0541,
  0x0542,  0x0543,
  0x0544,  0x0545,
  0x0546,  0x0547,
  0x0548,  0x0549,
  0x054A,  0x054B,
  0x054C,  0x054D,
  0x054E,  0x054F,
  0x0550,  0x0551,
  0x0552,  0x0553,
  0x0554,  0x0555,
  0x0556,  0x0587,
  0x0588,  0x0589,
  0x058A,  0x058B,
  0x058C,  0x058D,
  0x058E,  0x058F,
  0x0590,  0x0591,
  0x0592,  0x0593,
  0x0594,  0x0595,
  0x0596,  0x0597,
  0x0598,  0x0599,
  0x059A,  0x059B,
  0x059C,  0x059D,
  0x059E,  0x059F,
  0x05A0,  0x05A1,
  0x05A2,  0x05A3,
  0x05A4,  0x05A5,
  0x05A6,  0x05A7,
  0x05A8,  0x05A9,
  0x05AA,  0x05AB,
  0x05AC,  0x05AD,
  0x05AE,  0x05AF,
  0x05B0,  0x05B1,
  0x05B2,  0x05B3,
  0x05B4,  0x05B5,
  0x05B6,  0x05B7,
  0x05B8,  0x05B9,
  0x05BA,  0x05BB,
  0x05BC,  0x05BD,
  0x05BE,  0x05BF,
  0x05C0,  0x05C1,
  0x05C2,  0x05C3,
  0x05C4,  0x05C5,
  0x05C6,  0x05C7,
  0x05C8,  0x05C9,
  0x05CA,  0x05CB,
  0x05CC,  0x05CD,
  0x05CE,  0x05CF,
  0x05D0,  0x05D1,
  0x05D2,  0x05D3,
  0x05D4,  0x05D5,
  0x05D6,  0x05D7,
  0x05D8,  0x05D9,
  0x05DA,  0x05DB,
  0x05DC,  0x05DD,
  0x05DE,  0x05DF,
  0x05E0,  0x05E1,
  0x05E2,  0x05E3,
  0x05E4,  0x05E5,
  0x05E6,  0x05E7,
  0x05E8,  0x05E9,
  0x05EA,  0x05EB,
  0x05EC,  0x05ED,
  0x05EE,  0x05EF,
  0x05F0,  0x05F1,
  0x05F2,  0x05F3,
  0x05F4,  0x05F5,
  0x05F6,  0x05F7,
  0x05F8,  0x05F9,
  0x05FA,  0x05FB,
  0x05FC,  0x05FD,
  0x05FE,  0x05FF
};

static uint32_t plane1E[] = {
  0x0041,  0x0041,
  0x0042,  0x0042,
  0x0042,  0x0042,
  0x0042,  0x0042,
  0x0043,  0x0043,
  0x0044,  0x0044,
  0x0044,  0x0044,
  0x0044,  0x0044,
  0x0044,  0x0044,
  0x0044,  0x0044,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0046,  0x0046,
  0x0047,  0x0047,
  0x0048,  0x0048,
  0x0048,  0x0048,
  0x0048,  0x0048,
  0x0048,  0x0048,
  0x0048,  0x0048,
  0x0049,  0x0049,
  0x0049,  0x0049,
  0x004B,  0x004B,
  0x004B,  0x004B,
  0x004B,  0x004B,
  0x004C,  0x004C,
  0x004C,  0x004C,
  0x004C,  0x004C,
  0x004C,  0x004C,
  0x004D,  0x004D,
  0x004D,  0x004D,
  0x004D,  0x004D,
  0x004E,  0x004E,
  0x004E,  0x004E,
  0x004E,  0x004E,
  0x004E,  0x004E,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x0050,  0x0050,
  0x0050,  0x0050,
  0x0052,  0x0052,
  0x0052,  0x0052,
  0x0052,  0x0052,
  0x0052,  0x0052,
  0x0053,  0x0053,
  0x0053,  0x0053,
  0x0053,  0x0053,
  0x0053,  0x0053,
  0x0053,  0x0053,
  0x0054,  0x0054,
  0x0054,  0x0054,
  0x0054,  0x0054,
  0x0054,  0x0054,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0056,  0x0056,
  0x0056,  0x0056,
  0x0057,  0x0057,
  0x0057,  0x0057,
  0x0057,  0x0057,
  0x0057,  0x0057,
  0x0057,  0x0057,
  0x0058,  0x0058,
  0x0058,  0x0058,
  0x0059,  0x0059,
  0x005A,  0x005A,
  0x005A,  0x005A,
  0x005A,  0x005A,
  0x0048,  0x0054,
  0x0057,  0x0059,
  0x1E9A,  0x0053,
  0x1E9C,  0x1E9D,
  0x1E9E,  0x1E9F,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0041,  0x0041,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0045,  0x0045,
  0x0049,  0x0049,
  0x0049,  0x0049,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x004F,  0x004F,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0055,  0x0055,
  0x0059,  0x0059,
  0x0059,  0x0059,
  0x0059,  0x0059,
  0x0059,  0x0059,
  0x1EFA,  0x1EFB,
  0x1EFC,  0x1EFD,
  0x1EFE,  0x1EFF
};

static uint32_t plane1F[] = {
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0395,  0x0395,
  0x0395,  0x0395,
  0x0395,  0x0395,
  0x1F16,  0x1F17,
  0x0395,  0x0395,
  0x0395,  0x0395,
  0x0395,  0x0395,
  0x1F1E,  0x1F1F,
  0x0397,  0x0397,
  0x0397,  0x0397,
  0x0397,  0x0397,
  0x0397,  0x0397,
  0x0397,  0x0397,
  0x0397,  0x0397,
  0x0397,  0x0397,
  0x0397,  0x0397,
  0x0399,  0x0399,
  0x0399,  0x0399,
  0x0399,  0x0399,
  0x0399,  0x0399,
  0x0399,  0x0399,
  0x0399,  0x0399,
  0x0399,  0x0399,
  0x0399,  0x0399,
  0x039F,  0x039F,
  0x039F,  0x039F,
  0x039F,  0x039F,
  0x1F46,  0x1F47,
  0x039F,  0x039F,
  0x039F,  0x039F,
  0x039F,  0x039F,
  0x1F4E,  0x1F4F,
  0x03A5,  0x03A5,
  0x03A5,  0x03A5,
  0x03A5,  0x03A5,
  0x03A5,  0x03A5,
  0x1F58,  0x03A5,
  0x1F5A,  0x03A5,
  0x1F5C,  0x03A5,
  0x1F5E,  0x03A5,
  0x03A9,  0x03A9,
  0x03A9,  0x03A9,
  0x03A9,  0x03A9,
  0x03A9,  0x03A9,
  0x03A9,  0x03A9,
  0x03A9,  0x03A9,
  0x03A9,  0x03A9,
  0x03A9,  0x03A9,
  0x0391,  0x1FBB,
  0x0395,  0x1FC9,
  0x0397,  0x1FCB,
  0x0399,  0x1FDB,
  0x039F,  0x1FF9,
  0x03A5,  0x1FEB,
  0x03A9,  0x1FFB,
  0x1F7E,  0x1F7F,
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0397,  0x0397,
  0x0397,  0x0397,
  0x0397,  0x0397,
  0x0397,  0x0397,
  0x0397,  0x0397,
  0x0397,  0x0397,
  0x0397,  0x0397,
  0x0397,  0x0397,
  0x03A9,  0x03A9,
  0x03A9,  0x03A9,
  0x03A9,  0x03A9,
  0x03A9,  0x03A9,
  0x03A9,  0x03A9,
  0x03A9,  0x03A9,
  0x03A9,  0x03A9,
  0x03A9,  0x03A9,
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0391,  0x1FB5,
  0x0391,  0x0391,
  0x0391,  0x0391,
  0x0391,  0x1FBB,
  0x0391,  0x1FBD,
  0x0399,  0x1FBF,
  0x1FC0,  0x1FC1,
  0x0397,  0x0397,
  0x0397,  0x1FC5,
  0x0397,  0x0397,
  0x0395,  0x1FC9,
  0x0397,  0x1FCB,
  0x0397,  0x1FCD,
  0x1FCE,  0x1FCF,
  0x0399,  0x0399,
  0x0399,  0x1FD3,
  0x1FD4,  0x1FD5,
  0x0399,  0x0399,
  0x0399,  0x0399,
  0x0399,  0x1FDB,
  0x1FDC,  0x1FDD,
  0x1FDE,  0x1FDF,
  0x03A5,  0x03A5,
  0x03A5,  0x1FE3,
  0x03A1,  0x03A1,
  0x03A5,  0x03A5,
  0x03A5,  0x03A5,
  0x03A5,  0x1FEB,
  0x03A1,  0x1FED,
  0x1FEE,  0x1FEF,
  0x1FF0,  0x1FF1,
  0x03A9,  0x03A9,
  0x03A9,  0x1FF5,
  0x03A9,  0x03A9,
  0x039F,  0x1FF9,
  0x03A9,  0x1FFB,
  0x03A9,  0x1FFD,
  0x1FFE,  0x1FFF
};

static uint32_t plane21[] = {
  0x2100,  0x2101,
  0x2102,  0x2103,
  0x2104,  0x2105,
  0x2106,  0x2107,
  0x2108,  0x2109,
  0x210A,  0x210B,
  0x210C,  0x210D,
  0x210E,  0x210F,
  0x2110,  0x2111,
  0x2112,  0x2113,
  0x2114,  0x2115,
  0x2116,  0x2117,
  0x2118,  0x2119,
  0x211A,  0x211B,
  0x211C,  0x211D,
  0x211E,  0x211F,
  0x2120,  0x2121,
  0x2122,  0x2123,
  0x2124,  0x2125,
  0x2126,  0x2127,
  0x2128,  0x2129,
  0x212A,  0x212B,
  0x212C,  0x212D,
  0x212E,  0x212F,
  0x2130,  0x2131,
  0x2132,  0x2133,
  0x2134,  0x2135,
  0x2136,  0x2137,
  0x2138,  0x2139,
  0x213A,  0x213B,
  0x213C,  0x213D,
  0x213E,  0x213F,
  0x2140,  0x2141,
  0x2142,  0x2143,
  0x2144,  0x2145,
  0x2146,  0x2147,
  0x2148,  0x2149,
  0x214A,  0x214B,
  0x214C,  0x214D,
  0x214E,  0x214F,
  0x2150,  0x2151,
  0x2152,  0x2153,
  0x2154,  0x2155,
  0x2156,  0x2157,
  0x2158,  0x2159,
  0x215A,  0x215B,
  0x215C,  0x215D,
  0x215E,  0x215F,
  0x2160,  0x2161,
  0x2162,  0x2163,
  0x2164,  0x2165,
  0x2166,  0x2167,
  0x2168,  0x2169,
  0x216A,  0x216B,
  0x216C,  0x216D,
  0x216E,  0x216F,
  0x2160,  0x2161,
  0x2162,  0x2163,
  0x2164,  0x2165,
  0x2166,  0x2167,
  0x2168,  0x2169,
  0x216A,  0x216B,
  0x216C,  0x216D,
  0x216E,  0x216F,
  0x2180,  0x2181,
  0x2182,  0x2183,
  0x2184,  0x2185,
  0x2186,  0x2187,
  0x2188,  0x2189,
  0x218A,  0x218B,
  0x218C,  0x218D,
  0x218E,  0x218F,
  0x2190,  0x2191,
  0x2192,  0x2193,
  0x2194,  0x2195,
  0x2196,  0x2197,
  0x2198,  0x2199,
  0x219A,  0x219B,
  0x219C,  0x219D,
  0x219E,  0x219F,
  0x21A0,  0x21A1,
  0x21A2,  0x21A3,
  0x21A4,  0x21A5,
  0x21A6,  0x21A7,
  0x21A8,  0x21A9,
  0x21AA,  0x21AB,
  0x21AC,  0x21AD,
  0x21AE,  0x21AF,
  0x21B0,  0x21B1,
  0x21B2,  0x21B3,
  0x21B4,  0x21B5,
  0x21B6,  0x21B7,
  0x21B8,  0x21B9,
  0x21BA,  0x21BB,
  0x21BC,  0x21BD,
  0x21BE,  0x21BF,
  0x21C0,  0x21C1,
  0x21C2,  0x21C3,
  0x21C4,  0x21C5,
  0x21C6,  0x21C7,
  0x21C8,  0x21C9,
  0x21CA,  0x21CB,
  0x21CC,  0x21CD,
  0x21CE,  0x21CF,
  0x21D0,  0x21D1,
  0x21D2,  0x21D3,
  0x21D4,  0x21D5,
  0x21D6,  0x21D7,
  0x21D8,  0x21D9,
  0x21DA,  0x21DB,
  0x21DC,  0x21DD,
  0x21DE,  0x21DF,
  0x21E0,  0x21E1,
  0x21E2,  0x21E3,
  0x21E4,  0x21E5,
  0x21E6,  0x21E7,
  0x21E8,  0x21E9,
  0x21EA,  0x21EB,
  0x21EC,  0x21ED,
  0x21EE,  0x21EF,
  0x21F0,  0x21F1,
  0x21F2,  0x21F3,
  0x21F4,  0x21F5,
  0x21F6,  0x21F7,
  0x21F8,  0x21F9,
  0x21FA,  0x21FB,
  0x21FC,  0x21FD,
  0x21FE,  0x21FF
};

static uint32_t plane24[] = {
  0x2400,  0x2401,
  0x2402,  0x2403,
  0x2404,  0x2405,
  0x2406,  0x2407,
  0x2408,  0x2409,
  0x240A,  0x240B,
  0x240C,  0x240D,
  0x240E,  0x240F,
  0x2410,  0x2411,
  0x2412,  0x2413,
  0x2414,  0x2415,
  0x2416,  0x2417,
  0x2418,  0x2419,
  0x241A,  0x241B,
  0x241C,  0x241D,
  0x241E,  0x241F,
  0x2420,  0x2421,
  0x2422,  0x2423,
  0x2424,  0x2425,
  0x2426,  0x2427,
  0x2428,  0x2429,
  0x242A,  0x242B,
  0x242C,  0x242D,
  0x242E,  0x242F,
  0x2430,  0x2431,
  0x2432,  0x2433,
  0x2434,  0x2435,
  0x2436,  0x2437,
  0x2438,  0x2439,
  0x243A,  0x243B,
  0x243C,  0x243D,
  0x243E,  0x243F,
  0x2440,  0x2441,
  0x2442,  0x2443,
  0x2444,  0x2445,
  0x2446,  0x2447,
  0x2448,  0x2449,
  0x244A,  0x244B,
  0x244C,  0x244D,
  0x244E,  0x244F,
  0x2450,  0x2451,
  0x2452,  0x2453,
  0x2454,  0x2455,
  0x2456,  0x2457,
  0x2458,  0x2459,
  0x245A,  0x245B,
  0x245C,  0x245D,
  0x245E,  0x245F,
  0x2460,  0x2461,
  0x2462,  0x2463,
  0x2464,  0x2465,
  0x2466,  0x2467,
  0x2468,  0x2469,
  0x246A,  0x246B,
  0x246C,  0x246D,
  0x246E,  0x246F,
  0x2470,  0x2471,
  0x2472,  0x2473,
  0x2474,  0x2475,
  0x2476,  0x2477,
  0x2478,  0x2479,
  0x247A,  0x247B,
  0x247C,  0x247D,
  0x247E,  0x247F,
  0x2480,  0x2481,
  0x2482,  0x2483,
  0x2484,  0x2485,
  0x2486,  0x2487,
  0x2488,  0x2489,
  0x248A,  0x248B,
  0x248C,  0x248D,
  0x248E,  0x248F,
  0x2490,  0x2491,
  0x2492,  0x2493,
  0x2494,  0x2495,
  0x2496,  0x2497,
  0x2498,  0x2499,
  0x249A,  0x249B,
  0x249C,  0x249D,
  0x249E,  0x249F,
  0x24A0,  0x24A1,
  0x24A2,  0x24A3,
  0x24A4,  0x24A5,
  0x24A6,  0x24A7,
  0x24A8,  0x24A9,
  0x24AA,  0x24AB,
  0x24AC,  0x24AD,
  0x24AE,  0x24AF,
  0x24B0,  0x24B1,
  0x24B2,  0x24B3,
  0x24B4,  0x24B5,
  0x24B6,  0x24B7,
  0x24B8,  0x24B9,
  0x24BA,  0x24BB,
  0x24BC,  0x24BD,
  0x24BE,  0x24BF,
  0x24C0,  0x24C1,
  0x24C2,  0x24C3,
  0x24C4,  0x24C5,
  0x24C6,  0x24C7,
  0x24C8,  0x24C9,
  0x24CA,  0x24CB,
  0x24CC,  0x24CD,
  0x24CE,  0x24CF,
  0x24B6,  0x24B7,
  0x24B8,  0x24B9,
  0x24BA,  0x24BB,
  0x24BC,  0x24BD,
  0x24BE,  0x24BF,
  0x24C0,  0x24C1,
  0x24C2,  0x24C3,
  0x24C4,  0x24C5,
  0x24C6,  0x24C7,
  0x24C8,  0x24C9,
  0x24CA,  0x24CB,
  0x24CC,  0x24CD,
  0x24CE,  0x24CF,
  0x24EA,  0x24EB,
  0x24EC,  0x24ED,
  0x24EE,  0x24EF,
  0x24F0,  0x24F1,
  0x24F2,  0x24F3,
  0x24F4,  0x24F5,
  0x24F6,  0x24F7,
  0x24F8,  0x24F9,
  0x24FA,  0x24FB,
  0x24FC,  0x24FD,
  0x24FE,  0x24FF
};

static uint32_t planeFF[] = {
  0xFF00,  0xFF01,
  0xFF02,  0xFF03,
  0xFF04,  0xFF05,
  0xFF06,  0xFF07,
  0xFF08,  0xFF09,
  0xFF0A,  0xFF0B,
  0xFF0C,  0xFF0D,
  0xFF0E,  0xFF0F,
  0xFF10,  0xFF11,
  0xFF12,  0xFF13,
  0xFF14,  0xFF15,
  0xFF16,  0xFF17,
  0xFF18,  0xFF19,
  0xFF1A,  0xFF1B,
  0xFF1C,  0xFF1D,
  0xFF1E,  0xFF1F,
  0xFF20,  0xFF21,
  0xFF22,  0xFF23,
  0xFF24,  0xFF25,
  0xFF26,  0xFF27,
  0xFF28,  0xFF29,
  0xFF2A,  0xFF2B,
  0xFF2C,  0xFF2D,
  0xFF2E,  0xFF2F,
  0xFF30,  0xFF31,
  0xFF32,  0xFF33,
  0xFF34,  0xFF35,
  0xFF36,  0xFF37,
  0xFF38,  0xFF39,
  0xFF3A,  0xFF3B,
  0xFF3C,  0xFF3D,
  0xFF3E,  0xFF3F,
  0xFF40,  0xFF21,
  0xFF22,  0xFF23,
  0xFF24,  0xFF25,
  0xFF26,  0xFF27,
  0xFF28,  0xFF29,
  0xFF2A,  0xFF2B,
  0xFF2C,  0xFF2D,
  0xFF2E,  0xFF2F,
  0xFF30,  0xFF31,
  0xFF32,  0xFF33,
  0xFF34,  0xFF35,
  0xFF36,  0xFF37,
  0xFF38,  0xFF39,
  0xFF3A,  0xFF5B,
  0xFF5C,  0xFF5D,
  0xFF5E,  0xFF5F,
  0xFF60,  0xFF61,
  0xFF62,  0xFF63,
  0xFF64,  0xFF65,
  0xFF66,  0xFF67,
  0xFF68,  0xFF69,
  0xFF6A,  0xFF6B,
  0xFF6C,  0xFF6D,
  0xFF6E,  0xFF6F,
  0xFF70,  0xFF71,
  0xFF72,  0xFF73,
  0xFF74,  0xFF75,
  0xFF76,  0xFF77,
  0xFF78,  0xFF79,
  0xFF7A,  0xFF7B,
  0xFF7C,  0xFF7D,
  0xFF7E,  0xFF7F,
  0xFF80,  0xFF81,
  0xFF82,  0xFF83,
  0xFF84,  0xFF85,
  0xFF86,  0xFF87,
  0xFF88,  0xFF89,
  0xFF8A,  0xFF8B,
  0xFF8C,  0xFF8D,
  0xFF8E,  0xFF8F,
  0xFF90,  0xFF91,
  0xFF92,  0xFF93,
  0xFF94,  0xFF95,
  0xFF96,  0xFF97,
  0xFF98,  0xFF99,
  0xFF9A,  0xFF9B,
  0xFF9C,  0xFF9D,
  0xFF9E,  0xFF9F,
  0xFFA0,  0xFFA1,
  0xFFA2,  0xFFA3,
  0xFFA4,  0xFFA5,
  0xFFA6,  0xFFA7,
  0xFFA8,  0xFFA9,
  0xFFAA,  0xFFAB,
  0xFFAC,  0xFFAD,
  0xFFAE,  0xFFAF,
  0xFFB0,  0xFFB1,
  0xFFB2,  0xFFB3,
  0xFFB4,  0xFFB5,
  0xFFB6,  0xFFB7,
  0xFFB8,  0xFFB9,
  0xFFBA,  0xFFBB,
  0xFFBC,  0xFFBD,
  0xFFBE,  0xFFBF,
  0xFFC0,  0xFFC1,
  0xFFC2,  0xFFC3,
  0xFFC4,  0xFFC5,
  0xFFC6,  0xFFC7,
  0xFFC8,  0xFFC9,
  0xFFCA,  0xFFCB,
  0xFFCC,  0xFFCD,
  0xFFCE,  0xFFCF,
  0xFFD0,  0xFFD1,
  0xFFD2,  0xFFD3,
  0xFFD4,  0xFFD5,
  0xFFD6,  0xFFD7,
  0xFFD8,  0xFFD9,
  0xFFDA,  0xFFDB,
  0xFFDC,  0xFFDD,
  0xFFDE,  0xFFDF,
  0xFFE0,  0xFFE1,
  0xFFE2,  0xFFE3,
  0xFFE4,  0xFFE5,
  0xFFE6,  0xFFE7,
  0xFFE8,  0xFFE9,
  0xFFEA,  0xFFEB,
  0xFFEC,  0xFFED,
  0xFFEE,  0xFFEF,
  0xFFF0,  0xFFF1,
  0xFFF2,  0xFFF3,
  0xFFF4,  0xFFF5,
  0xFFF6,  0xFFF7,
  0xFFF8,  0xFFF9,
  0xFFFA,  0xFFFB,
  0xFFFC,  0xFFFD,
  0xFFFE,  0xFFFF
};

static uint32_t *mysql_unicode_normalize_table[256] = {
 plane00, plane01, plane02, plane03, plane04, plane05,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL, plane1E, plane1F,
    NULL, plane21,    NULL,    NULL, plane24,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,
    NULL,    NULL,    NULL,    NULL,    NULL,    NULL,    NULL, planeFF

};
/* based on mysql-5.5.29/strings/ctype-utf8.c: end */

static inline unsigned int
unichar_to_utf8(uint32_t unichar, char *output)
{
  unsigned int n_bytes;

  if (unichar < 0x80) {
    output[0] = unichar;
    n_bytes = 1;
  } else if (unichar < 0x0800) {
    output[0] = ((unichar >> 6) & 0x1f) | 0xc0;
    output[1] = (unichar & 0x3f) | 0x80;
    n_bytes = 2;
  } else if (unichar < 0x10000) {
    output[0] = (unichar >> 12) | 0xe0;
    output[1] = ((unichar >> 6) & 0x3f) | 0x80;
    output[2] = (unichar & 0x3f) | 0x80;
    n_bytes = 3;
  } else if (unichar < 0x200000) {
    output[0] = (unichar >> 18) | 0xf0;
    output[1] = ((unichar >> 12) & 0x3f) | 0x80;
    output[2] = ((unichar >> 6) & 0x3f) | 0x80;
    output[3] = (unichar & 0x3f) | 0x80;
    n_bytes = 4;
  } else if (unichar < 0x4000000) {
    output[0] = (unichar >> 24) | 0xf8;
    output[1] = ((unichar >> 18) & 0x3f) | 0x80;
    output[2] = ((unichar >> 12) & 0x3f) | 0x80;
    output[3] = ((unichar >> 6) & 0x3f) | 0x80;
    output[4] = (unichar & 0x3f) | 0x80;
    n_bytes = 5;
  } else {
    output[0] = (unichar >> 30) | 0xfc;
    output[1] = ((unichar >> 24) & 0x3f) | 0x80;
    output[2] = ((unichar >> 18) & 0x3f) | 0x80;
    output[3] = ((unichar >> 12) & 0x3f) | 0x80;
    output[4] = ((unichar >> 6) & 0x3f) | 0x80;
    output[5] = (unichar & 0x3f) | 0x80;
    n_bytes = 6;
  }

  return n_bytes;
}

static void
normalize(grn_ctx *ctx, grn_obj *string)
{
  const char *original, *rest;
  unsigned int original_length_in_bytes, rest_length;
  char *normalized;
  unsigned int normalized_length_in_bytes = 0;
  unsigned int normalized_n_characters = 0;
  grn_encoding encoding;
  int flags;
  grn_bool remove_blank_p;

  encoding = grn_string_get_encoding(ctx, string);
  flags = grn_string_get_flags(ctx, string);
  remove_blank_p = flags & GRN_STRING_REMOVE_BLANK;
  grn_string_get_original(ctx, string, &original, &original_length_in_bytes);
  normalized = GRN_PLUGIN_MALLOC(ctx, original_length_in_bytes + 1);
  rest = original;
  rest_length = original_length_in_bytes;
  while (rest_length > 0) {
    int character_length;
    int plane;
    uint32_t low_code;

    character_length = grn_tokenizer_charlen(ctx, rest, rest_length, encoding);
    if (character_length == 0) {
      break;
    }

    switch (character_length) {
    case 1 :
      plane = 0x00;
      low_code = rest[0] & 0x7f;
      break;
    case 2 :
      plane = 0x00;
      low_code = (rest[0] & 0x1f << 6) + (rest[1] & 0x3f);
      break;
    case 3 :
      plane = rest[0] & 0x0f;
      low_code =
        ((rest[0] & 0x0f) << 12) +
        ((rest[1] & 0x3f) << 6) +
        (rest[2] & 0x3f);
      break;
    case 4 :
      plane = ((rest[0] & 0x07) << 6) + (rest[1] & 0x3f);
      low_code =
        ((rest[0] & 0x07) << 15) +
        ((rest[1] & 0x3f) << 12) +
        ((rest[2] & 0x3f) << 6) +
        (rest[3] & 0x3f);
      if (plane > 0xff) {
        plane = -1;
      }
      break;
    default :
      plane = -1;
      low_code = 0x00;
      break;
    }

    if (remove_blank_p && character_length == 1 && rest[0] == ' ') {
      /* TODO: set GRN_CHAR_BLANK */
    } else {
      if (plane >= 0x00 && mysql_unicode_normalize_table[plane]) {
        uint32_t normalized_code;
        unsigned int n_bytes;
        normalized_code = mysql_unicode_normalize_table[plane][low_code];
        n_bytes = unichar_to_utf8(normalized_code,
                                  normalized + normalized_length_in_bytes);
        normalized_length_in_bytes += n_bytes;
      } else {
        int i;
        for (i = 0; i < character_length; i++) {
          normalized[normalized_length_in_bytes + i] = rest[i];
        }
        normalized_length_in_bytes += character_length;
      }
      normalized_n_characters++;
    }

    rest += character_length;
    rest_length -= character_length;
  }

  if (rest_length == 0) {
    grn_string_set_normalized(ctx,
                              string,
                              normalized,
                              normalized_length_in_bytes,
                              normalized_n_characters);
  } else {
    /* TODO: report error */
    GRN_PLUGIN_FREE(ctx, normalized);
  }
}

static grn_obj *
mysql_general_ci_next(GNUC_UNUSED grn_ctx *ctx,
                      GNUC_UNUSED int nargs,
                      grn_obj **args,
                      GNUC_UNUSED grn_user_data *user_data)
{
  grn_obj *string = args[0];
  grn_encoding encoding;

  encoding = grn_string_get_encoding(ctx, string);
  if (encoding != GRN_ENC_UTF8) {
    GRN_PLUGIN_ERROR(ctx,
                     GRN_FUNCTION_NOT_IMPLEMENTED,
                     "[normalizer][mysql-general-ci] "
                     "UTF-8 encoding is only supported: %s",
                     grn_encoding_to_string(encoding));
    return NULL;
  }
  normalize(ctx, string);
  return NULL;
}

grn_rc
GRN_PLUGIN_INIT(grn_ctx *ctx)
{
  return ctx->rc;
}

grn_rc
GRN_PLUGIN_REGISTER(grn_ctx *ctx)
{
  grn_normalizer_register(ctx, "NormalizerMySQLGeneralCI", -1,
                          NULL, mysql_general_ci_next, NULL);
  return GRN_SUCCESS;
}

grn_rc
GRN_PLUGIN_FIN(GNUC_UNUSED grn_ctx *ctx)
{
  return GRN_SUCCESS;
}
