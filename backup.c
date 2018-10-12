/* program testing connection to mysql 

Compile:
gcc -I /usr/include/mysql MySQLCexample.c -l mysqlclient
To run the C code, use the command:
./a.out

Documentation for the C API can be found at
https://dev.mysql.com/doc/refman/5.5/en/c-api.html

*/

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

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
    printf("#             $1. Time-based query                #\n");
    printf("#             $2. Associated query                #\n");
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
    printf("#   Press <1> for time-base queries               #\n");
    printf("#   press <2> for associated queries              #\n");
    printf("#   Press <0> to leave:-)                         #\n");
    printf("#                                                 #\n");
    printf(" =================================================\n");
}
void show_user_moment(MYSQL *sock,char* name)
{
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(qbuffer,"select content,post_time from moment, user where nickname='%s' and moment.user_id= user.user_id;",name);
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    printf("moments of %s:\n",name);
    while (row = mysql_fetch_row(res)) {
        printf("	%s\n",row[0]);
    }
    mysql_free_result(res);
}
void show_user(MYSQL *sock)
{
    char qbuffer[200];
    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(qbuffer,"select nickname from user");
    if(mysql_query(sock, qbuffer)) {
        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
        exit(1);
    }
    if(!(res = mysql_store_result(sock))) {
        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
        exit(1);
    }
    printf("# Top users: \n");
    while (row = mysql_fetch_row(res)) {
        printf("%s | ",row[0]);
    }
    printf("\n");
    mysql_free_result(res);
}
//// Example: show database command
//    sprintf(qbuffer,"show databases");
//    printf(qbuffer);
//    printf("\n");
//    if(mysql_query(sock, qbuffer)) {
//        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
//        exit(1);
//    }
//    if(!(res = mysql_store_result(sock))) {
//        fprintf(stderr,"Couldn't get result from %s\n",mysql_error(sock));
//        exit(1);
//    }
//    while (row = mysql_fetch_row(res)) {
//        printf("Database: %s\n",row[0]);
//    }
//    mysql_free_result(res);

//    sprintf(qbuffer,"create table test (a int, b char(5))");
//    printf(qbuffer);
//    printf("\n");
//    if(mysql_query(sock, qbuffer)) {
//        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
//        printf("Note from TA: Make sure your real programs can elegantly handle cases like this\n");
//	//exit(1);
//    }

//void init(MYSQL mysql, MYSQL *sock)
//MYSQL * init(void)
//{
//    MYSQL mysql, *sock;
//    char *db = "ab_lizhou";
//    char *usr = "lizhou";
//    char *pwd = "fietahxu";
//
//    char qbuffer[200];
//
//    mysql_init(&mysql);
//    if (!(sock = mysql_real_connect(&mysql,"localhost",usr,pwd,db,0,NULL,0))) {
//        fprintf(stderr,"Couldn't connect to engine!\n%s\n\n",mysql_error(&mysql));
//        perror("");
//        exit(1);
//    }
//    mysql.reconnect = 1;
//
//
////    sprintf(qbuffer,"use %s", db);
////    if(mysql_query(sock, qbuffer)) {
////        fprintf(stderr, "Query failed (%s)\n",mysql_error(sock));
////        exit(1);
////    }
//    return sock;
//}

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

    printf("Choose query type: ");
    int cmd; 
    scanf("%d",&cmd);

    switch(cmd) {
    	case 0:
            {printf("bye!\n");}
    	    break;
        case 1:
	    printf("Enter y to continue, others to end\n");
	    char c; scanf(" %c", &c);
	    while(c=='y'||c=='Y')
            {
		show_user(sock);
		printf("Type the user to display whose moments:\n");
		char name[100];
		scanf(" %s",name);
		show_user_moment(sock,name);

		//sprintf(qbuffer,"select nickname from user");
		//sprintf(qbuffer,"select * from user;");
		//test(sock);
	        printf("Enter Y to continue, 0 to end\n");
            	scanf(" %c", &c);
            }
    	    break;
        case 2:
            while(1)
            {printf("Next...bye\n");break;}
    	    break;
	default: break;
    }
 }

    mysql_close(sock);
    exit(0);
}










