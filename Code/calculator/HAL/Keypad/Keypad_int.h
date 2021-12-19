/*
 * Keypad_int.h
 *
 *  Created on: Dec 13, 2021
 *      Author: Ahmed El-Gaafrawy
 */

#ifndef HAL_KEYPAD_KEYPAD_INT_H_
#define HAL_KEYPAD_KEYPAD_INT_H_

ES_t Keypad_enuInit(void);

ES_t Keypad_enuGetPressed(u8 * Copy_pu8PressedKey);

#define KEYPAD_NOT_PRESSED			0xff

#endif /* HAL_KEYPAD_KEYPAD_INT_H_ */
