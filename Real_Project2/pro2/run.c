#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h>
#include <signal.h>
#include<sys/wait.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
#include "raylib.h"

#define sem "SEM_"
#define sem2 "SEM2_"

struct item_pro
{
char title[50];
int quntity;
char semname [20];
int busy;
};

struct item_pro2
{
char title[50];
int quntity;
char semname [20];
int busy;
};

struct mesg_buffer { 
    long mesg_type; 
    int t; 
} message;









int main(){ 

    FILE *fptr;
    fptr = fopen("arg.txt", "r");
    char user_def[100];
    fgets(user_def, 100, fptr);
    fclose(fptr);

    int user[10];
    sscanf(user_def,"%d,%d,%d,%d,%d,%d,%d,%d,%d.",&user[0],&user[1],&user[2],&user[3],&user[4],&user[5],&user[6],&user[7],&user[8]);

    key_t keysh1 = ftok("items",65);
    int shid = shmget(keysh1, user[2]*sizeof(struct item_pro),0666|IPC_CREAT);
    struct item_pro *items = (struct item_pro*) shmat(shid, NULL,0);

    key_t keysh2 = ftok("products",66);
    int shid2 = shmget(keysh2, user[2]*sizeof(struct item_pro2),0666|IPC_CREAT);
    struct item_pro2 *prod = (struct item_pro2*) shmat(shid2, NULL,0);
    printf("%d\n",shid);
    printf("%d\n",shid2);

    FILE *profi;
    profi = fopen("items.txt","r");
    char str1[30];
    int i = 0;
    while (fgets(str1,30,profi) != NULL && i<user[2]){
        sscanf(str1,"%s %d",&items[i].title, &items[i].quntity);
        items[i].busy = 0;
        sprintf(items[i].semname, "%s%d",sem,i);
        sem_t *semaphore = sem_open(items[i].semname, O_CREAT, 0666, 0); // Create/open named semaphore
        sem_post(semaphore);
        sem_close(semaphore);
        i++;
    }
    

    FILE *profi2;
    profi2 = fopen("products.txt","r");
    char str2[30];
    i = 0;
    while (fgets(str2,30,profi2) != NULL && i<user[2]){
        sscanf(str2,"%s %d",&prod[i].title, &prod[i].quntity);
        prod[i].busy = 0;
        sprintf(prod[i].semname, "%s%d",sem2,i);
        sem_t *semaphore = sem_open(prod[i].semname, O_CREAT, 0666, 1); // Create/open named semaphore
        sem_close(semaphore);
        i++;
    }

    
    


    sem_t *SemArr[user[0]*2];

     for(int h=0;h<user[0]*2;h++){
        char sem_name[20];  // Adjust the size according to your naming convention
        sprintf(sem_name, "Sem3_%d", h);
        SemArr[h] = sem_open(sem_name, O_CREAT, 0666, 0);
    }

    pid_t Teams [user[0]];

    for(int i=0;i<user[0];i++){
        Teams[i] = fork();
        if(Teams[i] == 0){
            char indexStr[10]; // Buffer to hold the index as a string
            snprintf(indexStr, sizeof(indexStr), "%d", i);
            if (execl("./team", "team", indexStr, NULL) < 0) {
                fprintf(stderr, "Execution failed\n");
                return 1;
            }
        }

    }
    


    sem_t *SemArrCus[user[5]];

     for(int h=0;h<user[5];h++){
        char sem_name[20];  // Adjust the size according to your naming convention
        sprintf(sem_name, "Sem4_%d", h);
        SemArrCus[h] = sem_open(sem_name, O_CREAT, 0666, 0);
    }

    pid_t Cus [user[5]];

    time_t t;
    srand((unsigned) time(&t));
    for(int i=0;i<user[5];i++){
        Cus[i] = fork();
        if(Cus[i] == 0){
            char indexStr[10]; // Buffer to hold the index as a string
            snprintf(indexStr, sizeof(indexStr), "%d", i);
            int seed = rand()%1000 + 1 ;
            seed += i;
            char indexStr2[10]; // Buffer to hold the index as a string
            snprintf(indexStr2, sizeof(indexStr2), "%d", seed); 
            if (execl("./cus", "cus", indexStr,indexStr2, NULL) < 0) {
                fprintf(stderr, "Execution failed\n");
                return 1;
            }
        }

    }
            
    

    int ti = 1;
    sleep(2);

    while (ti < 20)
    {
        int sum = 0;
        for(int j=0;j<user[2];j++){
            sum += prod[j].quntity; 
    }
    sleep(1);
    for(int j=0;j<user[2];j++){
        printf(" ITEMS: title: %s || qun: %d || semname: %s\n",items[j].title,items[j].quntity,items[j].semname);
        printf(" PRODUCTS: title: %s || qun: %d || semname: %s\n",prod[j].title,prod[j].quntity,prod[j].semname);
    }
    
    printf("#################################################################### %d\n",ti);
    
    for(int h=0;h<user[0]*2;h++){
        sem_post(SemArr[h]);
    }
    for(int h=0;h<user[5];h++){
        sem_post(SemArrCus[h]);
    }
    ti++; 
    if (sum <= 0){
        break;
    }
    }


    for(int i=0;i<user[0];i++){
        kill(Teams[i],SIGTERM);
    }

    for(int i=0;i<user[5];i++){
        kill(Cus[i],SIGTERM);
    }


    printf("%d\n",shid);
    if (shmctl(shid, IPC_RMID, NULL) == -1) {
        perror("shmctl1");
        exit(EXIT_FAILURE);
    }
    printf("%d\n",shid2);
    if (shmctl(shid2, IPC_RMID, NULL) == -1) {
        perror("shmctl2");
        exit(EXIT_FAILURE);
    }

    for(int h=0;h<user[0]*2;h++){
        sem_close(SemArr[h]);
    }

    for(int h=0;h<user[5];h++){
        sem_close(SemArrCus[h]);
    }


/*
    pid_t pidCash;
    pid_t pidshop;
    pid_t pidgui;

    pidCash = fork();

    if(pidCash == 0){
        execl("./cash","cash",NULL);
    }
    else if (pidCash == -1)
    {
        perror("fork");
        exit(1);
    }

    pidshop = fork();

    if(pidshop == 0){
        execl("./shop","shop",NULL);
    }
    else if (pidshop == -1)
    {
        perror("fork");
        exit(1);
    }

    pidgui = fork();

    if(pidgui == 0){
        execl("./GUI","GUI",NULL);
    }
    else if (pidgui == -1)
    {
        perror("fork");
        exit(1);
    }


    int stat;
    waitpid(pidCash,&stat,0);

    kill(pidshop,SIGTERM);

    kill(pidgui,SIGTERM);


    
    FILE *fptr;

  
    fptr = fopen("end.txt", "r");

  
    char myString[100];
  
  fgets(user_def, 100, fptr);
  


const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "END_SCREEN");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText(user_def, 300, 200, 32, RED);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

*/
    return 0;
}
