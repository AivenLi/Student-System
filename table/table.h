/*
 *  Filename: table/table.h
 *  Author: AivenLi
 *	Copyright 2019-2020 Aiven.Li All rights reserved.
 *
 *  声明：
 *      1. 本软件的所有内容仅供个人学习，不保证内容的正确性。通过使用本软件
 *  随之而来的风险与作者无关。当使用本软件时，代表你已接受作者的免费声明等条款。
 *		2. 未经作者允许，不可用作商业用途，否则后果自负。
 */
#ifndef _TABLE_H_
#define _TABLE_H_

#include "../student/student.h"
#include "../include/mtype.h"
/*
typedef struct table_s *table_p;
typedef struct table_s  table_b;
*/
#define MAX_ITEMS_LEN     1024
#define MAX_LINE_LENGTH   1024
#define MAX_TITLE_ITEMS   100
#define MAX_CONTENT_ITEMS 4096
#define MAX_TEXT_LENGTH   40
#define BASE_LEN 6
#define BORDER_SYMBOL     '+'
#define HORIZONTAL_SYMBOL '-'
#define VERTICAL_SYMBOL   '|'
#define SPACE_SYMBOL      ' '
/*
struct table_s {

	char line[MAX_LINE_LENGTH];
	char title[MAX_TITLE_ITEMS][MAX_TEXT_LENGTH];
	char content[MAX_CONTENT_ITEMS][MAX_TEXT_LENGTH];
	int title_items;
};

extern void init_table(table_p table);
extern void get_title(table_p table, char const *title[]);
*/

extern int show_base(teacher_p thead, student_p shead, _bool is_teacher);
extern int show_student(class_p chead);
extern int show_class(class_p head);

#endif