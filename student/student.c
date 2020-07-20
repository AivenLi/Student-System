/*
 *  Filename: student/student.h
 *  Author: AivenLi
 *	Copyright 2019-2020 Aiven.Li All rights reserved.
 *
 *  声明：
 *      1. 本软件的所有内容仅供个人学习，不保证内容的正确性。通过使用本软件
 *  随之而来的风险与作者无关。当使用本软件时，代表你已接受作者的免费声明等条款。
 *		2. 未经作者允许，不可用作商业用途，否则后果自负。
 */
#include "student.h"
#include "../input/myinput.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static _bool has_same_classname(char const *name, class_p _class);

static _bool has_same_classname(char const *name, class_p _class) {

	class_p temp;

	temp = _class;
	while ( temp != NULL ) {

		if ( !strcmp(name, temp->name) ) {

			return TRUE;
		}
		temp = temp->next;
	}
	return FALSE;
}

list_p create_list(void) {

	list_p list;

	list = (list_p)malloc(sizeof(list_b));
#ifdef NEED_JUDGE
	if ( list == NULL ) {

		return NULL;
	}
#endif
	list->item = NULL;
	list->prev = NULL;
	list->next = NULL;
	return list;
}
void free_list(list_p list) {

	list_p temp;

	while ( list != NULL ) {

		temp = list->next;
		if ( list->item != NULL ) {

			free(list->item);
		}
		free(list);
		list = temp;
	}
}

base_p create_base(void) {

	base_p base;

	base = (base_p)malloc(sizeof(base_b));
	if ( base != NULL ) {
	
		base->age = 0;
		base->gender = MALE;
		memset(base->name,   0, PEOPLE_NAME_LENGTH);
		memset(base->id,     0, ID_LENGTH);
		memset(base->passwd, 0, PASSWD_LENGTH);
	}
	return base;
}

void free_base(base_p base) {

	if ( base != NULL ) {
	
		free(base);
	}
}

teacher_p create_teacher(void) {

	teacher_p teacher;

	teacher = (teacher_p)malloc(sizeof(teacher_b));
	if ( teacher != NULL ) {

		teacher->base = create_base();
#ifdef NEED_JUDGE
		if ( teacher->base == NULL ) {

			free(teacher);
			return NULL;
		}
#endif
		teacher->_class = NULL;
		teacher->next = NULL;
		teacher->prev = NULL;
	}
	return teacher;
}

void free_teacher(teacher_p teacher) {

	teacher_p temp;

	while ( teacher != NULL ) {

		temp = teacher->next;
		free_base(teacher->base);
		free_class(teacher->_class);
		free(teacher);
		teacher = temp;
	}
}

student_p create_student(void) {
	
	student_p student;

	student = (student_p)malloc(sizeof(student_b));
	if ( student != NULL ) {

		student->base = create_base();
#ifdef NEED_JUDGE
		if ( student->base == NULL ) {

			perror("malloc");
			free(student);
			return NULL;
		}
#endif
		student->score = create_score();
#ifdef NEED_JUDGE
		if ( student->score == NULL ) {

			perror("malloc");
			free(student->base);
			free(student);
			return NULL;
		}
#endif
		student->gpa = 0.0f;
		student->next = NULL;
		student->prev = NULL;
	}
	return student;
}

void free_student(student_p student) {

	student_p temp;
	
	while ( student != NULL ) {

		temp = student->next;
		free_base(student->base);
		free_score(student->score);
		free(student);
		student = temp;
	}
}

class_p create_class(void) {

	class_p _class;

	_class = (class_p)malloc(sizeof(class_b));
	if ( _class != NULL ) {

		_class->teacher = create_base();
#ifdef NEED_JUDGE
		if ( _class->teacher == NULL ) {

			perror("malloc");
			free(_class);
			return NULL;
		}
#endif
		_class->subject = create_subject();
#ifdef NEED_JUDGE
		if ( _class->subject == NULL ) {

			perror("malloc");
			free(_class->teacher);
			free(_class);
			return NULL;
		}
#endif
		_class->student = NULL;
		memset(_class->name, 0, CLASS_NAME_LENGTH);
		_class->prev   = NULL;
		_class->next   = NULL;
	}
	return _class;
}

void free_class(class_p _class) {

	class_p temp;

	while ( _class != NULL ) {

		temp = _class->next;
		free_base(_class->teacher);
		free_student(_class->student);
		free_subject(_class->subject);
		free(_class);
		_class = temp;
	}
}

subject_p create_subject(void) {

	subject_p subject;

	subject = (subject_p)malloc(sizeof(subject_b));
	if ( subject != NULL ) {

		memset(subject->name, 0, SUBJECT_NAME_LENGTH);
		subject->credit = 0.0f;
		subject->prev = NULL;
		subject->next = NULL;
	}
	return subject;
}

void free_subject(subject_p subject) {

	subject_p temp;

	while ( subject != NULL ) {

		temp = subject->next;
		free(subject);
		subject = temp;
	}
}

score_p create_score(void) {

	score_p score;

	score = (score_p)malloc(sizeof(score_b));
	if ( score != NULL ) {

		score->score = 0.0f;
		score->prev  = NULL;
		score->next  = NULL;
	}
	return score;
}

void free_score(score_p score) {

	score_p temp;

	while ( score != NULL ) {

		temp = score->next;
		free(score);
		score = temp;
	}
}

teacher_p delete_teacher_node(teacher_p head, teacher_p node) {

	if ( node == NULL ) {

		return head;
	}
	if ( node->prev == NULL && node->next == NULL ) {
		
		free_teacher(node);
		head = NULL;
	} else if ( node->prev == NULL && node->next != NULL ) {

		head = head->next;
		head->prev = NULL;
		node->next = NULL;
		free_teacher(node);
	} else if ( node->prev != NULL && node->next == NULL ) {

		node->prev->next = NULL;
		//node->prev = NULL; // 这一步可有可无
		free_teacher(node);
	} else if ( node->prev != NULL && node->next != NULL ) {

		node->prev->next = node->next;
		node->next->prev = node->prev;
		node->next = NULL;
		//node->prev = NULL; // 这一步可有可无
		free_teacher(node);
	}
	return head;
}

