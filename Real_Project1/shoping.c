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

#define SEM1 "/SEM1"
#define SEM2 "/SEM2"

#define CUS_NUM 30
#define ITEMS_NUM 7


struct mesg_buffer { 
    long mesg_type; 
    pid_t rqypid; 
} messagei;

struct mesg_buffer2 { 
    long mesg_type; 
    int numOfarri;
    int cus_arr[CUS_NUM]; 
} message2;

 
struct item{
    char title[15];
    int price;
    int all_qun;
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
    int total_cost;
};


void sig_handler(int sig){
    if(sig == SIGTERM){
        exit(EXIT_SUCCESS);
    }
}


void made_cart(struct item items[], struct purch pru[],int size){
    for(int i=0; i<size; i++){

        int n = rand()%7;
        strcpy(pru[i].obj, items[n].title);
        pru[i].quntity = (rand()%5) + 1;
        pru[i].pruqun = 0;
    }
    pru[size-1].quntity = -1;
}

void make_Cus(struct Customer cus[],int si){

    for (int i=0; i<si; i++){

        cus[i].id = i + 1;
        cus[i].arrivl = (rand()%10) + 2;
        cus[i].leave = (rand()%20) + 3;
        cus[i].total_cost = 0;
        if(cus[i].arrivl >= cus[i].leave){
            cus[i].leave = cus[i].arrivl + 2;
        }

    }

}



int main(){

    signal(SIGTERM, sig_handler);

    key_t key; 
    int msgid;  
    key = ftok("que4", 40);
    msgid = msgget(key, 0666 | IPC_CREAT); 
    msgrcv(msgid, &messagei, sizeof(messagei), 1, 0);
    msgctl(msgid, IPC_RMID, NULL);


    key_t key2; 
    int msgid2;  
    key2 = ftok("que2", 20);
    msgid2 = msgget(key2, 0666 | IPC_CREAT);
    message2.mesg_type = 1;

    sem_t *sem1;
    sem1 = sem_open(SEM1,O_CREAT,0666,0);
    sem_t *sem2;
    sem2 = sem_open(SEM2,O_CREAT,0666,0);

    if (sem1 == SEM_FAILED) {
        perror("sem_open");
        // Handle the error here, if needed
        return 1; // Exiting the program due to the failed semaphore creation/opening
    }

    int fd;
 
    // FIFO file path
    char * myfifo = "fifo2.txt";
 
    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo, 0666);
    char send[30];



    time_t t;
    srand((unsigned) time(&t));

    FILE *itemf;
    itemf = fopen("items.txt","r");
    struct Customer Cus_list[CUS_NUM];
    struct item items[ITEMS_NUM];

    char str1 [30];
    int i = 0;

    while (fgets(str1,30,itemf) != NULL && i<7){
        sscanf(str1,"%s %d %d",&items[i].title, &items[i].price, &items[i].all_qun);
        i++;
    }








    make_Cus(Cus_list,CUS_NUM);

    for (int o=0;o<CUS_NUM;o++){


        int size = (rand()%5) + 2;
        for(int u=0; u<size; u++){
            int n = rand()%7;
            strcpy(Cus_list[o].cart[u].obj, items[n].title);
            Cus_list[o].cart[u].quntity = (rand()%5) + 1;
            Cus_list[o].cart[u].pruqun = 0;
        }
        Cus_list[o].cart[size-1].quntity = -1;
    }





    for (int i=0; i<CUS_NUM;i++){
        printf("\n\n\nid: %d, arr: %d, leave: %d, ",Cus_list[i].id,Cus_list[i].arrivl,Cus_list[i].leave);
        int k = 0;
        while (Cus_list[i].cart[k].quntity != -1){
            printf("%s and quntity: %d, ",Cus_list[i].cart[k].obj,Cus_list[i].cart[k].quntity);
            k++;
        }
    }
    printf("\n\n\nid: %d, arr: %d, leave: %d, ",Cus_list[CUS_NUM-1].id,Cus_list[CUS_NUM-1].arrivl,Cus_list[CUS_NUM-1].leave);


    int timee = 0;
    int tobre = 0;

    int arrivl_num = 0;

    while (1){
        for(int i=0; i<CUS_NUM; i++){
            if(Cus_list[i].arrivl == timee){
                message2.cus_arr[arrivl_num] = Cus_list[i].id;
                arrivl_num++;
                for(int j=0;j<ITEMS_NUM;j++){
                    if(items[j].all_qun == 0){
                        continue;
                    }
                    int k = 0;
                    while (Cus_list[i].cart[k].quntity != -1) {
                        if (strcmp(items[j].title, Cus_list[i].cart[k].obj) == 0) {
                        if (items[j].all_qun >= Cus_list[i].cart[k].quntity) {
                            items[j].all_qun -= Cus_list[i].cart[k].quntity;
                            Cus_list[i].cart[k].pruqun = Cus_list[i].cart[k].quntity;
                            Cus_list[i].total_cost += Cus_list[i].cart[k].pruqun * items[j].price;
                        } else {
                            Cus_list[i].cart[k].pruqun = items[j].all_qun;
                            items[j].all_qun = 0;
                        }
                    }
                        k++;
                    }
                }
            }

            if(Cus_list[i].leave == timee){
                tobre++;
                int k = 0;
                int total_qun = 0;
                while (Cus_list[i].cart[k].quntity != -1){
                    total_qun += Cus_list[i].cart[k].quntity;
                    k++; 
                    }

                sprintf(send,"%d,%d,%d,%d",Cus_list[i].id,Cus_list[i].leave,Cus_list[i].total_cost,total_qun);
                fd = open(myfifo, O_WRONLY);
                if(fd == -1){
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                write(fd, send, strlen(send)+1);
                close(fd);
                sem_post(sem1);
                sem_wait(sem2);
 
            }
        }
        if(arrivl_num > 0){
            kill(messagei.rqypid,SIGUSR1);
            message2.numOfarri = arrivl_num;
            msgsnd(msgid2,&message2,sizeof(message2),0);
            arrivl_num = 0;
        }
        sleep(1.5);
        sem_post(sem1);
        sem_wait(sem2);
      timee++;
      
    }







}
