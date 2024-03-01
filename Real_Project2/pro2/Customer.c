#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <signal.h>
#include <sys/shm.h>

#define SEM1 "/SEM1"
#define SEM2 "/SEM2"

#define CUS_NUM 30
#define ITEMS_NUM 7


 
struct item_pro
{
char title[50];
int quntity;
char semname [20];
int busy;
};

struct purch{
    char obj[15];
    int quntity;
    int pruqun;
};

struct Customer{
    int id;
    int leave;
    int arrivl;
    struct purch cart[6];
};


void sig_handler(int sig){
    if(sig == SIGTERM){
        exit(EXIT_SUCCESS);
    }
}







int main(int argc, char *argv[]){

    int id = atoi(argv[1]);
    int seed = atoi(argv[2]);
    FILE *fptr;
    fptr = fopen("arg.txt", "r");
    char user_def[100];
    fgets(user_def, 100, fptr);
    fclose(fptr);

    int user[10];
    sscanf(user_def,"%d,%d,%d,%d,%d,%d,%d,%d,%d.",&user[0],&user[1],&user[2],&user[3],&user[4],&user[5],&user[6],&user[7],&user[8]);

    signal(SIGTERM, sig_handler);
    struct Customer Cust;

    key_t keysh1 = ftok("items",65);
    int shid = shmget(keysh1, user[2]*sizeof(struct item_pro),0666);
    struct item_pro *items = (struct item_pro*) shmat(shid, NULL,0);

    srand((unsigned) seed);

    char sem_name[20];  
    sprintf(sem_name, "Sem4_%d", id);
    sem_t *sem1;
    sem1 = sem_open(sem_name,O_CREAT,0666,0);

        int val = 100;
        int hg = sem_getvalue(sem1,&val);
        //printf("CusId: %d semval:%d \n",id,val);








        Cust.id = id;
        Cust.arrivl = (rand()%user[6]) + 3;
        Cust.leave = Cust.arrivl + 1;


        int size = (rand()%(user[2]-2)) + 2;
        for(int u=0; u<size; u++){
            int n = rand()%user[2];
            strcpy(Cust.cart[u].obj, items[n].title);
            Cust.cart[u].quntity = (rand()%user[8]) + 1;
            Cust.cart[u].pruqun = 0;
        }
        Cust.cart[size-1].quntity = -1;





    
        
    //printf("\n\n\nid: %d, arr: %d, leave: %d, ",Cust.id,Cust.arrivl,Cust.leave);


    int timee = 1;
    

    int arrivl_num = 0;

    while (1){
            if(Cust.arrivl == timee){
                //printf("arr %d and time: %d\n",Cust.arrivl,timee);
                for(int j=0;j<ITEMS_NUM;j++){
                    if(items[j].quntity == 0){
                        continue;
                    }
                    int k = 0;
                    while (Cust.cart[k].quntity != -1) {
                        if (strcmp(items[j].title, Cust.cart[k].obj) == 0) {
                            sem_t *semaphore = sem_open(items[j].semname, 0);
                            sem_wait(semaphore);
                            printf("Cust id: %d || buy: %s || qun: %d \n",Cust.id,items[j].title,Cust.cart[k].quntity);
                        if (items[j].quntity >= Cust.cart[k].quntity) {
                            items[j].quntity -= Cust.cart[k].quntity;
                            Cust.cart[k].pruqun = Cust.cart[k].quntity;
                        } else {
                            Cust.cart[k].pruqun = items[j].quntity;
                            items[j].quntity = 0;
                        }
                        sem_post(semaphore);
                    }
                        k++;
                    }
                }
                break;
            }
      timee++;
      sem_wait(sem1);
      
    }







}
