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
#ifndef _STUDENT_H_
#define _STUDENT_H_

#include "../include/mtype.h"
#include <stdio.h>
/*
 *  调试信息
 */
#define MYDEBUG(str) printf("Debug: %s\n", str)
/*
 *  比较简单的比较大小宏定义
 */
#define MAX(a, b) ( ( a ) > ( b ) ? ( a ) : ( b ) )

#define NEED_ENCODE        /* 文件加密                                     */
#define NEED_JUDGE         /* 申请内存时判断是否成功                       */
#define NEED_DEBUG         /* 在程序开始前打印所有用户信息，调试时方便登录 */
#define USE_SORT_CALLBACK  /* 排序使用回调函数方式                         */
#define USE_GUI TRUE       /* 使用界面                                     */
#define USE_CMD FALSE      /* 使用命令行                                   */

#undef NEED_DEBUG
/*
 *  系统级用户默认参数，ROOT_PASSWD基本上已经不用，系统在初始化时已经
 *  重置了密码
 */
#define ROOT_NAME      "root"
#define ROOT_PASSWD    "123456"
#define ROOT_ID        "0000000000"
#define ROOT_AGE       0
#define ROOT_GENDER    0
/*
 *  写入文件的分隔符，
 *  按“一条”信息写入文件，“，”是信息之间的分隔符，
 *  “#”是“一条”信息之间的分隔符。
 */
#define SPLIT_SYMBOL   ','
#define END_SYMBOL     '#'
/*
 *  区分“教师”和“学生”的id符号，在输入id时将
 *  该标志连接到id的后面。
 */
#define TEACHER_ID_FLAG "1"
#define STUDENT_ID_FLAG "2"
/*
 *  写入文件时信息的顺序。
 */
#define ID_INDEX     0
#define PASSWD_INDEX 1
#define NAME_INDEX   2
#define AGE_INDEX    3
#define GENDER_INDEX 4
/*
 *  性别，0-男，1-女
 */
#define MALE         0
#define FEMALE       1
/*
 *  修改信息时不同信息的符号。
 */
#define UPDATE_CLASS_NAME    1
#define UPDATE_CLASS_TEACHER 2
#define UPDATE_CLASS_SUBJECT 3
#define DELETE_CLASS_SUBJECT 4
#define ADD_CLASS_SUBJECT    5
#define RETURN_PREVIOUS      0
#define SEARCH_CLASS_SUBJECT 6 
#define ADD_STUDENT_INFO 1
#define DEL_STUDENT_INFO 2
#define SEA_STUDENT_INFO 3
#define UPD_STUDENT_INFO 4
#define SEARCH_ALL   1
#define SEARCH_CLASS 2
#define SORT_GPA     3
#define SORT_ID      4
#define SEARCH_ID    5
#define ROOT_MENU_LINE "+-------------------+\n"
/*
 *  配置文件和数据文件的路径参数。
 */
#define CONFIG_FILE_PATH    "studata/"
#define FILE_LAST_NAME      ".stu"
#define FILE_LAST_TEMP_NAME ".tmp"
#define ROOT_CONFIG_FILE "studata/user.stu"
#define USER_CONFIG_FILE ROOT_CONFIG_FILE
/*
 *  临时文件，新版本已经丢弃。
 */
#define TEMP_USER_CONFIG_FILE "studata/user.stu.tmp"
#define CLASS_CONFIG_FILE "studata/class.stu"
/*
 *  打开文件模式。
 */
#define FILE_READ "rb"
#define FILE_WRITE "wb"
#define FILE_ADD "ab+"

#define PEOPLE_NAME_LENGTH  21 /* 人名长度        */
#define CLASS_NAME_LENGTH   31 /* 班级名称长度    */
#define SUBJECT_NAME_LENGTH 21 /* 学科名称长度    */
#define ID_LENGTH           11 /* 学号/职工号长度 */
#define PASSWD_LENGTH       21 /* 密码长度        */
/*
 *  定义各种数据类型。
 */
