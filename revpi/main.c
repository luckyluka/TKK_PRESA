#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include "kunbus.h"
#include "main.h"

#define PORTNO 1500

void initServer();
void initMain();
void initCommAKDPress();
void diagnostics();
void receiveRequest();
void sendResponse(int reqId);
void error(const char *msg)
{
  perror(msg);
  exit(1);
}

int main()
{
  initServer();
  initCommAKDPress();
  initMain();
 
  while(program == 1)
  {
    if(readVariableValue("I_1_i03") && step == 0)
    {
      step = 1;
    }
    else if(readVariableValue("I_2_i03"))
    {
      step = 0;
    }
    receiveRequest();
    diagnostics();
    
  }    
  close(newsockfd);
  close(sockfd);
  return 0; 
}

void initServer()
{
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) 
  {   
    error("ERROR opening socket");
  }
   
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORTNO);
   
  if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
  { 
    error("ERROR on binding");
  }

  listen(sockfd,5);
  puts("Waiting for incoming connection\n");
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd < 0) 
  {
    error("ERROR on accept");
  }
  printf("Connection Accepted\n");
}

void initCommAKDPress() 
{
  ip_adrs = "192.168.1.13";
  s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  AKD_server.sin_family = AF_INET;
  AKD_server.sin_port = htons(502);
  AKD_server.sin_addr.s_addr = inet_addr(ip_adrs);
  printf("connecting to press drive\n");
  conn_AKD = connect(s, (struct sockaddr *)&AKD_server, sizeof(struct sockaddr_in));
  if (conn_AKD < 0) 
  {   
    error("ERROR connecting");
  }
  else
  {
    printf("AKD connected\n");
  }
}  


void initMain()
{
  program = 1;
  PiControlHandle_g = -1; 
  
  int * clear1 =  (int*)(&obufCl[0]);
  int * clear2 =  (int*)(&obufCl[2]);
  int * clear3 =  (int*)(&obufCl[4]);
  int * clear4 =  (int*)(&obufCl[6]);
  int * clear5 =  (int*)(&obufCl[7]);
  int * clear6 =  (int*)(&obufCl[8]);
  int * clear7 =  (int*)(&obufCl[10]);
  int * clear8 =  (int*)(&obufCl[12]);
  int * clear9 =  (int*)(&obufCl[16]);

  memset(obufCl, 0, 17);
  * clear1 = transId;   
  * clear2 = htons(0);
  * clear3 = htons(11);
  * clear4 = 1;
  * clear5 = 16;
  * clear6 = htons(530);
  * clear7 = htons(2);
  * clear8 = 4;
  * clear9 = 2; 

  int * writePosTen1 = (int*)(&writePosTenBuff[0]);
  int * writePosTen2 = (int*)(&writePosTenBuff[2]);
  int * writePosTen3 = (int*)(&writePosTenBuff[4]);
  int * writePosTen4 = (int*)(&writePosTenBuff[6]);
  int * writePosTen5 = (int*)(&writePosTenBuff[7]);
  int * writePosTen6 = (int*)(&writePosTenBuff[8]);
  int * writePosTen7 = (int*)(&writePosTenBuff[10]);
  int * writePosTen8 = (int*)(&writePosTenBuff[12]);
  int * writePosTen11 = (int*)(&writePosTenBuff[21]);
  int * writePosTen12 = (int*)(&writePosTenBuff[28]);
  int * writePosTen13 = (int*)(&writePosTenBuff[29]);
  int * writePosTen14 = (int*)(&writePosTenBuff[33]);
  int * writePosTen15 = (int*)(&writePosTenBuff[40]);
  int * writePosTen16 = (int*)(&writePosTenBuff[44]);
  int * writePosTen17 = (int*)(&writePosTenBuff[48]);
  int * writePosTen18 = (int*)(&writePosTenBuff[52]);
      
  memset(writePosTenBuff, 0, 58);
  * writePosTen1 = transId;   
  * writePosTen2 = htons(0);
  * writePosTen3 = htons(47);
  * writePosTen4 = 1;
  * writePosTen5 = 16;
  * writePosTen6 = htons(8192);
  * writePosTen7 = htons(20);
  * writePosTen8 = 40;
  * writePosTen11 = htonl(2000000);   
  * writePosTen12 = 16;           
  * writePosTen13 = htonl(5000000);  
  * writePosTen14 = htonl(5000000);  
  * writePosTen15 = 100;            
  * writePosTen16 = 0;            
  * writePosTen17 = 1;            
  * writePosTen18 = 1;  

  int * moveTask1 =  (int*)(&obufMT[0]);
  int * moveTask2 =  (int*)(&obufMT[2]);
  int * moveTask3 =  (int*)(&obufMT[4]);
  int * moveTask4 =  (int*)(&obufMT[6]);
  int * moveTask5 =  (int*)(&obufMT[7]);
  int * moveTask6 =  (int*)(&obufMT[8]);
  int * moveTask7 =  (int*)(&obufMT[10]);
  int * moveTask8 =  (int*)(&obufMT[12]);
  int * moveTask9 =  (int*)(&obufMT[13]);
  
  memset(obufMT, 0, 17);
  * moveTask1 = transId;   
  * moveTask2 = htons(0);
  * moveTask3 = htons(11);
  * moveTask4 = 1;
  * moveTask5 = 16;
  * moveTask6 = htons(2014);
  * moveTask7 = htons(2);
  * moveTask8 = 4;
  * moveTask9 = 2;
 
  int * moveTask1Next =  (int*)(&obufMTN[0]);
  int * moveTask2Next =  (int*)(&obufMTN[2]);
  int * moveTask3Next =  (int*)(&obufMTN[4]);
  int * moveTask4Next =  (int*)(&obufMTN[6]);
  int * moveTask5Next =  (int*)(&obufMTN[7]);
  int * moveTask6Next =  (int*)(&obufMTN[8]);
  int * moveTask7Next =  (int*)(&obufMTN[10]);
  int * moveTask8Next =  (int*)(&obufMTN[12]);
  int * moveTask9Next =  (int*)(&obufMTN[13]);
  
  memset(obufMTN, 0, 17);
  * moveTask1Next = transId;   
  * moveTask2Next = htons(0);
  * moveTask3Next = htons(11);
  * moveTask4Next = 1;
  * moveTask5Next = 16;
  * moveTask6Next = htons(2016);
  * moveTask7Next = htons(2);
  * moveTask8Next = 4;
  * moveTask9Next = 2;

  int * drvSave1 =  (int*)(&obufDS[0]);
  int * drvSave2 =  (int*)(&obufDS[2]);
  int * drvSave3 =  (int*)(&obufDS[4]);
  int * drvSave4 =  (int*)(&obufDS[6]);
  int * drvSave5 =  (int*)(&obufDS[7]);
  int * drvSave6 =  (int*)(&obufDS[8]);
  int * drvSave7 =  (int*)(&obufDS[10]);
  int * drvSave8 =  (int*)(&obufDS[12]);
  int * drvSave9 =  (int*)(&obufDS[13]);

  memset(obufDS, 0, 17);
  * drvSave1 = transId;   
  * drvSave2 = htons(0);
  * drvSave3 = htons(11);
  * drvSave4 = 1;
  * drvSave5 = 16;
  * drvSave6 = htons(8210);
  * drvSave7 = htons(2);
  * drvSave8 = 4;
  * drvSave9 = htonl(1);
}



