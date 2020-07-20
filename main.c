/*
 *  Filename: main.c
 *  Author: AivenLi
 *	Copyright 2019-2020 Aiven.Li All rights reserved.
 *
 *
 *  声明：
 *      1. 本软件的所有内容仅供个人学习，不保证内容的正确性。通过使用本软件
 *  随之而来的风险与作者无关。当使用本软件时，代表你已接受作者的免费声明等条款。
 *		2. 未经作者允许，不可用作商业用途，否则后果自负。
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include "student/student.h"
#include "input/myinput.h"
#include "include/mtype.h"
#include "table/table.h"

#ifdef NEED_DEBUG
static int get_basemsg_string_len(base_p bmsg);             /* 获得基本信息所占的字符长度                     */
#endif
static _bool is_legal_id(char const *id, _bool is_teacher); /* 判断ID是否合法，不相同和数字字符则合法         */
static _bool upd_basemsg(base_p bmsg);                      /* 更新基本信息                                   */
static void check_config_file(void);                        /* 程序启动时先检测配置文件                       */
static teacher_p init_teacher(void);                        /* 初始化教师链表                                 */
static void init_class_student(void);                       /* 初始化学生信息，将学生信息链表连接到对应的班级 */      
static _bool save_teacher(teacher_p teacher);               /* 将教师信息保存到文件                           */
static _bool save_class(class_p _class);                    /* 将班级信息保存到文件                           */
static FILE *write_file(char *str, FILE *fp);               /* 版本问题，该函数是之前版本的，                 */
static void get_user_info(char *id, char *passwd);          /* 获取用户信息，登录时用                         */
static class_p get_my_class(void);                          /* 用户登录后获得用户管理的班级                   */
static teacher_p login(char const *id, char const *passwd, teacher_p teachers);
static FILE *write_basemsg_to_file(base_p bmsg, FILE *fp);  /* 将基本信息写入文件                             */
static FILE *read_basemsg_from_file(base_p bmsg, FILE *fp); /* 读取                                           */
#if ( USE_GUI == TRUE )                                     /* 使用界面                                       */
static int print_root_menu(void);                           /* 打印root用户界面                               */
static int print_user_menu(void);                           /* 打印普通用户界面，但未使用，都调用root用户界面 */
static int print_search_student_menu(void);                  /* 查询学生信息界面                              */
static int print_student_menu(void);
#endif
#if ( USE_CMD == TRUE )                                     /* 使用命令行模式                                 */
                                                            /* 暂未开发，命令行模式相对较难                   */
#endif

static _bool upd_name(char *old_name);
static _bool upd_id(char *old_id, _bool is_teacher);
static _bool upd_passwd(char *old_passwd);
static _bool upd_gender(int *old_gender);
static _bool upd_age(int *old_age);

static void print_user_basemsg(base_p bmsg);
static void print_table(int times);

static void root_opt(void);
static void user_opt(void);
static void add_teacher(void);
static void del_teacher(void);
static void sea_teacher(void);
static void upd_teacher(void);
static void upd_myself(void);
static void opt_student(void);
static void add_class(void);
static void del_class(void);
static void sea_class(void);
static void upd_class(void);
static void add_student(class_p _class);
static void del_student(class_p _class);
static void sea_student(class_p _class);
static void upd_student(class_p _class);

teacher_p t_head = NULL;
teacher_p user   = NULL;
student_p s_head = NULL;
class_p   c_head = NULL;
//filemap user_file;

int main(void) {

	char username[PEOPLE_NAME_LENGTH+1000];
	char userpasswd[PASSWD_LENGTH];
	teacher_p teacher;
	
	system("title 学生信息管理系统(v 1.0)");
	check_config_file();

	teacher = init_teacher();
	c_head  = get_class_list();
	init_class_student();
	if ( teacher == NULL ) {

		return 0;
	} else {

		t_head = teacher;
		user = teacher;
#ifdef NEED_DEBUG
		show_base(teacher, NULL, TRUE); /* 调试信息，打开的话将会在登录前显示所有用户。 */
#endif
	}
	while ( 1 ) {
		
		get_user_info(username, userpasswd);
		user = login(username, userpasswd, teacher);
		if ( user != NULL ) {

			break;
		}
	}
#if ( USE_GUI == TRUE )
	if ( !strcmp(user->base->name, ROOT_NAME) ) {

		root_opt();
	} else {

		user->_class = get_my_class();
		root_opt();
	}
#endif
	free_teacher(t_head);
	free_class(c_head);
	free_student(s_head);
	return 0;
}
/*
 *  检查配置文件。通过检查配置文件来判断是否存在用户，
 *  基本上只要user.stu不存在或者被篡改就程序就会默认是
 *  第一次运行，需要重置root密码。
 */
static void check_config_file(void) {

	FILE *fp;
	char passwd[PASSWD_LENGTH];
	char temp_passwd[PASSWD_LENGTH];
	base_p bmsg;

	fp = fopen(ROOT_CONFIG_FILE, FILE_READ);
	if ( fp == NULL ) {

		printf("初始化完成...以下是系统级账户，请创建一个验证密码\n");
		printf("用户名: %s\n", ROOT_NAME);
again_input:
		printf("请输入密码: ");
		input_passwd(passwd, PASSWD_LENGTH);
		printf("再次确认: ");
		input_passwd(temp_passwd, PASSWD_LENGTH);
		if ( strcmp(passwd, temp_passwd) ) {

			printf("\n两次密码不一致，请重新输入\n");
			memset(passwd, 0, PASSWD_LENGTH);
			memset(temp_passwd, 0, PASSWD_LENGTH);
			goto again_input;
		}
		fp = fopen(ROOT_CONFIG_FILE, FILE_WRITE);
		if ( fp == NULL ) {

			perror(ROOT_CONFIG_FILE);
			exit(0);
		}
		bmsg = create_base();
#ifdef NEED_JUDGE
		if ( bmsg == NULL ) {

			printf("内存不足\n");
			fclose(fp);
			remove(ROOT_CONFIG_FILE);
			exit(0);
		}
#endif
		strcpy(bmsg->id, ROOT_ID);
		strcpy(bmsg->passwd, passwd);
		strcpy(bmsg->name, ROOT_NAME);
		bmsg->age = ROOT_AGE;
		bmsg->gender = ROOT_GENDER;
		fp = write_basemsg_to_file(bmsg, fp);
		if ( fp == NULL ) {

			printf("内存不足\n");
			fclose(fp);
			remove(ROOT_CONFIG_FILE);
			printf("Failed\n");
		} else {
			
			printf("\n请记住root账户的密码：%s，并重新启动程序\n", passwd);
		}
		fclose(fp);
		free_base(bmsg);
		fp = fopen(CLASS_CONFIG_FILE, FILE_READ);
		if ( fp == NULL ) {

			fp = fopen(CLASS_CONFIG_FILE, FILE_WRITE);
			if ( fp == NULL ) {

				perror(CLASS_CONFIG_FILE);
			} else {

				fclose(fp);
			}
		} else {

			fclose(fp);
		}
		system("pause");
		exit(0);
	} else {
		
		fclose(fp);
		if ( access(CLASS_CONFIG_FILE, 0) ) {

			fp = fopen(CLASS_CONFIG_FILE, FILE_WRITE);
			if ( fp == NULL ) {

				perror(CLASS_CONFIG_FILE);
				exit(0);
			} else {

				fclose(fp);
			}
		}
	}
}
/*
 *  初始化教师信息，返回一条存有所有教师信息的链表
 */
