/*
 *  Filename: table/table.c
 *  Author: AivenLi
 *	Copyright 2019-2020 Aiven.Li All rights reserved.
 *
 *  ������
 *      1. ��������������ݽ�������ѧϰ������֤���ݵ���ȷ�ԡ�ͨ��ʹ�ñ����
 *  ��֮�����ķ����������޹ء���ʹ�ñ����ʱ���������ѽ������ߵ�������������
 *		2. δ��������������������ҵ��;���������Ը���
 */
#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *get_line_symbol(char *line, int len);
static void print_space(int times);
static void init_base_len(int *items, int len);
static int init_subject_len(int *items, subject_p subject_h);
static int get_score_space(float score);

static char *get_line_symbol(char *line, int len) {

	while ( len-- ) {

		*line++ = HORIZONTAL_SYMBOL;
	}
	*line++ = BORDER_SYMBOL;
	return line;
}

static void print_space(int times) {

	if ( times < 0 ) {

		return;
	}
	while ( times-- ) {

		printf(" ");
	}
}

static void init_base_len(int *items, int len) {

	while ( len-- ) {

		*items++ = BASE_LEN;
	}
}

static int init_subject_len(int *items, subject_p subject_h) {

	subject_p subject;
	int total = 0;

	subject = subject_h;
	while ( subject != NULL ) {

		total++;
		*items = (int)strlen(subject->name) + 2;
		if ( *items < 5 ) {

			*items = 5;
		}
		items++;
		subject = subject->next;
	}
	return total;
}

static int get_score_space(float score) {

	if ( score >= 0.0f && score < 10.0f ) {

		return 1;
	} else if ( score >= 10.0f && score < 100.0f ) {

		return 2;
	} else {

		return 3;
	}
}

int show_base(teacher_p thead, student_p shead, _bool is_teacher) {

	/*
	int numbers    = BASE_LEN;
	int name_len   = BASE_LEN;
	int id_len     = BASE_LEN;
	int passwd_len = BASE_LEN;
	int gender_len = BASE_LEN;
	int age_len    = BASE_LEN;
	*/
	int total;
	int items_len[BASE_LEN] = { BASE_LEN, BASE_LEN, BASE_LEN, BASE_LEN, BASE_LEN, BASE_LEN };
	int index      = 0;
	int i;
	char line[MAX_LINE_LENGTH];
	char age[4];
	char *p;
	char index_item[BASE_LEN]; // �����ݳ���9999��֮����������
	char title[][BASE_LEN] = { "���", "����", "�Ա�", "����", "ID", "����"};
	teacher_p teacher = thead;
	student_p student = shead;
	
	if ( is_teacher ) {

		while ( teacher != NULL ) {
	
			items_len[1] = MAX(items_len[1], (int)strlen(teacher->base->name));
			items_len[4] = MAX(items_len[4], (int)strlen(teacher->base->id));
			items_len[5] = MAX(items_len[5], (int)strlen(teacher->base->passwd));
			teacher = teacher->next;
		}
	} else {

		while ( student != NULL ) {
	
			items_len[1] = MAX(items_len[1], (int)strlen(student->base->name));
			items_len[4] = MAX(items_len[4], (int)strlen(student->base->id));
			items_len[5] = MAX(items_len[5], (int)strlen(student->base->passwd));
			student = student->next;
		}
	}
	items_len[1] += 2;
	items_len[4] += 2;
	items_len[5] += 2;
	p = line;
	*p++ = BORDER_SYMBOL;
	for ( i = 0; i < BASE_LEN; i++ ) {

		p = get_line_symbol(p, items_len[i]);
	}
	*p = '\0';
	teacher = thead;
	student = shead;
	printf("\n%s\n", line);
	for ( i = 0; i < BASE_LEN; i++ ) {

		printf("| %s", title[i]);
		print_space(items_len[i] - strlen(title[i]) - 1);
	}
	printf("|\n%s\n", line);
	total = 0;
	i = 0;
	index = 0;
	if ( is_teacher ) {

		while ( teacher != NULL ) {

			printf("| %s", itoa(++index, index_item, 10));    // ���
			print_space(items_len[i++] - strlen(index_item)-1);
			printf("| %s", teacher->base->name);  // ����
			print_space(items_len[i++] - strlen(teacher->base->name) - 1);
			printf("| %s", teacher->base->gender == 0 ? "��" : "Ů"); // �Ա�
			print_space(items_len[i++] - 2 - 1);
			printf("| %s", itoa(teacher->base->age, age, 10) ); // ����
			print_space(items_len[i++] - strlen(age) - 1);
			printf("| %s", teacher->base->id);                //  id
			print_space(items_len[i++] - strlen(teacher->base->id) - 1);
			printf("| %s", teacher->base->passwd);            // ����
			print_space(items_len[i] - strlen(teacher->base->passwd) - 1);
			i = 0;
			printf("|\n");
			printf("%s\n", line);
			teacher = teacher->next;
		}
	} else {

		while ( student != NULL ) {

			printf("| %s", itoa(++index, index_item, 10));    // ���
			print_space(items_len[i++] - strlen(index_item)-1);
			printf("| %s", student->base->name);  // ����
			print_space(items_len[i++] - strlen(student->base->name) - 1);
			printf("| %s", student->base->gender == 0 ? "��" : "Ů"); // �Ա�
			print_space(items_len[i++] - 2 - 1);
			printf("| %s", itoa(student->base->age, age, 10) ); // ����
			print_space(items_len[i++] - strlen(age) - 1);
			printf("| %s", student->base->id);                //  id
			print_space(items_len[i++] - strlen(student->base->id) - 1);
			printf("| %s", student->base->passwd);            // ����
			print_space(items_len[i] - strlen(student->base->passwd) - 1);
			i = 0;
			printf("|\n%s\n", line);
			student = student->next;
		}
	}
	return index;
}