void receiveRequest()
{
  //printf("RECEIVING REQUEST\n");
  FD_SET(newsockfd, &fdsTCP);
  n = select(32, &fdsTCP, NULL, NULL, &tv);
  n = recv(newsockfd, recvReadBuff, 57, 0);
  //printf("REQUEST RECEIVED\n");
  
  if(recvReadBuff[0] == 1) /* SEND VARIABLE DATA */
  {
    sendResponse(1);
  } 
  else if(recvReadBuff[0] == 2) /* SWITCH OUTPUTS */
  {
    if(recvReadBuff[1] < 15)
    {
      sprintf(outputWriteBuff, "O_%d",  recvReadBuff[1]);
    }
    else if(recvReadBuff[1] > 14 && recvReadBuff[1] < 29)
    {
      sprintf(outputWriteBuff, "O_%d_i03",  (recvReadBuff[1]-14));
    }
    writeVariableValue(outputWriteBuff, recvReadBuff[2]);
    printf("%s\n", outputWriteBuff);
    sendResponse(2);
  }
  else if(recvReadBuff[0] == 3) /* START MOTION */
  {
    step=1;
    sendResponse(3);
  }
  else if(recvReadBuff[0] == 4) /* SEND STEP INFO */
  {
    sendResponse(4);
  }
  else if(recvReadBuff[0] == 5) /* CONTINUE - MOVE FOR ONE STEP */
  {
    if(step != 0) /* or max step */
    {
      step++;
    }
    sendResponse(5);
  }
  else if(recvReadBuff[0] == 6) /* GET SELECTED CAN DATA */
  {
    selectedCan = recvReadBuff[1]; 
    writeVariableValue("O_4_i03", 1);
    writeVariableValue("O_12", 1);
    usleep(100000);
    writeVariableValue("O_4_i03", 0);
    writeVariableValue("O_12", 0);
    sendResponse(6);
  }
  else if(recvReadBuff[0] == 7) /* GET PRESS PARAMETER DATA */
  {
    press = (recvReadBuff[1])*1000; 
    sendResponse(7);
  }
  else if(recvReadBuff[0] == 8) /* STOP MOTION */
  {
    step = 0;
    sendResponse(8);
  }
}


