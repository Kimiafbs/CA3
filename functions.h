typedef struct Post Post;
typedef struct user user;
typedef struct like_list like_list;
void command(char *input , user *head , int *loginstatus , user *current , FILE *accounts , FILE *postfile);
char *get_pp();
char *get_input();
void signup(user *head , FILE *accounts);
int login(user *head , int loginstatus , FILE *postfile , FILE *accounts);
void get_post(user *current , FILE *postfile , FILE *accounts);
void like_post(user *head , user *current , FILE *postfile);
void delete(user *current);
void info(user *current);
void find(user *head);
void display(user *head);
FILE *accounts;
FILE *postfile;

struct Post{
    int post_id;
    char *text;
    int likes;
    like_list *thelist;
    Post *next_post;
};

struct user{
    int *is_it_my_post;
    char *username;
    char *password;
    user *next_user;
    Post *firstpost;
};

struct like_list{
    char *username;
    like_list *next;
};

void command(char *input , user *head , int *loginstatus , user *current , FILE *accounts , FILE *postfile){
    int q = 0;
    for (int i = 0 ; i < 8 ; i++){
        switch (i){
            case 0:
                if(strcmp(input , "signup") == 0){
                    if(*loginstatus != 0){
                        printf("%s\n" , "you've logged in before.");
                        fflush(stdin);
                        break;
                    }
                    else{
                        signup(head , accounts);
                        q = 1;
                    }
                }
                break;
            case 1:
                if(strcmp(input , "login") == 0){
                    if (*loginstatus != 0){
                        printf("%s\n" , "you've logged in before.");
                        q = 1;
                        break;
                    }
                    else{
                        *loginstatus = login(current , *loginstatus , postfile , accounts);
                        q = 1;
                    }
                }
                break;
            case 2:
                if(strcmp(input , "post") == 0){
                    if(*loginstatus == 0){
                        printf("%s\n" , "Please login first.");
                        q = 1;
                        break;
                    }
                    else{
                        get_post(current , postfile , accounts);
                        q = 1;
                    }
                }
                break;
            case 3:
                if(strcmp(input , "like") == 0){
                    if (*loginstatus == 0){
                        printf("%s\n" , "Please login first.");
                        q = 1;
                        break;
                    }
                    else{
                        q = 1;
                        like_post(head , current , postfile);
                        fflush(stdin);
                    }
                }
                break;
            case 4:
                if(strcmp(input , "logout") == 0){
                    if (*loginstatus == 0){
                        printf("%s\n" , "Please login first.");
                        q = 1;
                        break;
                    }
                    else{
                        *loginstatus = 0;
                        q = 1;
                        system("cls");
                        printf("%s\n" , "You logged out.");
                        fflush(stdin);
                        break;
                    }
                }
                break;
            case 5:
                if(strcmp(input , "delete") == 0){
                    if (*loginstatus == 0){
                        printf("%s\n" , "Please login first.");
                        q = 1;
                        break;
                    }
                    else{
                        delete(current);
                        q = 1;
                    }
                }
                fflush(stdin);
                break;
            case 6:
                if(strcmp(input , "find_user") == 0){
                    if (*loginstatus == 0){
                        printf("%s\n" , "Please login first.");
                        q = 1;
                        break;
                    }
                    else{
                        find(head);
                        q = 1;
                    }
                }
                break;
            case 7:
                if (loginstatus == 0){
                    printf("%s\n" , "Please login first.");
                    q = 1;
                    break;
                }
                if(strcmp(input , "info") == 0){
                    if (*loginstatus == 0){
                        printf("%s\n" , "Please login first.");
                        break;
                        q = 1;
                    }
                    else{
                        info(current);
                        q = 1;
                    }
                }
                break;
        }
    }
    if (q == 0){
        printf("%s\n" , "command not found");
    }
}

char *get_input(){
    char*str = (char*)(malloc(sizeof(char)));
    int n = -1;
    while(str[n] != '\n' && str[n] != ' '){
        n++;
        str = (char*)realloc(str , sizeof(char)*(n+1));
        str[n] = getchar();
    }
    str[n] = '\0';
    return str;
}

char *get_pp(){
    char *str = (char*)malloc(sizeof(char));
    int n = -1;
    while(str[n] != '\n'){
        n++;
        str = (char*)realloc(str , sizeof(char)*(n+1));
        str[n] = getchar();
    }
    str[n] = '\0';
    return str;
}