int show_student(class_p chead) {

	/* 
	 *  ͳ�Ƴɼ�����ƽ�������� 
	 */
	int excellent = 0; /* ��������   */
	int good      = 0; /* ��������   */
	int medium    = 0; /* �е�����   */
	int pass      = 0; /* ��������   */
	int no_pass   = 0; /* ���������� */
	int index;
	int subject_total;
	int i;
	int items_len[MAX_LINE_LENGTH];
	char line[MAX_LINE_LENGTH];
	char index_item[BASE_LEN+1];
//	char subject_name[SUBJECT_NAME_LENGTH];
	char base_title[][20] = { "���", "����", "�Ա�", "����", "ID", "����", "ƽ������"};
	char *p;
	subject_p subject;
	student_p student;
	score_p score;

	if ( chead == NULL ) {

		printf("�༶Ϊ��\n");
		return -1;
	}
	subject = chead->subject;
	init_base_len(items_len, BASE_LEN+1); /* ��á���š�������ѧ�š��ڱ������ռ�ĳ��� */
	items_len[6] = 10;
	subject_total = init_subject_len(&items_len[7], subject); /* ���ѧ�������ڱ������ռ�ĳ��� */
	student = chead->student;
	while ( student != NULL ) {

		items_len[1] = MAX(items_len[1], (int)strlen(student->base->name));
		items_len[4] = MAX(items_len[4], (int)strlen(student->base->id));
		items_len[5] = MAX(items_len[5], (int)strlen(student->base->passwd));
		student = student->next;
	}
	items_len[1] += 2;
	items_len[4] += 2;
	items_len[5] += 2;
	p = line;
	*p++ = BORDER_SYMBOL;
	for ( i = 0; i < BASE_LEN+1; i++ ) {

		p = get_line_symbol(p, items_len[i]);
	}
	for ( subject = chead->subject, i = BASE_LEN+1; i < subject_total+BASE_LEN+1; i++, subject = subject->next ) {

		p = get_line_symbol(p, items_len[i]);
	}
	*p = '\0';
	/*
	 *  ��ӡ����
	 */
	printf("��ǰ�༶: %s\n%s\n", chead->name, line);
	for ( i = 0; i < BASE_LEN+1; i++ ) {

		printf("| %s", base_title[i]);
		print_space(items_len[i] - (int)strlen(base_title[i]) - 1);
	}
	for ( subject = chead->subject, i = BASE_LEN+1; i < subject_total+BASE_LEN+1; i++, subject = subject->next ) {

		printf("| %s", subject->name);
		print_space(items_len[i] - (int)strlen(subject->name) - 1);
	}
	printf("|\n%s\n", line);
	/*
	 *  ��ӡ����
	 */
	index = 0;
	student = chead->student;
	i = 0;
	while ( student != NULL ) {

		printf("| %s", itoa(++index, index_item, 10));    /* ��� */
		print_space(items_len[i++] - (int)strlen(index_item) - 1);
		printf("| %s", student->base->name); /* ���� */
		print_space(items_len[i++] - (int)strlen(student->base->name) - 1);
		printf("| %s", student->base->gender == 0 ? "��" : "Ů"); /* �Ա� */
		print_space(items_len[i++] - 2 - 1);
		printf("| %s", itoa(student->base->age, index_item, 10)); /* ���� */
		print_space(items_len[i++] - strlen(index_item) - 1);
		printf("| %s", student->base->id);  /* ѧ�ţ�ID�� */
		print_space(items_len[i++] - (int)strlen(student->base->id) - 1);
		printf("| %s", student->base->passwd); /* ���� */
		print_space(items_len[i++] - (int)strlen(student->base->passwd) - 1);
		printf("| %0.1f", student->gpa);
		print_space(items_len[i++] - ( get_score_space(student->gpa) + 2 ) - 1);
		/*
		 *  ���Ƴɼ�
		 */
		score = student->score;
		while ( score != NULL ) {

			printf("| %0.1f", score->score);
			print_space(items_len[i++] - ( get_score_space(score->score) + 2 ) - 1);
			score = score->next;
		}
		printf("|\n%s\n", line);
		if ( student->gpa >= 4.0f ) {

			excellent++;
		} else if ( student->gpa < 4.0f && student->gpa >= 3.0f ) {

			good++;
		} else if ( student->gpa < 3.0f && student->gpa >= 2.0f ) {

			medium++;
		} else if ( student->gpa < 2.0f && student->gpa >= 1.0f ) {

			pass++;
		} else {

			no_pass++;
		}
		student = student->next;
		i = 0;
	}
	printf("��%d�ˣ�����%d�ˣ�����%d�ˣ��е�%d�ˣ�����%d�ˣ�������%d��\n", 
		index, excellent, good, medium, pass, no_pass);
	return index;
}

