/*
 * LCD_int.h
 *
 *  Created on: Nov 10, 2021
 *      Author: Ahmed El-Gaafrawy
 */

#ifndef LCD_INT_H_
#define LCD_INT_H_

#include "LCD_config.h"
ES_t LCD_enuInit(void);

ES_t LCD_enuSendCommand(u8 Copy_u8Byte);

ES_t LCD_enuSendData(u8 Copy_u8Byte);

ES_t LCD_enuSendString(const char * Copy_ccString);

ES_t LCD_enuSendIntegerNum(s32 Copy_s32Num);

ES_t LCD_enuSendFloatNum(f32 Copy_f32Num);
void LCD_voidSendPosition(u8 Copy_LineNUM,u8 Copy_u8Col);

#endif /* LCD_INT_H_ */