void sendResponse(int reqId)
{
  if(reqId == 1)
  {
    int * sendWrite0 = (int*)(&sendWriteBuff[0]);
    int * sendWrite1 = (int*)(&sendWriteBuff[1]);
    int * sendWrite2 = (int*)(&sendWriteBuff[2]);
    int * sendWrite3 = (int*)(&sendWriteBuff[3]);
    int * sendWrite4 = (int*)(&sendWriteBuff[4]);
    int * sendWrite5 = (int*)(&sendWriteBuff[5]);
    int * sendWrite6 = (int*)(&sendWriteBuff[6]);
    int * sendWrite7 = (int*)(&sendWriteBuff[7]);
    int * sendWrite8 = (int*)(&sendWriteBuff[8]);
    int * sendWrite9 = (int*)(&sendWriteBuff[9]);
    int * sendWrite10 = (int*)(&sendWriteBuff[10]);
    int * sendWrite11 = (int*)(&sendWriteBuff[11]);
    int * sendWrite12 = (int*)(&sendWriteBuff[12]);
    int * sendWrite13 = (int*)(&sendWriteBuff[13]);
    int * sendWrite14 = (int*)(&sendWriteBuff[14]);
    int * sendWrite15 = (int*)(&sendWriteBuff[15]);
    int * sendWrite16 = (int*)(&sendWriteBuff[16]);
    int * sendWrite17 = (int*)(&sendWriteBuff[17]);
    int * sendWrite18 = (int*)(&sendWriteBuff[18]);
    int * sendWrite19 = (int*)(&sendWriteBuff[19]);
    int * sendWrite20 = (int*)(&sendWriteBuff[20]);
    int * sendWrite21 = (int*)(&sendWriteBuff[21]);
    int * sendWrite22 = (int*)(&sendWriteBuff[22]);
    int * sendWrite23 = (int*)(&sendWriteBuff[23]);
    int * sendWrite24 = (int*)(&sendWriteBuff[24]);
    int * sendWrite25 = (int*)(&sendWriteBuff[25]);
    int * sendWrite26 = (int*)(&sendWriteBuff[26]);
    int * sendWrite27 = (int*)(&sendWriteBuff[27]);
    int * sendWrite28 = (int*)(&sendWriteBuff[28]);
    int * sendWrite29 = (int*)(&sendWriteBuff[29]);
    int * sendWrite30 = (int*)(&sendWriteBuff[30]);
    int * sendWrite31 = (int*)(&sendWriteBuff[31]);
    int * sendWrite32 = (int*)(&sendWriteBuff[32]);
    int * sendWrite33 = (int*)(&sendWriteBuff[33]);
    int * sendWrite34 = (int*)(&sendWriteBuff[34]);
    int * sendWrite35 = (int*)(&sendWriteBuff[35]);
    int * sendWrite36 = (int*)(&sendWriteBuff[36]);
    int * sendWrite37 = (int*)(&sendWriteBuff[37]);
    int * sendWrite38 = (int*)(&sendWriteBuff[38]);
    int * sendWrite39 = (int*)(&sendWriteBuff[39]);
    int * sendWrite40 = (int*)(&sendWriteBuff[40]);
    int * sendWrite41 = (int*)(&sendWriteBuff[41]);
    int * sendWrite42 = (int*)(&sendWriteBuff[42]);
    int * sendWrite43 = (int*)(&sendWriteBuff[43]);
    int * sendWrite44 = (int*)(&sendWriteBuff[44]);
    int * sendWrite45 = (int*)(&sendWriteBuff[45]);
    int * sendWrite46 = (int*)(&sendWriteBuff[46]);
    int * sendWrite47 = (int*)(&sendWriteBuff[47]);
    int * sendWrite48 = (int*)(&sendWriteBuff[48]);
    int * sendWrite49 = (int*)(&sendWriteBuff[49]);
    int * sendWrite50 = (int*)(&sendWriteBuff[50]);
    int * sendWrite51 = (int*)(&sendWriteBuff[51]);
    int * sendWrite52 = (int*)(&sendWriteBuff[52]);
    int * sendWrite53 = (int*)(&sendWriteBuff[53]);
    int * sendWrite54 = (int*)(&sendWriteBuff[54]);
    int * sendWrite55 = (int*)(&sendWriteBuff[55]);
    int * sendWrite56 = (int*)(&sendWriteBuff[56]);
   
    * sendWrite0 = 1;
    * sendWrite1 = readVariableValue("I_1");
    * sendWrite2 = readVariableValue("I_2");
    * sendWrite3 = readVariableValue("I_3");
    * sendWrite4 = readVariableValue("I_4");
    * sendWrite5 = readVariableValue("I_5");
    * sendWrite6 = readVariableValue("I_6");
    * sendWrite7 = readVariableValue("I_7");
    * sendWrite8 = readVariableValue("I_8");
    * sendWrite9 = readVariableValue("I_9");
    * sendWrite10 = readVariableValue("I_10");
    * sendWrite11 = readVariableValue("I_11");
    * sendWrite12 = readVariableValue("I_12");
    * sendWrite13 = readVariableValue("I_13");
    * sendWrite14 = readVariableValue("I_14");
    * sendWrite15 = readVariableValue("I_1_i03");
    * sendWrite16 = readVariableValue("I_2_i03");
    * sendWrite17 = readVariableValue("I_3_i03");
    * sendWrite18 = readVariableValue("I_4_i03");
    * sendWrite19 = readVariableValue("I_5_i03");
    * sendWrite20 = readVariableValue("I_6_i03");
    * sendWrite21 = readVariableValue("I_7_i03");
    * sendWrite22 = readVariableValue("I_8_i03");
    * sendWrite23 = readVariableValue("I_9_i03");
    * sendWrite24 = readVariableValue("I_10_i03");
    * sendWrite25 = readVariableValue("I_11_i03");
    * sendWrite26 = readVariableValue("I_12_i03");
    * sendWrite27 = readVariableValue("I_13_i03");
    * sendWrite28 = readVariableValue("I_14_i03");

    * sendWrite29 = readVariableValue("O_1");
    * sendWrite30 = readVariableValue("O_2");
    * sendWrite31 = readVariableValue("O_3");
    * sendWrite32 = readVariableValue("O_4");
    * sendWrite33 = readVariableValue("O_5");
    * sendWrite34 = readVariableValue("O_6");
    * sendWrite35 = readVariableValue("O_7");
    * sendWrite36 = readVariableValue("O_8");
    * sendWrite37 = readVariableValue("O_9");
    * sendWrite38 = readVariableValue("O_10");
    * sendWrite39 = readVariableValue("O_11");
    * sendWrite40 = readVariableValue("O_12");
    * sendWrite41 = readVariableValue("O_13");
    * sendWrite42 = readVariableValue("O_14");
    * sendWrite43 = readVariableValue("O_1_i03");
    * sendWrite44 = readVariableValue("O_2_i03");
    * sendWrite45 = readVariableValue("O_3_i03");
    * sendWrite46 = readVariableValue("O_4_i03");
    * sendWrite47 = readVariableValue("O_5_i03");
    * sendWrite48 = readVariableValue("O_6_i03");
    * sendWrite49 = readVariableValue("O_7_i03");
    * sendWrite50 = readVariableValue("O_8_i03");
    * sendWrite51 = readVariableValue("O_9_i03");
    * sendWrite52 = readVariableValue("O_10_i03");
    * sendWrite53 = readVariableValue("O_11_i03");
    * sendWrite54 = readVariableValue("O_12_i03");
    * sendWrite55 = readVariableValue("O_13_i03");
    * sendWrite56 = readVariableValue("O_14_i03");
    
    FD_ZERO(&fdsTCP);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
  
    n = select(32, NULL, &fdsTCP, NULL, &tv); 
  
    n = send(newsockfd, sendWriteBuff, 57, 0);
    memset(sendWriteBuff, 0, 57);
    //printf("RESPONSE SENT reqId:%d\n", reqId);

  }
  else if(reqId == 2)
  {
    int * sendRead0 = (int*)(&sendReadBuff[0]);
    * sendRead0 = 2;
    
    FD_ZERO(&fdsTCP);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
  
    n = select(32, NULL, &fdsTCP, NULL, &tv); 
   
    n = send(newsockfd, sendReadBuff, 1, 0);
    memset(sendReadBuff, 0, 1);
    //printf("RESPONSE SENT reqId:%d\n", reqId);
  }
  else if(reqId == 3)
  {
    int * sendWrite0 = (int*)(&sendWriteBuff[0]);
    * sendWrite0 = 3;
    
    FD_ZERO(&fdsTCP);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
  
    n = select(32, NULL, &fdsTCP, NULL, &tv); 
    
    n = send(newsockfd, sendWriteBuff, 1, 0);
    memset(sendWriteBuff, 0, 1);
    //printf("RESPONSE SENT reqId:%d\n", reqId);
  }
  
  else if(reqId == 4)
  {
    int * sendWrite0 = (int*)(&sendWriteBuff[0]);
    int * sendWrite1 = (int*)(&sendWriteBuff[1]);
    * sendWrite0 = 4;
    * sendWrite1 = step;
    
    FD_ZERO(&fdsTCP);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
  
    n = select(32, NULL, &fdsTCP, NULL, &tv); 
   
    n = send(newsockfd, sendWriteBuff, 2, 0);
    memset(sendWriteBuff, 0, 2);
    //printf("RESPONSE SENT reqId:%d\n", reqId);
  }
  else if(reqId == 5)
  {
    int * sendWrite0 = (int*)(&sendWriteBuff[0]);
    * sendWrite0 = 5;
    
    FD_ZERO(&fdsTCP);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
  
    n = select(32, NULL, &fdsTCP, NULL, &tv); 
   
    n = send(newsockfd, sendWriteBuff, 1, 0);
    memset(sendWriteBuff, 0, 1);
    //printf("RESPONSE SENT reqId:%d\n", reqId);
  }
  else if(reqId == 6)
  {
    int * sendWrite0 = (int*)(&sendWriteBuff[0]);
    * sendWrite0 = 6;
    
    FD_ZERO(&fdsTCP);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
  
    n = select(32, NULL, &fdsTCP, NULL, &tv); 
   
    n = send(newsockfd, sendWriteBuff, 1, 0);
    memset(sendWriteBuff, 0, 1);
    //printf("RESPONSE SENT reqId:%d\n", reqId);
  }
  else if(reqId == 7)
  {
    int * sendWrite0 = (int*)(&sendWriteBuff[0]);
    * sendWrite0 = 7;
    
    FD_ZERO(&fdsTCP);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
  
    n = select(32, NULL, &fdsTCP, NULL, &tv); 
   
    n = send(newsockfd, sendWriteBuff, 1, 0);
    memset(sendWriteBuff, 0, 1);
    //printf("RESPONSE SENT reqId:%d\n", reqId);
  }

  else if(reqId == 8)
  {
    int * sendWrite0 = (int*)(&sendWriteBuff[0]);
    * sendWrite0 = 8;
    
    FD_ZERO(&fdsTCP);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
  
    n = select(32, NULL, &fdsTCP, NULL, &tv); 
   
    n = send(newsockfd, sendWriteBuff, 1, 0);
    memset(sendWriteBuff, 0, 1);
    //printf("RESPONSE SENT reqId:%d\n", reqId);
  }
}

