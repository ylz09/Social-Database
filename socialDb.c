/* program testing connection to mysql 

Compile:
gcc -I /usr/include/mysql socialDb.c -l mysqlclient
To run the C code, use the command:
./a.out

Documentation for the C API can be found at
https://dev.mysql.com/doc/refman/5.5/en/c-api.html

*/

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void show_all_moments(MYSQL *sock)
{
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(qbuffer,"select mom_index, content, nickname, post_time from moment, user where user.user_id=moment.user_id;");
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    printf("Top 10 Moment info: \n");
    int cnt=0;
    while (row = mysql_fetch_row(res)) {
        printf(ANSI_COLOR_YELLOW "   ID:%s, %-40s  published by: %-10s at %-10s \n" ANSI_COLOR_RESET ,row[0],row[1],row[2],row[3]);
	cnt=cnt+1;
	if(cnt>10) break;
    }
    printf("\n");
    mysql_free_result(res);
}

void user_like_what(MYSQL *sock,char* name)
{
// e.g. return a user's like wechat list
    char qbuffer[2000];
    MYSQL_RES *res;
    MYSQL_ROW row;

    //sprintf(qbuffer,"select content,comment_time from comment, user where nickname='%s' and moment.user_id= user.user_id;",name);
    sprintf(qbuffer,"select moment.content, nickname from moment natural join user where moment.mom_index in (select moment.mom_index from likes, moment,user where user.nickname='%s' and user.user_id = likes.user_id and likes.mom_index= moment.mom_index);",name);
    //printf("query:%s\n",qbuffer);
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    printf("[%s] like these moments:\n",name);
    while (row = mysql_fetch_row(res)) {
        printf("-> %-40s published by [%s]\n",row[0],row[1]);
	//int cnt=0;
	//while(row[cnt] != NULL) {printf("      %s\n",row[cnt]);cnt++;}
    }
    printf("\n");
    mysql_free_result(res);
}

void show_user(MYSQL *sock)
{
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(qbuffer,"select user_id,nickname from user");
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    printf("#users: \n");
    while (row = mysql_fetch_row(res)) {
        printf(ANSI_COLOR_YELLOW "id:%s,name:%s\n"ANSI_COLOR_RESET ,row[0],row[1] );
    }
    printf("\n");
    mysql_free_result(res);
}

void rename_user(MYSQL *sock)
{
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    show_user(sock);
    printf("Input the user id you want to rename:\n");
    int uid;
    scanf("%d",&uid);
    printf("Input the new name:\n");
    char name[30];
    scanf(" %[^\n]",name);
    

    sprintf(qbuffer,"update user set nickname='%s' where user_id=%d;",name,uid);
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    printf("#update users info: \n");
    show_user(sock);
    //printf("\n");
    mysql_free_result(res);
}
void delete_user(MYSQL *sock)
{
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    show_user(sock);
    //printf("Input the user id you want to rename:\n");
    //int uid;
    //scanf("%d",&uid);
    printf("Input the user you want to delete in above list:\n");
    char name[30];
    scanf(" %[^\n]",name);
    

    sprintf(qbuffer,"delete from user where nickname='%s'",name);
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    printf("#update users info: \n");
    show_user(sock);
    //printf("\n");
    mysql_free_result(res);
}
void show_like(MYSQL *sock)
{
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(qbuffer,"select like_index, nickname from likes, user where likes.user_id = user.user_id;");
    //printf("query:%s\n",qbuffer);
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    printf("All like info:\n");
    while (row = mysql_fetch_row(res)) {
        printf("id:%s, name:%s\n",row[0],row[1]);
	//int cnt=0;
	//while(row[cnt] != NULL) {printf("      %s\n",row[cnt]);cnt++;}
    }
    printf("\n");
    mysql_free_result(res);
	
}
void delete_like(MYSQL *sock)
{
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    show_like(sock);
    //printf("Input the user id you want to rename:\n");
    //int uid;
    //scanf("%d",&uid);
    printf("Input the like id want to delete in above list:\n");
    int lid;
    scanf("%d",&lid);
    //char name[30];
    //scanf(" %[^\n]",name);
    

    sprintf(qbuffer,"delete from likes where like_index=%d",lid);
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    printf("#update like info: \n");
    show_like(sock);
    //printf("\n");
    mysql_free_result(res);
}

