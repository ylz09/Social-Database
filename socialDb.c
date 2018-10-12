/* program testing connection to mysql 

Compile:
gcc -I /usr/include/mysql socialDb.c -l mysqlclient
To run the C code, use the command:
./a.out

Documentation for the C API can be found at
https://dev.mysql.com/doc/refman/5.5/en/c-api.html

*/

#include "socialDB.h"


int main(void) {

    MYSQL mysql, *sock;
    MYSQL_RES *res;
    MYSQL_ROW row;

    welcome();

    //--------------------Initialize the connnection-------------------

    char *db = "social_db";
    char *usr = "ylz";
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

