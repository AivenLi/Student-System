/*
 *  Filename: include/mtype.h
 *  Author: AivenLi
 *	Copyright 2019-2020 Aiven.Li All rights reserved.
 *
 *  声明：
 *      1. 本软件的所有内容仅供个人学习，不保证内容的正确性。通过使用本软件
 *  随之而来的风险与作者无关。当使用本软件时，代表你已接受作者的免费声明等条款。
 *		2. 未经作者允许，不可用作商业用途，否则后果自负。
 */
#ifndef _MTYPE_H_
#define _MTYPE_H_

#define IS_NUMBER(data) ( ( ( data ) >= ( '0' ) ) && ( ( data ) <= ( '9' ) ) )
#define IS_LETTER(data) ( ( ( ( data ) >= ( 'a' ) ) && ( ( data ) <= ( 'z' ) ) ) ||\
						  ( ( ( data ) >= ( 'A' ) ) && ( ( data ) <= ( 'Z' ) ) ) )
#define IS_CHINESE(data) ( ( data ) & ( 0x80 ) )

#define TRUE  1
#define FALSE 0
typedef char           _bool;
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;



#endif