typedef struct people_base_info *base_p;
typedef struct people_base_info  base_b;
typedef struct teacher_s *teacher_p;
typedef struct teacher_s  teacher_b;
typedef struct student_s *student_p;
typedef struct student_s  student_b;
typedef struct class_s   *class_p;
typedef struct class_s    class_b;
typedef struct subject_s *subject_p;
typedef struct subject_s  subject_b;
typedef struct score_s   *score_p;
typedef struct score_s    score_b;
typedef struct list_item *list_p;
typedef struct list_item  list_b;
/*
 *  写入文件的结构体。
 */
struct list_item {

	char *item;
	list_p prev; // 这里应该用单链表，但是双项链表操作更为方便
	list_p next;
};

struct people_base_info {

	char name[PEOPLE_NAME_LENGTH+1]; /* 名字 */
	char id[ID_LENGTH+1];            /* id   */
	char passwd[PASSWD_LENGTH+1];    /* 密码 */
	int age;                         /* 年龄 */
	int gender;                      /* 性别 */
};

struct teacher_s {

	base_p base;    /* 教师基本信息 */
	class_p _class; /* 教师管理的班级 */
	teacher_p prev; /* 上一个节点 */
	teacher_p next; /* 下一个节点 */
};

struct student_s {

	base_p base;    /* 基本信息             */
	float gpa;      /* 绩点                 */
	score_p score;  /* 各科目对应的成绩链表 */
	student_p prev;
	student_p next;
};

struct class_s {

	char name[CLASS_NAME_LENGTH]; /* 班级名称             */
	base_p teacher;               /* 班主任信息           */
	subject_p subject;            /* 班级要学习的相关科目 */
	student_p student;            /* 班级里的学生         */
	class_p prev;
	class_p next;
};

struct subject_s {

	char name[SUBJECT_NAME_LENGTH]; /* 科目名称 */
	float credit;                   /* 科目学分 */
	subject_p prev;
	subject_p next;
};

struct score_s {

	float score;  /* 分数 */
	score_p prev;
	score_p next;
};
/*
 *  创建和释放各种数据的链表节点函数。
 */
extern list_p     create_list     ( void              );
extern base_p     create_base     ( void              );
extern teacher_p  create_teacher  ( void              );
extern student_p  create_student  ( void              );
extern class_p    create_class    ( void              );
extern subject_p  create_subject  ( void              );
extern score_p    create_score    ( void              );
extern void       free_list       ( list_p    list    );
extern void       free_base       ( base_p    base    );
extern void       free_teacher    ( teacher_p teacher );
extern void       free_student    ( student_p student );
extern void       free_class      ( class_p   _class  );
extern void       free_subject    ( subject_p subject );
extern void       free_score      ( score_p   score   );

#ifdef NEED_DEBUG
/*
 *  打印学生信息，新版本添加表格打印方式，该函数已经不用。
 */
extern int        print_student       ( student_p student );
extern void       print_list          ( list_p    list    );
#endif
/*
 *  保存学生信息，即写入文件。
 */
extern _bool save_student(student_p student, char const *filename);
/*
 *  在控制台打印提示确认操作的信息
 */
extern _bool confirm_opt(char const *str);
/*
 *  复制节点函数，函数返回一个数据相同的节点，注意，是两个数据一样但
 *  完全独立的节点。
 */
extern base_p    copy_base                  ( base_p    bmsg    );
extern class_p   copy_class                 ( class_p   _class  );
extern subject_p copy_subject               ( subject_p subject );
extern student_p copy_student               ( student_p source  );
extern score_p   copy_score                 ( score_p   source  );
/*
 *  链表操作函数，删除节点，替换节点，交换节点。
 */
