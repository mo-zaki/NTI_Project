/*
 * LCD_prog.c
 *
 *  Created on: Nov 10, 2021
 *      Author: Ahmed El-Gaafrawy
 */
#include "../../LIBRARY/stdTypes.h"
#include "../../LIBRARY/errorState.h"

#include "../../MCAL/DIO/DIO_int.h"

#include "LCD_priv.h"
#include "LCD_config.h"

#include <util/delay.h>

ES_t LCD_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	DIO_enuSetPinDirection(RS_PORT, RS_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(RW_PORT, RW_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(EN_PORT, EN_PIN, DIO_u8OUTPUT);

	DIO_enuSetPinDirection(D7_PORT, D7_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D6_PORT, D6_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D5_PORT, D5_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D4_PORT, D4_PIN, DIO_u8OUTPUT);

#if LCD_MODE == EIGHT_BIT

	DIO_enuSetPinDirection(D3_PORT, D3_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D2_PORT, D2_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D1_PORT, D1_PIN, DIO_u8OUTPUT);
	DIO_enuSetPinDirection(D0_PORT, D0_PIN, DIO_u8OUTPUT);

	_delay_ms(35);
	DIO_enuSetPinValue(RS_PORT, RS_PIN, DIO_u8LOW);
	WriteNLatch(0x38);

#elif LCD_MODE == FOUR_BIT
	_delay_ms(35);

	//function set
	DIO_enuSetPinValue(RS_PORT, RS_PIN, DIO_u8LOW);
	WriteNLatch(0x28);

#endif

	//display on/off
	_delay_ms(1);
	DIO_enuSetPinValue(RS_PORT, RS_PIN, DIO_u8LOW);
	WriteNLatch(0x0F);

	//clear display
	_delay_ms(1);
	DIO_enuSetPinValue(RS_PORT, RS_PIN, DIO_u8LOW);
	WriteNLatch(0x01);

	//entry mode
	_delay_ms(2);
	DIO_enuSetPinValue(RS_PORT, RS_PIN, DIO_u8LOW);
	WriteNLatch(0x06);

	return Local_enuErrorState;
}

ES_t LCD_enuSendString(const char * Copy_ccString)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_ccString != NULL)
	{
		while (*Copy_ccString != '\0')
		{
			DIO_enuSetPinValue(RS_PORT, RS_PIN, DIO_u8HIGH);
			WriteNLatch(*Copy_ccString);

			Copy_ccString++;
		}
	}
	else
	{
		Local_enuErrorState = ES_NULL_POINTER;
	}

	return Local_enuErrorState;
}


ES_t LCD_enuSendCommand(u8 Copy_u8Byte)
{
	ES_t Local_enuErrorState = ES_NOK;

	DIO_enuSetPinValue(RS_PORT, RS_PIN, DIO_u8LOW);
	WriteNLatch(Copy_u8Byte);

	return Local_enuErrorState;
}

ES_t LCD_enuSendData(u8 Copy_u8Byte)
{
	ES_t Local_enuErrorState = ES_NOK;

	DIO_enuSetPinValue(RS_PORT, RS_PIN, DIO_u8HIGH);
	WriteNLatch(Copy_u8Byte);

	return Local_enuErrorState;
}