static teacher_p init_teacher(void) {

	teacher_p teacher;
	teacher_p t_curr;
	teacher_p t_temp;
	list_p list;
	list_p l_temp;
	char *p;

	list = get_file_list(USER_CONFIG_FILE, END_SYMBOL);
	if ( list == NULL ) {

		printf("暂无教师信息\n");
		return NULL;
	}
	teacher = create_teacher();
#ifdef NEED_JUDGE
	if ( teacher == NULL ) {

		perror("malloc");
		free_list(list);
		return NULL;
	}
#endif
	
	l_temp = list;
	t_curr = teacher;
	while ( l_temp != NULL ) {

		p = l_temp->item;
		p = get_list_base(t_curr->base, p, SPLIT_SYMBOL);
		l_temp = l_temp->next;
		t_temp = create_teacher();
#ifdef NEED_JUDGE
		if ( t_temp == NULL ) {

			perror("malloc");
			free_list(list);
			free_teacher(teacher);
			return NULL;
		}
#endif
		t_curr->next = t_temp;
		t_temp->prev = t_curr;
		t_curr = t_temp;
	}
	teacher = delete_teacher_node(teacher, t_curr);
	free_list(list);
	return teacher;
}
/*
 *  初始化班级信息，从文件中读取数据到内存。
 */
 static void init_class_student(void) {

	 class_p c;
	 char *file;
	 student_p student;

	 c = c_head;
	 while ( c != NULL ) {

		file = get_class_file_name(c->name);
		if ( file != NULL ) {

			c->student = get_class_student(file);
			//print_student(c->student);
			
			if ( c->student != NULL ) {

				if ( s_head == NULL ) {

					s_head = copy_student(c->student);
				} else {

					student = s_head;
					while ( student->next != NULL ) {

						student = student->next;
					}
					student->next = copy_student(c->student);
					student->next->prev = student;
				}
			}
			free(file);
		}
		c = c->next;
	 }
 }
/*
 *  保存改动过的教师信息到文件，成功返回TRUE，失败返回FALSE。
 */
static _bool save_teacher(teacher_p teacher) {

	teacher_p temp;
	FILE *fp;

	fp = fopen(TEMP_USER_CONFIG_FILE, FILE_WRITE);
	if ( fp == NULL ) {

		perror(USER_CONFIG_FILE);
		return FALSE;
	}
	temp = teacher;
	while ( temp != NULL ) {

		fp = write_basemsg_to_file(temp->base, fp);
		temp = temp->next;
	}
	fclose(fp);
	remove(USER_CONFIG_FILE);
	rename(TEMP_USER_CONFIG_FILE, USER_CONFIG_FILE);
	return TRUE;
}
/*
 *  同上。
 */
static _bool save_class(class_p _class) {

	class_p temp;
	subject_p subject;
	char class_name[CLASS_NAME_LENGTH];
	char id[ID_LENGTH];
	char subject_name[SUBJECT_NAME_LENGTH];
	char credit[4];
	FILE *fp;

	if ( _class == NULL ) {

		return FALSE;
	}
	fp = fopen(CLASS_CONFIG_FILE, FILE_WRITE);
	if ( fp == NULL ) {

		perror(CLASS_CONFIG_FILE);
		return FALSE;
	}
	temp = _class;
	while ( temp != NULL ) {
		
	//	printf("%s %s ", temp->name, temp->teacher->id);
		strcpy(class_name, temp->name);
		strcpy(id, temp->teacher->id);
		fp = write_file(class_name, fp);
		fp = write_file(id, fp);
		subject = temp->subject;
		while ( subject != NULL ) {

			strcpy(subject_name, subject->name);
			fp = write_file(subject_name, fp);
			subject->credit *= 1.0f;
			gcvt(subject->credit, 2, credit);
			fp = write_file(credit, fp);
	//		printf(" %s(%0.1f) ", subject->name, subject->credit);
			subject = subject->next;
		}
	//	printf("\n");
		fputc(encode_char(END_SYMBOL), fp);
		temp = temp->next;
	}
	fclose(fp);
	return TRUE;
	
}
/*
 *  保存学生信息
 */
