#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "graphics_sdl.h"
#include "main.h"

int init()    /* things needed to start sdl2 properly */
{
  int flags;
  int innited;
 
  flags = IMG_INIT_JPG|IMG_INIT_PNG;
  innited = IMG_Init(flags);
	
  if((SDL_Init(SDL_INIT_VIDEO||SDL_INIT_EVENTS)) != 0)
  {
    SDL_Log("Unable to initialize SDL:%s ", SDL_GetError());
    return 1;                                                                               
  }  

  window = SDL_CreateWindow("IT-Elektronika", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
   

  if(window == NULL)
  {
    return 1;
  }		  

  renderer = SDL_CreateRenderer(window, - 1, SDL_RENDERER_SOFTWARE);
  if(renderer == NULL)
  {
    ;
    /*printf("RENDERER IS NULL\n");*/
  }

  if((innited&flags) != flags)
  {
    
    printf("IMG_INIT: %s\n", IMG_GetError());
  }

  if(TTF_Init() != 0)
  {
    printf("TTF ERROR: %s\n", TTF_GetError());
  }

  #ifdef RPI
  smallText = TTF_OpenFont("/home/pi/TKK_PRESA/font/DejaVuSansMono.ttf", 30);
  if(smallText == NULL)
  {
    printf("NO FONT FOUND:%s\n", TTF_GetError());
  }
  regularText = TTF_OpenFont("/home/pi/TKK_PRESA/font/DejaVuSansMono.ttf", 45);
  if(regularText == NULL)
  {
    printf("NO FONT FOUND\n");
  }
  #endif
  #ifdef LUKA
  smallText = TTF_OpenFont("/home/luka/TKK_PRESA_/font/DejaVuSansMono.ttf", 30);
  if(smallText == NULL)
  {
    printf("NO FONT FOUND\n");
  }
  regularText = TTF_OpenFont("/home/luka/TKK_PRESA_/font/DejaVuSansMono.ttf", 45);
  if(regularText == NULL)
  {
    printf("NO FONT FOUND\n");
  }
  #endif

  return 15;
}


void initVars()
{
 /* color definitions  */
  whiteColor.r = 255;
  whiteColor.g = 255;
  whiteColor.b = 255;
  
  blackColor.r = 0;
  blackColor.g = 0;
  blackColor.b = 0;
}

void freeTexture(void)  
{
  if(texture != NULL)
  {
    SDL_DestroyTexture(texture);	  
    texture = NULL;
    textureWidth = 0;
    textureHeight = 0;
  }
}

void renderBackground(void)     
{
  switch(backgroundColor)
  {
    case 0:
      SDL_SetRenderDrawColor(renderer, 0, 178, 0, 255); /* green */
      break; 
    case 1:
      SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); /* yellow */
      break;
    case 2:
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); /* red */
      break;
  }
  SDL_RenderClear(renderer);
}

void render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
  SDL_Rect renderQuad;
  renderQuad.x = x;
  renderQuad.y = y;
  renderQuad.w = textureWidth;
  renderQuad.h = textureHeight;

  if(clip != NULL)
  {
    renderQuad.w = clip -> w;
    renderQuad.h = clip -> h;
  }
  SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
}

int renderText(const char *text, TTF_Font *textFont,  SDL_Color textColor)  
{
  SDL_Surface* textSurface;
  textSurface = TTF_RenderText_Solid(textFont, text, textColor);
  freeTexture();

  if(textSurface == NULL)
  {
    printf("Unable to render text surface! SDL Error: %s\n", SDL_GetError());
  }
  else
  {
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if(texture == NULL)
    {
      printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
      textureWidth = textSurface -> w;
      textureHeight = textSurface -> h;
    }
    SDL_FreeSurface(textSurface);
  }
  return texture != NULL;
}

