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
#define SEM3 "/SEM3"

#define CASH_NUM 10
#define QUE_SIZE 30
#define CUS_NUM 30



struct mesg_buffer { 
    long mesg_type; 
    pid_t rqypid; 
} message1;

struct mesg_buffer3 { 
    long mesg_type; 
    int numOfleave;
    int pos[CASH_NUM];
    int cus_arr[CUS_NUM];
    int stat; 
} message3;

struct customer{
    int id;
    int leave;
    int cost;
    int total_items;
    int pai;
};


struct casher{
    int id;
    int behavior;
    int speed;
    int timee;
    int stat;
    int income;
    struct customer que[QUE_SIZE];
};


int pos(struct customer cus[]){

    for (int i=0;i<QUE_SIZE;i++){
        if(cus[i].id == -1){
            return i;
        }
    }
    return -1;

}


int qun(struct customer cus[]){
    int total = 0;
    for (int i=0;i<QUE_SIZE;i++){
        if(cus[i].id == -1){
            return total;
        }
        total += cus[i].total_items;
    }
    return total;

}


int qulity(struct casher c){

    int weghit = pos(c.que)*15 + qun(c.que) - c.speed*2 - c.behavior;
    if(c.behavior <= 0 || pos(c.que) == -1){
        weghit = 1000000;
        return weghit;
    }
    return weghit;
}

int best_cash(struct casher cash[]){
    int best = 0;
    for (int i=0;i<CASH_NUM;i++){
        if(qulity(cash[i]) < qulity(cash[best])){
            best = i;
        }

    }

    return best;

}


struct customer readCus(char *myfifo,int fd1,char str2[],struct customer emp){
    char str1[30];
    struct customer cus1;
    fd1 = open(myfifo,O_RDONLY);
     int c = read(fd1, str1, 30);

        if(strcmp(str1,str2) != 0){
        strcpy(str2,str1);
        sscanf(str1, "%d,%d,%d,%d", &cus1.id, &cus1.leave, &cus1.cost, &cus1.total_items);
        cus1.pai = (rand() % 20) + 1;
        close(fd1);
        return cus1;
}
else{
    close(fd1);
    return emp;
}

}





