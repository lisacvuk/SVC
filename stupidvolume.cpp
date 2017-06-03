#include <SDL2/SDL.h>
#include <alsa/asoundlib.h>
#include <iostream>
#include <iomanip>

#define SPEED 140.0f

bool running = true;

void loop(SDL_Renderer* renderer);
void input();
void checkPosition();

bool direction = true;

SDL_Rect rect1 = {90,50,20,200};
SDL_Rect rect2 = {90,250,20,-1};

SDL_Rect rect3 = {0, 300, 200, 20};
SDL_Rect rect5 = {90, 300, 20, 20};
SDL_Rect rect4 = {0, 295, 8, 30};

float NOW = SDL_GetTicks();
float LAST = 0.0f;
double deltaTime = 0.0f;

float rect4x = 0.0f;

void SetAlsaMasterVolume(long volume)
{
    long min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "Master";

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);

    snd_mixer_close(handle);
}

int main(int argc, char* argv[]){
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window* window = SDL_CreateWindow("Stupid Volume Control (SVC)", 0, 0, 200, 350, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  while(running){
    loop(renderer);
  }
  return 0;
}
void input(SDL_Event e){
  if(SDL_PollEvent(&e)){
    if(e.type == SDL_QUIT){
      running = false;
    }
    if(e.type == SDL_KEYDOWN){
      if(e.key.keysym.sym == SDLK_SPACE){
        checkPosition();
      }
    }
  }
}
void checkPosition(){
  if(rect4.x+4 > 90 && rect4.x+4 < 110){
    rect2.h = rect2.h - 10;
    if(rect2.h < -200){
      rect2.h = -200;
    }
  }
  else{
    rect2.h = -1;
  }
  SetAlsaMasterVolume(-(rect2.h)/200.0f*100.0f);
}
void loop(SDL_Renderer* renderer){
  LAST = NOW;
  NOW = SDL_GetTicks();
  deltaTime = (double)((NOW - LAST)/1000.0f);
  SDL_Event e;
  input(e);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderDrawRect(renderer, &rect1);
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderFillRect(renderer, &rect2);
  SDL_RenderFillRect(renderer, &rect3);
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  if(direction == true){
    rect4x = (double)(rect4x + SPEED*deltaTime);
  }else{
    rect4x = (double)(rect4x - SPEED*deltaTime);
  }
  if(rect4x + rect4.w > rect3.w){
    direction = false;
  }
  if(rect4x < rect3.x){
    direction = true;
  }
  rect4.x = rect4x;
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, &rect5);
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderFillRect(renderer, &rect4);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderPresent(renderer);
}
