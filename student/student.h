/*
 *  Filename: student/student.h
 *  Author: AivenLi
 *	Copyright 2019-2020 Aiven.Li All rights reserved.
 *
 *  ������
 *      1. ��������������ݽ�������ѧϰ������֤���ݵ���ȷ�ԡ�ͨ��ʹ�ñ����
 *  ��֮�����ķ����������޹ء���ʹ�ñ����ʱ���������ѽ������ߵ�������������
 *		2. δ��������������������ҵ��;���������Ը���
 */
#ifndef _STUDENT_H_
#define _STUDENT_H_

#include "../include/mtype.h"
#include <stdio.h>
/*
 *  ������Ϣ
 */
#define MYDEBUG(str) printf("Debug: %s\n", str)
/*
 *  �Ƚϼ򵥵ıȽϴ�С�궨��
 */
#define MAX(a, b) ( ( a ) > ( b ) ? ( a ) : ( b ) )

#define NEED_ENCODE        /* �ļ�����                                     */
#define NEED_JUDGE         /* �����ڴ�ʱ�ж��Ƿ�ɹ�                       */
#define NEED_DEBUG         /* �ڳ���ʼǰ��ӡ�����û���Ϣ������ʱ�����¼ */
#define USE_SORT_CALLBACK  /* ����ʹ�ûص�������ʽ                         */
#define USE_GUI TRUE       /* ʹ�ý���                                     */
#define USE_CMD FALSE      /* ʹ��������                                   */

#undef NEED_DEBUG
/*
 *  ϵͳ���û�Ĭ�ϲ�����ROOT_PASSWD�������Ѿ����ã�ϵͳ�ڳ�ʼ��ʱ�Ѿ�
 *  ����������
 */
#define ROOT_NAME      "root"
#define ROOT_PASSWD    "123456"
#define ROOT_ID        "0000000000"
#define ROOT_AGE       0
#define ROOT_GENDER    0
/*
 *  д���ļ��ķָ�����
 *  ����һ������Ϣд���ļ�������������Ϣ֮��ķָ�����
 *  ��#���ǡ�һ������Ϣ֮��ķָ�����
 */
#define SPLIT_SYMBOL   ','
#define END_SYMBOL     '#'
/*
 *  ���֡���ʦ���͡�ѧ������id���ţ�������idʱ��
 *  �ñ�־���ӵ�id�ĺ��档
 */
#define TEACHER_ID_FLAG "1"
#define STUDENT_ID_FLAG "2"
/*
 *  д���ļ�ʱ��Ϣ��˳��
 */
#define ID_INDEX     0
#define PASSWD_INDEX 1
#define NAME_INDEX   2
#define AGE_INDEX    3
#define GENDER_INDEX 4
/*
 *  �Ա�0-�У�1-Ů
 */
#define MALE         0
#define FEMALE       1
/*
 *  �޸���Ϣʱ��ͬ��Ϣ�ķ��š�
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
 *  �����ļ��������ļ���·��������
 */
#define CONFIG_FILE_PATH    "studata/"
#define FILE_LAST_NAME      ".stu"
#define FILE_LAST_TEMP_NAME ".tmp"
#define ROOT_CONFIG_FILE "studata/user.stu"
#define USER_CONFIG_FILE ROOT_CONFIG_FILE
/*
 *  ��ʱ�ļ����°汾�Ѿ�������
 */
#define TEMP_USER_CONFIG_FILE "studata/user.stu.tmp"
#define CLASS_CONFIG_FILE "studata/class.stu"
/*
 *  ���ļ�ģʽ��
 */
#define FILE_READ "rb"
#define FILE_WRITE "wb"
#define FILE_ADD "ab+"

#define PEOPLE_NAME_LENGTH  21 /* ��������        */
#define CLASS_NAME_LENGTH   31 /* �༶���Ƴ���    */
#define SUBJECT_NAME_LENGTH 21 /* ѧ�����Ƴ���    */
#define ID_LENGTH           11 /* ѧ��/ְ���ų��� */
#define PASSWD_LENGTH       21 /* ���볤��        */
/*
 *  ��������������͡�
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
 *  д���ļ��Ľṹ�塣
 */