int main() {
    

    key_t key; 
    int msgid;  
    key = ftok("que1", 10);
    msgid = msgget(key, 0666 | IPC_CREAT); 
    msgrcv(msgid, &message1, sizeof(message1), 1, 0);
    msgctl(msgid, IPC_RMID, NULL);


    key_t key2; 
    int msgid2;  
    key2 = ftok("que3", 30);
    msgid2 = msgget(key2, 0666 | IPC_CREAT);
    message3.mesg_type = 1;

    

    sem_t *sem1;
    sem1 = sem_open(SEM1,O_CREAT,0666,0);
    if(sem1 == SEM_FAILED){
        perror("faild");
        return 1;
    }
    
    sem_t *sem2;
    sem2 = sem_open(SEM2,O_CREAT,0666,0);
    if(sem2 == SEM_FAILED){
        perror("faild");
        return 1;
    }
    sem_t *sem3;
    sem3 = sem_open(SEM3,O_CREAT,0666,0);

    if(sem3 == SEM_FAILED){
        perror("faild");
        return 1;
    }


    struct customer emp;
    emp.id = -1;
    time_t t;
    srand((unsigned) time(&t));

    struct customer cus1 = emp;
    struct customer cus2 = emp;
    

    char str1[30];
    int i = 0;
    
 





        
    int fd1;
 
    char * myfifo = "fifo2.txt";
 
    mkfifo(myfifo, 0666);
 
    char str2[30];
    fd1 = open(myfifo,O_RDONLY);
    int c = read(fd1, str2, 30);;   
    close(fd1);

    







   

   
    struct casher Cash[CASH_NUM];

    for (int u = 0; u < CASH_NUM; u++) {
        Cash[u].id = u + 1;
        Cash[u].behavior = (rand() % 50) + 1;
        Cash[u].speed = (rand() % 5) + 1;
        Cash[u].timee = 0;
        Cash[u].stat = 0;
        Cash[u].income = 0;
        for (int y=0;y<QUE_SIZE;y++){
            Cash[u].que[y] = emp;
        }
    }




  





    int num_neg = 0;
    int num_pai = 0;
    int w = 0;
    int leave_num1 = 0;
    int leave_num2 = 0;
    int leave_num3 = 0;
    int leave_num4 = 0;
    while (1) {
        sem_wait(sem1);
        cus1 = readCus(myfifo,fd1,str2,emp);
        
            if (cus1.id != -1) {
                int loc = best_cash(Cash);
                int loc_que = pos(Cash[loc].que);
                Cash[loc].que[loc_que] = cus1;
                Cash[loc].timee = cus1.total_items / Cash[loc].speed +2;
                Cash[loc].stat = 1;
                message3.pos[leave_num1] = loc;
                message3.cus_arr[leave_num1] = cus1.id;
                leave_num1++;
                sem_post(sem2);
                continue;
                
            }

         
         if(leave_num1 > 0){
            message3.numOfleave = leave_num1;
            message3.stat = 1;
            kill(message1.rqypid, SIGUSR2);
            msgsnd(msgid2,&message3,sizeof(message3),0);
            leave_num1 = 0;
            for(int g=0;g<CUS_NUM;g++){
                message3.cus_arr[g] = -1;
            }
            sem_wait(sem3);
        }


        for (i = 0; i < CASH_NUM; i++) {
            Cash[i].behavior--;
            if (Cash[i].behavior == 0) {
                num_neg += 1;
                if (Cash[i].stat == 1) {
                    if (Cash[i].que[1].id != -1) {
                        int j = 1;
                        while (Cash[i].que[j].id != -1) {
                            int loc = best_cash(Cash);
                            int loc_que = pos(Cash[loc].que);
                            Cash[loc].que[loc_que] = Cash[i].que[j];
                            
                            message3.pos[leave_num2] = loc;
                            message3.cus_arr[leave_num2] = Cash[i].que[j].id;
                            leave_num2++;
                            
                            if (Cash[loc].timee == 0) {
                                Cash[loc].timee = Cash[i].que[j].total_items / Cash[loc].speed +2;
                                Cash[loc].stat = 1;
                            }
                            Cash[i].que[j] = emp;
                            j++;
                        }
                    }
                }
            }


        }

         if(leave_num2 > 0){
            message3.numOfleave = leave_num2;
            message3.stat = 2;
            msgsnd(msgid2,&message3,sizeof(message3),0);
            kill(message1.rqypid, SIGUSR2);
            leave_num2 = 0;
            for(int g=0;g<CUS_NUM;g++){
                message3.cus_arr[g] = -1;
            }
            sem_wait(sem3);
        }

        


        for (i=0;i<CASH_NUM;i++){
         if (Cash[i].stat == 1){
             Cash[i].timee --;
             if(Cash[i].timee == 0){
                
                message3.cus_arr[leave_num3] = Cash[i].que[0].id;
                leave_num3++;
                 
                 Cash[i].income += Cash[i].que[0].cost;
                 if(Cash[i].que[1].id == -1){
                     Cash[i].stat = 0;
                     Cash[i].que[0] = emp;
                 } else{
                     int j = 0;
                     while (Cash[i].que[j].id != -1){
                         Cash[i].que[j] = Cash[i].que[j+1];
                         j++;
                     }
                     Cash[i].timee = Cash[i].que[0].total_items / Cash[i].speed + 1;
                 }
             }
         }
        }

        if(leave_num3 > 0){
            message3.numOfleave = leave_num3;
            message3.stat = 3;
            msgsnd(msgid2,&message3,sizeof(message3),0);
            kill(message1.rqypid, SIGUSR2);
            leave_num3 = 0;
            for(int g=0;g<CUS_NUM;g++){
                message3.cus_arr[g] = -1;
            }
            sem_wait(sem3);
        }

         


        for (i=0;i<CASH_NUM;i++){
            int j=0;
            while (Cash[i].que[j].id != -1){
                Cash[i].que[j].pai--;
                if (Cash[i].que[j].pai == 0){
                    num_pai++;
                    if(j == 0){
                        Cash[i].que[j].pai += Cash[i].timee +1;
                        continue;
                    }
                    message3.cus_arr[leave_num4] = Cash[i].que[j].id;
                    leave_num4++;
                    int g =j;
                    while (Cash[i].que[g].id != -1){
                        Cash[i].que[g] = Cash[i].que[g+1];
                        g++;
                    }
                }
                j++;
            }
        }

        


        if(leave_num4 > 0){
            message3.numOfleave = leave_num4;
            message3.stat = 4;
            msgsnd(msgid2,&message3,sizeof(message3),0);
            kill(message1.rqypid, SIGUSR2);
            leave_num4 = 0;
            for(int g=0;g<CUS_NUM;g++){
                message3.cus_arr[g] = -1;
            }
            sem_wait(sem3);
        }
       

        printf("###########################################--- %d\n",w);
        for (i=0;i<CASH_NUM;i++){
            printf("id: %d || time: %d || income: %d || stat: %d || speed: %d || beh: %d\n",Cash[i].id,Cash[i].timee,Cash[i].income,Cash[i].stat,Cash[i].speed,Cash[i].behavior);
            int j = 0;
            while (Cash[i].que[j].id != -1){
                printf("id: %d || leave: %d || cost: %d || total: %d || pai: %d\n",Cash[i].que[j].id,Cash[i].que[j].leave,Cash[i].que[j].cost,Cash[i].que[j].total_items,Cash[i].que[j].pai);
                j++;
            }
            printf("------------------------------\n");
        }
        

        

        sleep(1.5);
        w++;
        sem_post(sem2);

         if(num_neg>CASH_NUM/2){
            printf("\n neg");
            FILE *end;
            end = fopen("end.txt","w");
            fprintf(end,"Negative_End");
            break;
        }
        else if (num_pai>CUS_NUM/2){
            printf("\n pai");
            FILE *end;
            end = fopen("end.txt","w");
            fprintf(end,"Impatient_End");
            break;
        }
        int c=0;
        for (i=0;i<CASH_NUM;i++){
            if(Cash[i].income > 320){
                c = 1;
            }
        }
        if (c == 1){
            printf("\n income");
            FILE *end;
            end = fopen("end.txt","w");
            fprintf(end,"Income_End");
            break;
        }
    }

    



    printf("End");
}