void signup(user *head , FILE *accounts){
    accounts = fopen("accounts.txt" , "a");
    char *user_name;
    char *password1;
    user_name = get_input();
    password1 = get_pp();
    user *temp = head;
    while(temp->next_user != NULL){
        if(strcmp(temp->username , user_name) == 0){
            printf("%s\n" , "this username is already taken");
            return;
        }
       temp=temp->next_user;
    }
    if(strcmp(temp->username , user_name) == 0){
        printf("%s\n" , "this username is already taken");
        return;
    }
    temp->next_user = (user*)malloc(sizeof(user));
    temp=temp->next_user;
    temp->username=user_name;
    temp->password=password1;
    temp->firstpost = NULL;
    temp->next_user = NULL;
    temp->is_it_my_post = (int*)calloc(1 , sizeof(int));
    printf("%s\n" , "You signed up successfully.");
    fprintf(accounts , "%s " , temp->username);
    fprintf(accounts , "%s " , temp->password);
    fprintf(accounts , "%d" , 0);
    fprintf(accounts, "\n");
    fclose(accounts);
    return;
}

int login(user *head , int loginstatus , FILE *postfile , FILE *accounts){
    accounts = fopen("accounts.txt" , "a");
    postfile = fopen("posts.txt" , "a");
    loginstatus = 0;
    char *user_name = (char*)malloc(sizeof(char));
    char *password1 = (char*)malloc(sizeof(char));
    user_name = get_input();
    password1 = get_pp();
    user *temp;
    temp = head;
    do{
        if(strcmp(user_name , temp->username) != 0){
            if(temp->next_user != NULL){
                temp = temp->next_user;
            }
            if(temp->next_user == NULL && strcmp(user_name , temp->username) != 0){
                break;
            }
        }
        else{
            if(strcmp(password1 , temp->password) == 0){
                printf("%s\n" , "You logged in successfully.");
                loginstatus = 1;
                display(head);
                break;
            }
            else{
                printf("%s\n" , "Wrong password.");
                break;
                return loginstatus;
            }
        }
    }while (temp != NULL);
    if(loginstatus == 0 && temp->next_user == NULL){
        printf("%s\n" , "User not found.");
        return loginstatus;
    }
    loginstatus = 1;
    char *input = (char*)malloc(sizeof(char));
    while (loginstatus){
        if(loginstatus == 1){
            input = get_input();
            command(input , head , &loginstatus , temp , accounts , postfile);
        }
        else{
            return 0;
        }
    }
}

void get_post(user *current , FILE *postfile , FILE *accounts){
    postfile = fopen("posts.txt" , "a");
    accounts = fopen("account.txt" , "a");
    Post *newpost = (Post*)malloc(sizeof(Post));
    newpost->text = get_pp();
    newpost->next_post = NULL;
    Post *temppost;
    if (current->firstpost == NULL){
        current->firstpost = newpost;
    }
    else{
        temppost = current->firstpost;
        while(temppost->next_post != NULL){
            temppost = temppost->next_post;
        }
        temppost->next_post = newpost;
    }
    int m = 0;
    while(current->is_it_my_post[m] != 0){
        m += 1;
    }
    current->is_it_my_post = realloc(current->is_it_my_post , sizeof(int) * (m+2));
    current->is_it_my_post[m] = m+1;
    printf("Post_id : %d\n" , current->is_it_my_post[m]);
    newpost->post_id = m+1;
    newpost->thelist = NULL;
    newpost->likes = 0;
    current->is_it_my_post[m+1] = 0;
    fprintf(accounts , "%s " , current->username);
    fprintf(accounts , "%s " , current->password);
    fprintf(accounts , "%d" , m+1);
    fprintf(accounts, "\n");
    fclose(accounts);
    fprintf(postfile , "%s " , newpost->text);
    fprintf(postfile , "%s " , current->username);
    fprintf(postfile , "%d" , newpost->likes);
    fprintf(postfile, "\n");
    fclose(postfile);
    return;
}