void renderStatusBar()
{
  int i;
  int x1_1;
  int x1_2;
  int y1_1; 
  int y1_2;

  int x2_1;
  int x2_2;
  int y2_1; 
  int y2_2;

  int x3_1;
  int x3_2;
  int y3_1; 
  int y3_2;

  x1_1 = 0;
  y1_1 = 80;
  x1_2 = 1280;
  y1_2 = 80;

  x2_1 = 0;
  y2_1 = 0;
  x2_2 = 1280;
  y2_2 = 0;

  x3_1 = 0;
  y3_1 = 0;
  x3_2 = 0;
  y3_2 = 80;
 
  for(i = 0; i < 10; i++)
  {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderDrawLine(renderer, x1_1, y1_1+i, x1_2, y1_2+i);
    SDL_RenderDrawLine(renderer, x2_1, y2_1, x2_2, y2_2);
    SDL_RenderDrawLine(renderer, x3_1, y3_1, x3_2, y3_2);
  }

  switch(sbarText)
  {
    case 0:
      renderText("STATUS", regularText, blackColor); 
      break;

    case 1:
      renderText("MENU", regularText, blackColor);
      break;
    
    case 2:
      renderText("IZBIRA VELIKOSTI DOZE", regularText, blackColor);
      break;
    
    case 3:
      renderText("I / O STATUS", regularText, blackColor);
      break;

    case 4: 
      renderText("DIAGNOSTIKA", regularText, blackColor);
      break;

    case 5: 
      renderText("NASTAVLJANJE CASA", regularText, blackColor);
      break;    

    case 6: 
      renderText("POVEZOVANJE", regularText, blackColor);
      break;    
    
    case 7: 
      renderText("NAPAKA - povezovanje", regularText, blackColor);
      break;    
 
    case 8:
      renderText("NAPAKA - ", regularText, blackColor);
      break;

    case 9:
      renderText("NAPAKA - ", regularText, blackColor);
      break;

    case 10:
      renderText("NAPAKA - ", regularText, blackColor);
      break;

    case 11:
      renderText("NAPAKA PREOBREMENITEV PRESE", regularText, blackColor);
      break;

    case 12:
      renderText("KOT ZAZNAVANJA", regularText, blackColor);
      break;
  
    case 13: 
      renderText("NAPAKA VARNOSTNA ZAVESA", regularText, blackColor);
      break;
    
    case 14:
      renderText("STOP TOTAL", regularText, blackColor);
      break;

    case 15:
      renderText("NAPAKA DRIVERJA MOTORJA", regularText, blackColor);
      break;

    case 16:
      renderText("NAPAKA SKLOPKE", regularText, blackColor);
      break;

    case 17:
      renderText("NAPAKA POZICIJE", regularText, blackColor);
      break;
    
    case 18:
      renderText("NAPAKA OLJE", regularText, blackColor);
      break;

    case 19:
      renderText("NAPAKA ZRAK", regularText, blackColor);
      break;

    case 20:
      renderText("NAPAKA HORIZONTALNI POMIK", regularText, blackColor);
      break;

    case 21:
      renderText("NAPAKA VERTIKALNI POMIK", regularText, blackColor);
      break;
   
    case 22:
      renderText("USTAVLJANJE MOTORJA", regularText, blackColor);
      break;
  } 
  render(30, 10, NULL, 0.0, NULL, SDL_FLIP_NONE);
  clockButton(1050, 20, 50, 100, tmBuff); 
  
  
}

void renderContent()
{
  switch(page)
  {
    case 0:
      pageZero(0); /* main - landing page */
      backgroundColor = 1;
      sbarText = 0;
      break;
    
    case 1:
      pageOne(1); /* menu */
      backgroundColor = 1;
      sbarText = 1;
      break;
   
    case 2:
      pageTwo(2);   /* can size */
      backgroundColor = 1;
      sbarText = 2;
      break;
    
    case 3:
      pageThree(3);   /* */
      backgroundColor = 1;
      sbarText = 3;
      break;
    
    case 4:
      pageFour(4);
      backgroundColor = 1;
      sbarText = 4;
      break;

    case 5:
      pageFive(5);
      backgroundColor = 1;
      sbarText = 5;
      break;

    case 6:
      pageSix(6);
      backgroundColor = 1;
      sbarText = 6;
      break;

    case 7:
      pageSeven(7);
      backgroundColor = 2;
      sbarText = 7;
      break;

    case 8:
      pageEight(8);
      backgroundColor = 2;
      break;
  }
  oldtimestamp=timestamp;
  cycleCounter++;
}