int insert_user(MYSQL *sock,char* name, int uid){
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(qbuffer,"select nickname,user_id from user");
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    while (row = mysql_fetch_row(res)) {
	if(*name == *row[0]){
		//printf("user %s exist!\n",name); 
		return atoi(row[1]);}
    }
    //printf("\n");
    mysql_free_result(res);

    char buffer[300];
    sprintf(buffer,"insert into user values(%d,'%s','male','universe')",uid,name);
    if(mysql_query(sock, buffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    printf("successfully inserted user: %s\n",name);
    return uid;

}
void insert_like(MYSQL *sock){
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(qbuffer,"select max(like_index),max(user.user_id) from likes,user");
    //printf("query:%s\n",qbuffer);
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    row = mysql_fetch_row(res);
    int lid = atoi(row[0])+1;
    int uid = atoi(row[1])+1;
    //printf("mid: %d, uid: %d",mid,uid);
    mysql_free_result(res);

    show_all_moments(sock);
    printf("Input the moment id you like:\n");
    int mid;
    scanf("%d",&mid);

    printf("input your name:\n");
    char name[30];
    scanf(" %[^\n]",name);
    //printf("max uid: %d\n",uid);
    uid=insert_user(sock,name,uid);

    char buffer[300];
    sprintf(buffer,"insert into likes values(%d,%d,%d,curdate())",lid,mid,uid);
    //printf("query:%s\n",qbuffer);
    if(mysql_query(sock, buffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    //printf("%s's like list:\n");
    user_like_what(sock,name);
    //printf("These people like [%s]:\n",mom);
    //while (row = mysql_fetch_row(res)) {
    //    printf("-> %s \n",row[0]);
    //    //int cnt=0;
    //    //while(row[cnt] != NULL) {printf("      %s\n",row[cnt]);cnt++;}
    //}
    //printf("\n");
}
void insert_moment(MYSQL *sock){
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(qbuffer,"select max(moment.mom_index),max(user.user_id) from moment,user");
    //printf("query:%s\n",qbuffer);
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    row = mysql_fetch_row(res);
    int mid = atoi(row[0])+1;
    int uid = atoi(row[1])+1;
    //printf("mid: %d, uid: %d",mid,uid);
    mysql_free_result(res);

    printf("insert a moment(<3000 charators):\n");
    char moment[3000];
    scanf(" %[^\n]",moment);
    printf("input your name:\n");
    char name[30];
    scanf(" %[^\n]",name);
    printf("max uid: %d\n",uid);
    uid=insert_user(sock,name,uid);

    char buffer[300];
    sprintf(buffer,"insert into moment values(%d,%d,'%s',curdate())",mid,uid,moment);
    //printf("query:%s\n",qbuffer);
    if(mysql_query(sock, buffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    printf("inserted success!\n");
    //printf("These people like [%s]:\n",mom);
    //while (row = mysql_fetch_row(res)) {
    //    printf("-> %s \n",row[0]);
    //    //int cnt=0;
    //    //while(row[cnt] != NULL) {printf("      %s\n",row[cnt]);cnt++;}
    //}
    //printf("\n");
}

void test(MYSQL *sock)
{
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(qbuffer,"describe user;");
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    while (row = mysql_fetch_row(res)) {
        printf("user info: %s\n",row[0]);
    }
    mysql_free_result(res);
}

void welcome()
{
    printf(" =================================================\n");
    printf("#                                                 #\n");
    printf("#               WELCOME TO SDB!                   #\n");
    printf("#              (Social DataBase)                  #\n");
    printf("#                                                 #\n");
    printf(" =================================================\n");
    printf("#                 We support:                     #\n");
    printf("#             $1. User mode                       #\n");
    printf("#             $2. Administritor mode              #\n");
    printf("#                                                 #\n");
    printf(" =================================================\n");

}

void show_table(MYSQL *sock)
{
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(qbuffer,"show tables");
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    printf("#                  DB info:                       #\n");
    printf("# Tables:                                         #\n");
    printf("#   ");
    while (row = mysql_fetch_row(res)) {
        printf("%s | ",row[0]);
    }
    printf("            #\n");
    mysql_free_result(res);

    printf("# Usages:                                         #\n");
    printf("#   Press <1> user mode                           #\n");
    printf("#   press <2> administritor mode                  #\n");
    printf("#   Press <0> to leave:-)                         #\n");
    printf("#                                                 #\n");
    printf(" =================================================\n");
}


void who_com_mom(MYSQL *sock,char* mom)
{
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    //sprintf(qbuffer,"select content,comment_time from comment, user where nickname='%s' and moment.user_id= user.user_id;",name);
    sprintf(qbuffer,"select comment.content, nickname,comment_time from comment,moment,user where moment.content='%s' and moment.mom_index=comment.mom_index and comment.user_id = user.user_id;",mom);
    //printf("query:%s\n",qbuffer);
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    printf("These people commented on [%s]:\n",mom);
    while (row = mysql_fetch_row(res)) {
        printf("---> %-40s, comment by [%s], at %s\n",row[0],row[1],row[2]);
	//int cnt=0;
	//while(row[cnt] != NULL) {printf("      %s\n",row[cnt]);cnt++;}
    }
    printf("\n");
    mysql_free_result(res);
}

void who_like_mom(MYSQL *sock,char* mom)
{
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    //sprintf(qbuffer,"select content,comment_time from comment, user where nickname='%s' and moment.user_id= user.user_id;",name);
    sprintf(qbuffer,"select distinct nickname from moment,likes,user where moment.content='%s' and moment.mom_index=likes.mom_index and likes.user_id = user.user_id",mom);
    //printf("query:%s\n",qbuffer);
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    printf("These people like [%s]:\n",mom);
    while (row = mysql_fetch_row(res)) {
        printf("-> %s \n",row[0]);
	//int cnt=0;
	//while(row[cnt] != NULL) {printf("      %s\n",row[cnt]);cnt++;}
    }
    printf("\n");
    mysql_free_result(res);
}
void who_like_me(MYSQL *sock,char* me)
{
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    //sprintf(qbuffer,"select content,comment_time from comment, user where nickname='%s' and moment.user_id= user.user_id;",name);
    sprintf(qbuffer,"select nickname from user where user_id in ( select distinct likes.user_id  from likes,moment,user where nickname='%s' and likes.mom_index = moment.mom_index and moment.user_id = user.user_id);",me);
    //printf("query:%s\n",qbuffer);
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    printf("These people like [%s]:\n",me);
    while (row = mysql_fetch_row(res)) {
        printf("-> %s \n",row[0]);
	//int cnt=0;
	//while(row[cnt] != NULL) {printf("      %s\n",row[cnt]);cnt++;}
    }
    printf("\n");
    mysql_free_result(res);
}

void show_user_comment(MYSQL *sock,char* name)
{
// a user's all comment
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    //sprintf(qbuffer,"select content,comment_time from comment, user where nickname='%s' and moment.user_id= user.user_id;",name);
    sprintf(qbuffer,"select content, comment_time from comment, user where nickname='%s' and comment.user_id= user.user_id;",name);
    //printf("query:%s\n",qbuffer);
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    printf("All comments of [%s]:\n",name);
    while (row = mysql_fetch_row(res)) {
        printf("-> %-40s [%s]\n",row[0],row[1]);
	//int cnt=0;
	//while(row[cnt] != NULL) {printf("      %s\n",row[cnt]);cnt++;}
    }
    printf("\n");
    mysql_free_result(res);
}

void show_user_moment(MYSQL *sock,char* name)
{
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    //sprintf(qbuffer,"select content,post_time from moment, user where nickname='%s' and moment.user_id= user.user_id;",name);
    sprintf(qbuffer,"select content, post_time from moment, user where nickname='%s' and moment.user_id= user.user_id;",name);
    //printf("query:%s\n",qbuffer);
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    printf("All moments published by [%s]:\n",name);
    while (row = mysql_fetch_row(res)) {
        printf(ANSI_COLOR_BLUE"-> %-40s at [%s]\n"ANSI_COLOR_RESET,row[0],row[1]);
	//int cnt=0;
	//while(row[cnt] != NULL) {printf("      %s\n",row[cnt]);cnt++;}
    }
    printf("\n");
    mysql_free_result(res);
}

void show_all_comments(MYSQL *sock)
{
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(qbuffer,"select content,nickname,comment_time from comment,user where comment.user_id = user.user_id;");
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    printf("top comments: \n");
    int cnt=0;
    while (row = mysql_fetch_row(res)) {
        printf(ANSI_COLOR_YELLOW  "comment: %-40s  commented by: %-10s at %-10s \n" ANSI_COLOR_RESET ,row[0],row[1],row[2]);
	cnt=cnt+1;
	if(cnt>10) break;
    }
    printf("\n");
    mysql_free_result(res);
}

int main(void) {

    MYSQL mysql, *sock;
    MYSQL_RES *res;
    MYSQL_ROW row;
	
    welcome();

    //--------------------Initialize the connnection-------------------
    //init(mysql,sock);
    //sock=init();
    char *db = "ab_lizhou";
    char *usr = "lizhou";
    char *pwd = "fietahxu";

    char qbuffer[200];

    mysql_init(&mysql);
    if (!(sock = mysql_real_connect(&mysql,"localhost",usr,pwd,db,0,NULL,0))) {
        fprintf(stderr,"Couldn't connect to engine!\n%s\n\n",mysql_error(&mysql));
        perror("");
        exit(1);
    }
    mysql.reconnect = 1; 

    sprintf(qbuffer,"use %s", db);
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    //--------------------Initialize connnection end-------------------
    show_table(sock);

 while(1){
    
    printf(ANSI_COLOR_RED "Choose user mode(1 user mode, 2 admin mode,0 exit): \n" ANSI_COLOR_RESET);
    int cmd; 
    scanf("%d",&cmd);

    switch(cmd) {
        case 1:
	    printf("Into user mode\n");
	    //char c; 
	    //scanf(" %c", &c);
	    int c =1;
	    while(c)
            {
		printf(ANSI_COLOR_CYAN "press 1 show user info; 2 show top10 moments; 3 show top10 comments\n" ANSI_COLOR_RESET );
		printf(ANSI_COLOR_CYAN "press 4 search user moments; 5 search user comments; 6 search user likes\n" ANSI_COLOR_RESET );
		printf(ANSI_COLOR_CYAN "press 7 search who like this user; 8 search who like this moment;\n" ANSI_COLOR_RESET );
		printf(ANSI_COLOR_CYAN "press 9 search who comments on a moment;\n" ANSI_COLOR_RESET );
		int type; scanf("%d",&type);
		char name[100];
		char moment[100];
		switch(type){
			case 1:
				show_user(sock);break;
			case 2:
				show_all_moments(sock);break;
			case 3:
				show_all_comments(sock); break;
			case 4:
			    	while(1){
				show_user(sock);
				printf("Type the user name to search whose moments, 0 to return:\n");
				scanf(" %s",name);
				if(strcmp(name,"0")==0) break;
				show_user_moment(sock,name);
				}
				break;
			case 5:
			    	while(1){
				show_user(sock);
				printf("Type the user to search whose comments, 0 to return::\n");
				scanf(" %s",name);
				if(strcmp(name,"0")==0) break;
                		show_user_comment(sock,name);
				}
				break;
			case 6:
			    	while(1){
				show_user(sock);
				printf("Type the user to search whose likes, 0 to return::\n");
				scanf(" %s",name);
				if(strcmp(name,"0")==0) break;
                		user_like_what(sock,name);
				}
				break;
			case 7:
			    	while(1){
				show_user(sock);
				printf("Type the user to search whose like hie/hser:, 0 to return:\n");
				scanf(" %s",name);
				if(strcmp(name,"0")==0) break;
                		who_like_me(sock,name);
				}
				break;
			case 8:
			    	while(1){
				show_all_moments(sock);
				printf("Type the moment to see who like it, 0 to return:\n");
				scanf(" %[^\n]",moment);
				if(strcmp(moment,"0")==0) break;
				who_like_mom(sock,moment);
				}
				break;
			case 9:
			    	while(1){
				show_all_moments(sock);
				printf("Type the moment to see who comment it, 0 to return:\n");
				scanf(" %[^\n]",moment);
				//scanf(" %s",moment);
				if(strcmp(moment,"0")==0) break;
				who_com_mom(sock,moment);
				}
				break;
			default: break;
			}
		//test(sock);
	        printf("Enter 1 to continue, 0 to return 00 to exit\n");
            	scanf(" %d", &c);
            }
    	    break;
        case 2:
		printf(ANSI_COLOR_CYAN"Welcom to administritor mode\n"ANSI_COLOR_RESET);
            while(1){
		printf(ANSI_COLOR_CYAN"press 1 to continue; 0 to exit\n"ANSI_COLOR_RESET);
		int ctrl=0;
		scanf("%d",&ctrl);
		if(ctrl==0) break;
		printf(ANSI_COLOR_CYAN"press 1 to insert a moment\n"ANSI_COLOR_RESET);
		printf(ANSI_COLOR_CYAN"press 2 to insert a like\n"ANSI_COLOR_RESET);
		printf(ANSI_COLOR_CYAN"press 3 to insert a user\n"ANSI_COLOR_RESET);
		printf(ANSI_COLOR_CYAN"press 4 to rename a user\n"ANSI_COLOR_RESET);
		printf(ANSI_COLOR_CYAN"press 5 to delete a user\n"ANSI_COLOR_RESET);
		printf(ANSI_COLOR_CYAN"press 6 to delete a like\n"ANSI_COLOR_RESET);
		int type; scanf("%d",&type);
		switch(type){
			case 1:
				printf(ANSI_COLOR_CYAN"insert a moment to DB\n"ANSI_COLOR_RESET);
				insert_moment(sock);
				break;
			case 2:
				printf(ANSI_COLOR_CYAN"insert like info to DB\n"ANSI_COLOR_RESET);
				insert_like(sock);
				break;
			case 3:
				printf(ANSI_COLOR_CYAN"insert user to DB\n"ANSI_COLOR_RESET);
    				printf(ANSI_COLOR_CYAN"input user name:\n"ANSI_COLOR_RESET);
    				char name[30];
    				scanf(" %[^\n]",name);
				insert_user(sock,name, 7);
				break;
			case 4:
				printf(ANSI_COLOR_CYAN"rename user\n"ANSI_COLOR_RESET);
				rename_user(sock);
				break;
			case 5:
				printf(ANSI_COLOR_CYAN"delete a user\n"ANSI_COLOR_RESET);
				delete_user(sock);
				break;
			case 6:
				printf(ANSI_COLOR_CYAN"delete a like\n"ANSI_COLOR_RESET);
				delete_like(sock);
				break;
			default:break;
		}
		
	    }
    	    break;
	default: break;
    }
    if ( cmd == 0)
    {printf("bye bye !\n");break;}
 }

    mysql_close(sock);
    exit(0);
}