int show_class(class_p head) {

	int total = 0;
	int i;
	int number_len = BASE_LEN;
	int class_name_len = 8;
	int teacher_name_len = 8;
	int item_len[3] = { BASE_LEN, 8, 8 };
	char line[MAX_LINE_LENGTH];
	char index_item[BASE_LEN];
	char title[3][CLASS_NAME_LENGTH] = { "���", "�༶����", "������ID"};
	char *p;
	class_p _class;
	subject_p subject;

	_class = head;
	while ( _class != NULL ) {

		item_len[1] = MAX(item_len[1], (int)strlen(_class->name));
		item_len[2] = MAX(item_len[2], (int)strlen(_class->teacher->id));
		_class = _class->next;
	}
	item_len[1] += 2;
	item_len[2] += 2;
	p = line;
	*p++ = BORDER_SYMBOL;
	for ( i = 0; i < 3; i++ ) {

		p = get_line_symbol(p, item_len[i]);
	}
	*p = '\0';
	printf("%s\n", line);
	/*
	 *  ����
	 */
	for ( i = 0; i < 3; i++ ) {

		printf("| %s", title[i]);
		print_space(item_len[i] - (int)strlen(title[i]) - 1);
	}
	printf("|\n%s\n", line);
	_class = head;
	i = 0;
	while ( _class != NULL ) {

		printf("| %s", itoa(++total, index_item, 10));    /* ��� */
		print_space(item_len[i++] - (int)strlen(index_item) - 1);
		printf("| %s", _class->name); /* �༶���� */
		print_space(item_len[i++] - (int)strlen(_class->name) - 1);
		printf("| %s", _class->teacher->id); /* ������ */
		print_space(item_len[i++] - (int)strlen(_class->teacher->id) - 1);
		printf("| ");
		subject = _class->subject;
		while ( subject != NULL ) {

			printf("--%s(%0.1f) ", subject->name, subject->credit);
			subject = subject->next;
		}
		printf("|\n%s\n", line);
		i = 0;
		_class = _class->next;
	}
	return total;
}