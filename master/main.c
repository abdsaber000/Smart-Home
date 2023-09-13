#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "MCAL/DIO/int.h"
#include "HAL/LCD/int.h"
#include "HAL/KeyBad/int.h"
#include "HAL/Sensor/int.h"
#include "HAL/Motor/int.h"
#include "HAL/StepperMotor/int.h"
#include <avr/interrupt.h>
#include"LIB/BIT_Math.h"
#include "MCAL/Interrupt/int.h"
#include "MCAL/ADC/int.h"
#include "MCAL/Timer/int.h"
#include "MCAL/SPI/int.h"
#include "MCAL/EEPROM/EEPROM.h"
#include "min.h"
u8  password[PASS_SIZE];
int main(void){
	LCD_Init();
	KeypadInit();
	/*int i = 0;
	int number = 0;
	for(; i < 4; i++){
		while(1){
			int val = KeypadGetKey();
			if(val){
				number = number * 10 + val - '0';
				break;
			}
		}
	}

	 LCD_WriteNum(number);*/

//	init_SPI_Master();
//	LCD_WriteString("HUSSEIN");
//	setAdminGuestScreen();
	showSettingsScreen();
	while(1){
//		SPI_Start_Trans();
//		SPI_Transceive('A');
//		_delay_ms(500);
//		SPI_Transceive('B');
//		_delay_ms(500);
//		SPI_Transceive('C');
//		_delay_ms(500);
//		SPI_Stop_Trans();


	}
}

u8 check_EEPROM_Flag(void) {
	u8 flag_val = EEPROM_read(EEPROM_FLAG_ADDRESS); //EEPROM_ReadByteFromAddress(EEPROM_FLAG_ADDRESS);

	return flag_val == EEPROM_EMPTY_ADDRESS;
}

void getAdminPass(void) {
	EEPROM_ReadBlockFromAddress(EEPROM_ADMIN_ADDRESS, password, PASS_SIZE);
//	return password;
}

void getGuestPass(void) {
	EEPROM_ReadBlockFromAddress(EEPROM_GUEST_ADDRESS, password, PASS_SIZE);
//	return password;
}

void setAdminPass(u8 pass[]) {
	EEPROM_WriteBlockToAddress(EEPROM_ADMIN_ADDRESS, pass, PASS_SIZE);
}

void setGuestPass(u8 pass[]) {
	EEPROM_WriteBlockToAddress(EEPROM_GUEST_ADDRESS, pass, PASS_SIZE);
}

void setAdminGuestScreen(void) {
	u8 adminPass[PASS_SIZE], guestPass[PASS_SIZE];
//	adminPass = (u8) calloc(PASS_SIZE, sizeof(u8));
//	guestPass = (u8) calloc(PASS_SIZE, sizeof(u8));
	LCD_WriteString("Enter Admin Password : ");
	LCD_GoTO(2, 0);
	u8 i;
	for (i = 0; i < PASS_SIZE; i++) {
		while (1) {
			u8 password_digit = KeypadGetKey();
			if (password_digit) {
				LCD_WriteChar(password_digit);
				adminPass[i] = password_digit;
				break;
			}
		}
	}
	_delay_ms(500);
	LCD_Clear();
	/* do the same as above for guest */

	LCD_WriteString("Enter Guest Password : ");
	LCD_GoTO(2, 0);
	for (i = 0; i < PASS_SIZE; i++) {
		while (1) {
			u8 password_digit = KeypadGetKey();
			if (password_digit) {
				LCD_WriteChar(password_digit);
				guestPass[i] = password_digit;
				break;
			}
		}
	}

	/* save the passwords in EEPROM and make the flag address with 0 */

	EEPROM_write(EEPROM_FLAG_ADDRESS, EEPROM_FLAG_NOT_EMPTY);
	setAdminPass(adminPass);
	setGuestPass(guestPass);

	_delay_ms(500);
	LCD_Clear();
	LCD_WriteString("Registration is");
	LCD_GoTO(2, 0);
	LCD_WriteString("Successful !");
	_delay_ms(1000);
	LCD_Clear();
	
}

u8 checkUserPassScreen(void) {

	u8 trial_number = MAX_TRIAL_NUMBER;

	u8 user_type = NOT_VALID_USER;

	LCD_WriteString("Select User Type:");
	LCD_GoTO(2, 0);

	LCD_WriteString("1:Admin 2:Guest");

	while (1) {
		user_type = KeypadGetKey();
		user_type -= '0';
		if (user_type == ADMIN || user_type == GUEST)
			break;
	}

	while (trial_number--) {

		LCD_Clear();


		LCD_WriteString("Enter Password:");
		LCD_GoTO(2, 0);

		switch (user_type) {
		case ADMIN:
			getAdminPass();
			break;
		case GUEST:
			getGuestPass();
			break;
		}

		u8 i;

		u8 is_correct_password = CORRECT_PASSWORD;
		for (i = 0; i < PASS_SIZE; i++) {
			u8 password_digit;

			while (1) {
				password_digit = KeypadGetKey();
				if (password_digit)
					break;
			}
			LCD_WriteChar(password[i]);
			if (password_digit != password[i]) {
				is_correct_password = WRONG_PASSWORD;
			}
		}

		if (!is_correct_password) {

			LCD_Clear();
			LCD_WriteString("incorrect !");
			LCD_GoTO(2, 0);
			LCD_WriteString("left tries : ");
			LCD_WriteChar(trial_number + '0');

			_delay_ms(1000);

			continue;
		}
		LCD_Clear();
		LCD_WriteString("Correct Password !");
		_delay_ms(500);
		LCD_Clear();
		return user_type;

	}

	return NOT_VALID_USER;

}

u8 showSettingsScreen(void) {

	if(check_EEPROM_Flag()){
		setAdminGuestScreen();
	}
	return checkUserPassScreen();

}

void showRoomGuestScreen(u8 room){

}

void showRoomAdminScreen(u8 room){

}

void showAcScreen(void){

}

void showAdminScreen(void){
	LCD_WriteString("1:ROOM1 2:ROOM2");
	LCD_GoTO(2 , 0);
	LCD_WriteString("3:ROOM3 4:AC");

	u8 user_choice ;

	while(1){
		user_choice = KeypadGetKey();
		user_choice -= '0';
		if(user_choice == ROOM1 || user_choice == ROOM2 || user_choice == ROOM3){
			showRoomAdminScreen(user_choice);
			break;
		}

		if(user_choice == AC){
			showAcScreen();
			break;
		}
	}
}


void showGuestScreen(void){
	LCD_WriteString("1:ROOM1 2:ROOM2");
	LCD_GoTO(2 , 0);
	LCD_WriteString("3:ROOM3");

	u8 user_choice ;

	while(1){
		user_choice = KeypadGetKey();
		user_choice -= '0';
		if(user_choice == ROOM1 || user_choice == ROOM2 || user_choice == ROOM3){
			showRoomGuestScreen(user_choice);
			break;
		}


	}
}


void showMainScreen(u8 userType){
	switch(userType){
	case ADMIN:
		showAdminScreen();
		break;
	case GUEST:
		showGuestScreen();
		break;
	}
}