static ES_t WriteNLatch (u8 Copy_u8Byte)
{
	ES_t Local_enuErrorState = ES_NOK;

	ES_t arr[12]={ES_NOK,ES_NOK,ES_NOK,ES_NOK,ES_NOK,ES_NOK
			,ES_NOK,ES_NOK,ES_NOK,ES_NOK,ES_NOK,ES_NOK};

	arr[0]=DIO_enuSetPinValue(RW_PORT, RW_PIN, DIO_u8LOW);
	arr[1]=DIO_enuSetPinValue(EN_PORT, EN_PIN, DIO_u8LOW);

#if LCD_MODE == EIGHT_BIT
	arr[2]=DIO_enuSetPinValue(D7_PORT , D7_PIN,   ((Copy_u8Byte>>7)&1)  );
	arr[3]=DIO_enuSetPinValue(D6_PORT , D6_PIN,   ((Copy_u8Byte>>6)&1)  );
	arr[4]=DIO_enuSetPinValue(D5_PORT , D5_PIN,   ((Copy_u8Byte>>5)&1)  );
	arr[5]=DIO_enuSetPinValue(D4_PORT , D4_PIN,   ((Copy_u8Byte>>4)&1)  );
	arr[6]=DIO_enuSetPinValue(D3_PORT , D3_PIN,   ((Copy_u8Byte>>3)&1)  );
	arr[7]=DIO_enuSetPinValue(D2_PORT , D2_PIN,   ((Copy_u8Byte>>2)&1)  );
	arr[8]=DIO_enuSetPinValue(D1_PORT , D1_PIN,   ((Copy_u8Byte>>1)&1)  );
	arr[9]=DIO_enuSetPinValue(D0_PORT , D0_PIN,   ((Copy_u8Byte>>0)&1)  );

	arr[10]=DIO_enuSetPinValue(EN_PORT,EN_PIN, DIO_u8HIGH);
	_delay_ms(5);
	arr[11]=DIO_enuSetPinValue(EN_PORT, EN_PIN, DIO_u8LOW);
	_delay_ms(5);

	for (u8 i=0; i<12;i++)
	{
		if (arr[i] != ES_OK)
		{
			return ES_NOK;
		}
	}

	Local_enuErrorState = ES_OK;

#elif LCD_MODE == FOUR_BIT

	if (Copy_u8Byte == 0x28)
	{
		DIO_enuSetPinValue(D7_PORT , D7_PIN,   ((Copy_u8Byte>>7)&1)  );
		DIO_enuSetPinValue(D6_PORT , D6_PIN,   ((Copy_u8Byte>>6)&1)  );
		DIO_enuSetPinValue(D5_PORT , D5_PIN,   ((Copy_u8Byte>>5)&1)  );
		DIO_enuSetPinValue(D4_PORT , D4_PIN,   ((Copy_u8Byte>>4)&1)  );

		DIO_enuSetPinValue(EN_PORT,EN_PIN, DIO_u8HIGH);
		_delay_ms(5);
		DIO_enuSetPinValue(EN_PORT, EN_PIN, DIO_u8LOW);
		_delay_ms(5);
	}

	DIO_enuSetPinValue(D7_PORT , D7_PIN,   ((Copy_u8Byte>>7)&1)  );
	DIO_enuSetPinValue(D6_PORT , D6_PIN,   ((Copy_u8Byte>>6)&1)  );
	DIO_enuSetPinValue(D5_PORT , D5_PIN,   ((Copy_u8Byte>>5)&1)  );
	DIO_enuSetPinValue(D4_PORT , D4_PIN,   ((Copy_u8Byte>>4)&1)  );

	DIO_enuSetPinValue(EN_PORT,EN_PIN, DIO_u8HIGH);
	_delay_ms(5);
	DIO_enuSetPinValue(EN_PORT, EN_PIN, DIO_u8LOW);
	_delay_ms(5);

	DIO_enuSetPinValue(D7_PORT , D7_PIN,   ((Copy_u8Byte>>3)&1)  );
	DIO_enuSetPinValue(D6_PORT , D6_PIN,   ((Copy_u8Byte>>2)&1)  );
	DIO_enuSetPinValue(D5_PORT , D5_PIN,   ((Copy_u8Byte>>1)&1)  );
	DIO_enuSetPinValue(D4_PORT , D4_PIN,   ((Copy_u8Byte>>0)&1)  );

	DIO_enuSetPinValue(EN_PORT,EN_PIN, DIO_u8HIGH);
	_delay_ms(5);
	DIO_enuSetPinValue(EN_PORT, EN_PIN, DIO_u8LOW);
	_delay_ms(5);
#endif

	return Local_enuErrorState;
	//dead code
}

ES_t LCD_enuSendIntegerNum(s32 Copy_s32Num)
{
	ES_t Local_enuErrorState = ES_NOK;

	s32 Local_s32Reverse=0;
	u8 Local_u8Remainder=0 , Local_u8NumDigit=0;

	if(Copy_s32Num <0)
	{
		DIO_enuSetPinValue(RS_PORT , RS_PIN , DIO_u8HIGH);
		WriteNLatch('-');
		Copy_s32Num *= -1 ;
	}
	while (Copy_s32Num)
	{
		Local_u8Remainder = Copy_s32Num % 10;
		Local_s32Reverse = Local_s32Reverse * 10 + Local_u8Remainder;
		Copy_s32Num /=10;
		Local_u8NumDigit ++;
	}

	while (Local_u8NumDigit)
	{
		Local_u8Remainder = Local_s32Reverse % 10;
		DIO_enuSetPinValue(RS_PORT , RS_PIN , DIO_u8HIGH);
		WriteNLatch(Local_u8Remainder + '0');
		Local_s32Reverse /= 10;
		Local_u8NumDigit --;
	}

	return Local_enuErrorState;
}
void LCD_voidSendPosition(u8 Copy_LineNUM,u8 Copy_u8Col)
{
	if (Copy_u8Col<=39)
	{
		switch (Copy_LineNUM)
		{
		case  LCD_LINE1: LCD_enuSendCommand(0x80 +Copy_u8Col);
		break ;
		case LCD_LINE2: LCD_enuSendCommand(0xc0 +Copy_u8Col);
		break;
		}
	}
	else
	{
		/*Error*/
	}
}
