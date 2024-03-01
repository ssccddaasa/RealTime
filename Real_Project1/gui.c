#include "raylib.h"
#include "math.h"
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


#define CASH_NUM 10
#define CUS_NUM 30

#define SEM3 "/SEM3"






int arrca [CASH_NUM];

int arrcus [CUS_NUM];


int arrca2 [CASH_NUM];
int arrcus1 [CUS_NUM];
int arrcus2 [CUS_NUM];
int arrcus3 [CUS_NUM];
int arrcus4 [CUS_NUM];

sem_t *sem3;


struct mesg_bufferi { 
    long mesg_type; 
    pid_t rqypid; 
} messagei;

struct mesg_buffer { 
    long mesg_type; 
    pid_t rqypid; 
} message1;

struct mesg_buffer2 { 
    long mesg_type; 
    int numOfarri; 
    int cus_arr[CUS_NUM];
} message2;

struct mesg_buffer3 { 
    long mesg_type; 
    int numOfleave;
    int pos[CASH_NUM];
    int cus_arr[CUS_NUM];
    int stat; 
} message3;

struct Ball{
    Vector2 ball;
    int id;
};


void sigusr1_handler(int signum){
    key_t key; 
    int msgid;  
    key = ftok("que2", 20);
    msgid = msgget(key, 0666 | IPC_CREAT); 
    msgrcv(msgid, &message2, sizeof(message2), 1, 0);
    for(int i=0;i<CUS_NUM;i++){
        arrcus[i] = -1;
    }

    int numofcus = message2.numOfarri;
    
    for(int i=0;i<numofcus;i++){
        arrcus[i] = message2.cus_arr[i];
    }
}


void sigusr2_handler(int signum){
    key_t key; 
    int msgid;  
    key = ftok("que3", 30);
    msgid = msgget(key, 0666 | IPC_CREAT); 
    msgrcv(msgid, &message3, sizeof(message3), 1, 0);

    if(message3.stat == 1){
        for(int i=0;i<CUS_NUM;i++){
            arrcus1[i] = -1;
        }

        int numofcus = message3.numOfleave;
    
        for(int i=0;i<numofcus;i++){
            arrcus1[i] = message3.cus_arr[i];
        }

        for (int i=0;i<numofcus;i++){
            arrca[i] = message3.pos[i];
    }
    
    }


     else if(message3.stat == 2){
        for(int i=0;i<CUS_NUM;i++){
            arrcus2[i] = -1;
        }

        int numofcus = message3.numOfleave;
    
        for(int i=0;i<numofcus;i++){
            arrcus2[i] = message3.cus_arr[i];
        }

        for (int i=0;i<numofcus;i++){
            arrca2[i] = message3.pos[i];
    }
 
    }


    else if(message3.stat == 3){
        for(int i=0;i<CUS_NUM;i++){
            arrcus3[i] = -1;
        }

        int numofcus = message3.numOfleave;
    
        for(int i=0;i<numofcus;i++){
            arrcus3[i] = message3.cus_arr[i];
        }

        

    }


    else if(message3.stat == 4){
        for(int i=0;i<CUS_NUM;i++){
            arrcus4[i] = -1;
        }

        int numofcus = message3.numOfleave;
    
        for(int i=0;i<numofcus;i++){
            arrcus4[i] = message3.cus_arr[i];
        }
        


    }
sem_post(sem3);

}


void sig_handler(int sig){
    if(sig == SIGTERM){
        exit(EXIT_SUCCESS);
    }
}









void moveBall(Vector2 *currentPosition, Vector2 targetPosition, const float ballSpeed) {
    // Calculate direction towards the target
    Vector2 direction = { 0 };
    direction.x = targetPosition.x - currentPosition->x;
    direction.y = targetPosition.y - currentPosition->y;

    // Normalize direction
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    direction.x /= length;
    direction.y /= length;

    // Update ball position
    currentPosition->x += direction.x * ballSpeed;
    currentPosition->y += direction.y * ballSpeed;

    // If the ball overshoots the target, set it to the target directly
    if ((direction.x > 0 && currentPosition->x > targetPosition.x) || (direction.x < 0 && currentPosition->x < targetPosition.x)) {
        currentPosition->x = targetPosition.x;
       
        
    }
    if ((direction.y > 0 && currentPosition->y > targetPosition.y) || (direction.y < 0 && currentPosition->y < targetPosition.y)) {
        currentPosition->y = targetPosition.y;
        
      
    }
}



