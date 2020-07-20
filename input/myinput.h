/*
 *  Filename: input/myinput.h
 *  Author: AivenLi
 *	Copyright 2019-2020 Aiven.Li All rights reserved.
 *
 *  声明：
 *      1. 本软件的所有内容仅供个人学习，不保证内容的正确性。通过使用本软件
 *  随之而来的风险与作者无关。当使用本软件时，代表你已接受作者的免费声明等条款。
 *		2. 未经作者允许，不可用作商业用途，否则后果自负。
 */
#ifndef _MYINPUT_H_
#define _MYINPUT_H_

#include <stdio.h>

extern char input_char(void);
extern char *input_passwd(char *passwd, int const len);
extern char *input_string(char *str, int const len);
extern double input_number(double min, double max);

#endif