/*
void diagnostics()
{
  switch(step)
  {
    case 0:
      break;

    case 1:
      printf("STEP: %d\n", step);  
      printf("SELECTED CAN: %d\n", selectedCan);
      break;
    
    case 2:
      printf("STEP: %d\n", step);
      writeVariableValue("O_9", 1);
      usleep(100000);
      writeVariableValue("O_9", 0);
      printf("first pos\n");
      if(readVariableValue("I_11"))
      {
        step = 3;
      }
      break;

    case 3:   
      usleep(3000000);
      writeVariableValue("O_10", 1);
      usleep(100000);
      writeVariableValue("O_10", 0);
      printf("measurement position\n");
      step = 4;
      break;

    case 4:
      if(readVariableValue("I_12"))
      {
        step = 5;
      }
      break;
    
    case 5:
    {
      printf("STEP: %d\n", step);
      int w;
      int * read1 = (int*)(&readBuff[0]);
      int * read2 = (int*)(&readBuff[2]);
      int * read3 = (int*)(&readBuff[4]);
      int * read4 = (int*)(&readBuff[6]);
      int * read5 = (int*)(&readBuff[7]);
      int * read6 = (int*)(&readBuff[8]);
      int * read7 = (int*)(&readBuff[10]);
      int * clear1 =  (int*)(&obufCl[0]);
      int * clear9 =  (int*)(&obufCl[16]);
      int * writePosTen1 = (int*)(&writePosTenBuff[0]);
      int * writePosTen9 =  (int*)(&writePosTenBuff[16]);
      int * writePosTen10 = (int*)(&writePosTenBuff[17]);
      int * moveTask1Next =  (int*)(&obufMTN[0]); 
      int * moveTask9Next =  (int*)(&obufMTN[13]);
      int * drvSave1 = (int*)(&obufDS[0]);
   
      memset(readBuff, 0, 12);
      * read2 = htons(0);
      * read3 = htons(6);
      * read4 = 1;
      * read5 = 3;
      * read6 = htons(2072);
      * read7 = htons(2);
      * clear9 = 10;      
      * writePosTen9 = 10;       
     * moveTask9Next = htonl(10000);                
         
      * read1 = transId;   
      sendModbus(conn_AKD, s, readBuff, 12, readBuff_recv, 50, "read feedback position");
      w = ((readBuff_recv[10]<<16) + (readBuff_recv[11]<<8) + readBuff_recv[12]);     
      * writePosTen10 = htonl(w + press);
      printf("POSITION FEEDBACK:%d\n", w);
     
      * clear1 = transId;
      sendModbus(conn_AKD, s, obufCl, 17, ibufCl, 50, "position 10 cleared");
       
      * writePosTen1 = transId;       
      sendModbus(conn_AKD, s, writePosTenBuff, 53, writePosTenBuff_recv, 50, "position 10 parameter");
    
      * moveTask1Next = transId;
      sendModbus(conn_AKD, s, obufMTN, 17, ibufMTN, 50, "second position parameter");
      
      * drvSave1 = transId;
      sendModbus(conn_AKD, s, obufDS, 17, ibufDS, 50, "save to drive");
      
      step = 6;
      break;
	   
    }

    case 6:
      printf("STEP: %d\n", step);
      printf("ready to perform press\n");
      writeVariableValue("O_9", 1);
      usleep(100000); 
      writeVariableValue("O_9", 0);
      if(readVariableValue("I_11"))
      {
        step = 7;
      }
      break;

    case 7:
      printf("STEP: %d\n", step);
      writeVariableValue("O_1", 1);
      usleep(100000);
      writeVariableValue("O_1", 0);
      usleep(100000);
      writeVariableValue("O_1", 1);
      
      step = 8;
      break;

    case 8:
      printf("STEP: %d\n", step);
      writeVariableValue("O_10", 1);
      usleep(100000);
      step = 9;
      break;
 
    case 9:
      printf("STEP: %d\n", step);
      if(readVariableValue("I_5")==1)
      {
        writeVariableValue("O_2", 1);
	writeVariableValue("O_1", 0);
        step = 10;
      }
      break;
   
    case 10:
      printf("STEP: %d\n", step);
      if(readVariableValue("I_11")==1)
      {
        step = 11;
      }
      break;
    
    case 11:
      printf("STEP: %d\n", step);
      writeVariableValue("O_10", 0);
      usleep(100000);
      step = 12;
      break;

    case 12:
      printf("STEP: %d\n", step);
      writeVariableValue("O_9", 1);
      step = 13;
      break;

    case 13:
      printf("STEP: %d\n", step);
      if(readVariableValue("I_11")==1)
      {
        step = 14;
      }
      break;

    case 14:
      printf("STEP: %d\n", step);
      writeVariableValue("O_9", 0);
      step = 15;
      break;
   
    case 15:
      printf("STEP: %d\n", step);
      writeVariableValue("O_2", 0);
      step = 16;
      break;
     
    case 16:
    {
      printf("STEP: %d\n", step);
      
      int * moveTask1 =  (int*)(&obufMT[0]); 
      int * moveTask9 =  (int*)(&obufMT[13]);
      int * moveTask1Next =  (int*)(&obufMTN[0]); 
      int * moveTask9Next =  (int*)(&obufMTN[13]);
      int * drvSave1 = (int*)(&obufDS[0]);
   
      * moveTask1 = transId;
      if(selectedCan == 0)
      {
	* moveTask9 = htonl(1000);                
	* moveTask9Next = htonl(2000);
      }
      else if(selectedCan == 1)
      {
	* moveTask9 = htonl(3000);
	* moveTask9Next = htonl(4000);
      }
      else if(selectedCan == 2)
      {
       * moveTask9 = htonl(5000);
       * moveTask9Next = htonl(6000);     
      }
      else if(selectedCan == 3)
      {
	* moveTask9 = htonl(7000);
	* moveTask9Next = htonl(8000); 
      }

      sendModbus(conn_AKD, s, obufMT, 17, ibufMT, 50, "move task 1 - revert to original positions");
      * moveTask1Next = transId;
	
      sendModbus(conn_AKD, s, obufMTN, 17, ibufMTN, 50, "move task 2 - revert to original positions");
 
      * drvSave1 = transId;
      sendModbus(conn_AKD, s, obufDS, 17, ibufDS, 50, "save to drive");
 
      transId++;
      step = 0;
    }
  }
}
*/
void diagnostics()
{
  switch(step)
  {
    case 0:
      break;

    case 1:  /* gripper - pomik na zgornjo pozicijo */
      printf("STEP: %d\n", step);
      printf("gripper - pomik na zgornjo pozicijo\n");
      writeVariableValue("0_1", 0);
      writeVariableValue("0_2", 1);
      usleep(delay_time);
      writeVariableValue("0_2", 0);
      writeVariableValue("O_11", 0);
      writeVariableValue("O_12", 0);
      writeVariableValue("O_13", 0);
      writeVariableValue("O_14", 0);
      writeVariableValue("O_1_i03", 0);
      if(selectedCan == 0) /* mala */
      {
        writeVariableValue("O_11", 1); 
        usleep(delay_time2);
        writeVariableValue("O_1_i03", 1);
        usleep(delay_time); 
        writeVariableValue("O_1_i03", 0);      
      }
      else if(selectedCan == 1) /* mala 2 */
      {
        writeVariableValue("O_11", 1);
        writeVariableValue("O_12", 1);
        usleep(delay_time2);
        writeVariableValue("O_1_i03", 1);
        usleep(delay_time);
        writeVariableValue("O_1_i03", 0);      
      }
      else if(selectedCan == 2) /* srednja */
      {
        writeVariableValue("O_11", 1);
        writeVariableValue("O_13", 1);
        usleep(delay_time2);
        writeVariableValue("O_1_i03", 1);
        usleep(delay_time); 
        writeVariableValue("O_1_i03", 0);      
      }
      else if(selectedCan == 3) /* velika */
      {
        writeVariableValue("O_11", 1);
        writeVariableValue("O_12", 1);
        writeVariableValue("O_13", 1);
        usleep(delay_time2); 
        writeVariableValue("O_1_i03", 1);
        usleep(delay_time); 
        writeVariableValue("O_1_i03", 0);      
      }
      step = 2;
      break;

    case 2:  /* preverim ali je bil premik izveden */
      printf("STEP: %d\n", step);
      printf("preverim ali je bil premik izveden\n");
      writeVariableValue("O_11", 0);
      writeVariableValue("O_12", 0);
      writeVariableValue("O_13", 0);
      writeVariableValue("O_14", 0);
      writeVariableValue("O_1_i03", 0);
     
      if(readVariableValue("I_12"))
      {
	step = 3;
      }
      break;
 
    case 3: /* presa - premik na zgornjo pozicijo */
      printf("STEP: %d\n", step);
      printf("presa - premik na zgornjo pozicijo\n");
      writeVariableValue("O_9", 1);
      usleep(delay_time); 
      writeVariableValue("O_9", 0);
      step = 4;
      break;

    case 4: /* preverim ali je bil pomik izveden */
      printf("STEP: %d\n", step);
      printf("preverim ali je bil pomik izveden\n");
      if(readVariableValue("I_11"))
      { 
        step = 5;
      }
      break;

    case 5: /* gripper - premik na spodnjo pozicijo */
      printf("STEP: %d\n", step);
      printf("gripper - premik na spodnjo pozicijo\n");
      writeVariableValue("O_1_i03", 1);
      usleep(delay_time); 
      writeVariableValue("O_1_i03", 0);        

      writeVariableValue("O_11", 0);
      writeVariableValue("O_12", 0);
      writeVariableValue("O_13", 0);
      writeVariableValue("O_14", 0);
    
      if(selectedCan == 0) /* mala */
      {
	writeVariableValue("O_11", 1);
      }
      else if(selectedCan == 1) /* mala 2 */
      {
	writeVariableValue("O_11", 1);
	writeVariableValue("O_12", 1);
      }
      else if(selectedCan == 2) /* srednja */
      {
	writeVariableValue("O_11", 1);
	writeVariableValue("O_13", 1);
      }
      else if(selectedCan == 3) /* velika*/
      {
	writeVariableValue("O_11", 1);
	writeVariableValue("O_12", 1);
	writeVariableValue("O_13", 1);
      } 

      step = 7;
      break;
  
    case 6: /* preverim ali je bil premik izveden */
      printf("STEP: %d\n", step);
      printf("preverim ali je bil premik izveden\n");
    
      if(readVariableValue("I_12"))
      {
        step = 7;
      }      
      break;

    case 7: /* presa - premik na spodnjo pozicijo */
      printf("STEP: %d\n", step);
      printf("presa - premik na spodnjo pozicijo\n");
      writeVariableValue("O_1", 1);
      writeVariableValue("O_10", 1);
      step = 8;
      break;
    
    case 8: /* preverim ali sta presa in gripper izvedla pomik */
      printf("STEP: %d\n", step);
      printf("preverim ali sta presa in gripper izvedla pomik\n");
      if(readVariableValue("I_11") && readVariableValue("I_12")) /* second condition used if both moves at the same time*/
      {
       
       step = 9;
      }
      break;
    
    case 9: /* presa - pomik v zgornjo pozicijo*/
      printf("STEP: %d\n", step);
      printf("presa - pomik v zgornjo pozicijo\n");    
      writeVariableValue("O_9", 1);
      step = 11; /*no need to wait here*/
      break;

    case 10: /* preverim ali je bil pomik izveden */
      printf("STEP: %d\n", step);
      printf("preverim ali je bil pomik izveden\n");
      if(readVariableValue("I_11"))
      {
	step = 11;
      }
      break;

    case 11: /* gripper - pomik v zgornjo pozicijo */
      printf("STEP: %d\n", step);
      printf("gripper - pomik v zgornjo pozicijo\n");
      writeVariableValue("O_1_i03", 1);
      writeVariableValue("O_2", 1);
      writeVariableValue("O_1", 0);
      usleep(delay_time);
      writeVariableValue("O_2", 0);
      writeVariableValue("O_1_i03", 0);      
      writeVariableValue("O_11", 0);
      writeVariableValue("O_12", 0);
      writeVariableValue("O_13", 0);
      writeVariableValue("O_14", 0);
     
      if(selectedCan == 0) /* mala */
      {
        writeVariableValue("O_12", 1);
      }
      else if(selectedCan == 1) /* mala 2 */
      {
        writeVariableValue("O_13", 1);
      }
      else if(selectedCan == 2) /* srednja */
      {
        writeVariableValue("O_12", 1);
        writeVariableValue("O_13", 1);
      }
      else if(selectedCan == 3) /* velika */
      {
        writeVariableValue("O_14", 1);
      }

      step = 12;
      break;
    
    case 12:  /* preverim ali je pomik izveden */
      printf("STEP: %d\n", step);
      printf("preverim ali je pomik izveden\n");
      
      if(readVariableValue("I_12"))
      {
        step = 13;
      }
      break;

    case 13: /* miza - pomik za eno dozo */
      printf("STEP: %d\n", step);
      printf("miza - pomik za eno dozo\n");
  
      writeVariableValue("O_7", 1);
      //usleep(delay_time);
      writeVariableValue("O_9", 0);
      writeVariableValue("O_10", 0);

      step = 14;
      break;

    case 14: /* preverim ali je bil premik izveden */
      printf("STEP: %d\n", step);
      printf("preverim ali je bil premik izveden\n");
      usleep(50000);
      writeVariableValue("O_7", 0);
      step = 15;
      break;

    case 15:
      printf("STEP: %d\n", step);
      printf("Izklop stopa meritve\n");
      step = 5; /* for purposes of simulation*/
      break;
  }
}