_bool save_student(student_p student, char const *filename) {

	FILE *fp;
	student_p stu;
	score_p score;
	list_p list;
	list_p clist;
	list_p tlist;
//	int len;
	char age[4];
	char gender[2];
	char ascore[3];
//	char *buffer;
	char *split = ",";
	char *end   = "#";
	_bool result;

	if ( student == NULL ) {

		fp = fopen(filename, FILE_WRITE);
		if ( fp == NULL ) {

			perror(filename);
		} else {

			fclose(fp);
		}
		return TRUE;
	}
	list = create_list();
#ifdef NEED_JUDGE
	if ( list == NULL ) {

		perror("malloc");
		return FALSE;
	}
#endif
	clist = list;
	stu = student;
	while ( stu != NULL ) {

		clist->item = (char*)malloc(sizeof(char) * 1024);
#ifdef NEED_JUDGE
		if ( clist->item == NULL ) {

			perror("malloc");
			free_list(list);
			return FALSE;
		}
#endif
		memset(clist->item, 0, 1024);
		strcat(clist->item, stu->base->id);
		strcat(clist->item, split);
		strcat(clist->item, stu->base->passwd);
		strcat(clist->item, split);
		strcat(clist->item, stu->base->name);
		strcat(clist->item, split);
		strcat(clist->item, itoa(stu->base->age, age, 10));
		strcat(clist->item, split);
		strcat(clist->item, itoa(stu->base->gender, gender, 10));
		strcat(clist->item, split);
		score = stu->score;
		while ( score != NULL ) {

			gcvt(score->score, 2, ascore);
			strcat(clist->item, ascore);
			strcat(clist->item, split);
			score = score->next;
		}
		strcat(clist->item, end);
		stu = stu->next;
		tlist = create_list();
#ifdef NEED_JUDGE
		if ( tlist == NULL ) {

			perror("malloc");
			free_list(list);
			return FALSE;
		}
#endif
		clist->next = tlist;
		tlist->prev = clist;
		clist = tlist;
	}
	clist->prev->next = NULL;
	clist->prev = NULL;
	free_list(clist);
	result = write_list(list, filename);
//	printf("sfdsjd;fs\n");
	free_list(list);
//	printf("sdlfsdln\n");
	return result;
}
/*
 *  用户输入用户名、密码。其中用户名为id，因为id唯一。
 */
static void get_user_info(char *id, char *passwd) {

	printf("职工号/学号: ");
	input_string(id, ID_LENGTH);
	printf("密码: ");
	input_passwd(passwd, PASSWD_LENGTH);
}
/*
 *  登录用户不是root，
 *  返回一条属于该用户的班级的链表。
 */
static class_p get_my_class(void) {

	class_p head = NULL;
	class_p _class;
	class_p user_class_c = NULL;
	class_p user_class_t;

	_class = c_head;
	
	while ( _class != NULL ) {

		if ( !strcmp(user->base->id, _class->teacher->id) ) {

			user_class_t = copy_class(_class);
#ifdef NEED_JUDGE
			if ( user_class_t == NULL ) {

				return NULL;
			}
#endif
			if ( head == NULL ) {

				head = user_class_t;
				user_class_c = head;
			} else {

				user_class_c->next = user_class_t;
				user_class_t->prev = user_class_c;
				user_class_c = user_class_t;
			}
		}
		_class = _class->next;
	}
	return head;
}
/*
 *  登录，即检测是否本系统的用户，
 *  若登录成功则返回该教师的信息，
 *  失败则返回 NULL。
 */
static teacher_p login(char const *id, char const *passwd, teacher_p teachers) {

	teacher_p teacher;

	teacher = teachers;
	if ( !strcmp(id, ROOT_NAME) ) {

		if ( !strcmp(passwd, teacher->base->passwd) ) {

			return teacher;
		}
	}
	while ( teacher != NULL ) {

		if ( !strcmp(id, teacher->base->id) && !strcmp(passwd, teacher->base->passwd) ) {

			printf("欢迎回来, %s\n", teacher->base->name);
			return teacher;
		}
		teacher = teacher->next;
	}
	printf("账号或密码错误！！！\n");
	return NULL;
}
/*
 *  写入用户信息
 */
static FILE *write_basemsg_to_file(base_p bmsg, FILE *fp) {

	char name[PEOPLE_NAME_LENGTH];
	char id[ID_LENGTH];
	char passwd[PASSWD_LENGTH];
	char age[4];
	char gender[2];

	if ( bmsg == NULL ) {

		return fp;
	}

//	printf("\n%s\n%s\n%s\n%d\n%d\n",bmsg->name,bmsg->passwd, bmsg->id,bmsg->age, bmsg->gender);

	memset(age, 0, 4);
	memset(gender, 0, 2);
	memset(name, 0, PEOPLE_NAME_LENGTH);
	memset(id, 0, ID_LENGTH);
	memset(passwd, 0, PASSWD_LENGTH);
	itoa(bmsg->age, age, 10);
	itoa(bmsg->gender, gender, 10);
	strcpy(name, bmsg->name);
	strcpy(id, bmsg->id);
	strcpy(passwd, bmsg->passwd);
	fp = write_file(id, fp);
	fp = write_file(passwd, fp);
	fp = write_file(name, fp);
	fp = write_file(age, fp);
	fp = write_file(gender, fp);
	fputc(encode_char(END_SYMBOL), fp);
	return fp;
}
static FILE *write_file(char *str, FILE *fp) {

	str = encode_str(str);
	if ( str != NULL ) {

		while ( *str != '\0' ) {

			fputc(*str++, fp);
		}
	}
	fputc(encode_char(SPLIT_SYMBOL), fp);
	return fp;
}
/*
 *  读取用户信息
 */
static FILE *read_basemsg_from_file(base_p bmsg, FILE *fp) {

//	char ch;
	char age[4];
	char gender[2];
	char *p;
	static FILE *read_file(char *str, uint32 len, FILE *fp);

	if ( bmsg == NULL || fp == NULL ) {

		return fp;
	}
	memset(age, 0, 4);
	memset(gender, 0, 2);
	fp = read_file(bmsg->id, ID_LENGTH, fp);
//	printf("%s\n", bmsg->id);
	fp = read_file(bmsg->passwd, PASSWD_LENGTH, fp);
//	printf("%s\n", bmsg->passwd);
	fp = read_file(bmsg->name, PEOPLE_NAME_LENGTH, fp);
//	printf("%s\n", bmsg->name);
	fp = read_file(age, 4, fp);
	fp = read_file(gender, 2, fp);
	//printf("decode: \"%s\"-\"%s\"\n", age, gender);
	p = age;
	bmsg->age = 0;
	while ( *p != '\0' ) {

		bmsg->age = ( bmsg->age * 10 ) + ( *p++ - '0' );
	}
	bmsg->gender = gender[0] - '0';
	return fp;
}
static FILE *read_file(char *str, uint32 len, FILE *fp) {

	char ch;
	len += 1;
	while ( len-- ) {

		ch = fgetc(fp);
		if ( ch == EOF ) {

			break;
		}
		ch = decode_char(ch);
	//	putchar(ch);
		if ( ch == ',' ) {

			break;
		}
		*str++ = ch;
	}
	*str = '\0';
	if ( len == 0 ) {

		printf("The config file has been error\n");
		fclose(fp);
		exit(0);
	}
//	printf("\n************\n");
	return fp;
}
/*
 *  GUI
 */