void touchUpdate()   /* handling touch events */
{
  while(SDL_PollEvent(&event) != 0 )
  {
    #ifdef RPI
    if(event.type == SDL_FINGERDOWN)  
    {
      timestamp = event.tfinger.timestamp;
      touchLocation.x = event.tfinger.x;
      touchLocation.y = event.tfinger.y;
    }
    if(event.type == SDL_KEYDOWN)
    {
      if(event.key.keysym.sym == SDLK_ESCAPE)
      {
        program = 0;        
      }
      
    }
    #endif
    #ifdef LUKA
    if(event.type == SDL_MOUSEBUTTONDOWN)
    {
      timestamp = event.button.timestamp;
      touchLocation.x = event.button.x;
      touchLocation.y = event.button.y;
    }
    if(event.type == SDL_KEYDOWN)
    {
      if(event.key.keysym.sym == SDLK_ESCAPE)
      {
        program = 0;        
      }
      
    }
    #endif
  }  
}



void saveButton(int x, int y, int w, int h, char *text)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderDrawLine(renderer, x, y, (x+w), y);
  SDL_RenderDrawLine(renderer, (x+w), y, (x+w), (y+h)); 
  SDL_RenderDrawLine(renderer, (x+w), (y+h), x, (y+h));
  SDL_RenderDrawLine(renderer, x, (y+h), x, y);
  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
  {
    for(i = 0; i < 10; i++)
    {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderDrawLine(renderer, x, y+i, (x+w), y+i);
      SDL_RenderDrawLine(renderer, (x+w+i), y, (x+w+i), (y+h)); 
      SDL_RenderDrawLine(renderer, (x+w), (y+h-i), x, (y+h-i));
      SDL_RenderDrawLine(renderer, x+i, (y+h), x+i, y);
    }

    #ifdef RPI   
    fp_can_size = fopen("/home/pi/TKK_PRESA/data/can_size.txt", "w");
    #endif
    #ifdef LUKA
    fp_can_size = fopen("/home/luka/TKK_PRESA_/data/can_size.txt", "w");
    #endif
    posCounter = 0;

    /* writing position values to AKD registers and saving values to file */
    if(selected[0])
    {
      int * clear1 =  (int*)(&obufCl[0]);
      int * clear9 =  (int*)(&obufCl[16]);
      int * posOneB1 = (int*)(&obufOneB[0]);
      int * posOneB9 =  (int*)(&obufOneB[16]);
      int * posOneB10 = (int*)(&obufOneB[17]);
      int * moveTask1 =  (int*)(&obufMT[0]); 
      int * moveTask9 =  (int*)(&obufMT[13]);
      int * drvSave1 = (int*)(&obufDS[0]);
     
      * clear1 = transId;
      * clear9 = 2;      
      FD_ZERO(&fds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;
    
      conn = select(32, NULL, &fds, NULL, &tv);
      conn = send(s, obufCl, 17, 0);
      printf("Message Sent! - clear position - small\n");
      FD_SET(s, &fds);
      conn = select(32, &fds, NULL, NULL, &tv);
      conn = recv(s, ibufCl, 50 , 0);
      transId++;
            
      * posOneB1 = transId;       
      * posOneB9 = 2;       
      * posOneB10 = htonl((AKD_frame_posSmall+modifier)*1000);
      FD_ZERO(&fds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;
    
      conn = select(32, NULL, &fds, NULL, &tv);
      conn = send(s, obufOneB, 53, 0);
      printf("Message Sent! - position parameter - small\n");
      FD_SET(s, &fds);
      conn = select(32, &fds, NULL, NULL, &tv);
      conn = recv(s, ibufOneB, 50 , 0);
      transId++;
      
      * moveTask1 = transId;
      * moveTask9 = htonl(1000); /* starting task */                 
      FD_ZERO(&fds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;
    
      conn = select(32, NULL, &fds, NULL, &tv);
      conn = send(s, obufMT, 17, 0);
      printf("Message Sent! - start task - small\n");
      FD_SET(s, &fds);
      conn = select(32, &fds, NULL, NULL, &tv);
      conn = recv(s, ibufMT, 50 , 0);
      transId++;

      * drvSave1 = transId;
      FD_ZERO(&fds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;
    
      conn = select(32, NULL, &fds, NULL, &tv);
      conn = send(s, obufDS, 17, 0);
      printf("Message Sent! - save to drive - small\n");
      FD_SET(s, &fds);
      conn = select(32, &fds, NULL, NULL, &tv);
      conn = recv(s, ibufDS, 50 , 0);
      transId++;

      fprintf(fp_can_size, "%d\n", 0);
    }
    else if(selected[1])
    {
      int * clear1 =  (int*)(&obufCl[0]);    
      int * clear9 =  (int*)(&obufCl[16]);
      int * posOneB1 = (int*)(&obufOneB[0]);
      int * posOneB9 = (int*)(&obufOneB[16]);
      int * posOneB10 = (int*)(&obufOneB[17]);
      int * moveTask1 =  (int*)(&obufMT[0]);
      int * moveTask9 =  (int*)(&obufMT[13]);
      int * drvSave1 = (int*)(&obufDS[0]);
     
      * clear1 = transId;           
      * clear9 = 4;           
   
      FD_ZERO(&fds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;
    
      conn = select(32, NULL, &fds, NULL, &tv);
      conn = send(s, obufCl, 17, 0);
      printf("Message Sent! - clear position - medium\n");
      FD_SET(s, &fds);
      conn = select(32, &fds, NULL, NULL, &tv);
      conn = recv(s, ibufCl, 50 , 0);
      transId++;
     
      * posOneB1 = transId; 
      * posOneB9 = 4;
      * posOneB10 = htonl((AKD_frame_posMedium+modifier)*1000); 
      FD_ZERO(&fds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;
    
      conn = select(32, NULL, &fds, NULL, &tv);
      conn = send(s, obufOneB, 53, 0);
      printf("Message Sent! - position parameter - medium\n");
      FD_SET(s, &fds);
      conn = select(32, &fds, NULL, NULL, &tv);
      conn = recv(s, ibufOneB, 50 , 0);
      transId++;
     
      * moveTask1 = transId;
      * moveTask9 = htonl(3000);
      FD_ZERO(&fds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;
    
      conn = select(32, NULL, &fds, NULL, &tv);
      conn = send(s, obufMT, 17, 0);
      printf("Message Sent! - start task - medium\n");
      FD_SET(s, &fds);
      conn = select(32, &fds, NULL, NULL, &tv);
      conn = recv(s, ibufMT, 50 , 0);
      transId++;
 
      * drvSave1 = transId;
      FD_ZERO(&fds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;
    
      conn = select(32, NULL, &fds, NULL, &tv);
      conn = send(s, obufDS, 17, 0);
      printf("Message Sent! - save to drive - medium\n");
      FD_SET(s, &fds);
      conn = select(32, &fds, NULL, NULL, &tv);
      conn = recv(s, ibufDS, 50 , 0);
      transId++;

      fprintf(fp_can_size, "%d\n", 1);   
    }  
    else if(selected[2])
    { 
      int * clear1 =  (int*)(&obufCl[0]);
      int * clear9 =  (int*)(&obufCl[16]);
      int * posOneB1 = (int*)(&obufOneB[0]);    
      int * posOneB9 = (int*)(&obufOneB[16]);    
      int * posOneB10 = (int*)(&obufOneB[17]);    
      int * moveTask1 =  (int*)(&obufMT[0]);
      int * moveTask9 =  (int*)(&obufMT[13]);
      int * drvSave1 = (int*)(&obufDS[0]);
      
      * clear1 = transId;           
      * clear9 = 6;           

      FD_ZERO(&fds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;
    
      conn = select(32, NULL, &fds, NULL, &tv);
      conn = send(s, obufCl, 17, 0);
      printf("Message Sent! - clear position - big\n");
      FD_SET(s, &fds);
      conn = select(32, &fds, NULL, NULL, &tv);
      conn = recv(s, ibufCl, 50 , 0);
      transId++;
      
      * posOneB1 = transId;  
      * posOneB9 = 6;  
      * posOneB10 = htonl((AKD_frame_posBig+modifier)*1000);  
      FD_ZERO(&fds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;
    
      conn = select(32, NULL, &fds, NULL, &tv);
      conn = send(s, obufOneB, 53, 0);
      printf("Message sent! - position parameter - big\n");
      FD_SET(s, &fds);
      conn = select(32, &fds, NULL, NULL, &tv);
      conn = recv(s, ibufOneB, 50 , 0);
      transId++;
      
      * moveTask1 = transId;
      * moveTask9 = htonl(5000);
      FD_ZERO(&fds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;
    
      conn = select(32, NULL, &fds, NULL, &tv);
      conn = send(s, obufMT, 17, 0);
      printf("Message Sent! - start task -big\n");
      FD_SET(s, &fds);
      conn = select(32, &fds, NULL, NULL, &tv);
      conn = recv(s, ibufMT, 50 , 0);
      transId++;

      * drvSave1 = transId;
      FD_ZERO(&fds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;
    
      conn = select(32, NULL, &fds, NULL, &tv);
      conn = send(s, obufDS, 17, 0);
      printf("Message Sent! - save to drive - big\n");
      FD_SET(s, &fds);
      conn = select(32, &fds, NULL, NULL, &tv);
      conn = recv(s, ibufDS, 50 , 0);
      transId++;

      fprintf(fp_can_size, "%d\n", 2);
    }
    fclose(fp_can_size);

  }
  renderText(text, smallText,  blackColor);
  render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 
}

void renderAdmin(int x, int y, int w, int h, int curr_page, int gotoNum) 
{
  int i;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  for(i = 0; i < 5; ++i)
  {
    SDL_RenderDrawLine(renderer, x, y+i, (x+w), y+i);
    SDL_RenderDrawLine(renderer, (x+w+i), y, (x+w+i), (y+h)); 
  
    SDL_RenderDrawLine(renderer, x+i, (y+h), x+i, y);
  }
  renderText("...", regularText, blackColor);
  render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2+10)), NULL, 0.0, NULL, SDL_FLIP_NONE); 

  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp && cycleCounter != cycleCheck)
  {
    cycleCheck = cycleCounter;
    page = gotoNum;
    page_stage[curr_page] = 2;
  }
}

void up_button(int x,  int y, int *incrementee, int incrementor, int max)   
{
  SDL_Surface *imageSurface;
  freeTexture();
  #ifdef RPI
  imageSurface = IMG_Load("/home/pi/TKK_PRESA/images/up_black.png");
  #endif
  #ifdef LUKA
  imageSurface = IMG_Load("/home/luka/TKK_PRESA_/images/up_black.png");
  #endif
  if(imageSurface == NULL)
  {
    printf("Unable to render image surface! SDL_ImageError: %s\n", IMG_GetError());
  }
  else
  {
    texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if(texture == NULL)
    {
      printf("Unable to create texture from rendered image! SDL_ImageError: %s\n", SDL_GetError());
    }
    else
    {
      textureWidth = imageSurface -> w;
      textureHeight = imageSurface -> h;
    }
    SDL_FreeSurface(imageSurface);
  }

  render(x, y, NULL, 0.0, NULL, SDL_FLIP_NONE);

  if(touchLocation.x > x && touchLocation.x < x+imageSurface->w && touchLocation.y > y && touchLocation.y < y + imageSurface->h && timestamp > oldtimestamp)
  {
    if(*incrementee < max)
    {
      *incrementee = *incrementee + incrementor;
    }
  }
}

void down_button(int x, int y, int *decrementee, int decrementor, int min)     
{
  SDL_Surface *imageSurface;
  freeTexture();
  #ifdef RPI
  imageSurface = IMG_Load("/home/pi/TKK_PRESA/images/down_black.png");
  #endif
  #ifdef LUKA
  imageSurface = IMG_Load("/home/luka/TKK_PRESA_/images/down_black.png");
  #endif
  if(imageSurface == NULL)
  {
    printf("Unable to render image surface! SDL_ImageError: %s\n", IMG_GetError());
  }
  else
  {
    texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if(texture == NULL)
    {
      printf("Unable to create texture from rendered image! SDL_ImageError: %s\n", SDL_GetError());
    }
    else
    {
      textureWidth = imageSurface -> w;
      textureHeight = imageSurface -> h;
    }
    SDL_FreeSurface(imageSurface);
  }	
  render(x, y, NULL, 0.0, NULL, SDL_FLIP_NONE);

  if(touchLocation.x > x && touchLocation.x < x+imageSurface->w && touchLocation.y > y && touchLocation.y < y + imageSurface->h && timestamp > oldtimestamp)
  {
    if(*decrementee > min)
    {
      *decrementee = *decrementee - decrementor;
    }
  }
}

void left_button(int x,  int y)   
{
  SDL_Surface *imageSurface;
  freeTexture();
  #ifdef RPI
  imageSurface = IMG_Load("/home/pi/TKK_PRESA/images/left_200.png");
  #endif
  #ifdef LUKA
  imageSurface = IMG_Load("/home/luka/TKK_PRESA_/images/left_200.png");
  #endif
  
  if(imageSurface == NULL)
  {
    printf("Unable to render image surface! SDL_ImageError: %s\n", IMG_GetError());
  }
  else
  {
    texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if(texture == NULL)
    {
      printf("Unable to create texture from rendered image! SDL_ImageError: %s\n", SDL_GetError());
    }
    else
    {
      textureWidth = imageSurface -> w;
      textureHeight = imageSurface -> h;
    }
    SDL_FreeSurface(imageSurface);
  }

  render(x, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
  
  if(touchLocation.x > x && touchLocation.x < x+imageSurface->w && touchLocation.y > y && touchLocation.y < y + imageSurface->h && timestamp > oldtimestamp)
  {
    ;
  }
}

void right_button(int x,  int y)   
{
  SDL_Surface *imageSurface;
  freeTexture();
  #ifdef RPI
  imageSurface = IMG_Load("/home/pi/TKK_PRESA/images/right_200.png");
  #endif
  #ifdef LUKA
  imageSurface = IMG_Load("/home/luka/TKK_PRESA_/images/right_200.png");
  #endif
  
  if(imageSurface == NULL)
  {
    printf("Unable to render image surface! SDL_ImageError: %s\n", IMG_GetError());
  }
  else
  {
    texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if(texture == NULL)
    {
      printf("Unable to create texture from rendered image! SDL_ImageError: %s\n", SDL_GetError());
    }
    else
    {
      textureWidth = imageSurface -> w;
      textureHeight = imageSurface -> h;
    }
    SDL_FreeSurface(imageSurface);
  }

  render(x, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
  
  if(touchLocation.x > x && touchLocation.x < x+imageSurface->w && touchLocation.y > y && touchLocation.y < y + imageSurface->h && timestamp > oldtimestamp)
  {
    ;
  }
}

void button(int x, int y, int w, int h, char *text, int id)  
{
  int i;
  if(selected[id]==0)
  {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderDrawLine(renderer, x, y, (x+w), y);
    SDL_RenderDrawLine(renderer, (x+w), y, (x+w), (y+h)); 
    SDL_RenderDrawLine(renderer, (x+w), (y+h), x, (y+h));
    SDL_RenderDrawLine(renderer, x, (y+h), x, y);
    renderText(text, smallText,  blackColor);
    render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 
  }
  else if(selected[id]==1)
  {
    for(i = 0; i < 10; i++)
    {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderDrawLine(renderer, x, y+i, (x+w), y+i);
      SDL_RenderDrawLine(renderer, (x+w+i), y, (x+w+i), (y+h)); 
      SDL_RenderDrawLine(renderer, (x+w), (y+h-i), x, (y+h-i));
      SDL_RenderDrawLine(renderer, x+i, (y+h), x+i, y);
    }
    renderText(text, smallText,  blackColor);
    render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE);
  }
  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp && selected[id] == 0)
  {
    selected[0] = 0;
    selected[1] = 0;
    selected[2] = 0;
    selected[id] = 1;
  }
}

void goToButton(int x, int y, int w, int h, char *text, int curr_page, int goToNum)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderDrawLine(renderer, x, y, (x+w), y);
  SDL_RenderDrawLine(renderer, (x+w), y, (x+w), (y+h)); 
  SDL_RenderDrawLine(renderer, (x+w), (y+h), x, (y+h));
  SDL_RenderDrawLine(renderer, x, (y+h), x, y);
  renderText(text, regularText,  blackColor);
  render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 
 
  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
  {
    cycleCheck = cycleCounter;
    page = goToNum;
    page_stage[curr_page] = 2;
  }
}


void saveTime(int x, int y, int w, int h, char *text)
{
  renderText(text, smallText,  blackColor);
  render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE);
  
  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
  {
    sprintf(setTBuff, "echo ine36dun2f1 | sudo -S timedatectl set-time '%04d-%02d-%02d %02d:%02d:00'", year, month, day, hour, minute);
    system("echo ine36dun2f1 | sudo -S timedatectl set-ntp 0");
    system(setTBuff);
    system("echo ine36dun2f1 | sudo -S timedatectl set-ntp 1");
  } 
}


void clockButton(int x, int y, int h, int w, char *tmBuff)
{

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  sprintf(tmBuff, "%02d:%02d", tm.tm_hour, tm.tm_min);
  renderText(tmBuff, smallText, blackColor);
  render(1050, 23, NULL, 0.0, NULL, SDL_FLIP_NONE);

  
  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
  {
    page = 5;
    page_stage[page] = 0;
  }
}


void outputButton(int x, int y, int w, int h, int id)
{
  sprintf(buff_outputs[id], "O_%d: %d",id, buff_outputs_val[id]);
  renderText(buff_outputs[id], regularText, blackColor);
  render(x, y, NULL, 0.0, NULL, SDL_FLIP_NONE);


  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
  {
    if(buff_outputs_val[id] == 0)
    {
      buff_outputs_val[id] = 1;
    }
    else
    {
      buff_outputs_val[id] = 0;
    }
  }
}


void start_button(int x, int y, int w, int h)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderDrawLine(renderer, x, y, (x+w), y);
  SDL_RenderDrawLine(renderer, (x+w), y, (x+w), (y+h)); 
  SDL_RenderDrawLine(renderer, (x+w), (y+h), x, (y+h));
  SDL_RenderDrawLine(renderer, x, (y+h), x, y);

  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
  {
    if(step == 0)
    {
      step = 1;  
    } 
  }
  renderText("START", regularText, blackColor);
  render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 

}

void stop_button(int x, int y, int w, int h)
{
   SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
   SDL_RenderDrawLine(renderer, x, y, (x+w), y);
   SDL_RenderDrawLine(renderer, (x+w), y, (x+w), (y+h)); 
   SDL_RenderDrawLine(renderer, (x+w), (y+h), x, (y+h));
   SDL_RenderDrawLine(renderer, x, (y+h), x, y);

  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
  {
    step = 0;
  }
  renderText("STOP", regularText, blackColor);
  render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 

}

void continue_button(int x, int y, int w, int h, int stepMax)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderDrawLine(renderer, x, y, (x+w), y);
  SDL_RenderDrawLine(renderer, (x+w), y, (x+w), (y+h)); 
  SDL_RenderDrawLine(renderer, (x+w), (y+h), x, (y+h));
  SDL_RenderDrawLine(renderer, x, (y+h), x, y);

  if(touchLocation.x > x && touchLocation.x < x+w && touchLocation.y > y && touchLocation.y < y + h && timestamp > oldtimestamp)
  {
    if(step > 0 && step < stepMax)
    {
      step = step + 1;
    }
  }
  renderText("NADALJUJ", regularText, blackColor);
  render(x+((w/2)-(textureWidth/2)), y + ((h/2)-(textureHeight/2)), NULL, 0.0, NULL, SDL_FLIP_NONE); 
}