extern teacher_p delete_teacher_node        ( teacher_p head, teacher_p node                               );
extern class_p   delete_class_node          ( class_p   head, class_p   node                               );
extern student_p delete_student_node        ( student_p head, student_p node                               );
extern score_p   delete_score_node          ( score_p   head, score_p   node                               );
extern class_p   replace_class_node         ( class_p   head, class_p   old_class,   class_p   new_class   );
extern subject_p replace_class_subject_node ( subject_p head, subject_p old_subject, subject_p new_subject );
extern student_p replace_student_node       ( student_p head, student_p old_student, student_p new_student );
extern subject_p delete_subject_node        ( subject_p head, subject_p node                               );
extern score_p   delete_student_score       ( score_p   head, score_p   node                               );
extern void      swap_student_node          ( student_p a,    student_p b                                  );
/*
 *  打印菜单函数。
 */
#if ( USE_GUI == TRUE )
extern int       print_class_menu           ( char const *classname                                        );
extern int       print_class_name           ( class_p    _class                                            );
extern int       print_search_user_menu     ( void                                                         );
extern int       print_class_subject        ( subject_p   subject                                          );
#endif

#if ( USE_CMD == TRUE )

#endif
/*
 *  增删查改各种信息函数
 */
extern subject_p add_subject            ( void                                    );
extern _bool     add_class_subject      ( class_p   _class                        );
extern _bool     update_class_name      ( class_p   head,    class_p   _class     );
extern _bool     update_class_teacher   ( class_p   _class                        );
extern _bool     update_class_subject   ( subject_p subject                       );
extern _bool     upd_student_score      ( subject_p subject, score_p   score      );
extern _bool     upd_class_subject_info ( subject_p subject                       );
extern _bool     delete_class_subject   ( class_p   _class                        );
extern student_p get_student_info       ( class_p   _class                        );
extern _bool     get_subject_score      ( score_p   score,   subject_p subject    );
extern _bool     get_basemsg            ( base_p    bmsg,    _bool     is_teacher );
extern class_p   get_class_tail         ( class_p   _class                        );
extern class_p   get_class_info         ( class_p   _class                        );
extern _bool     get_class_name         ( char      *name,   class_p   _class     );
extern _bool     get_class_teacher      ( char      *id                           );
extern _bool     get_class_subject      ( subject_p subject                       );
extern void      calc_student_gpa       ( class_p   chead                         );
/*
 *  list，是写入文件时将信息写入list统一格式，然后在写入文件。
 *  读取时也是按“条”从文件读取数据，然后解析数据。
 */
extern list_p    get_file_list          ( char const *filename, char const split                       );
extern int       get_class_file_len     ( char const *classname                                        );
extern char      *get_class_file_name   ( char const *classname                                        );
extern student_p get_class_student      ( char const *filename                                         );
extern _bool     write_list             ( list_p     list,      char const *filename                   );
extern char      *get_list_base         ( base_p     bmsg,      char       *p,        char const split );
extern class_p   get_class_list         ( void                                                         );
/*
 *  字符串操作的各种函数。
 */
extern _bool     is_legal_name          (char const  *name                                             );
extern char      *get_split_string      (char        *dest,     char       *str,      char split       );
extern void      swap_string            (char        *a,        char       *b                          );
/*
 *  加/解密函数。
 */
extern char      encode_char            (char        ch                                                );
extern char      decode_char            (char        ch                                                );
extern char      *encode_str            (char        *str                                              );
extern char      *decode_str            (char        *str                                              );
/*
 *  查询学生信息的函数，排序，查找。
 */
#ifdef USE_SORT_CALLBACK
extern class_p   sort_student_template  (class_p     _class,    _bool      (*sort_ways)(void *a, void *b));
extern _bool     sort_for_gpa           (void        *a,        void       *b                          );
extern _bool     sort_for_id            (void        *a,        void       *b                          );
#else
extern class_p   sort_gpa               (class_p     _class                                            );
extern class_p   sort_id                (class_p     _class                                            );
#endif
extern class_p   search_student_id      (class_p     _class                                            );

#endif