class_p delete_class_node(class_p head, class_p node) {

	if ( node == NULL ) {

		return head;
	}
	if ( node->prev == NULL && node->next == NULL ) {
		
		free_class(node);
		head = NULL;
	} else if ( node->prev == NULL && node->next != NULL ) {

		head = head->next;
		head->prev = NULL;
		node->next = NULL;
		free_class(node);
	} else if ( node->prev != NULL && node->next == NULL ) {

		node->prev->next = NULL;
		free_class(node);
	} else if ( node->prev != NULL && node->next != NULL ) {

		node->prev->next = node->next;
		node->next->prev = node->prev;
		node->next = NULL;
		free_class(node);
	}
	return head;
}

student_p delete_student_node(student_p head, student_p node) {

	if ( node == NULL ) {

		return head;
	}
	if ( node->prev == NULL && node->next == NULL ) {
		
		free_student(node);
		head = NULL;
	} else if ( node->prev == NULL && node->next != NULL ) {

		head = head->next;
		head->prev = NULL;
		node->next = NULL;
		free_student(node);
	} else if ( node->prev != NULL && node->next == NULL ) {

		node->prev->next = NULL;
		node->prev = NULL;
		free_student(node);
	} else if ( node->prev != NULL && node->next != NULL ) {

		node->prev->next = node->next;
		node->next->prev = node->prev;
		node->next = NULL;
		free_student(node);
	}
	return head;
}

score_p delete_score_node(score_p head, score_p node) {

	if ( node == NULL ) {

		return head;
	}
	if ( node->prev == NULL && node->next == NULL ) {
		
		free_score(node);
		head = NULL;
	} else if ( node->prev == NULL && node->next != NULL ) {

		head = head->next;
		head->prev = NULL;
		node->next = NULL;
		free_score(node);
	} else if ( node->prev != NULL && node->next == NULL ) {

		node->prev->next = NULL;
		node->prev = NULL;
		free_score(node);
	} else if ( node->prev != NULL && node->next != NULL ) {

		node->prev->next = node->next;
		node->next->prev = node->prev;
		node->next = NULL;
		node->prev = NULL;
		free_score(node);
	}
	return head;
}

class_p replace_class_node(class_p head, class_p old_class, class_p new_class) {

	if ( old_class == NULL ) { // 空链表

		return head;
	} else if ( old_class->prev == NULL && old_class->next == NULL ) { // 只有一个节点

		head = new_class;
		free_class(old_class);
	} else if ( old_class->prev == NULL && old_class->next != NULL ) { // 头结点

		new_class->next = old_class->next;
		old_class->next->prev = new_class;
		old_class->next = NULL;
		head = new_class;
		free_class(old_class);
	} else if ( old_class->prev != NULL && old_class->next == NULL ) { // 尾节点

		old_class->prev->next = new_class;
		new_class->prev = old_class->prev;
		old_class->prev = NULL;
		free_class(old_class);
	} else if ( old_class->prev != NULL && old_class->next != NULL ) { // 中间节点

		old_class->prev->next = new_class;
		old_class->next->prev = new_class;
		new_class->next = old_class->next;
		new_class->prev = old_class->prev;
		old_class->next = NULL;
		old_class->prev = NULL;
		free_class(old_class);
	}
	return head;
}

subject_p replace_class_subject_node(subject_p head, subject_p old_subject, subject_p new_subject) {

	if ( old_subject == NULL ) { // 空链表

		return head;
	} else if ( old_subject->prev == NULL && old_subject->next == NULL ) { // 只有一个节点

		head = new_subject;
		free_subject(old_subject);
	} else if ( old_subject->prev == NULL && old_subject->next != NULL ) { // 头结点

		new_subject->next = old_subject->next;
		old_subject->next->prev = new_subject;
		old_subject->next = NULL;
		head = new_subject;
		free_subject(old_subject);
	} else if ( old_subject->prev != NULL && old_subject->next == NULL ) { // 尾节点

		old_subject->prev->next = new_subject;
		new_subject->prev = old_subject->prev;
		old_subject->prev = NULL;
		free_subject(old_subject);
	} else if ( old_subject->prev != NULL && old_subject->next != NULL ) { // 中间节点

		old_subject->prev->next = new_subject;
		old_subject->next->prev = new_subject;
		new_subject->next = old_subject->next;
		new_subject->prev = old_subject->prev;
		old_subject->next = NULL;
		old_subject->prev = NULL;
		free_subject(old_subject);
	}
	return head;
}

student_p replace_student_node(student_p head, student_p old_student, student_p new_student) {

	if ( old_student == NULL ) { // 空链表

		return head;
	} else if ( old_student->prev == NULL && old_student->next == NULL ) { // 只有一个节点

		head = new_student;
		free_student(old_student);
	} else if ( old_student->prev == NULL && old_student->next != NULL ) { // 头结点

		new_student->next = old_student->next;
		old_student->next->prev = new_student;
		old_student->next = NULL;
		head = new_student;
		free_student(old_student);
	} else if ( old_student->prev != NULL && old_student->next == NULL ) { // 尾节点

		old_student->prev->next = new_student;
		new_student->prev = old_student->prev;
		old_student->prev = NULL;
		free_student(old_student);
	} else if ( old_student->prev != NULL && old_student->next != NULL ) { // 中间节点

		old_student->prev->next = new_student;
		old_student->next->prev = new_student;
		new_student->next = old_student->next;
		new_student->prev = old_student->prev;
		old_student->next = NULL;
		old_student->prev = NULL;
		free_student(old_student);
	}
	return head;
}

#if ( USE_GUI == TRUE )
int print_class_menu(char const *classname) {

	int selected;
	printf("当前操作的班级: %s\n", classname);
	printf(ROOT_MENU_LINE);
	printf("| 1. 修改班级名称   |\n");
	printf(ROOT_MENU_LINE);
	printf("| 2. 修改班级班主任 |\n");
	printf(ROOT_MENU_LINE);
	printf("| 3. 修改班级科目   |\n");
	printf(ROOT_MENU_LINE);
	printf("| 4. 删除班级科目   |\n");
	printf(ROOT_MENU_LINE);
	printf("| 5. 新增班级科目   |\n");
	printf(ROOT_MENU_LINE);
	printf("| 6. 查询班级科目   |\n");
	printf(ROOT_MENU_LINE);
	printf("| 0. 返回上一层     |\n");
	printf(ROOT_MENU_LINE);
	printf("| 请选择: ");
	selected = (int)input_number(0, 6);
	printf(ROOT_MENU_LINE);
	return selected;
}

