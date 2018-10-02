
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <string.h>

#define SHM_SIZE 1024

/**
* This class is for practice with reading
* and writing to shared memory
*
* @author Allison Bolen
* @author Alec Allain
* @version 9/28/18
*/

/** Global variables */
char userInput;
int shmId;
char* shmPtr;
struct shmid_ds buffer;
typedef struct {
  int turn;
  char message[500];
} memToken;

/**
* Main method
*/
// the reader gets to read when the turn is when its 1
int main() {
  key_t key = ftok("test.txt",65);
  printf("Key: %d\n",key);
  memToken token;
  int shmId;
  char *shmPtr;
  char *i = "";
  token.turn = 0;
  strcpy(token.message, i);
  if ((shmId = shmget (key, SHM_SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
     perror ("i can't get no..\n");
     exit (1);
  }
  // get the pointer
  if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
     perror ("can't attach\n");
     exit (1);
  }

  while(strcmp(token.message, "quit") != 0){
     while(token.turn == 0){
       // it is not the readers turn to change the tun var
       // reading for updates on the turn variable
       printf("Cheking for updates.\n");
       memcpy(&token, shmPtr, sizeof(memToken));
     }
     // This is the critical section where you read for real
     // you change the turn variable so that it is the writers turn
     printf("Got the message: %s", token.message);
     token.turn = 0;
     strcpy(token.message, i);
     memcpy(shmPtr, &token, sizeof(memToken));

   }
   // detach
   if (shmdt (shmPtr) < 0) {
      perror ("just can't let go\n");
      exit (1);
   }
   // clean
   if (shmctl (shmId, IPC_RMID, 0) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }

   return 0;
}
