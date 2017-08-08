#ifdef LCM_DRIVER_SSD1306


#include "SSD1306.h"
#include "LCM12864.h"
#include "SPI.h"


// �Դ滺�������ݸ�ʽ
//    0 1 2 3 4 5 6 7 . 7F
//   +-+-+-+-+-+-+-+-+-+-+
// 00|B|B|B|B|B|B|B|B|.|B|
// 01|Y|Y|Y|Y|Y|Y|Y|Y|.|Y|
// 02|T|T|T|T|T|T|T|T|.|T|
// 03|E|E|E|E|E|E|E|E|.|E|
// 04| | | | | | | | |.| |
// 05|0|0|0|0|0|0|0|0|.|0|
// 06|0|0|0|0|0|0|0|0|.|7|
// 07|0|1|2|3|4|5|6|7|.|F|
//   +-+-+-+-+-+-+-+-+-+-+
// 08|B|B|B|B|B|B|B|B|.|B|
// 09|Y|Y|Y|Y|Y|Y|Y|Y|.|Y|
// 0A|T|T|T|T|T|T|T|T|.|T|
// 0B|E|E|E|E|E|E|E|E|.|E|
// 0C| | | | | | | | |.| |
// 0D|0|0|0|0|0|0|0|0|.|0|
// 0E|8|8|8|8|8|8|8|8|.|F|
// 0F|0|1|2|3|4|5|6|7|.|F|
//   +-+-+-+-+-+-+-+-+-+-+
//   |.|.|.|.|.|.|.|.|.|.|
//   +-+-+-+-+-+-+-+-+-+-+
// 18|B|B|B|B|B|B|B|B|.|B|
// 19|Y|Y|Y|Y|Y|Y|Y|Y|.|Y|
// 1A|T|T|T|T|T|T|T|T|.|T|
// 1B|E|E|E|E|E|E|E|E|.|E|
// 1C| | | | | | | | |.| |
// 1D|1|1|1|1|1|1|1|1|.|1|
// 1E|8|8|8|8|8|8|8|8|.|F|
// 1F|0|1|2|3|4|5|6|7|.|F|
//   +-+-+-+-+-+-+-+-+-+-+ <--- 12832����Ϊֹ
//   |.|.|.|.|.|.|.|.|.|.|
//   +-+-+-+-+-+-+-+-+-+-+
// 38|B|B|B|B|B|B|B|B|.|B|
// 39|Y|Y|Y|Y|Y|Y|Y|Y|.|Y|
// 3A|T|T|T|T|T|T|T|T|.|T|
// 3B|E|E|E|E|E|E|E|E|.|E|
// 3C| | | | | | | | |.| |
// 3D|3|3|3|3|3|3|3|3|.|3|
// 3E|8|8|8|8|8|8|8|8|.|F|
// 3F|0|1|2|3|4|5|6|7|.|F|
//   +-+-+-+-+-+-+-+-+-+-+ <--- 12864����Ϊֹ


void SSD1306_Init(){
	LCM12864_CS = 0;
	Delay(100);
	SSD1306_SoftReset();
	Delay(100);
	SSD1306_SetPowerControl();
	Delay(5);
	SSD1306_SetContrast0(3);
	SSD1306_SetContrast1(0x28);
	SSD1306_SetBias(0);
	SSD1306_SetFlipX(1);
	SSD1306_SetFlipY(0);
	SSD1306_SetYOffset(0);
	SSD1306_SetDisplay(1);
	LCM12864_CS = 1;
}