int print_search_user_menu(void) {
	//ROOT_MENU_LINE
	int selected;
	printf("+-------------------+\n");
	printf("| 1. 查询所有       |\n");
	printf(ROOT_MENU_LINE);
	printf("| 2. 输入查询       |\n");
	printf(ROOT_MENU_LINE);
	printf("| 0. 返回上一层     |\n");
	printf(ROOT_MENU_LINE);
	printf("| 请输入: ");
	selected = (int)input_number(1, 2);
	printf(ROOT_MENU_LINE);
	return selected;
}

int print_class_name(class_p _class) {

	class_p temp;
	int i;
	
	i = 1;
	temp = _class;
	while ( temp != NULL ) {

		printf("%d. %s %s\n", i++, temp->name, temp->teacher->id);
		temp = temp->next;
	}
	return i-1;
}

/*
 *  增加科目
 */
_bool add_class_subject(class_p _class) {

	subject_p new_subject;
	subject_p temp;
	student_p student;
	score_p score;
	char *file;

	new_subject = create_subject();
#ifdef NEED_JUDGE
	if ( new_subject == NULL ) {

		perror("malloc");
		return FALSE;
	}
#endif
	if ( !get_class_subject(new_subject) ) {

		free_subject(new_subject);
		return FALSE;
	}
	temp = _class->subject;
	if ( temp == NULL ) {

		_class->subject = new_subject;
		new_subject->prev = _class->subject;
	} else {
		
		while ( temp->next != NULL ) {

			temp = temp->next;
		}
		temp->next = new_subject;
		new_subject->prev = temp;
	}
	if ( _class->student != NULL ) {

		student = _class->student;
		while ( student != NULL ) {

			score = student->score;
			while ( score->next != NULL ) {

				score = score->next;
			}
			score->next = create_score();
#ifdef NEED_JUDGE
			if ( score->next == NULL ) {

				perror("Add score");
				exit(0);
			}
#endif
			score->next->prev = score;
			student = student->next;
		}
		file = get_class_file_name(_class->name);
#ifdef NEED_JUDGE
		if ( file == NULL ) {

			perror("malloc");
			_class->subject = delete_subject_node(_class->subject, new_subject);
			_class->student->score = delete_score_node(_class->student->score, score->next);
			return FALSE;
		}
#endif
		save_student(_class->student, file);
		free(file);
	}
	return TRUE;
}
/*
 *  修改科目的信息
 */
_bool upd_class_subject_info(subject_p subject) {

	char name[SUBJECT_NAME_LENGTH];

	strcpy(name, subject->name);
	printf("%s\n", name);
input_name:
	printf("请输入科目名称: ");
	input_string(subject->name, SUBJECT_NAME_LENGTH);
	if ( subject->name[0] != '\0' ) {

		if ( !is_legal_name(subject->name) ) {

			printf("名称不合法，请重新输入\n");
			goto input_name;
		}
		strcpy(name, subject->name);
	}
	printf("请输入\"%s\"的学分: ", name);
	subject->credit = (float)input_number(0.0, 10.0);
	return TRUE;
}
/*
 *  更新学生成绩
 */
_bool upd_student_score(subject_p subject, score_p score) {

	_bool result = FALSE;
	score_p score_t;
	subject_p subject_t;
	float s;

	score_t = score;
	subject_t = subject;
	while ( score_t != NULL ) {

		printf("请输入\"%s\"的成绩: ", subject_t->name);
		s = (float)input_number(0, 100);
		if ( s >= 0.0f ) {

			result = TRUE;
			score_t->score = s;
		}
		subject_t = subject_t->next;
		score_t = score_t->next;
	}
	if ( score_t == NULL && subject_t != NULL ) {

		printf("出现了一个未知错误，本次修改不生效\n");
		return FALSE;
	}
	return result;
}
/*
 *  添加学生
 */
student_p get_student_info(class_p _class) {

	extern student_p s_head;
	student_p student;
	student_p stutemp;
	student_p stucurr;
	
	student = create_student();
#ifdef NEED_JUDGE
	if ( student == NULL ) {

		perror("malloc");
		return NULL;
	}
#endif
	if ( s_head == NULL ) {

		s_head = student;
	} else {

		stutemp = s_head;
		while ( stutemp->next != NULL ) {

			stutemp = stutemp->next;
		}
		stutemp->next = student;
		student->prev = stutemp;
	}
	stucurr = student;
	while ( TRUE ) {

		if ( !get_basemsg(stucurr->base, FALSE) ) {

			free_student(student);
			return NULL;
		}
		if ( !get_subject_score(stucurr->score, _class->subject) ) {

			free_student(student);
			return NULL;
		}
		if ( !confirm_opt("是否继续添加") ) {

			break;
		}
		stutemp = create_student();
#ifdef NEED_JUDGE
		if ( stutemp == NULL ) {

			perror("malloc");
			free_student(student);
			return NULL;
		}
#endif
		stucurr->next = stutemp;
		stutemp->prev = stucurr;
		stucurr = stutemp;
	}
	return student;
}

_bool get_subject_score(score_p score, subject_p subject) {

	score_p cscore;
	score_p tscore;
	subject_p tsubject;


	tsubject = subject;
	cscore = score;
	while ( tsubject != NULL ) {

		printf("请输入科目\"%s\"的成绩: ", tsubject->name);
		cscore->score = (float)input_number(0, 100);
		if ( cscore->score < 0.0f ) {

			cscore->score = 0.0f;
		}
		tscore = create_score();
#ifdef NEED_JUDGE
		if ( tscore == NULL ) {

			perror("malloc");
			free_score(score);
			return FALSE;
		}
#endif
		cscore->next = tscore;
		tscore->prev = cscore;
		cscore = tscore;
		tsubject = tsubject->next;
	}
	cscore->prev->next = NULL;
	cscore->prev = NULL;
	free_score(cscore);
	return TRUE;
}

