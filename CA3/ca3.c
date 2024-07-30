#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"functions.h"

int main(){
    accounts = fopen("accounts.txt" , "a");
    postfile = fopen("posts.txt" , "a");
    fclose(accounts);
    fclose(postfile);
    int loginstatus = 0;
    int true = 1;
    char *input = (char*)malloc(sizeof(char));
    user *dummy = (user*)malloc(sizeof(user));
    dummy->username = "";
    dummy->password = "";
    dummy->next_user = NULL;
    while (true){
        input = get_input();
        command(input , dummy , &loginstatus , dummy , accounts , postfile);
    }
}