#if ( USE_GUI == TRUE )
/*
 *  超级管理员界面
 */
static int print_root_menu(void) {

	printf(ROOT_MENU_LINE);
	printf("|   1. 创建用户     |\n");
	printf(ROOT_MENU_LINE);
	printf("|   2. 删除用户     |\n");
	printf(ROOT_MENU_LINE);
	printf("|   3. 查询用户     |\n");
	printf(ROOT_MENU_LINE);
	printf("|   4. 修改用户     |\n");
	printf(ROOT_MENU_LINE);
	printf("|   5. 修改我的     |\n");
	printf(ROOT_MENU_LINE);
	printf("|   6. 学生管理     |\n");
	printf(ROOT_MENU_LINE);
	printf("|   7. 创建班级     |\n");
	printf(ROOT_MENU_LINE);
	printf("|   8. 删除班级     |\n");
	printf(ROOT_MENU_LINE);
	printf("|   9. 查询班级     |\n");
	printf(ROOT_MENU_LINE);
	printf("|   10.修改班级     |\n");
	printf(ROOT_MENU_LINE);
	printf("|   0. 退    出     |\n");
	printf(ROOT_MENU_LINE);
	printf("|   请选择: ");
	return (int)input_number(0, 10);
}
/*
 *  普通用户界面
 */
static int print_user_menu(void) {

	printf(ROOT_MENU_LINE);
	printf("|   1. 创建班级     |\n");
	printf(ROOT_MENU_LINE); 
	printf("|   2. 删除班级     |\n");
	printf(ROOT_MENU_LINE);
	printf("|   3. 查询班级     |\n");
	printf(ROOT_MENU_LINE);
	printf("|   4. 修改班级     |\n");
	printf(ROOT_MENU_LINE);
	printf("|   5. 修改我的信息 |\n");
	printf(ROOT_MENU_LINE);
	printf("|   0. 退    出     |\n");
	printf(ROOT_MENU_LINE);
	printf("|   请选择: ");
	return (int)input_number(0, 5);
}

static int print_student_menu(void) {

	printf(ROOT_MENU_LINE);
	printf("|  1. 录入学生信息  |\n");
	printf(ROOT_MENU_LINE);
	printf("|  2. 删除学生信息  |\n");
	printf(ROOT_MENU_LINE);
	printf("|  3. 查询学生信息  |\n");
	printf(ROOT_MENU_LINE);
	printf("|  4. 修改学生信息  |\n");
	printf(ROOT_MENU_LINE);
	printf("|  0. 返回上一层    |\n");
	printf(ROOT_MENU_LINE);
	printf("|  请选择: ");
	return (int)input_number(0, 4);
}

static int print_search_student_menu(void) {

	printf(ROOT_MENU_LINE);
	printf("| 1. 查看所有学生   |\n");
	printf(ROOT_MENU_LINE);
	printf("| 2. 查看本班级学生 |\n");
	printf(ROOT_MENU_LINE);
	printf("| 3. 按绩点排名查看 |\n");
	printf(ROOT_MENU_LINE);
	printf("| 4. 按学号排名查看 |\n");
	printf(ROOT_MENU_LINE);
	printf("| 5. 按学号查询     |\n");
	printf(ROOT_MENU_LINE);
	printf("| 0. 返回上一层     |\n");
	printf(ROOT_MENU_LINE);
	printf("| 请选择: ");
	return (int)input_number(1, 5);
}

#endif

#if ( USE_CMD == TRUE )
/*
 *  命令行
 */
#endif
/*
 *  超级管理员
 */
static void root_opt(void) {

	int selected = -1;
	/*
	 *  函数指针数组，每一个元素指向的是一个函数的地址。
	 */
	void (*function[10])(void) = { 
								add_teacher, 
								del_teacher,
								sea_teacher,
								upd_teacher,
								upd_myself,
								opt_student,
								add_class,
								del_class,
								sea_class,
								upd_class,
							};
	while ( ( selected = print_root_menu() ) ) {

		printf(ROOT_MENU_LINE);
		if ( selected < 0 ) {

			continue;
		}
		function[selected-1]();
	}
	printf(ROOT_MENU_LINE);
}
/*
 *  普通用户(即教师)
 */
static void user_opt(void) {

	int selected = -1;

	while ( selected ) {

		selected = print_user_menu();
		printf(ROOT_MENU_LINE);
	}
}
/*
 *  添加教师信息。
 */