_bool update_class_subject(subject_p subject) {

	subject_p temp;
	subject_p new_subject;
	int i;
	int selected;

	temp = subject;
	i = print_class_subject(temp);
	if ( i <= 0 ) {

		printf("放弃修改\n");
		return FALSE;
	}
	printf("请输入要修改的科目序号: ");
	selected = (int)input_number(1, i);
	if ( selected <= 0 ) {

		printf("放弃修改\n");
		return FALSE;
	}
	i = 1;
	temp = subject;
	while ( i < selected ) {

		temp = temp->next;
		i++;
	}
	new_subject = create_subject();
#ifdef NEED_JUDGE
	if ( new_subject == NULL ) {

		perror("malloc");
		return FALSE;
	}
#endif
	strcpy(new_subject->name, temp->name);
	if ( !upd_class_subject_info(new_subject) ) {

		printf("放弃修改\n");
		free_subject(new_subject);
		return FALSE;
	}
	if ( new_subject->name[0] != '\0' ) {

		strcpy(temp->name, new_subject->name);
	}
	if ( new_subject->credit > 0.0f ) {

		temp->credit = new_subject->credit;
	}
	free_subject(new_subject);
	return TRUE;
}
_bool delete_class_subject(class_p _class) {

	student_p student;
	score_p score;
	char *file;
	int i;
	int selected;
	subject_p temp;
	
	i = print_class_subject(_class->subject);
	if ( i < 1 ) {

		printf("当前无科目\n");
		return FALSE;
	}
	printf("请输入要删除的科目序号: ");
	selected = (int)input_number(1, i);
	if ( selected <= 0 ) {

		printf("放弃删除\n");
		return FALSE;
	}
	temp = _class->subject;
	i = 1;
	while ( i++ < selected ) {

		temp = temp->next;
	}
	if ( _class->subject->prev == NULL && _class->subject->next == NULL ) {

		printf("一个班级最少含有一门学科\n");
		return FALSE;
	}
	_class->subject = delete_subject_node(_class->subject, temp);
	if ( _class->student != NULL ) {

		student = _class->student;
		while ( student != NULL ) {

			i = 1;
			score = student->score;
			while ( i++ < selected ) {

				score = score->next;
			}
			student->score = delete_student_score(student->score, score);
			student = student->next;
		}
		file = get_class_file_name(_class->name);
#ifdef NEED_JUDGE
		if ( file == NULL ) {

			perror("malloc");
			exit(0);
		}
#endif
		save_student(_class->student, file);
		free(file);
	}
	return TRUE;
}

int print_class_subject(subject_p subject) {

	subject_p temp;
	int i;

	i = 1;
	temp = subject;
	while ( temp != NULL ) {

		printf("%d. %s(%0.1f)\n", i++, temp->name, temp->credit);
		temp = temp->next;
	}
	return i-1;
}
class_p get_class_info(class_p _class) {

	class_p c;
	subject_p new_subject;
	subject_p temp;

	c = create_class();
	if ( c == NULL ) {
		
		perror("malloc");
		return NULL;
	}
	if ( !get_class_name(c->name, _class) ) {

		printf("放弃添加\n");
		free_class(c);
		return NULL;
	}
	if ( !get_class_teacher(c->teacher->id) ) {

		printf("放弃添加\n");
		free_class(c);
		return NULL;
	}
	temp = c->subject;
	while ( TRUE ) {
		
		if ( !get_class_subject(temp) ) {

			printf("放弃添加\n");
			free_class(c);
			return NULL;
		}
		if ( confirm_opt("是否继续添加科目") ) {

			new_subject = create_subject();
#ifdef NEED_JUDGE
			if ( new_subject == NULL ) {

				perror("malloc");
				printf("无法继续添加\n");
				return c;
			}
#endif
			temp = c->subject;
			while ( temp->next != NULL ) {

				temp = temp->next;
			}
			temp->next = new_subject;
			new_subject->prev = temp;
			temp = new_subject;
		} else {

			break;
		}
	}
	return c;
}

_bool confirm_opt(char const *str) {

	char ch;
	printf("%s？(y/n) ", str);
	ch = input_char();
	if ( ch == 'Y' || ch == 'y' ) {

		return TRUE;
	} else {

		return FALSE;
	}
}

_bool get_class_name(char *name, class_p _class) {

	char temp[CLASS_NAME_LENGTH];
	
input_again:
	printf("班级名称: ");
	input_string(temp, CLASS_NAME_LENGTH);
	if ( temp[0] == '\0' ) {

		return FALSE;
	}
	if ( !is_legal_name(temp) ) {

		printf("名称不合法，请重新输入\n");
		goto input_again;
	}
	if ( has_same_classname(temp, _class) ) {

		printf("已存在相同名称的班级，请重新输入\n");
		goto input_again;
	}
	strcpy(name, temp);
	return TRUE;
}
_bool get_class_teacher(char *id) {

	char temp[ID_LENGTH];
	printf("班主任ID: ");
	input_string(temp, ID_LENGTH);
	if ( temp[0] == '\0' ) {

		return FALSE;
	}
	strcpy(id, temp);
	return TRUE;
}
_bool get_class_subject(subject_p subject) {

	char name[SUBJECT_NAME_LENGTH];
	float credit;

input_name:
	printf("科目名称: ");
	input_string(name, SUBJECT_NAME_LENGTH);
	if ( name[0] == '\0' ) {

		return FALSE;
	}
	if ( !is_legal_name(name) ) {

		printf("名称不合法，请重新输入\n");
		goto input_name;
	}
input_credit:
	printf("科目学分: ");
	credit = (float)input_number(0, 10);
	if ( credit <= 0.0 ) {

		goto input_credit;
	}
	strcpy(subject->name, name);
	subject->credit = credit;
	return TRUE;
}

subject_p add_subject(void) {

	subject_p temp;

	temp = create_subject();
#ifdef NEED_JUDGE
	if ( temp == NULL ) {

		perror("malloc");
		return NULL;
	}
#endif
	if ( !get_class_subject(temp) ) {

		free_subject(temp);
		temp = NULL;
	}
	return temp;
}

#endif

#if ( USE_CMD == TRUE )

#endif

_bool update_class_name(class_p head, class_p _class) {

	return get_class_name(_class->name, head);
}
_bool update_class_teacher(class_p _class) {

	return get_class_teacher(_class->teacher->id);
}


subject_p delete_subject_node(subject_p head, subject_p node) {

	if ( node == NULL ) {

		return head;
	}
	if ( node->prev == NULL && node->next == NULL ) {
		
		free_subject(node);
		head = NULL;
	} else if ( node->prev == NULL && node->next != NULL ) {

		head = head->next;
		head->prev = NULL;
		node->next = NULL;
		free_subject(node);
	} else if ( node->prev != NULL && node->next == NULL ) {

		node->prev->next = NULL;
		node->prev = NULL;
		free_subject(node);
	} else if ( node->prev != NULL && node->next != NULL ) {

		node->prev->next = node->next;
		node->next->prev = node->prev;
		node->next = NULL;
		node->prev = NULL;
		free_subject(node);
	}
	return head;
}

