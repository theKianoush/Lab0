#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>



/*********************************************************/
/* this is the server for lab0. it will receive a msg    */
/* from the client, and print it out. Make this your     */
/* own. Comment it, modularize it, learn it, fix it, etc */
/*********************************************************/

int main(int argc, char *argv[])
{
  int sd; /* socket descriptor */
  int i;  /* loop variable */
  struct sockaddr_in server_address; /* my address */
  struct sockaddr_in from_address;  /* address of sender */
  char bufferReceived[1000]; // used in recvfrom
  int portNumber; // get this from command line
  int rc; // always need to check return codes!
  socklen_t fromLength;
  int flags = 0; // used for recvfrom
  
  /* first, decide if we have the right number of parameters */
  if (argc < 2){
    printf("usage is: server <portnumber>\n");
    exit (1);
  }


  /* first create a socket */
  sd = socket(AF_INET, SOCK_DGRAM, 0); /* create a socket */

  /* always check for errors */
  if (sd == -1){ /* means some kind of error occured */
    perror ("socket");
    exit(1); /* just leave if wrong number entered */
  }

  /* now fill in the address data structure we use to sendto the server */
  for (i=0;i<strlen(argv[1]); i++){
    if (!isdigit(argv[1][i]))
      {
	printf ("The Portnumber isn't a number!\n");
	exit(1);
      }
  }

  portNumber = strtol(argv[1], NULL, 10); /* many ways to do this */

  if ((portNumber > 65535) || (portNumber < 0)){
    printf ("you entered an invalid port number\n");
    exit (1);
  }

  server_address.sin_family = AF_INET; /* use AF_INET addresses */
  server_address.sin_port = htons(portNumber); /* convert port number */
  server_address.sin_addr.s_addr = INADDR_ANY; /* any adapter */
  
  /* the next step is to bind to the address */
  rc = bind (sd, (struct sockaddr *)&server_address,
	     sizeof(struct sockaddr ));
  
  if (rc < 0){
    perror("bind");
    exit (1);
  }
  memset (bufferReceived, 0, 1000); // zero out the buffers in C

  /* NOTE - you MUST MUST MUST give fromLength an initial value */
  fromLength = sizeof(struct sockaddr_in);

  rc = recvfrom(sd, bufferReceived, 1000, flags,
		(struct sockaddr *)&from_address, &fromLength);

  /* check for any possible errors */
  if (rc <=0){
    perror ("recvfrom");
    printf ("leaving, due to socket error on recvfrom\n");
    exit (1);
  }
  /* print out what i get */
  printf ("received '%s'\n", bufferReceived);
    

} // end of main()