void SSD1306_Refresh(){
	u8 x, y;
	LCM12864_CS = 0;
#ifdef USE_12832
	for(y=0;y<4;++y){
#else
	for(y=0;y<8;++y){
#endif
		SSD1306_SetPageAddr(y);
		SSD1306_SetColumnAddr(0);
		for(x=0;x<128;++x){
#ifdef USE_SOFTWARE_HFLIP
			if(isHFlip)
				SSD1306_SendData(VRAM[(127 - x) + (y << 7)]);
			else
				SSD1306_SendData(VRAM[x + (y << 7)]);
#else
			//SSD1306_SetColumnAddr(x);
			SSD1306_SendData(VRAM[x + (y << 7)]);
#endif
		}
	}
	LCM12864_CS = 1;
}

/*
��/����ʾ
*/
void SSD1306_SetDisplay(bit isOn){
	if(isOn)
		SSD1306_SendCommand(0xAF);
	else
		SSD1306_SendCommand(0xAE);
}

/*
����Y�������ƫ����
*/
void SSD1306_SetYOffset(u8 offset){
	SSD1306_SendCommand(0x40 + (offset & 0x3F));
}

/*
����ҳ��ַ
*/
void SSD1306_SetPageAddr(u8 page){
	SSD1306_SendCommand(0xB0 + (page & 7));
}

/*
�����е�ַ
*/
void SSD1306_SetColumnAddr(u8 col){
	u8 hi, lo;
	col &= 0x7F;
	hi = col >> 4;
	lo = col & 0xF;
	SSD1306_SendCommand(0x10 + hi);
	SSD1306_SendCommand(lo);
}

/*
��/��ˮƽ��ת
*/
void SSD1306_SetFlipX(bit isFlipX){
#ifdef USE_SOFTWARE_HFLIP
	isHFlip = isFlipX;
#else
	if(isFlipX)
		SSD1306_SendCommand(0xA1);
	else
		SSD1306_SendCommand(0xA0);
#endif
}

/*
��/�ش�ֱ��ת
*/
void SSD1306_SetFlipY(bit isFlipY){
	if(isFlipY)
		SSD1306_SendCommand(0xC8);
	else
		SSD1306_SendCommand(0xC0);
}

/*
����/����
*/
void SSD1306_SetNegative(bit isNegative){
	if(isNegative)
		SSD1306_SendCommand(0xA7);
	else
		SSD1306_SendCommand(0xA6);
}

/*
ȫ��ʾ
*/
void SSD1306_DisplayAll(bit isDisplayAll){
	if(isDisplayAll)
		SSD1306_SendCommand(0xA5);
	else
		SSD1306_SendCommand(0xA4);
}

/*
����ƫѹ��
*/
void SSD1306_SetBias(bit bias){
	if(bias)
		SSD1306_SendCommand(0xA3);
	else
		SSD1306_SendCommand(0xA2);
}

/*
��λ
*/
void SSD1306_SoftReset(){
	SSD1306_SendCommand(0xE2);
}

/*
�����ڲ���Դ����
*/
void SSD1306_SetPowerControl(){
	SSD1306_SendCommand(0x2F);
}

/*
���ڶԱȶ�
��˵��ʵ���ڶԱȶ�Ч��Ҳ����ô���ԣ����ʤ���ޡ���
*/
void SSD1306_SetContrast(u8 contrast){
	SSD1306_SendCommand(0x81);
	SSD1306_SendCommand(contrast);
}

/*
�ղ���
*/
void SSD1306_Nop(){
	SSD1306_SendCommand(0xE3);
}

/*
���ַ����32λ��ַƫ����
*/
u32 SSD1306_GetOffsetFromChar(u8 fontSize, u16 chr){
	//u8 hi = chr >> 8;
	//u8 lo = chr & 0xFF;
	//u32 offset = ((lo - 0x40) + (0xFE - 0x40 + 1) * (hi - 0x81)) * 2 * fontSize;
	//return offset;
	u8 i = 0;
	for(;i<CHARACTER_COUNT;++i){
		if(textMap[i] == chr)
			break;
	}
	if(i >= CHARACTER_COUNT)
		i = 0;

	return i * 2 * fontSize;
}

static bit WinbondFlash_Read(u32 offset, u8* buf, u8 size){
	memcpy(buf, fontText + offset, size);
	return 1;
}

/*
��ָ���ַ����Ƶ�VRAM������
x��СȡֵΪ-fontSize�����ȡֵΪ127
y��СȡֵΪ-fontSize�����ȡֵΪ31��63
*/
bit SSD1306_DrawChar(u8 fontSize, u16 chr, s8 x, s8 y, bit c){
	u8 i;
    if(x < -fontSize)x = -fontSize;
    if(x > 127)return 1;
    if(y < -fontSize)y = -fontSize;
#ifdef USE_12832
	if(y > 31)return 1;
#else
	if(y > 63)return 1;
#endif
	if(WinbondFlash_Read(SSD1306_GetOffsetFromChar(fontSize, chr), (u8*)tmpBuf, fontSize * sizeof(u16))){
		s8 page = y >> 3;
		for(i=0;i<fontSize;++i){
            // ���ˮƽ����С��0�������ٽ��л���
            if(x + i < 0)continue;
			// ���ˮƽ���򳬹�128�������ٽ��л���
			if(x + i < 128){
				// ���VRAMƫ����
				s16 vramOffset = x + i + (page << 7);
                // ���ҳ��ƫ����
                u8 offsetInPage = y & 7;
                if(vramOffset >= 0){
                    // д����
                    if(c)
                        VRAM[vramOffset] |= (tmpBuf[i] << offsetInPage) & 0xFF;
                    else
                        VRAM[vramOffset] &= ~(tmpBuf[i] << offsetInPage) & 0xFF;
                }
				// ƫ�Ƶ���һ��ҳ
				vramOffset += 128;
				// �����ֱ���򳬹�32��64�������ٽ��л���
#ifdef USE_12832
				if(vramOffset >= 512)continue;
#else
				if(vramOffset >= 1024)continue;
#endif
                if(vramOffset >= 0){
                    if(c)
                        VRAM[vramOffset] |= (tmpBuf[i] >> (8 - offsetInPage)) & 0xFF;
                    else
                        VRAM[vramOffset] &= ~(tmpBuf[i] >> (8 - offsetInPage)) & 0xFF;
                }
				vramOffset += 128;
				// �����ֱ���򳬹�32��64�������ٽ��л���
#ifdef USE_12832
				if(vramOffset >= 512)continue;
#else
				if(vramOffset >= 1024)continue;
#endif
                if(vramOffset >= 0){
                    if(c)
                        VRAM[vramOffset] |= (tmpBuf[i] >> (16 - offsetInPage)) & 0xFF;
                    else
                        VRAM[vramOffset] &= ~(tmpBuf[i] >> (16 - offsetInPage)) & 0xFF;
                }
			}
		}
		return 1;
	}
	return 0;
}

void SSD1306_DrawString(u8 fontSize, const char* str, s16 x, s16 y, bit c){
	u8 i = 0, len = strlen(str);
	while(i < len){
		if((u8)str[i] < 0x81){
            if(x >= -fontSize && x <= 127)
                SSD1306_DrawChar(fontSize, str[i], x, y, c);
			i++;
			x += (fontSize >> 1);
		}else{
			u16 tmp = str[i];
			tmp <<= 8;
			tmp |= (u8)str[i + 1];
            if(x >= -fontSize && x <= 127)
                SSD1306_DrawChar(fontSize, tmp, x, y, c);
			i += 2;
			x += fontSize;
		}
	}
}

/*
�ڻ���������λ�û���
*/
void SSD1306_DrawPixel(u8 x, u8 y, bit c){
	if(x > 127)return;
#ifdef USE_12832
	if(y > 31)return;
#else
	if(y > 63)return;
#endif
	if(c)
		VRAM[x + (y >> 3 << 7)] |= 1 << (y & 7);
	else
		VRAM[x + (y >> 3 << 7)] &= ~(1 << (y & 7));
}

/*
�ڻ���������λ�û�ֱ��
*/
/*
void SSD1306_DrawLine(u8 x0, u8 y0, u8 x1, u8 y1, bit c){
	s8 dx = x1 - x0;
	s8 dy = y1 - y0;
	s8 ux = ((dx > 0) << 1) - 1;	// x��������������1
	s8 uy = ((dy > 0) << 1) - 1;	// y��������������1
	s8 x = x0, y = y0, eps = 0;		// epsΪ�ۼ����

	dx = ABS(dx);
	dy = ABS(dy); 
	if(dx > dy){
		for(;x!=x1;x+=ux){
			SSD1306_DrawPixel(x, y, c);
			eps += dy;
			if((eps << 1) >= dx){
				y += uy;
				eps -= dx;
			}
		}
	}else{
		for(;y!=y1;y+=uy){
			SSD1306_DrawPixel(x, y, c);
			eps += dx;
			if((eps << 1) >= dy){
				x += ux;
				eps -= dy;
			}
		}
	}
}
*/

/*
�ڻ���������λ��������
*/
void SSD1306_FillRect(u8 x, u8 y, u8 w, u8 h, bit c){
	u8 i, j;
	if(x > 127)return;
	if(x + w > 128)w = 128 - x;
#ifdef USE_12832
	if(y > 31)return;
	if(y + h > 32)h = 32 - y;
#else
	if(y > 63)return;
	if(y + h > 64)h = 64 - y;
#endif
	
	for(i=x;i<=x+w-1;++i){
		u8 page0 = y >> 3;
		u8 page1 = (y + h) >> 3;
		u8 _y0 = y & 7;
		u8 _y1 = (y + h - 1) & 7;
		if(page0 == page1){
			u8 mask = 0;
			for(j=_y0;j<=_y1;++j){
				mask |= (1 << j);
			}
			if(c){
				VRAM[i + (page0 << 7)] |= mask;
			}else{
				VRAM[i + (page0 << 7)] &= ~mask;
			}
		}else{
			for(j=page0;j<=page1;++j){
				if(j == page0){
					if(c)
						VRAM[i + (j << 7)] |= (0xFF << _y0);
					else
						VRAM[i + (j << 7)] &= ~(0xFF << _y0);
				}else if(j == page1){
					if(c)
						VRAM[i + (j << 7)] |= (0xFF >> (7 - _y1));
					else
						VRAM[i + (j << 7)] &= ~(0xFF >> (7 - _y1));
				}else{
					if(c)
						VRAM[i + (j << 7)] = 0xFF;
					else
						VRAM[i + (j << 7)] = 0;
				}
			}
		}
	}
}

/*
�ڻ���������λ�û�Բ
*/
/*
void SSD1306_DrawCircle(u8 x1, u8 y1, u8 r, bit c){
	s16 x = 0, y = r;
	s8 d = 1 - r;
	while(y > x){
		SSD1306_DrawPixel(x1 + x, y1 + y, c);
		SSD1306_DrawPixel(x1 + y, y1 + x, c);
		SSD1306_DrawPixel(x1 - x, y1 + y, c);
		SSD1306_DrawPixel(x1 - y, y1 + x, c);
		SSD1306_DrawPixel(x1 - x, y1 - y, c);
		SSD1306_DrawPixel(x1 - y, y1 - x, c);
		SSD1306_DrawPixel(x1 + x, y1 - y, c);
		SSD1306_DrawPixel(x1 + y, y1 - x, c);
		if(d < 0){
			d += 2 * x + 3;
		}else{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
	}
}
*/


#endif