score_p delete_student_score(score_p head, score_p node) {

	if ( node == NULL ) {

		return head;
	}
	if ( node->prev == NULL && node->next == NULL ) {
		
		free_score(node);
		head = NULL;
	} else if ( node->prev == NULL && node->next != NULL ) {

		head = head->next;
		head->prev = NULL;
		node->next = NULL;
		free_score(node);
	} else if ( node->prev != NULL && node->next == NULL ) {

		node->prev->next = NULL;
		node->prev = NULL;
		free_score(node);
	} else if ( node->prev != NULL && node->next != NULL ) {

		node->prev->next = node->next;
		node->next->prev = node->prev;
		node->next = NULL;
		node->prev = NULL;
		free_score(node);
	}
	return head;
}

base_p copy_base(base_p bmsg) {

	base_p dest;

	if ( bmsg == NULL ) {

		return NULL;
	}
	dest = create_base();
#ifdef NEED_JUDGE
	if ( dest == NULL ) {

		perror("malloc");
		return NULL;
	}
#endif
	strcpy(dest->name, bmsg->name);
	strcpy(dest->passwd, bmsg->passwd);
	strcpy(dest->id, bmsg->id);
	dest->age = bmsg->age;
	dest->gender = bmsg->gender;
	return dest;
}

class_p copy_class(class_p _class) {

	class_p backup;

	if ( _class == NULL ) {

		return NULL;
	}
	backup = create_class();
#ifdef NEED_JUDGE
	if ( backup == NULL ) {

		perror("malloc");
		return NULL;
	}
#endif
	strcpy(backup->name, _class->name);
	strcpy(backup->teacher->id, _class->teacher->id);
	free_subject(backup->subject);
	backup->subject = copy_subject(_class->subject);
#ifdef NEED_JUDGE
	if ( backup->subject == NULL ) {

		free_class(backup);
		return NULL;
	}
#endif
	if ( _class->student != NULL ) {
	
		free_student(backup->student);
		backup->student = copy_student(_class->student);
#ifdef NEED_JUDGE
		if ( backup->student == NULL ) {

			free_class(backup);
		}
#endif
	}
	return backup;
}

subject_p copy_subject(subject_p subject) {

	subject_p backup;
	subject_p backup_c;
	subject_p backup_t;
	subject_p temp;

	if ( subject == NULL ) {

		return NULL;
	}
	backup = create_subject();
#ifdef NEED_JUDGE
	if ( backup == NULL ) {

		perror("malloc");
		return NULL;
	}
#endif
	temp = subject;
	backup_c = backup;
	strcpy(backup->name, temp->name);
	backup->credit = temp->credit;
	while ( temp->next != NULL ) {

		temp = temp->next;
		backup_t = create_subject();
#ifdef NEED_JUDGE
		if ( backup_t == NULL ) {

			perror("malloc");
			free_subject(backup);
			return NULL;
		}
#endif
		strcpy(backup_t->name, temp->name);
		backup_t->credit = temp->credit;
		backup_c->next = backup_t;
		backup_t->prev = backup_c;
		backup_c = backup_t;
	}
	return backup;
}
/*
 *  复制学生信息
 */
student_p copy_student(student_p source) {

	student_p student;
	student_p s_curr;
	student_p s_temp;

	if ( source == NULL ) {

		return NULL;
	}
	student = create_student();
#ifdef NEED_JUDGE
	if ( student == NULL ) {

		perror("malloc");
		return NULL;
	}
#endif
	s_curr = student;
	while ( source != NULL ) {

		free_base(s_curr->base);
		free_score(s_curr->score);
		s_curr->base = copy_base(source->base);
		s_curr->score = copy_score(source->score);
		s_curr->gpa = source->gpa;
#ifdef NEED_JUDGE
		if ( s_curr->base == NULL || s_curr->score == NULL ) {

			perror("malloc");
			free_student(student);
			return NULL;
		}
#endif
		s_temp = create_student();
#ifdef NEED_JUDGE
		if ( s_temp == NULL ) {

			perror("malloc");
			free_student(student);
			return NULL;
		}
#endif
		s_curr->next = s_temp;
		s_temp->prev = s_curr;
		s_curr = s_temp;
		source = source->next;
	}
	student = delete_student_node(student, s_curr);
	return student;
}
/*
 *  复制科目信息
 */
score_p copy_score(score_p source) {

	score_p score;
	score_p temp;
	score_p curr;

	if ( source == NULL ) {

		return NULL;
	}
	score = create_score();
#ifdef NEED_JUDGE
	if ( score == NULL ) {

		perror("malloc");
		return NULL;
	}
#endif
	curr = score;
	while ( source != NULL ) {

		curr->score = source->score;
		temp = create_score();
#ifdef NEED_JUDGE
		if ( temp == NULL ) {

			perror("malloc");
			free_score(score);
			return NULL;
		}
#endif
		curr->next = temp;
		temp->prev = curr;
		curr = temp;
		source = source->next;
	}
	score = delete_score_node(score, curr);
	return score;
}

void print_list(list_p list) {

	list_p temp;
	temp = list;
	while ( temp != NULL ) {

		printf("list: %s\n", temp->item);
		temp = temp->next;
	}
}
/*
 *  从一个文件中读取数据，
 *  返回值为一条存有很多项数据的链表。
 */
list_p get_file_list(char const *filename, char const split) {

	list_p head = NULL;
	list_p tlist;
	list_p clist;
	FILE *fp;
	char ch;
	char *buffer;
	char *bp;
	int current_size = 0;
	int total_size   = 100;

	fp = fopen(filename, FILE_READ);
	if ( fp == NULL ) {

		return NULL;
	}
	if ( fgetc(fp) == EOF ) {

		fclose(fp);
		return NULL;
	} else {

		fseek(fp, -1L, SEEK_CUR);
	}
	buffer = (char*)malloc(sizeof(char) * total_size);
#ifdef NEED_JUDGE
	if ( buffer == NULL ) {

		fclose(fp);
		return NULL;
	}
#endif
	head = create_list();
#ifdef NEED_JUDGE
	if ( head == NULL ) {

		free(buffer);
		fclose(fp);
		return NULL;
	}
#endif
	clist = head;
	while ( ( ch = fgetc(fp) ) != EOF ) {
	
		ch = decode_char(ch);
		if ( ch != split ) {

			*(buffer+current_size++) = ch;
			if ( current_size >= total_size ) {

				total_size += 100;
				bp = realloc(buffer, total_size);
#ifdef NEED_JUDGE
				if ( bp == NULL ) {

					free(buffer);
					free_list(head);
					fclose(fp);
					return NULL;
				}
#endif
				buffer = bp;
			}
		} else {

			*(buffer+current_size) = '\0';
			current_size = 0;
			clist->item = (char*)malloc(sizeof(char) * strlen(buffer)+1);
			tlist = create_list();
#ifdef NEED_JUDGE
			if ( tlist == NULL || clist->item == NULL ) {

				fclose(fp);
				free(buffer);
				free_list(head);
				return NULL;
			}
#endif
			strcpy(clist->item, buffer);
			clist->next = tlist;
			tlist->prev = clist;
			clist = tlist;
		}
	}
//	print_list(head);
	if ( tlist->item == NULL ) {

		clist = clist->prev;
		tlist = NULL;
		clist->next = NULL;
		free_list(tlist);
	}
	fclose(fp);
	free(buffer);
	return head;
}

