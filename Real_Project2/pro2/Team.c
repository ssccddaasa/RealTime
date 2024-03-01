#include <stdio.h>
#include <string.h>
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
#include <pthread.h>
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

struct cart
{
char title[50];
int quntity;
int ind;
};



struct cart Cart;
int by = 0;

int ter;

void sig_handler(int sig){
    if(sig == SIGTERM){
        ter = 1;
        printf("END2 wwwwwwwwwwwwwwwwwwwwwww\n");
        exit(EXIT_SUCCESS);
    }
}

int id;

void *TeamManger(void *arg){

  

    FILE *fptr;
    fptr = fopen("arg.txt", "r");
    char user_def[100];
    fgets(user_def, 100, fptr);
    fclose(fptr);

    int user[10];
    sscanf(user_def,"%d,%d,%d,%d,%d,%d,%d,%d,%d",&user[0],&user[1],&user[2],&user[3],&user[4],&user[5],&user[6],&user[7],&user[8]);

    char sem_name[20];  // Adjust the size according to your naming convention
    sprintf(sem_name, "Sem3_%d", id+user[0]);
    sem_t *sem1;
    sem1 = sem_open(sem_name,O_CREAT,0666,0);

    key_t keysh1 = ftok("items",65);
    int shid = shmget(keysh1, user[2]*sizeof(struct item_pro),0666);
    struct item_pro *items = (struct item_pro*) shmat(shid, NULL,0);

    key_t keysh2 = ftok("products",66);
    int shid2 = shmget(keysh2, user[2]*sizeof(struct item_pro2),0666);
    struct item_pro2 *prod = (struct item_pro2*) shmat(shid2, NULL,0);

    int timee = 1;
    while (1)
    {
        for(int i=0;i<user[2];i++){
            if(items[i].quntity < user[3]){
                if(items[i].busy == 0){
                    if(by == 0){
                        items[i].busy = 1;
                        by = 1;
                        if(prod[i].quntity >= user[4]){
                           prod[i].quntity -= user[4];
                           Cart.quntity = user[4];
                           strcpy(Cart.title,items[i].title);
                           Cart.ind = i;
                        }
                        if(prod[i].quntity < user[4]){
                           Cart.quntity = prod[i].quntity;
                           prod[i].quntity = 0;
                           if(Cart.quntity > 0){
                            strcpy(Cart.title,items[i].title);
                            Cart.ind = i;
                           }
                           items[i].busy = -1;
                        }
                    }

                }
            }
        }

        sem_wait(sem1);
        timee++;
    }
    

}


void *TeamEmp(void *arg){
    printf("START %d\n",id);

    char sem_name[20];  // Adjust the size according to your naming convention
    sprintf(sem_name, "Sem3_%d", id);
    sem_t *sem1;
    sem1 = sem_open(sem_name,O_CREAT,0666,0);


    FILE *fptr;
    fptr = fopen("arg.txt", "r");
    char user_def[100];
    fgets(user_def, 100, fptr);
    fclose(fptr);

    int user[10];
    sscanf(user_def,"%d,%d,%d,%d,%d,%d,%d,%d,%d",&user[0],&user[1],&user[2],&user[3],&user[4],&user[5],&user[6],&user[7],&user[8]);

    key_t keysh1 = ftok("items",65);
    int shid = shmget(keysh1, user[2]*sizeof(struct item_pro),0666);
    struct item_pro *items = (struct item_pro*) shmat(shid, NULL,0);
    sem_t *semaphore;
    int start = 0;

    int timee = 1;
    while (1)
    {
        
        printf("TeamId: %d || Cart obj: %s || Cart qun: %d || Cart ind: %d\n",id,Cart.title,Cart.quntity,Cart.ind);
      if(start == 0){
        if(Cart.ind != -1){
            start = 1;
            int val = 100;
            semaphore = sem_open(items[Cart.ind].semname, 0);
             int hg = sem_getvalue(semaphore,&val);
            printf("TeamId: %d semval:%d \n",id,val);
            if(sem_wait(semaphore) == -1){
                printf("sssssasdwqdscxz\n");
            }
        }

      }  

      if(start == 1){
        if(Cart.quntity > user[1]){
            Cart.quntity -= user[1];
            items[Cart.ind].quntity += user[1];
        }
        else if(Cart.quntity <= user[1]){
            items[Cart.ind].quntity += Cart.quntity;
            Cart.quntity = 0;
            start = 2;
        }
      }

      if(start == 2){
        sem_post(semaphore);
        sem_close(semaphore);
        start = 0;
        Cart.ind = -1;
      }

        sem_wait(sem1);
        timee++;
      
    }
    
}



int main(int argc, char *argv[]){
    ter = 0;

    signal(SIGTERM, sig_handler);

    id = atoi(argv[1]);

    Cart.ind = -1;
    pthread_t Man, Emp;
    int ch1,ch2;

    ch1 = pthread_create(&Man, NULL, TeamManger,NULL);
    if (ch1) {
        printf("Error: return code from pthread_create() is %d\n", ch1);
        exit(-1);
    }

    ch2 = pthread_create(&Emp, NULL, TeamEmp,NULL);
    if (ch2) {
        printf("Error: return code from pthread_create() is %d\n", ch2);
        exit(-1);
    }

    

    while (ter == 0)
    {
        sleep(1);
    }
    

    printf("END1 wwwwwwwwwwwwwwwwwwwwwww\n");

}