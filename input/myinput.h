/*
 *  Filename: input/myinput.h
 *  Author: AivenLi
 *	Copyright 2019-2020 Aiven.Li All rights reserved.
 *
 *  ������
 *      1. ��������������ݽ�������ѧϰ������֤���ݵ���ȷ�ԡ�ͨ��ʹ�ñ����
 *  ��֮�����ķ����������޹ء���ʹ�ñ����ʱ���������ѽ������ߵ�������������
 *		2. δ��������������������ҵ��;���������Ը���
 */
#ifndef _MYINPUT_H_
#define _MYINPUT_H_

#include <stdio.h>

extern char input_char(void);
extern char *input_passwd(char *passwd, int const len);
extern char *input_string(char *str, int const len);
extern double input_number(double min, double max);

#endif