static void add_teacher(void) {

	char ch;
	teacher_p temp;
	teacher_p c;

//	printf("%s\n", user->base->name);
	if ( strcmp(user->base->name, ROOT_NAME) ) {

		printf("无此权限\n");
		return;
	}
	temp = create_teacher();
	if ( temp == NULL || temp->base == NULL ) {

		perror("malloc");
		return;
	}
	if ( !get_basemsg(temp->base, TRUE) ) {
		
		printf("放弃添加\n");
		free_teacher(temp);
		return;
	}
	printf("新用户的信息\n姓名\t密码\tID\t年龄\t性别\n%s\t%s\t%s\t%d\t%d\n确认添加？(y/n) ", temp->base->name,
												   temp->base->passwd,
												   temp->base->id,
												   temp->base->age,
												   temp->base->gender);
	ch = input_char();
	if ( ch != 'Y' && ch != 'y' ) {

		printf("放弃添加\n");
		free_teacher(temp);
		return;
	}
	c = t_head;
	while ( c->next != NULL ) {

		c = c->next;
	}
	/*
	 *  将新增节点追到到链表的尾部
	 */
	c->next = temp;
	temp->prev = c;
	if ( !save_teacher(t_head) ) {
		/*
		 *  写入文件失败，则将新增节点从链表中删除。
		 */
		t_head = delete_teacher_node(t_head, temp);
		printf("添加失败\n");
	} else {

		printf("添加成功\n");
	}
}
static void del_teacher(void) {

	int i = 1;
	int total;
	int index;
	char ch = '#';
	char id[ID_LENGTH];
	teacher_p temp;

	if ( strcmp(user->base->name, ROOT_NAME) ) {

		printf("无此权限\n");
		return;
	}
	printf("删除教师\n当前有以下教师\n");
	temp = t_head;
	total = show_base(temp, NULL, TRUE);
	if ( total < 1 ) {

		printf("暂无教师\n");
		return;
	}
	printf("请输入序号进行删除: ");
	index = (int)input_number(1, total);
	total = 1;
//	temp = t_head;
	while ( total < index ) {

		temp = temp->next;
		total++;
	}
	if ( !strcmp(temp->base->name, ROOT_NAME) ) {

		printf("root用户不能修改\n");
		return;
	}
	printf("是否删除？(y/n) ");
	ch = input_char();
	if ( ch == 'Y' || ch == 'y' ) {
		/*
		 *  删除失败，需要还原从链表中删除的节点。 
		 */
		t_head = delete_teacher_node(t_head, temp);
		if ( save_teacher(t_head) ) {
		
			printf("删除成功\n");
		} else {

			strcpy(id, user->base->id);
			free_teacher(t_head);
			t_head = init_teacher();
			if ( t_head == NULL ) {

				printf("程序出现了一个致命的错误，请检查配置文件是否丢失\n");
				exit(0);
			} else {

				temp = t_head;
				while ( temp != NULL ) {

					if ( !strcmp(id, temp->base->id) ) {

						user = temp;
						break;
					}
					temp = temp->next;
				}
			}
			printf("删除失败\n");
		}
	}
}
static void sea_teacher(void) {

	teacher_p temp;
	teacher_p buffer;
	int selected;
	char id[ID_LENGTH];

	if ( strcmp(user->base->name, ROOT_NAME) ) {

		printf("无此权限\n");
		return;
	}
	while ( ( selected = print_search_user_menu() ) ) {

		temp = t_head;
		switch ( selected ) {

			case 1: 
				show_base(temp, NULL, TRUE);
				break;
			case 2:
				printf("请输入id: ");
				input_string(id, ID_LENGTH);
				while ( temp != NULL ) {

					if ( !strcmp(id, temp->base->id) ) {

						buffer = create_teacher();
						buffer->base = copy_base(temp->base);
						show_base(buffer, NULL, TRUE);
						free_teacher(buffer);
						break;
					}
					temp = temp->next;
				}
				if ( temp == NULL ) {

					printf("查无此人\n");
				}
				break;
			default:
				break;
		}		
	}
}
static void upd_teacher(void) {

	int i = 1;
	int total;
	int index;
	char ch = '#';
//	char id[ID_LENGTH];
	teacher_p temp;
	base_p btemp;
	base_p buffer;

	if ( strcmp(user->base->name, ROOT_NAME) ) {

		printf("无此权限\n");
		return;
	}
	printf("修改教师信息\n当前有以下教师\n");
	temp = t_head;
	total = show_base(temp, NULL, TRUE);
	if ( total < 1 ) {

		printf("暂无教师信息\n");
		return;
	}
	printf("请输入要修改的教师序号: ");
	index = (int)input_number(1, total);
	if ( index <= 0 ) {

		printf("放弃修改\n");
		return;
	}
	total = 1;
	temp = t_head;
	while ( total < index ) {

		temp = temp->next;
		total++;
	}
	btemp = copy_base(temp->base);
#ifdef NEED_JUDGE
	if ( btemp == NULL ) {

		printf("修改失败\n");
		return;
	}
#endif
	if ( upd_basemsg(btemp) ) {
	
		printf("是否修改？(y/n) ");
		ch = input_char();
	}
	if ( !strcmp(temp->base->name, ROOT_NAME) ) {

		printf("root用户不能修改名称\n");
		strcpy(btemp->name, ROOT_NAME);
	}
	if ( ch == 'Y' || ch == 'y' ) {

		buffer = temp->base; /* 保留原始信息，失败时还原 */
		temp->base = btemp;
		if ( save_teacher(t_head) ) {
		
			free_base(buffer);
			printf("修改成功\n");
		} else {

			temp->base = buffer;
			free_base(btemp);
			printf("修改失败\n");
		}
	} else {

		free_base(btemp);
	}
}
static void upd_myself(void) {

	char ch = '#';
//	char name[PEOPLE_NAME_LENGTH];
	char passwd[PASSWD_LENGTH];
//	int age;
//	int gender;
	base_p buffer;
	base_p btemp;
	teacher_p show;

	show = create_teacher();
	if ( show == NULL ) {

		show_base(user, NULL, TRUE);
	} else {

		free_base(show->base);
		show->base = copy_base(user->base);
		show_base(show, NULL, TRUE);
		free_teacher(show);
	}
	printf("修改我的信息\n");
	if ( !strcmp(user->base->name, ROOT_NAME) ) {
		
		printf("root用户只能修改密码\n");
		btemp = copy_base(user->base);
#ifdef NEED_JUDGE
		if ( btemp == NULL ) {

			printf("修改失败\n");
			return;
		}
#endif
		if ( upd_passwd(passwd) ) {

			printf("是否将密码修改为: \"%s\"? (y/n) ", passwd);
			ch = input_char();
			if ( ch == 'Y' || ch == 'y' ) {
				
				strcpy(user->base->passwd, passwd);
			} else {

				free_base(btemp);
				return;
			}
		} else {

			printf("放弃修改\n");
			free_base(btemp);
			return;
		}
	} else {

		btemp = copy_base(user->base);
#ifdef NEED_JUDGE
		if ( btemp == NULL ) {

			printf("修改失败\n");
			return;
		}
#endif
		if ( upd_basemsg(user->base) ) {

			if ( !confirm_opt("确认修改") ) {

				printf("放弃修改\n");
				free_base(btemp);
				return;
			}
		} else {

			printf("放弃修改\n");
			free_base(btemp);
			return;
		}
	}
	if ( save_teacher(t_head) ) {

		free_base(btemp);
		printf("修改成功\n");
	} else {

		buffer = user->base;
		user->base = btemp;
		free_base(buffer);
		printf("修改失败\n");
	}
}
/*
 *  学生管理
 */