_bool  write_list(list_p list, char const *filename) {

	list_p temp;
	char *p;
	FILE *fp;

	fp = fopen(filename, FILE_WRITE);
	if ( fp == NULL ) {

		perror(filename);
		return FALSE;
	}
	temp = list;
	while ( temp != NULL ) {

		p = encode_str(temp->item);
		while ( *p != '\0' ) {

			fputc(*p++, fp);
		}
		temp = temp->next;
	}
	fclose(fp);
	return TRUE;
}
/*
 *  从一条信息中提取（人的）基础信息（base）
 */
char *get_list_base(base_p bmsg, char *p, char const split) {

	char age[4];
	char gender[2];

	p = get_split_string(bmsg->id, p, split);
	p = get_split_string(bmsg->passwd, p, split);
	p = get_split_string(bmsg->name, p, split);
	p = get_split_string(age, p, split);
	p = get_split_string(gender, p, split);
	bmsg->age = atoi(age);
	bmsg->gender = atoi(gender);

	return p;
}
/*
 *  从文件中读取班级信息，返回一条存有各个班级信息的链表。
 */
class_p get_class_list(void) {
#define TEST
#ifdef TEST
	list_p list;
	list_p l_temp;
	class_p c_head;
	class_p c_curr;
	class_p c_temp;
	subject_p subject_c;
	subject_p subject_t;
	char credit[4];
	char *p;

	list = get_file_list(CLASS_CONFIG_FILE, END_SYMBOL);
	if ( list == NULL ) {

		return NULL;
	}
	l_temp = list;
	c_head = create_class();
#ifdef NEED_JUDGE
	if ( c_head == NULL ) {

		free_list(list);
		return NULL;
	}
#endif
	c_curr = c_head;
	while ( l_temp != NULL ) {

		p = l_temp->item;
		p = get_split_string(c_curr->name, p, SPLIT_SYMBOL);
		p = get_split_string(c_curr->teacher->id, p, SPLIT_SYMBOL);
		subject_c = c_curr->subject;
		while ( TRUE ) {

			p = get_split_string(subject_c->name, p, SPLIT_SYMBOL); /* 科目名称 */
			p = get_split_string(credit, p, SPLIT_SYMBOL); /* 科目学分 */
			subject_c->credit = (float)atof(credit);
			if ( p == NULL ) {

				break;
			}
			subject_t = create_subject();
#ifdef NEED_JUDGE
			if ( subject_t == NULL ) {

				perror("malloc");
				free_class(c_head);
				return NULL;
			}
#endif
			subject_c->next = subject_t;
			subject_t->prev = subject_c;
			subject_c = subject_t;
		}
		c_temp = create_class();
#ifdef NEED_JUDGE
		if ( c_temp == NULL ) {

			free_list(list);
			free_class(c_head);
			return NULL;
		}
#endif
		c_curr->next = c_temp;
		c_temp->prev = c_curr;
		c_curr = c_temp;
		l_temp = l_temp->next;
	}
	c_head = delete_class_node(c_head, c_curr);
	free_list(list);
	return c_head;
#endif
#ifndef TEST
	int current_size = 0;
	int total_size   = 100;
	char ch;
	char *buffer;
	char *p;
	char credit[4];
	class_p _class;
	class_p class_c;
	class_p class_t;
	subject_p subject_c;
	subject_p subject_t;
	FILE *fp;

	fp = fopen(CLASS_CONFIG_FILE, FILE_READ);
	if ( fp == NULL ) {

		perror(CLASS_CONFIG_FILE);
		return NULL;
	}
	if ( fgetc(fp) == EOF ) {

		fclose(fp);
		return NULL;
	} else {
		
		fseek(fp, -1L, SEEK_CUR);
	}
	_class = create_class();
	if ( _class == NULL ) {

		perror("malloc");
		fclose(fp);
		return NULL;
	}
	buffer = (char*)malloc(sizeof(char) * total_size);
	if ( buffer == NULL ) {

		perror("malloc");
		fclose(fp);
		return NULL;
	}
	class_c = _class;
	while ( ( ch = fgetc(fp) ) != EOF ) {

		ch = decode_char(ch);
		if ( ch != END_SYMBOL ) {

			*(buffer + current_size++) = ch;
			if ( current_size >= total_size ) {

				total_size += 100;
				p = realloc(buffer, total_size);
				if ( p == NULL ) {

					perror("realloc");
					free(buffer);
					free_class(_class);
					fclose(fp);
					return NULL;
				} else {

					buffer = p;
				}
			}
		} else {

			*(buffer + current_size) = '\0';
			current_size = 0;
			p = get_split_string(class_c->name, buffer, SPLIT_SYMBOL);   /* 班级名称 */
			p = get_split_string(class_c->teacher->id, p, SPLIT_SYMBOL); /* 班主任ID */
			subject_c = class_c->subject;
			while ( TRUE ) {

				p = get_split_string(subject_c->name, p, SPLIT_SYMBOL); /* 科目名称 */
				p = get_split_string(credit, p, SPLIT_SYMBOL); /* 科目学分 */
				subject_c->credit = (float)atof(credit);
				if ( p == NULL ) {

					break;
				}
				subject_t = create_subject();
				if ( subject_t == NULL ) {

					perror("malloc");
					free_class(_class);
					fclose(fp);
					return NULL;
				}
				subject_c->next = subject_t;
				subject_t->prev = subject_c;
				subject_c = subject_t;
			}
			if ( fgetc(fp) != EOF ) {

				fseek(fp, -1L, SEEK_CUR);
				class_t = create_class();
				if ( class_t == NULL ) {

					perror("malloc");
					free_class(_class);
					fclose(fp);
					return NULL;
				}
				class_c->next = class_t;
				class_t->prev = class_c;
				class_c = class_t;
			}
		}
	}
	free(buffer);
	fclose(fp);
	return _class;
#endif
}
/*
 *  得到班级链表的尾节点。
 */