void like_post(user *head , user *current , FILE *postfile){
    postfile = fopen("posts.txt" , "a");
    char *user_name = (char*)malloc(sizeof(char));
    user_name = get_input();
    int id;
    scanf("%d" , &id);
    user *temp;
    temp = head->next_user;
    Post *temppost;
    while(temp != NULL){
        if(strcmp(temp->username , user_name) == 0){
            temppost = temp->firstpost;
            break;
        }
        else{
            temp = temp->next_user;
        }
    }
    if(temp == NULL){
        printf("%s\n" , "User not found.");
        fflush(stdin);
        return;
    }
    while(temppost != NULL){
        like_list *templist;
        if(temppost->post_id == id){
            templist = temppost->thelist;
            if(templist == NULL){
                like_list *newlike = (like_list*)malloc(sizeof(like_list));
                temppost->thelist = newlike;
                newlike->username = current->username;
                newlike->next = NULL;
                temppost->thelist = newlike;
                temppost->likes += 1;
                break;
            }
            else{
                templist = temppost->thelist;
                while(templist->next != NULL){
                    if(strcmp(templist->username , current->username) == 0){
                        printf("%s\n" , "You've alreay liked before.");
                        fflush(stdin);
                        return;
                    }
                    else{
                        templist = templist->next;
                    }
                }
                like_list *newlike = (like_list*)malloc(sizeof(like_list));
                templist->next = newlike;
                newlike->username = current->username;
                temppost->likes += 1;
                newlike->next = NULL;
                fprintf(postfile , "%s " , temppost->text);
                fprintf(postfile , "%s " , current->username);
                fprintf(postfile , "%d" , temppost->likes);
                fprintf(postfile, "\n");
                fclose(postfile);
                return;
            }
        }
        else{
            temppost = temppost->next_post;
        }
    }
    if(temppost == NULL){
        printf("%s\n" , "Post_id not found.");
        fflush(stdin);
        return;
    }
}

void delete(user *current){
    int id;
    scanf("%d" , &id);
    Post *temp;
    temp = current->firstpost;
    if(temp->post_id == id){
        current->firstpost = temp->next_post;
        free(temp);
        return;
    }
    if(temp->next_post != NULL){
        while(temp->next_post->post_id == id){
            temp = temp->next_post;
        }
        if(temp->next_post->next_post != NULL){
                free(temp->next_post);
                temp->next_post = temp->next_post->next_post;
            }
            else{
                free(temp->next_post);
                temp->next_post = NULL;
            }
    }
    else{
        free(temp);
        current->firstpost = NULL;
    }
    return;
}

void info(user *current){
    printf("%s" , "Username : ");
    for (int i  = 0 ; i < strlen(current->username) ; i++){
        printf("%c" , current->username[i]);
    }
    printf("\n");
    printf("%s" , "Password : ");
    for (int i  = 0 ; i < strlen(current->password) ; i++){
        printf("%c" , current->password[i]);
    }
    printf("\n");
    Post *temp;
    temp = current->firstpost;
    while(temp != NULL){
        printf("Post_id : %d" , temp->post_id);
        printf("\n");
        for (int i  = 0 ; i < strlen(temp->text) ; i++){
            printf("%c" , temp->text[i]);
        }
        printf("\n");
        printf("Likes : %d" , temp->likes);
        printf("\n");
        temp = temp->next_post;
    }
    return;
}

void find(user *head){
    char *user_name = (char*)malloc(sizeof(char));
    user_name = get_input();
    user *current;
    current = head;
    do{
        if(strcmp(user_name , current->username) != 0){
            if(current->next_user != NULL){
                current = current->next_user;
            }
            if(current->next_user == NULL && strcmp(user_name , current->username) != 0){
                break;
            }
        }
    }while (current != NULL);
    Post *temp;
    temp = current->firstpost;
    while(temp != NULL){
        printf("Post_id : %d" , temp->post_id);
        printf("\n");
        for (int i  = 0 ; i < strlen(temp->text) ; i++){
            printf("%c" , temp->text[i]);
        }
        printf("\n");
        printf("Likes : %d" , temp->likes);
        printf("\n");
        temp = temp->next_post;
    }
    return;
}

void display(user *head){
    user *temp;
    temp = head->next_user;
    while(temp != NULL){
        Post *temppost;
        temppost = temp->firstpost;
        while(temppost != NULL){
            printf("%s" , "Username : ");
            for (int i  = 0 ; i < strlen(temp->username) ; i++){
                printf("%c" , temp->username[i]);
            }
            printf("\n");
            printf("Post_id : %d" , temppost->post_id);
            printf("\n");
            printf("Likes : %d" , temppost->likes);
            printf("\n");
            for (int i  = 0 ; i < strlen(temppost->text) ; i++){
                printf("%c" , temppost->text[i]);
            }
            printf("\n");
            temppost = temppost->next_post;
        }
        temp = temp->next_user;
    }
}