static void opt_student(void) {

	class_p _class;
	int total;
	int index;
	
	_class = c_head;
	if ( _class == NULL ) {

		printf("暂无班级\n");
		return;
	}
	printf("学生管理\n");
	if ( !strcmp(user->base->name, ROOT_NAME) ) {

		total = print_class_name(_class);
		if ( total < 1 ) {

			printf("暂无班级\n");
			return;
		}
		printf("请输入要操作的班级序号: ");
		index = (int)input_number(1, total);
	} else {

		total = print_class_name(user->_class);
		_class = user->_class;
		if ( total < 1 ) {

			printf("暂无班级\n");
			return;
		}
		printf("请输入要操作的班级序号: ");
		index = (int)input_number(1, total);
	}
	if ( index < 1 ) {

		return;
	}
	total = 1;
	while ( total < index ) {

		_class = _class->next;
		total++;
	}
	printf("当前操作的班级: %s %s\n", _class->name, _class->teacher->id);
	while ( ( index = print_student_menu() ) ) {

		switch ( index ) {

			case ADD_STUDENT_INFO:
				add_student(_class);
				break;
			case DEL_STUDENT_INFO:
				del_student(_class);
				break;
			case SEA_STUDENT_INFO:
				sea_student(_class);
				break;
			case UPD_STUDENT_INFO:
				upd_student(_class);
				break;
			default:
				break;
		}
	}
}

static void add_class(void) {

	class_p _class = NULL;
//	FILE *fp;
	printf("添加班级\n");
	/*
	 *  检查权限，只有root可以创建班级
	 */
	if ( strcmp(user->base->name, ROOT_NAME) ) {

		printf("无此权限\n");
		return;
	}
	if ( c_head == NULL ) {

		_class = get_class_info(NULL);
		if ( _class == NULL ) {

			return;
		}
		show_class(_class);
		c_head = _class;
	} else {

		_class = get_class_tail(c_head);
		_class->next = get_class_info(c_head);
		if ( _class->next == NULL ) {

			return;
		}
		_class->next->prev = _class;
	}
//	print_class(_class);
	if ( save_class(c_head) ) {

		printf("添加成功\n");
	} else {

		printf("添加失败\n");
		c_head = delete_class_node(c_head, _class);
		free_class(_class);
	}
}
static void del_class(void) {

	class_p _class;
	FILE *fp;
	char ch = '#';
	char *file;
	char class_name[CLASS_NAME_LENGTH];
	int i;
	int selected;

	printf("删除班级\n");
	if ( strcmp(user->base->name, ROOT_NAME) ) {

		printf("无此权限\n");
		return;
	}
	i = show_class(c_head);
	if ( i <= 0 ) {

		printf("暂无班级\n");
	}
	printf("请输入要删除的班级序号 ");
	selected = (int)input_number(1, i);
	if ( selected <= 0 ) {

		printf("放弃删除\n");
		return;
	}
	_class = c_head;
	i = 1;
	while ( i < selected ) {

		i++;
		_class = _class->next;
	}
	strcpy(class_name, _class->name);
	printf("是否删除班级: \"%s\"?(y/n) ", _class->name);
	ch = input_char();
	if ( ch != 'Y' && ch != 'y' ) {

		return;
	}
	file = get_class_file_name(_class->name);
	if ( file != NULL ) {
		/*
		 *  删除存放学生信息的班级文件，
		 *  不删除也不影响
		 */
		remove(file);
		free(file);
	}
	_class = delete_class_node(c_head, _class);
	c_head = _class;
	if ( _class == NULL ) {

		fp = fopen(CLASS_CONFIG_FILE, FILE_WRITE);
		if ( fp == NULL ) {

			perror(CLASS_CONFIG_FILE);
			printf("删除失败\n");
			return;
		}
		fclose(fp);
		printf("删除成功\n");
		remove(class_name);
	} else {

		if ( save_class(c_head) ) {
		
			printf("删除成功\n");
		} else {

			printf("删除失败\n");
		}
	}
}
static void sea_class(void) {

//	class_p _class = NULL;

	printf("查询班级\n");
	show_class(c_head);
	//print_class(c_head);
}
static void upd_class(void) {

	class_p _class;
	class_p backup;
	char ch;
	_bool opt_result = FALSE;
	int i;
	int selected;

	printf("更新班级\n");
	if ( strcmp(user->base->name, ROOT_NAME) ) {

		printf("无此权限\n");
		return;
	}
	i = show_class(c_head);
	if ( i <= 0 ) {

		printf("暂无班级\n");
		return;
	}
	printf("请输入要修改的班级序号: ");
	selected = (int)input_number(1, i);
	if ( selected <= 0 ) {

		printf("放弃修改\n");
		return;
	}
	_class = c_head;
	i = 1;
	while ( i < selected ) {

		_class = _class->next;
		i++;
	}
	/*
	 *  操作前先备份数据，失败时还原。
	 */
	backup = copy_class(_class);
	while ( TRUE ) {

		show_class(_class);
		selected = print_class_menu(_class->name);
		switch ( selected ) {

			case UPDATE_CLASS_NAME:
				opt_result = update_class_name(c_head, _class);
				break;
			case UPDATE_CLASS_TEACHER:
				opt_result = update_class_teacher(_class);
				break;
			case SEARCH_CLASS_SUBJECT:
				print_class_subject(_class->subject);
				break;
			case UPDATE_CLASS_SUBJECT:
				opt_result = update_class_subject(_class->subject);
				break;
			case DELETE_CLASS_SUBJECT:
				opt_result = delete_class_subject(_class);
				break;
			case ADD_CLASS_SUBJECT:
				opt_result = add_class_subject(_class);
				break;
			case RETURN_PREVIOUS:
				free_class(backup);
				return;
			default:
				break;
		}
		if ( opt_result ) {

			printf("确认修改？(y/n) ");
			ch = input_char();
			if ( ch != 'y' && ch != 'Y' ) {

				continue;
			}
			if ( save_class(c_head) ) {

				free_class(backup);
				backup = copy_class(_class); // 备份最新数据
				printf("修改成功\n");
			} else {

				c_head = replace_class_node(c_head, _class, backup);
				save_class(c_head);
				printf("修改失败\n");
			}
			opt_result = FALSE;
		}
	}
}
/*
 *  修改基本信息
 */
static _bool upd_name(char *old_name) {

	char new_name[PEOPLE_NAME_LENGTH];

	if ( old_name == NULL ) {

		return FALSE;
	}
	while ( TRUE ) {

		printf("姓  名: ");
		input_string(new_name, PEOPLE_NAME_LENGTH);
		if ( new_name[0] == '\0' ) {

			return FALSE;
		} else if ( !is_legal_name(new_name) ) {

			printf("名字不合法，请重新输入\n");
		} else {

			strcpy(old_name, new_name);
			break;
		}
	}
	return TRUE;
}