class_p get_class_tail(class_p _class) {

	class_p temp;

	if ( _class == NULL ) {

		return NULL;
	}
	temp = _class;
	while ( temp->next != NULL ) {

		temp = temp->next;
	}
	return temp;
}

/*
 *  获取文件名长度（包括路径）
 */
int get_class_file_len(char const *classname) {

	return (int)( strlen(CONFIG_FILE_PATH) + strlen(classname) + strlen(FILE_LAST_NAME) );
}
/*
 *  获取存放学生信息的文件名
 */
char *get_class_file_name(char const *classname) {

	char *file;
	int len;

	len = ( ( sizeof(char) * get_class_file_len(classname) ) + 1 );
	file = (char*)malloc(len);
#ifdef NEED_JUDGE
	if ( file == NULL ) {

		perror("malloc");
		return NULL;
	}
#endif
	memset(file, 0, len);
	strcat(file, CONFIG_FILE_PATH);
	strcat(file, classname);
	strcat(file, FILE_LAST_NAME);
	return file;
}
/*
 *
 */
student_p get_class_student(char const *filename) {

	list_p list;
	list_p l_temp;
	student_p student;
	student_p s_curr;
	student_p s_temp;
	score_p score;
	score_p tscore;
	char score_buffer[4];
	char *p;

	list = get_file_list(filename, END_SYMBOL);
	if ( list == NULL ) {

		return NULL;
	}
	/*
	l_temp = list;
	while ( l_temp != NULL ) {

		printf("list: %s\n", l_temp->item);
		l_temp = l_temp->next;
	}
	*/
	student = create_student();
#ifdef NEED_JUDGE
	if ( student == NULL ) {

		perror("malloc");
		free_list(list);
		return NULL;
	}
#endif
	l_temp = list;
	s_curr = student;
//	print_list(list);
	while ( l_temp != NULL ) {

		p = l_temp->item;
		p = get_list_base(s_curr->base, p, SPLIT_SYMBOL); /* 获得学生的基本信息 */
		/*
		 *  开始获取学生的各科成绩
		 */
		score = s_curr->score;
		while ( TRUE ) {

			p = get_split_string(score_buffer, p, SPLIT_SYMBOL);
			score->score = (float)atof(score_buffer);
			if ( p == NULL ) {

				break;
			}
			tscore = create_score();
#ifdef NEED_JUDGE
			if ( tscore == NULL ) {

				free_student(student);
				free_list(list);
				return NULL;
			}
#endif
			score->next = tscore;
			tscore->prev = score;
			score = tscore;
		}
		s_temp = create_student();
#ifdef NEED_JUDGE
		if ( s_temp == NULL ) {

			free_student(student);
			free_list(list);
			return NULL;
		}
#endif
		s_curr->next = s_temp;
		s_temp->prev = s_curr;
		s_curr = s_temp;
		l_temp = l_temp->next;
	}
	student = delete_student_node(student, s_curr);
	free_list(list);
	return student;
}
/*
 *  该函数用来提取一串有规律的子字符串。
 */
char *get_split_string(char *dest, char *str, char split) {

	if ( dest == NULL || str == NULL ) {

		return str;
	}
	if ( split == '\0' ) {

		return str;
	}
	while ( ( *str != split ) && ( *str != '\0' ) ) {

		*dest++ = *str++;
	}
	*dest = '\0';
	if ( *str == '\0' ) {

		return NULL;
	}
	str++;
	if ( *str == '\0' ) {

		return NULL;
	}
	return str;
}
/*
 *  检查用户名是否合法，
 *  不合法案例：root, 带有字母（大小写）、数字和中文意外的其他字符
 */
_bool is_legal_name(char const *name) {

	if ( !strcmp(name, ROOT_NAME) ) {

		return FALSE;
	}
	while ( *name != '\0' ) {
		/*
		 *  中文
		 */
		if ( !IS_CHINESE(*name) && !IS_NUMBER(*name) && !IS_LETTER(*name) ) {

			return FALSE;
		}
		name++;
	}
	return TRUE;
}

/*
 *  加密数据
 */
char *encode_str(char *str) {

#ifdef NEED_ENCODE
	static char encode_char(char ch);
	char *temp = str;

	while ( *temp != '\0' ) {

		*temp = encode_char(*temp);
		temp++;
	}
#endif
	return str;
}
/*
 *  解密数据
 */
char *decode_str(char *str) {

#ifdef NEED_ENCODE
	char *temp = str;

	while ( *temp != '\0' ) {

		*temp = decode_char(*temp);
		temp++;
	}
#endif
	return str;
}

char encode_char(char ch) {

#ifdef NEED_ENCODE
	return ( ( ch & 0x07 ) << 5 ) | ( ( ch & 0xf8 ) >> 3 );
#else
	return ch;
#endif
}
char decode_char(char ch) {

#ifdef NEED_ENCODE
	return ( ( ch & 0xe0 ) >> 5 ) | ( ( ch & 0x1f ) << 3 );
#else
	return ch;
#endif
}

void swap_string(char *a, char *b) {

	int al = strlen(a);
	int bl = strlen(b);
	char *temp = (char*)malloc(sizeof(char) * MAX(al, bl)+1);
	if ( temp == NULL ) {
		
		perror("malloc");
		return;
	}
	strcpy(temp, a);
	strcpy(a, b);
	strcpy(b, temp);
	free(temp);
}
/*
 *  计算学生的平均绩点（GPA），指的是学分绩点。
 */
void calc_student_gpa(class_p chead) {

	student_p student;
	subject_p subject;
	score_p score;
	int i;
	int subject_total = 0;
	float subject_credit[1024];
	float gpa;
	float t_gpa;
	float credit_sum;

	subject = chead->subject;
	student = chead->student;
	/*
	 *  获得该班级的科目总数以及各科目的学分
	 *  计算公式： 平均绩点 = ( 各科绩点 * 各科学分 ) 之和 / 总学分
	 */
	credit_sum = 0.0f;
	while ( subject != NULL ) {

		subject_credit[subject_total++] = subject->credit;
		credit_sum += subject->credit;
		subject = subject->next;
	}
	while ( student != NULL ) {

		score = student->score;
		i = 0;
		gpa = 0.0f;
		while ( score != NULL && i < subject_total ) {

			t_gpa = ( score->score / 10.0f - 5.0f ) * subject_credit[i++];
			gpa += t_gpa > 0.0f ? t_gpa : 0.0f;
			score = score->next;
		}
		student->gpa = gpa / credit_sum;
		student = student->next;
	}
}
/*
 *  交换student节点位置
 */