void sendModbus(int socket, int socket_fd, char *send_buff, int send_buff_size, char *receive_buff, int receive_buff_size, char *print_text)
{
  socket = send(socket_fd, send_buff, send_buff_size, 0);
  printf("%s\n", print_text);
  socket = recv(socket_fd, receive_buff, receive_buff_size , 0);
  transId++;
}

void detectFall(const char *var)
{
  int currentValue;
  int oldValue;
  currentValue = 1;
  oldValue = 1;
  fallDetected = 0;
  currentValue = readVariableValue(var);
  
  if(currentValue != oldValue)
  {
    if(currentValue == 0)
    {
      fallDetected = 1;
    } 
    else
    {
      fallDetected = 0;
    }
    oldValue = currentValue; 
  }
}

void detectRise(const char *var)
{
  int currentValue;
  int oldValue;
  currentValue = 0;
  oldValue = 0;
  riseDetected = 0;
  currentValue = readVariableValue(var);
  if(currentValue != oldValue)
  {
    if(currentValue == 1)
    {
      riseDetected = 1;
    }
    else
    {
      riseDetected = 0;
    }
    oldValue = currentValue;  
  }
}



/*
void timer(float measure)  CASOVNI ZAMIK 
{
  struct timespec start, stop;
  double accum;
    
  clock_gettime(CLOCK_REALTIME, &start);    
  while(accum < measure)
  {
    clock_gettime(CLOCK_REALTIME, &stop);

    accum = ( stop.tv_sec - start.tv_sec )
     + ( stop.tv_nsec - start.tv_nsec )
     / BILLION;
  }
}
*/