struct list_item {

	char *item;
	list_p prev; // ����Ӧ���õ���������˫�����������Ϊ����
	list_p next;
};

struct people_base_info {

	char name[PEOPLE_NAME_LENGTH+1]; /* ���� */
	char id[ID_LENGTH+1];            /* id   */
	char passwd[PASSWD_LENGTH+1];    /* ���� */
	int age;                         /* ���� */
	int gender;                      /* �Ա� */
};

struct teacher_s {

	base_p base;    /* ��ʦ������Ϣ */
	class_p _class; /* ��ʦ����İ༶ */
	teacher_p prev; /* ��һ���ڵ� */
	teacher_p next; /* ��һ���ڵ� */
};

struct student_s {

	base_p base;    /* ������Ϣ             */
	float gpa;      /* ����                 */
	score_p score;  /* ����Ŀ��Ӧ�ĳɼ����� */
	student_p prev;
	student_p next;
};

struct class_s {

	char name[CLASS_NAME_LENGTH]; /* �༶����             */
	base_p teacher;               /* ��������Ϣ           */
	subject_p subject;            /* �༶Ҫѧϰ����ؿ�Ŀ */
	student_p student;            /* �༶���ѧ��         */
	class_p prev;
	class_p next;
};

struct subject_s {

	char name[SUBJECT_NAME_LENGTH]; /* ��Ŀ���� */
	float credit;                   /* ��Ŀѧ�� */
	subject_p prev;
	subject_p next;
};

struct score_s {

	float score;  /* ���� */
	score_p prev;
	score_p next;
};
/*
 *  �������ͷŸ������ݵ�����ڵ㺯����
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
 *  ��ӡѧ����Ϣ���°汾��ӱ���ӡ��ʽ���ú����Ѿ����á�
 */
extern int        print_student       ( student_p student );
extern void       print_list          ( list_p    list    );
#endif
/*
 *  ����ѧ����Ϣ����д���ļ���
 */
extern _bool save_student(student_p student, char const *filename);
/*
 *  �ڿ���̨��ӡ��ʾȷ�ϲ�������Ϣ
 */
extern _bool confirm_opt(char const *str);
/*
 *  ���ƽڵ㺯������������һ��������ͬ�Ľڵ㣬ע�⣬����������һ����
 *  ��ȫ�����Ľڵ㡣
 */
extern base_p    copy_base                  ( base_p    bmsg    );
extern class_p   copy_class                 ( class_p   _class  );
extern subject_p copy_subject               ( subject_p subject );
extern student_p copy_student               ( student_p source  );
extern score_p   copy_score                 ( score_p   source  );
/*
 *  �������������ɾ���ڵ㣬�滻�ڵ㣬�����ڵ㡣
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
 *  ��ӡ�˵�������
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
 *  ��ɾ��ĸ�����Ϣ����
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
 *  list����д���ļ�ʱ����Ϣд��listͳһ��ʽ��Ȼ����д���ļ���
 *  ��ȡʱҲ�ǰ����������ļ���ȡ���ݣ�Ȼ��������ݡ�
 */
extern list_p    get_file_list          ( char const *filename, char const split                       );
extern int       get_class_file_len     ( char const *classname                                        );
extern char      *get_class_file_name   ( char const *classname                                        );
extern student_p get_class_student      ( char const *filename                                         );
extern _bool     write_list             ( list_p     list,      char const *filename                   );
extern char      *get_list_base         ( base_p     bmsg,      char       *p,        char const split );
extern class_p   get_class_list         ( void                                                         );
/*
 *  �ַ��������ĸ��ֺ�����
 */
extern _bool     is_legal_name          (char const  *name                                             );
extern char      *get_split_string      (char        *dest,     char       *str,      char split       );
extern void      swap_string            (char        *a,        char       *b                          );
/*
 *  ��/���ܺ�����
 */
extern char      encode_char            (char        ch                                                );
extern char      decode_char            (char        ch                                                );
extern char      *encode_str            (char        *str                                              );
extern char      *decode_str            (char        *str                                              );
/*
 *  ��ѯѧ����Ϣ�ĺ��������򣬲��ҡ�
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