static void add_student(class_p _class) {

	student_p student;
	student_p s_temp;
	student_p s_curr;
	student_p s_head_f;
//	subject_p subject;
//	FILE *fp;
	char *file;
//	_bool result = FALSE;

	if ( strcmp(user->base->name, ROOT_NAME) ) { /* 不是root */

		if ( strcmp(user->base->id, _class->teacher->id) ) { /* 也不是班主任 */

			printf("无此权限\n");
			return;
		}
	}
	s_temp = create_student();
#ifdef NEED_JUDGE
	if ( s_temp == NULL ) {

		perror("malloc");
		return;
	}
#endif
	s_head_f = s_head;
	if ( s_head_f != NULL ) {

		while ( s_head_f->next != NULL ) {

			s_head_f = s_head_f->next;
		}
	}
	student = s_temp;
	s_curr = s_temp;
	while ( TRUE ) {

		if ( !get_basemsg(s_curr->base, FALSE) ) {

			if ( s_curr->prev != NULL ) {

				s_curr->prev->next = NULL;
			}
			free_student(s_curr);
			if ( s_head_f->next != NULL ) {

				break;
			}
			printf("放弃添加\n");
			return;
		}
		if ( !get_subject_score(s_curr->score, _class->subject) ) {

			if ( s_curr->prev != NULL ) {

				s_curr->prev->next = NULL;
			}
			free_student(s_curr);
			if ( s_head_f->next != NULL ) {

				break;
			}
			printf("放弃添加\n");
			return;
		}
		if ( s_head == NULL ) {

			s_head = copy_student(s_curr);
#ifdef NEED_JUDGE
			if ( s_head == NULL ) {

				perror("malloc");
				free_student(student);
				return;
			}
#endif
		} else {

			s_temp = s_head;
			while ( s_temp->next != NULL ) {

				s_temp = s_temp->next;
			}
			s_temp->next = copy_student(s_curr);
#ifdef NEED_JUDGE
			if ( s_temp == NULL ) {

				perror("malloc");
				free_student(student);
				return;
			}
#endif
			s_temp->next->prev = s_temp;
		}
		if ( !confirm_opt("是否继续添加") ) {

			break;
		}
		s_temp = create_student();
#ifdef NEED_JUDGE
		if ( s_temp == NULL ) {

			perror("malloc");
			return;
		}
#endif
		s_curr->next = s_temp;
		s_temp->prev = s_curr;
		s_curr = s_temp;
	}
	file = get_class_file_name(_class->name); /* 获得要操作的班级对应的文件名 */
#ifdef NEED_JUDGE
	if ( file == NULL ) {

		free_student(student);
		return;
	}
#endif
	if ( _class->student == NULL ) {

		_class->student = student;
	} else {

		s_temp = _class->student;
		while ( s_temp->next != NULL ) {

			s_temp = s_temp->next;
		}
		s_temp->next = student;
		/* s_temp->prev->next-> = s_temp; // 这句写错了，应该是s_temp->next->prev = s_temp;或者student->prev = s_temp;*/
		student->prev = s_temp;
	}
	if ( save_student(_class->student, file) ) {

		printf("添加成功\n");
	} else {

		printf("添加失败\n");
		free(file);
		student = s_temp->next;
		s_temp->next = NULL;
		student->prev = NULL;
		free_student(student);
		return;
	}
	free(file);
}
static void del_student(class_p _class) {

	student_p student;
	student_p st_head;
	int total;
	int index;
	char id[ID_LENGTH];
	char *file;

	if ( _class == NULL || _class->student == NULL ) {

		printf("暂无学生信息\n");
		return;
	}
	student = _class->student;
	printf("当前有以下学生信息\n");
	total = show_base(NULL, student, FALSE);
	if ( total < 1 ) {

		printf("暂无学生信息\n");
		return;
	}
	printf("请输入要删除的学生序号: ");
	index = (int)input_number(1, total);
	if ( index <= 0 ) {

		printf("放弃删除\n");
		return;
	}
	if ( !confirm_opt("确认删除") ) {

		printf("放弃删除\n");
		return;
	}
	total = 1;
	student = _class->student;
	while ( total < index ) {

		student = student->next;
		total++;
	}
	strcpy(id, student->base->id);
	_class->student = delete_student_node(_class->student, student);
	st_head = s_head;
	while ( st_head != NULL ) {

		if ( !strcmp(st_head->base->id, id) ) {

			break;
		}
		st_head = st_head->next;
	}
	if ( st_head != NULL ) {
		
		s_head = delete_student_node(s_head, st_head);
	}
	file = get_class_file_name(_class->name);
	if ( file == NULL ) {

		perror("Error");
		return;
	}
	if ( save_student(_class->student, file) ) {

		printf("删除成功\n");
	} else {

		printf("删除失败\n");
	}
	free(file);
}
static void sea_student(class_p _class) {

//	student_p student;
	class_p c_temp;

	int selected;
	c_temp = c_head;
	if ( !strcmp(user->base->id, ROOT_ID) ) {

		while ( c_temp != NULL ) {

			calc_student_gpa(c_temp);
			c_temp = c_temp->next;
		}
	} else {

		calc_student_gpa(_class);
	}
	while ( ( selected = print_search_student_menu() ) ) {

		switch ( selected ) {

			case SEARCH_CLASS:
				show_student(_class);
				break;
			case SEARCH_ALL:
				if ( strcmp(user->base->id, ROOT_ID) ) {

					printf("无此权限\n");
					break;
				}
				c_temp = c_head;
				while ( c_temp != NULL ) {

					show_student(c_temp);
					c_temp = c_temp->next;
				}
				break;
			case SORT_GPA:
#ifdef USE_SORT_CALLBACK
				c_temp = sort_student_template(_class, sort_for_gpa);
#else
				c_temp = sort_gpa(_class);
#endif
				show_student(c_temp);
				free_class(c_temp);
				break;
			case SORT_ID:
#ifdef USE_SORT_CALLBACK
				c_temp = sort_student_template(_class, sort_for_id);
#else
				c_temp = sort_id(_class);
#endif
				show_student(c_temp);
				free_class(c_temp);
				break;
			case SEARCH_ID:
				c_temp = search_student_id(_class);
				show_student(c_temp);
				free_class(c_temp);
				break;
			default:
				break;
		}
	}
//	calc_student_gpa(_class); /* 计算平均绩点 */
//	show_student(_class);
//	system("pause");
}
static void upd_student(class_p _class) {

	student_p student;
	student_p backup;
//	student_p s_head_f;
	int total;
	int index;
	char *file;
	_bool result = FALSE;

	printf("当前有以下学生\n");
	total = show_student(_class);
	if ( total < 1 ) {

		printf("暂无学生信息\n");
		return;
	}
	printf("请选择: ");
	index = (int)input_number(1, total);
	if ( index < 1 ) {

		printf("放弃修改\n");
		return;
	}
	student = _class->student;
	total = 1;
	while ( total < index ) {

		student = student->next;
		total++;
	}
	backup = create_student(); /* 备份数据 */
#ifdef NEED_JUDGE
	if ( backup == NULL ) {

		perror("malloc");
		return;
	}
#endif
	free_base(backup->base);
	free_score(backup->score);
	backup->base = copy_base(student->base);
	backup->score = copy_score(student->score);
#ifdef NEED_JUDGE
	if ( backup->base == NULL || backup->score == NULL ) {

		free_student(backup);
		return;
	}
#endif
	result = upd_basemsg(student->base);
	result |= upd_student_score(_class->subject, student->score);
	if ( result ) {
		
		if ( !confirm_opt("确认修改") ) {

			_class->student = replace_student_node(_class->student, student, backup);
			printf("放弃修改\n");
			return;
		}
	} else {

		printf("放弃修改\n");
		_class->student = replace_student_node(_class->student, student, backup);
		return;
	}
	file = get_class_file_name(_class->name);
#ifdef NEED_JUDGE
	if ( file == NULL ) {

		perror("malloc");
		_class->student = replace_student_node(_class->student, student, backup);
		return;
	}
#endif
	if ( save_student(_class->student, file) ) {

		printf("修改成功\n");
		free_student(backup);
	} else {

		printf("修改失败\n");
		_class->student = replace_student_node(_class->student, student, backup);
	}
	free(file);
}

