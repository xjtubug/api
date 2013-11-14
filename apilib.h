#ifndef __BMYBBS_APILIB_H
#define __BMYBBS_APILIB_H

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ght_hash_table.h>
#include <json/json.h>
#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <libxml/xpath.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/xmlsave.h>

#include "bbs.h"
#include "ythtlib.h"
#include "ythtbbs.h"
#include "api_brc.h"

enum article_parse_mode {
	ARTICLE_PARSE_WITH_ANSICOLOR,		///< 将颜色转换为 HTML 样式
	ARTICLE_PARSE_WITHOUT_ANSICOLOR		///< 将 \033 字符转换为 [ESC]
};

struct attach_link {
	char link[256];
	unsigned int size;
	struct attach_link *next;
};

typedef char* api_template_t;
api_template_t api_template_create(const char * filename);
void api_template_set(api_template_t *tpl, const char *key, char *fmt, ...);
void api_template_free(api_template_t tpl);

struct UTMPFILE   *shm_utmp;
struct BCACHE     *shm_bcache;
struct UCACHE     *shm_ucache;
struct UCACHEHASH *shm_uidhash;
struct UINDEX     *shm_uindex;
int shm_init();

extern char *ummap_ptr;
extern int ummap_size;
int ummap();


int finduseridhash(struct useridhashitem *ptr, int size, const char *userid);
int getusernum(const char *id);
struct userec * getuser(const char *id);
char * getuserlevelname(unsigned userlevel);
int save_user_data(struct userec *x);

/**
 * @brief 从 sessid 中获取 utmp 的索引值
 * @param sessid 前三位为索引的sessionid
 * @return utmp 的索引值，注意是从0开始。
 */
int get_user_utmp_index(const char *sessid);

/**
 * @brief 计算uid为uid的用户当前在登录的个数
 * @param uid uid，从1开始索引
 * @return
 */
int count_uindex(int uid);

/**
 * @brief 检查用户 session 是否有效
 * @param x
 * @param sessid
 * @param appkey
 * @return api_error_code
 */
int check_user_session(struct userec *x, const char *sessid, const char *appkey);

int setbmhat(struct boardmanager *bm, int *online);
int setbmstatus(struct userec *ue, int online);

/**
 * @brief 字符串替换函数
 * @param ori 原始字符串
 * @param old 需要替换的字符串
 * @param new 替换的新字符串
 * @return 替换完成后的字符串
 * @warning ori 字符串应当存在在堆上，同样返回的字符串也位于堆上，使用完成记得 free。
 */
char *string_replace(char *ori, const char *old, const char *new);

/**
 * @brief 读取BMY文章内容，并转换为便于处理或者显示。
 * @param bname 版面名称
 * @param fname 帖子名称
 * @param mode 参见 enum article_parse_mode
 * @param attach_link_list 存放BMY附件链接的链表
 * @return 处理后的字符串，该字符串已转换为 UTF-8 编码。
 * @warning 在使用完成后记得 free。
 */
char *parse_article(const char *bname, const char *fname, int mode, struct attach_link **attach_link_list);

/**
 * @brief 将文章中的附件链接单独存放在 attach_link 链表中。
 * @param attach_link_list
 * @param link 附件链接
 * @param size 附件大小
 */
void add_attach_link(struct attach_link **attach_link_list, const char *link, const unsigned int size);

/**
 * @ 释放附件链表
 * @param attach_link_list
 */
void free_attach_link_list(struct attach_link *attach_link_list);

#endif