void swap_student_node(student_p a, student_p b) {

	student_p a_next;
	student_p a_prev;

	if ( a->next == b ) { /* 相邻节点，a在前 */

		if ( b->next != NULL ) {

			b->next->prev = a;
		}
		if ( a->prev != NULL ) {

			a->prev->next = b;
		}
		a->next = b->next;
		b->prev = a->prev;
		a->prev = b;
		b->next = a;
	} else if ( a->prev == b ) { /* 相邻节点，b在前 */

		if ( a->next != NULL ) {

			a->next->prev = b;
		}
		if ( b->prev != NULL ) {

			b->prev->next = a;
		}
		b->next = a->next;
		a->prev = b->prev;
		b->prev = a;
		a->next = b;
	} else { /* 非相邻节点 */

		if ( a->next != NULL ) {

			a->next->prev = b;
		}
		if ( a->prev != NULL ) {

			a->prev->next = b;
		}
		if ( b->next != NULL ) {

			b->next->prev = a;
		}
		if ( b->prev != NULL ) {

			b->prev->next = a;
		}
		a_next = a->next;
		a_prev = a->prev;
		a->next = b->next;
		a->prev = b->prev;
		b->next = a_next;
		b->prev = a_prev;
	}
}
#ifdef USE_SORT_CALLBACK
/*
 *  学生排序，回调方式
 */
class_p sort_student_template(class_p _class, _bool (*sort_ways)(void *a, void *b)) {

	class_p temp;
	student_p student;
	student_p s_temp;
	student_p s;

	if ( _class == NULL || _class->student == NULL ) {

		return NULL;
	}
	temp = copy_class(_class);
	if ( temp == NULL ) {

		return NULL;
	}
	/*
	 *  排名
	 */
	student = temp->student;
	s = student;
	while ( student->next != NULL ) {

		s_temp = student->next;
		while ( s_temp != NULL ) {

			if ( sort_ways(s_temp, student) ) {

				swap_student_node(s_temp, student);
				s = s_temp;
				s_temp = student;
				student = s;
			}
			s_temp = s_temp->next;
		}
		if ( student->next != NULL ) {

			s = student;
		}
		student = student->next;
	}
	/*
	 *  回到头结点
	 */
	while ( s->prev != NULL ) {

		s = s->prev;
	}
	temp->student = s;
	return temp;
}
_bool sort_for_gpa(void *a, void *b) {

	return ( (student_p) a )->gpa > ( (student_p) b )->gpa ? TRUE : FALSE;
}
_bool sort_for_id(void *a, void *b) {

	return strcmp(( (student_p) a)->base->id, ( (student_p) b )->base->id) < 0 ? TRUE : FALSE;
}
#else
/*
 *  学生排序，非回调方式
 */
/*
 *  以gpa排序，返回值为复制节点，需要手动释放
 */
class_p sort_gpa(class_p _class) {

	class_p temp;
	student_p student;
	student_p s_temp;
	student_p s;

	if ( _class == NULL || _class->student == NULL ) {

		return NULL;
	}
	temp = copy_class(_class);
	if ( temp == NULL ) {

		return NULL;
	}
	/*
	 *  排名
	 */
	student = temp->student;
	s = student;
	while ( student->next != NULL ) {

		s_temp = student->next;
		while ( s_temp != NULL ) {

			if ( s_temp->gpa > student->gpa ) {

				swap_student_node(s_temp, student);
				s = s_temp;
				s_temp = student;
				student = s;
			}
			s_temp = s_temp->next;
		}
		if ( student->next != NULL ) {

			s = student;
		}
		student = student->next;
	}
	/*
	 *  回到头结点
	 */
	while ( s->prev != NULL ) {

		s = s->prev;
	}
	temp->student = s;
	return temp;
}
/*
 *  以id排序，返回值为复制节点，需要手动释放
 */
class_p sort_id(class_p _class) {

	class_p temp;
	student_p student;
	student_p s_temp;
	student_p s;

	if ( _class == NULL || _class->student == NULL ) {

		return NULL;
	}
	temp = copy_class(_class);
	if ( temp == NULL ) {

		return NULL;
	}
	/*
	 *  排名
	 */
	student = temp->student;
	s = student;
	while ( student->next != NULL ) {

		s_temp = student->next;
		while ( s_temp != NULL ) {

			if ( strcmp(s_temp->base->id, student->base->id) < 0 ) {

				swap_student_node(s_temp, student);
				s = s_temp;
				s_temp = student;
				student = s;
			}
			s_temp = s_temp->next;
		}
		if ( student->next != NULL ) {

			s = student;
		}
		student = student->next;
	}
	/*
	 *  回到头结点
	 */
	while ( s->prev != NULL ) {

		s = s->prev;
	}
	temp->student = s;
	return temp;
}
#endif
/*
 *  按id查询，返回值为复制节点，需要手动释放
 */
class_p search_student_id(class_p _class) {

	char id[ID_LENGTH];
	class_p temp;
	student_p student;
	student_p s;

	temp = copy_class(_class);
	if ( temp == NULL ) {

		return NULL;
	}
	printf("请输入要查询的学生ID: ");
	input_string(id, ID_LENGTH);
	student = temp->student;
	while ( student != NULL ) {

		if ( !strcmp(student->base->id, id) ) {

			break;
		}
		student = student->next;
	}
	if ( student == NULL ) {

		printf("查无此人\n");
		free_class(temp);
		return NULL;
	} else if ( student->prev == NULL && student->next == NULL ) {
		
		return temp;
	} else if ( student->prev == NULL && student->next != NULL ) {

		s = student->next;
		student->next = NULL;
		s->prev = NULL;
		temp->student = student;
		free_student(s);
	} else if ( student->prev != NULL && student->next == NULL ) {

		student->prev->next = NULL;
		student->prev = NULL;
		free_student(temp->student);
		temp->student = student;
	} else /* if ( student->prev != NULL && student->next != NULL ) */ {

		student->next->prev = student->prev;
		student->prev->next = student->next;
		student->next = NULL;
		student->prev = NULL;
		free_student(temp->student);
		temp->student = student;
	}
	return temp;
}