static _bool upd_id(char *old_id, _bool is_teacher) {

	char new_id[ID_LENGTH+1];
	if ( old_id == NULL ) {

		return FALSE;
	}
	while ( TRUE ) {

		printf("ID: ");
		input_string(new_id, ID_LENGTH);
		if ( new_id[0] == '\0' ) {

			return FALSE;
		}
		if ( is_teacher ) {

			strcat(new_id, TEACHER_ID_FLAG);
		} else {

			strcat(new_id, STUDENT_ID_FLAG);
		}
	//	MYDEBUG(new_id);
		if ( !is_legal_id(new_id, is_teacher) ) {

			printf("已存在相同id，请重新输入\n");
		} else {

			strcpy(old_id, new_id);
			break;
		}
	}
	return TRUE;
}

static _bool upd_passwd(char *old_passwd) {

	char new_passwd[PASSWD_LENGTH];
	char new_temp_passwd[PASSWD_LENGTH];
	if ( old_passwd == NULL ) {

		return FALSE;
	}
	while ( TRUE ) {

		printf("密    码: ");
		input_passwd(new_passwd, PASSWD_LENGTH);
		if ( new_passwd[0] != '\0' ) {
			
			printf("确认密码: ");
			input_passwd(new_temp_passwd, PASSWD_LENGTH);
			if ( strcmp(new_passwd, new_temp_passwd) ) {

				printf("密码不一致，请重新输入\n");
			} else {

				strcpy(old_passwd, new_passwd);
				break;
			}
		} else {

			return FALSE;
		}
	}
	return TRUE;
}
static _bool upd_gender(int *old_gender) {

	int new_gender;
	printf("性别(0-男,1-女): ");
	new_gender = (int)input_number(0, 1);
	if ( new_gender != -1 ) {

		*old_gender = new_gender;
		return TRUE;
	}
	return FALSE;
}
static _bool upd_age(int *old_age) {

	int new_age;
	printf("年龄: ");
	new_age = (int)input_number(0, 100);
	if ( new_age != -1 ) {

		*old_age = new_age;
		return TRUE;
	}
	return FALSE;
}

/*
 *  检查id是否合法，
 *  不合法案例：非数字或已存在
 */
static _bool is_legal_id(char const *id, _bool is_teacher) {

	char const *temp_id = id;
	teacher_p tempt;
	student_p temps;
	
	while ( *temp_id != '\0' ) {

		if ( !IS_NUMBER(*temp_id) ) {

			return FALSE;
		}
		temp_id++;
	}
	if ( is_teacher ) {

		tempt = t_head;
		while ( tempt != NULL ) {

			if ( !strcmp(id, tempt->base->id) ) {
				
				return FALSE;
			}
			tempt = tempt->next;
		}
	} else {

		temps = s_head;
		while ( temps != NULL ) {

			if ( !strcmp(id, temps->base->id) ) {

				return FALSE;
			}
			temps = temps->next;
		}
	}
	return TRUE;
}
/*
 *  获取教师或学生的基本信息
 */
_bool get_basemsg(base_p bmsg, _bool is_teacher) {

	if ( !upd_name(bmsg->name) ) {

		return FALSE;
	}
	if ( !upd_passwd(bmsg->passwd) ) {

		return FALSE;
	}
	if ( !upd_id(bmsg->id, is_teacher) ) {

		return FALSE;
	}
	if ( !upd_gender(&bmsg->gender) ) {

		return FALSE;
	}
	if ( !upd_age(&bmsg->age) ) {

		return FALSE;
	}
	return TRUE;
}
/*
 *  更新基本信息
 */
static _bool upd_basemsg(base_p bmsg) {
	
	_bool update;
	update = FALSE;

	update =  upd_name(bmsg->name); 
	update |= upd_passwd(bmsg->passwd);
	update |= upd_gender(&bmsg->gender);
	update |= upd_age(&bmsg->age);
	return update;
}

#ifdef NEED_DEBUG
/*
 *  获得基本信息的的总长度
 */
static int get_basemsg_string_len(base_p bmsg) {

	int sum_len;
	int name_len;
	int id_len;
	int passwd_len;
	int age_len;
	int gender_len;
	char age[4];
	char gender[2];
	
	name_len = (int)strlen(bmsg->name);
	id_len = (int)strlen(bmsg->id);
	passwd_len = (int)strlen(bmsg->passwd);
	age_len = (int)strlen(itoa(bmsg->age, age, 10));
	gender_len = (int)strlen(itoa(bmsg->gender, gender, 10));
	
	sum_len = name_len + id_len + passwd_len + age_len + gender_len;
	return sum_len;
}
#endif