void moveBall2(Vector2 *currentPosition, Vector2 targetPosition, const float ballSpeed) {
    // Calculate direction towards the target
    Vector2 direction = { 0 };
    direction.x = targetPosition.x - currentPosition->x;
    direction.y = targetPosition.y - currentPosition->y;

    // Normalize direction
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    direction.x /= length;
    direction.y /= length;

    // Update ball position
    currentPosition->x += direction.x * ballSpeed;
    currentPosition->y += direction.y * ballSpeed;

    // If the ball overshoots the target, set it to the target directly
    if ((currentPosition->x == targetPosition.x) || (currentPosition->y == targetPosition.y)) {
        currentPosition->x = targetPosition.x;
        currentPosition->y = targetPosition.y;
        
    }
    
}












int main(void) {
    sem3 = sem_open(SEM3,O_CREAT,0666,0);

    for(int i=0;i<CUS_NUM;i++){
            arrcus[i] = -1;
            arrcus1[i] = -1;
            arrcus2[i] = -1;
            arrcus3[i] = -1;
            arrcus4[i] = -1;
        }


    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);
    signal(SIGTERM, sig_handler);

    key_t key; 
    int msgid;  
    key = ftok("que1", 10);
    if (key == -1) {
        perror("ftok");
        return 1;
    }
    msgid = msgget(key, 0666 | IPC_CREAT); 
    message1.mesg_type = 1;
    message1.rqypid = getpid();
    msgsnd(msgid,&message1,sizeof(message1),0);

    key_t key2; 
    int msgid2;  
    key2 = ftok("que4", 40);
    if (key2 == -1) {
        perror("ftok");
        return 1;
    }
    msgid2 = msgget(key2, 0666 | IPC_CREAT); 
    messagei.mesg_type = 1;
    messagei.rqypid = getpid();
    msgsnd(msgid2,&messagei,sizeof(messagei),0);  





    const int screenWidth = 800;
    const int screenHeight = 500;

    InitWindow(screenWidth, screenHeight, "Supermarket");

    struct Ball balls [CUS_NUM];
    for (int i = 0;i<CUS_NUM;i++){
        Vector2 ball2 = { 100.0f,((i+1)%4)*125.0f + 50};
        balls[i].ball = ball2;
        balls[i].id = i+1;
    }

    struct Ball balls2 [CUS_NUM];
    for (int i = 0;i<CUS_NUM;i++){
        Vector2 ball2 = { 375.0f,250.0f};
        balls2[i].ball = ball2;
        balls2[i].id = i+1;
    }

    Vector2 cashier [CASH_NUM];
    for (int i = 0;i<CASH_NUM;i++){
        Vector2 cashiere = { 700.0f,((i+1)%10)*47.0f + 35};
        cashier[i] = cashiere;
    }

    Vector2 target = { 350.0f, 225.0f };
    Vector2 target2 = { 800.0f, 225.0f };
    Vector2 size = {100,50};
    Vector2 size2 = {30,30};
    float ballSpeed = 15.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {



        for (int j = 0; j < CUS_NUM; j++){
            int u = 0;
            while (arrcus[u] != -1)
            {
                if(arrcus[u] == balls[j].id){
                    moveBall(&balls[j].ball, target, ballSpeed);
                }
                u++;
            }
            
        }
      
        for (int j = 0; j < CUS_NUM; j++){
            int u = 0;
            while (arrcus1[u] != -1)
            {
                if(arrcus1[u] == balls[j].id){
                    moveBall2(&balls2[j].ball, cashier[arrca[u]], ballSpeed);
                }
                u++;
            }
            
        }
       
        for (int j = 0; j < CUS_NUM; j++){
            int u = 0;
            while (arrcus2[u] != -1)
            {
                if(arrcus2[u] == balls[j].id){
                    moveBall2(&balls2[j].ball, cashier[arrca2[u]], ballSpeed);
                }
                u++;
            }
            
        }


        for (int j = 0; j < CUS_NUM; j++){
            int u = 0;
            while (arrcus3[u] != -1)
            {
                if(arrcus3[u] == balls[j].id){
                    moveBall(&balls2[j].ball, target2, ballSpeed);
                }
                u++;
            }
            
        }

        for (int j = 0; j < CUS_NUM; j++){
            int u = 0;
            while (arrcus4[u] != -1)
            {
                if(arrcus4[u] == balls[j].id){
                    moveBall(&balls2[j].ball, target2, ballSpeed);
                }
                u++;
            }
            
        }

        
        

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);
         for (int j = 0; j < CUS_NUM; j++){
            DrawCircleV(balls[j].ball, 20, MAROON);
        }

         for (int j = 0; j < CUS_NUM; j++){
            DrawCircleV(balls2[j].ball, 20, MAROON);
        }
        
         for (int j = 0; j < CASH_NUM; j++){
            DrawRectangleV(cashier[j],size2,BLUE);
        }
        //DrawCircleV(ball, 20, MAROON);
        DrawRectangleV(target,size,GREEN);

        EndDrawing();
    }

    CloseWindow();

    
    return 0;
}