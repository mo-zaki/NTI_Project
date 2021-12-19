/*
 * APP_Calculator.c
 *
 *  Created on: Dec 18, 2021
 *      Author: zaki
 */

/***********************************************/
/****************Linking Section****************/
/***********************************************/
#include "../LIBRARY/stdTypes.h"
#include "../LIBRARY/errorState.h"
#include "../MCAL/DIO/DIO_int.h"
#include "../HAL/LCD/LCD_int.h"
#include "../HAL/Keypad/Keypad_int.h"
#include  <util/delay.h>
#define  CLR_LCD	  0b00000001
/***********************************************/

/***********************************************/
/****************Prototype Section**************/
Boolean_T  division(s32 num_1,s32 num_2,s32 *value);
Boolean_T  ADD(s32 num_1,s32 num_2,s32 *value);
Boolean_T  SUB(s32 num_1,s32 num_2,s32 *value);
Boolean_T  multiply(s32 num_1,s32 num_2,s32 *value);
/***********************************************/

/**********************************************************/
s32 NUMBER(s32 *ptr)
{
	s32 y=0;
	y=((ptr[0])*1000)+((ptr[1])*100)+((ptr[2])*10)+ptr[3];
	return y;
}
/**********************************************************/
int main()
{
	/***********************************************/
	/****************Initalization******************/
	/***********************************************/
	LCD_enuInit();
	Keypad_enuInit();
	/***********************************************/
	s32 Local_s32Result=0	;
	Boolean_T STATE=FALSE;
	u8 Local_u8SW_Value = KEYPAD_NOT_PRESSED;
	u8 Local_u8Operator = 0;
	s32 NUM_1 = 0, NUM_2 = 0;

	while (1)
	{
		Keypad_enuGetPressed(&Local_u8SW_Value);
		if (Local_u8SW_Value != KEYPAD_NOT_PRESSED)
		{
			if (Local_u8SW_Value <= 9)
			{
				/*GET NUMBER_1*/

				if (!Local_u8Operator)
				{
					LCD_enuSendIntegerNum(Local_u8SW_Value);
					NUM_1 *= 10;
					NUM_1 += Local_u8SW_Value;
					if (Local_u8SW_Value == NULL)
					{
						LCD_enuSendData('0');
					}
				}
				/*GET NUMBER_2*/
				else
				{
					LCD_enuSendIntegerNum(Local_u8SW_Value);
					NUM_2 *= 10;
					NUM_2 += Local_u8SW_Value;
					if (Local_u8SW_Value == NULL)
					{
						LCD_enuSendData('0');
					}
				}
			}
			else
			{
				LCD_enuSendData(Local_u8SW_Value);
				if (Local_u8SW_Value == '=')
				{
					switch (Local_u8Operator)
					{
					case '+':

						ADD(NUM_1, NUM_2, &Local_s32Result);
						LCD_enuSendIntegerNum(Local_s32Result);

						break;
					case '-':
						SUB(NUM_1, NUM_2, &Local_s32Result);
						LCD_enuSendIntegerNum(Local_s32Result);
						break;
					case 'x':
						multiply(NUM_1,NUM_2,&Local_s32Result);
						LCD_enuSendIntegerNum(Local_s32Result);
						break;
					case'/':
						LCD_voidSendPosition(LCD_LINE2, 0);
						STATE=division(NUM_1,NUM_2,&Local_s32Result);
						if (STATE!=FALSE)
						{
							LCD_enuSendIntegerNum(Local_s32Result);
						}
						else
						{
							LCD_enuSendString("Unknown Error:");
						}
						break;
					}
				}
				else if (Local_u8SW_Value == 'C')
				{
					LCD_enuSendCommand(0x01);
					Local_u8SW_Value = KEYPAD_NOT_PRESSED, Local_u8Operator = 0;
					NUM_1 = 0, NUM_2 = 0;
				}
				else
				{
					Local_u8Operator = Local_u8SW_Value;
				}

			}

		}
	}
}

/**********************************************************/

Boolean_T  division(s32 num_1,s32 num_2,s32 *value)
{
	Boolean_T ErrorState=FALSE;
	if (num_2!=0)
	{
		*value=	num_1/num_2;
		ErrorState =TRUE;
	}
	else
	{
		ErrorState=FALSE;
	}
	return ErrorState;
}
/***********************************************************/
Boolean_T  ADD(s32 num_1,s32 num_2,s32 *value)
{
	*value=num_1+num_2;
	return TRUE;
}
/***********************************************************/
Boolean_T  SUB(s32 num_1,s32 num_2,s32 *value)
{
	*value=num_1-num_2;
	return TRUE;
}
/**********************************************************/
Boolean_T  multiply(s32 num_1,s32 num_2,s32 *value)
{
	*value=num_1*num_2;
	return TRUE;
}

