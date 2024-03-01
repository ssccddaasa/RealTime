#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h>
#include <signal.h>
#include<sys/wait.h>
#include <stdlib.h>
#include "raylib.h"














int main(){

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
  
  fgets(myString, 100, fptr);
  


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

            DrawText(myString, 300, 200, 32, RED);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
