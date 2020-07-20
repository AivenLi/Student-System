/*
 *  Filename: include/mtype.h
 *  Author: AivenLi
 *	Copyright 2019-2020 Aiven.Li All rights reserved.
 *
 *  ������
 *      1. ��������������ݽ�������ѧϰ������֤���ݵ���ȷ�ԡ�ͨ��ʹ�ñ����
 *  ��֮�����ķ����������޹ء���ʹ�ñ����ʱ���������ѽ������ߵ�������������
 *		2. δ��������������������ҵ��;���������Ը���
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