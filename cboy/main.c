#include "cJSON.h"
#include "emulation.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define CLOCK_SPEED 4194304
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#define WRAM 8192
#define VRAM 8192
#define SCREEN_X 160
#define SCREEN_Y 160

struct File read_file(const char *path) {
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    printf("File not found.");
    exit(EXIT_FAILURE);
  }
  fseek(file, 0, SEEK_END);
  uint32_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  void *data = calloc(1, file_size);
  fread(data, 1, file_size, file);
  fclose(file);
  const struct File f = {data, file_size};
  return f;
}

int run_sdl() {
  SDL_Window *window = NULL;
  SDL_Renderer *renerer = NULL;

  int res = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  if (res < 0) {
    SDL_Log("SLD_INIT error: %s", SDL_GetError());
    return -1;
  }

  window = SDL_CreateWindow("cboy", 592, 390, 0);
  if (window == NULL) {
    SDL_Log("SDL_CreateWindow: %s", SDL_GetError());
    return -2;
  }

  renerer = SDL_CreateRenderer(window, NULL);
  if (window == NULL) {
    SDL_Log("SDL_CreateRenderer: %s", SDL_GetError());
    return -3;
  }

  SDL_Log("SDL3 init");

  SDL_Event event;
  bool quit = 0;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_QUIT:
        SDL_Log("SDL3 event quit");
        quit = true;
        break;
      default:
        break;
      }

      SDL_Delay(1);
      /*cpu.cycleCount += 1;*/

      /*SDL_SetRenderDrawColor(renerer, 0, 0, 0, 0xFF);*/
      /*SDL_RenderClear(renerer);*/
      /**/
      /*SDL_RenderPresent(renerer);*/
    }
  }

  SDL_Log("SDL3 shutdown");

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renerer);
  SDL_Quit();

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Worng Argument\n");
    return 1;
  }

  uint32_t read_index = 0;
  struct CPU cpu = {{0, 0, 0, 0, 0, 0, 0}, {malloc(0xFFFF * 8)}};
  SDL_Log("%s \n", argv[1]);

  struct File rom = read_file(argv[1]);
  if (rom.data == NULL) {
    free(rom.data);
    printf("Error with reading *.gb");

    return -5;
  }

  SDL_Log("rom size: %d", rom.size);
  assert(rom.size <= (0x3FFF) * 8);
  memcpy(cpu.bus.memory, rom.data, rom.size);
  free(rom.data);

  const struct File opcode = read_file("opcodes.json");
  if (opcode.data == NULL) {
    printf("Error with opcodes");
    free(opcode.data);
    return -6;
  }

  cJSON *json = cJSON_ParseWithLength((char *)opcode.data, opcode.size);
  if (json == NULL) {
    printf("Error with json");
    free(rom.data);
    return -7;
  }

  struct DictionaryData data[] = {
      {"A", &cpu.registers.A},          {"B", &cpu.registers.BC.half[0]},
      {"C", &cpu.registers.BC.half[1]}, {"D", &cpu.registers.DE.half[0]},
      {"E", &cpu.registers.DE.half[1]}, {"H", &cpu.registers.HL.half[0]},
      {"B", &cpu.registers.HL.half[1]}};
  struct Dictionary operads_lookup = {6, data};
  SDL_Log("JSON loaded");

  int t = 0;
  while (t < 10) {
    cpu_step(&cpu, json, &operads_lookup);
    t++;
  }

  free(opcode.data);
  free(cpu.bus.memory);

  return 0;
}
