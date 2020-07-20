/*
 *  Filename: input/myinput.c
 *  Author: AivenLi
 *	Copyright 2019-2020 Aiven.Li All rights reserved.
 *
 *  ������
 *      1. ��������������ݽ�������ѧϰ������֤���ݵ���ȷ�ԡ�ͨ��ʹ�ñ����
 *  ��֮�����ķ����������޹ء���ʹ�ñ����ʱ���������ѽ������ߵ�������������
 *		2. δ��������������������ҵ��;���������Ը���
 */
#include "myinput.h"
#include <conio.h>
#include "../include/mtype.h"

char input_char(void) {

	char ch;
	fflush(stdin);
	ch = getchar();
	fflush(stdin);
	return ch;
}

char *input_passwd(char *passwd, int const len) {

//	char passwd[PASSWD_LENGTH];
	int i = 0;

	fflush(stdin);
	while ( TRUE ) {

		passwd[i] = getch();
		if ( passwd[i] == 0x08 ) {  // BackSpace

			i--;
			if ( i < 0 ) {

				i = 0;
			} else {

				putchar('\b');
				putchar(' ');
				putchar('\b');
			}
			
		} else if ( passwd[i] == 0x0d ) { // Enter

			passwd[i] = '\0';
			putchar('\n');
			break;
		} else {
			
			i++;
			if ( i > ( len-1 ) ) {

				i = len - 1;
			} else {

				putchar('*');
			}	
		}
	}
	fflush(stdin);
	return passwd;
}

char *input_string(char *str, int const len) {

	int i = 0;

	fflush(stdin);
	while ( 1 ) {

		str[i] = getch();
		if ( str[i] == 0x08 ) {  // BackSpace

			i--;
			if ( i < 0 ) {

				i = 0;
			} else {

				putchar('\b');
				putchar(' ');
				putchar('\b');
			}
			
		} else if ( str[i] == 0x0d ) { // Enter

			str[i] = '\0';
			putchar('\n');
			break;
		} else {
			
			i++;
			if ( i > ( len-1 ) ) {

				i = len - 1;
			} else {

				printf("%c", str[i-1]);
			}
		}
	}
	fflush(stdin);
	return str;
}
#define MAX_INT_NUMBER 13
double input_number(double min, double max) {

	_bool point = FALSE;
	int i;
	int buf;
	int integer;
	double decimal;
	double base_decimal = 10.0;
	double number;
	char str[MAX_INT_NUMBER];

	fflush(stdin);
	i = 0;
	integer = 0;
	decimal = 0.0;
	while ( 1 ) {

		str[i] = getch();
		
		if ( !point && str[i] == '.' && i != 0 ) {

			if ( str[0] == '-' && i == 1 ) {

				continue;
			}
			putchar(str[i]);
			point = TRUE;
			i++;
			continue;
		}
		if ( str[0] == '-' && i == 0 ) {
		
			if ( min >= 0.0 ) {

				continue;
			}
			i++;
			putchar('-');
		} else if ( str[i] >= '0' && str[i] <= '9' ) {

			if ( point ) {
				
				decimal = decimal + ( str[i] - '0' ) / base_decimal;
				base_decimal = base_decimal * 10.0;
			//	printf("***%0.5f**\n", decimal);
			} else {
					
				integer = integer * 10 + ( str[i] - '0' );
			}
		//	printf("**%0.5f**%0.5f**\n", integer, decimal);
			number = integer + decimal;
			if ( ( str[0] != '-' ) && ( number > max ) ) {

				integer = integer / 10;
				number = integer + decimal;
				continue;
			} else if ( ( str[0] == '-' ) && ( (-number) < min ) ) {

				integer = integer / 10;
				number = integer + decimal;
				continue;
			} else {

				i++;
				if ( i > MAX_INT_NUMBER-1 ) {

					i = MAX_INT_NUMBER - 1;
				} else {

					putchar(str[i-1]);
				}
			}
		} else if ( str[i] == 0x08 ) { // Backspace

			i--;
			if ( point ) {

				base_decimal = base_decimal / 10;      // ��С��ȫ���������
				buf = ( decimal * base_decimal ) / 10; // ȥ����λ��
				decimal = ( buf / base_decimal ) * 10; // ���������С��
			} else {

				integer = integer / 10;
			}
			if ( str[i] == '.' ) {

				point = FALSE;
				decimal = 0.0;
				base_decimal = 10.0;
			}
			number = integer + decimal;
			if ( i < 0 ) {

				i = 0;
			} else {

				putchar('\b');
				putchar(' ');
				putchar('\b');
			}
		} else if ( str[i] == 0x0d ) { // Enter

			str[i] = '\0';
			putchar('\n');
			break;
		}
	}
	fflush(stdin);
	if ( str[0] == '-' ) {

		return -number;
	} else if ( str[0] == '\0' ) {

		return -1.0;
	}
	return number;
}
#undef MAX_INT_NUMBER