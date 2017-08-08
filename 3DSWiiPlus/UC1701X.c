#ifdef LCM_DRIVER_UC1701X


#include "UC1701X.h"
#include "LCM12864.h"
#include "SPI.h"


#define USE_SOFTWARE_HFLIP


// 显存缓冲区数据格式
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
//   +-+-+-+-+-+-+-+-+-+-+ <--- 12832到此为止
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
//   +-+-+-+-+-+-+-+-+-+-+ <--- 12864到此为止


#ifdef USE_SOFTWARE_HFLIP
bit isHFlip;
#endif


/*
开/关显示
*/
static void UC1701X_SetDisplay(bit isOn){
	if(isOn)
		LCM12864_SendCommand(0xAF);
	else
		LCM12864_SendCommand(0xAE);
}

/*
设置Y方向卷轴偏移量
*/
static void UC1701X_SetYOffset(u8 offset){
	LCM12864_SendCommand(0x40 + (offset & 0x3F));
}

/*
设置页地址
*/
static void UC1701X_SetPageAddr(u8 page){
	LCM12864_SendCommand(0xB0 + (page & 7));
}

/*
设置列地址
*/
static void UC1701X_SetColumnAddr(u8 col){
	u8 hi, lo;
	col &= 0x7F;
	hi = col >> 4;
	lo = col & 0xF;
	LCM12864_SendCommand(0x10 + hi);
	LCM12864_SendCommand(lo);
}

/*
开/关水平翻转
*/
static void UC1701X_SetFlipX(bit isFlipX){
#ifdef USE_SOFTWARE_HFLIP
	isHFlip = isFlipX;
#else
	if(isFlipX)
		LCM12864_SendCommand(0xA1);
	else
		LCM12864_SendCommand(0xA0);
#endif
}

/*
开/关垂直翻转
*/
static void UC1701X_SetFlipY(bit isFlipY){
	if(isFlipY)
		LCM12864_SendCommand(0xC8);
	else
		LCM12864_SendCommand(0xC0);
}

/*
反显/正显
*/
static void UC1701X_SetNegative(bit isNegative){
	if(isNegative)
		LCM12864_SendCommand(0xA7);
	else
		LCM12864_SendCommand(0xA6);
}

/*
全显示
*/
static void UC1701X_DisplayAll(bit isDisplayAll){
	if(isDisplayAll)
		LCM12864_SendCommand(0xA5);
	else
		LCM12864_SendCommand(0xA4);
}

/*
设置偏压比
*/
static void UC1701X_SetBias(bit bias){
	if(bias)
		LCM12864_SendCommand(0xA3);
	else
		LCM12864_SendCommand(0xA2);
}

/*
软复位
*/
static void UC1701X_SoftReset(){
	LCM12864_SendCommand(0xE2);
}

/*
启动内部电源管理
*/
static void UC1701X_SetPowerControl(){
	LCM12864_SendCommand(0x2F);
}

/*
粗调对比度
*/
static void UC1701X_SetContrast0(u8 contrast){
	LCM12864_SendCommand(0x20 + (contrast & 7));
}

/*
精调对比度
*/
static void UC1701X_SetContrast1(u8 contrast){
	LCM12864_SendCommand(0x81);
	LCM12864_SendCommand(contrast & 0x3F);
}

/*
空操作
*/
static void UC1701X_Nop(){
	LCM12864_SendCommand(0xE3);
}

void UC1701X_Init(){
#ifdef USE_SOFTWARE_HFLIP
	isHFlip = 0;
#endif
	
	LCM12864_CS = 0;
	Delay(100);
	UC1701X_SoftReset();
	Delay(100);
	UC1701X_SetPowerControl();
	Delay(5);
	UC1701X_SetContrast0(3);
	UC1701X_SetContrast1(0x28);
	UC1701X_SetBias(0);
	UC1701X_SetFlipX(1);
	UC1701X_SetFlipY(0);
	UC1701X_SetYOffset(0);
	UC1701X_SetDisplay(1);
	LCM12864_CS = 1;
}

void UC1701X_Refresh(){
	u8 x, y;
	LCM12864_CS = 0;
#ifdef USE_12832
	for(y=0;y<4;++y){
#else
	for(y=0;y<8;++y){
#endif
		UC1701X_SetPageAddr(y);
		UC1701X_SetColumnAddr(0);
		for(x=0;x<128;++x){
#ifdef USE_SOFTWARE_HFLIP
			if(isHFlip)
				LCM12864_SendData(VRAM[(127 - x) + (y << 7)]);
			else
				LCM12864_SendData(VRAM[x + (y << 7)]);
#else
			//UC1701X_SetColumnAddr(x);
			LCM12864_SendData(VRAM[x + (y << 7)]);
#endif
		}
	}
	LCM12864_CS = 1;
}

/*
将字符缓冲区中的字符绘制到VRAM缓冲区
x最小取值为-fontSize，最大取值为127
y最小取值为-fontSize，最大取值为31或63
*/
void UC1701X_DrawChar(u8 fontSize, s8 x, s8 y, bit c){
	u8 i;
	s8 page = y >> 3;
    if(x < -fontSize)x = -fontSize;
    if(x > 127)return;
    if(y < -fontSize)y = -fontSize;
#ifdef USE_12832
	if(y > 31)return;
#else
	if(y > 63)return;
#endif
	
	for(i=0;i<fontSize;++i){
		// 如果水平方向小于0像素则不再进行绘制
		if(x + i < 0)continue;
		// 如果水平方向超过128像素则不再进行绘制
		if(x + i < 128){
			// 获得VRAM偏移量
			s16 vramOffset = x + i + (page << 7);
			// 获得页内偏移量
			u8 offsetInPage = y & 7;
			if(vramOffset >= 0){
				// 写数据
				if(c)
					VRAM[vramOffset] |= (tmpBuf[i] << offsetInPage) & 0xFF;
				else
					VRAM[vramOffset] &= ~(tmpBuf[i] << offsetInPage) & 0xFF;
			}
			// 偏移到下一个页
			vramOffset += 128;
			// 如果垂直方向超过32或64像素则不再进行绘制
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
			// 如果垂直方向超过32或64像素则不再进行绘制
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
}

/*
在缓冲区任意位置画点
*/
void UC1701X_DrawPixel(u8 x, u8 y, bit c){
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
在缓冲区任意位置填充矩形
*/
void UC1701X_FillRect(u8 x, u8 y, u8 w, u8 h, bit c){
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


#endif
