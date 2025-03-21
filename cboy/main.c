#include "cJSON.h"
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

struct CPU {
  uint16_t cycleCount;
  uint8_t instruction;
  struct Registers {
    uint8_t A;
    uint8_t F;
    uint16_t BC;
    uint16_t DE;
    uint16_t HL;
    uint16_t SP;
    uint16_t PC;
  } registers;
};

struct RAM {
  uint16_t size;
  void *buffer;
};

struct File {
  void *data;
  uint32_t size;
};

struct Instruction {
  char *mnemonic;
  bool *immediate;
  uint8_t *cycles;
  uint8_t bytes;
  uint8_t cycle_count;
  uint8_t operand_count;
  struct Operand {
    char *name;
    bool immediate;
    uint8_t bytes;
  } *operands;
  struct Flags {
    char *Z;
    char *N;
    char *H;
    char *C;
  } *flags;
  struct Opcode {
    bool prefixed;
    uint8_t val;
    char *id;
  } *opcode;
};

uint8_t get_opcode(struct File *rom, struct Opcode *opcode,
                   uint32_t *read_index) {
  char *opcode_str = malloc(4);
  if (opcode_str == NULL) {
    printf("malloc error");
    return -2;
  }
  opcode_str[0] = '0';
  opcode_str[1] = 'x';

  char byte = (char)((char *)rom->data + *read_index)[0];

  char *t = malloc(1);
  if (t == NULL) {
    printf("malloc error");
    return -3;
  }
  sprintf(t, "%02X", (uint8_t)byte);
  strncat(opcode_str, t, 2);
  opcode->val = byte;
  opcode->id = opcode_str;
  free(t);

  if (opcode->val == 0xCB) {
    opcode->prefixed = true;
    uint8_t err = get_opcode(rom, opcode, read_index);
    if (err > 0) {
      printf("prefix opcode error");
      return -4;
    }
  }

  return 0;
}

int serialize_instruction(struct Instruction *instruction, cJSON *json) {
  cJSON *bytes = cJSON_GetObjectItem(json, "bytes");
  instruction->bytes = cJSON_GetNumberValue(bytes);

  cJSON *cycles = cJSON_GetObjectItem(json, "cycles");
  uint8_t cycle_count = cJSON_GetArraySize(cycles);

  instruction->cycles = malloc(sizeof(uint8_t) * cycle_count);
  instruction->cycle_count = cycle_count;

  for (int i = 0; i < cycle_count; i++) {
    uint8_t cycle = cJSON_GetNumberValue(cycles->child);
    instruction->cycles[i] = cycle;
    cycles = cycles->next;
  }

  cJSON *mnemonic = cJSON_GetObjectItem(json, "mnemonic");
  instruction->mnemonic = cJSON_GetStringValue(mnemonic);

  cJSON *immediate = cJSON_GetObjectItem(json, "immediate");
  if (immediate != NULL) {
    bool imme = cJSON_GetNumberValue(immediate);
    instruction->immediate = &imme;
  }

  cJSON *operands = cJSON_GetObjectItem(json, "operand");
  if (operands != NULL) {
    uint8_t operand_count = cJSON_GetArraySize(operands);
    instruction->operands = malloc(sizeof(struct Operand) * operand_count);
    instruction->operand_count = operand_count;

    for (int i = 0; i < cycle_count; i++) {
      cJSON *j_name = cJSON_GetObjectItem(operands, "name");
      char *name = "";
      if (j_name != NULL) {
        name = cJSON_GetStringValue(j_name);
      }

      cJSON *j_immediate = cJSON_GetObjectItem(operands, "immediate");
      assert(j_immediate != NULL);
      bool immediate = cJSON_GetNumberValue(j_immediate);
      cJSON *j_bytes = cJSON_GetObjectItem(operands, "bytes");
      u_int8_t bytes = 0;
      if (j_bytes == NULL) {
        bytes = cJSON_GetNumberValue(j_immediate);
      }

      struct Operand operand = {name, immediate, bytes};

      instruction->operands[i] = operand;
      operands = operands->next;
    };
  }

  cJSON *flags = cJSON_GetObjectItem(json, "flags");
  if (flags == NULL) {
    return 0;
  }

  instruction->flags = malloc(sizeof(struct Flags));

  cJSON *Z = cJSON_GetObjectItem(flags, "Z");
  cJSON *N = cJSON_GetObjectItem(flags, "N");
  cJSON *H = cJSON_GetObjectItem(flags, "H");
  cJSON *C = cJSON_GetObjectItem(flags, "C");

  instruction->flags->Z = cJSON_GetStringValue(Z);
  instruction->flags->N = cJSON_GetStringValue(N);
  instruction->flags->H = cJSON_GetStringValue(H);
  instruction->flags->C = cJSON_GetStringValue(C);

  assert(instruction->flags->Z != NULL);
  assert(instruction->flags->N != NULL);
  assert(instruction->flags->H != NULL);
  assert(instruction->flags->Z != NULL);

  return 0;
}

void log_opcode(const struct Opcode *opcode) {
  SDL_Log("Opcode hex: %X \n", opcode->val);
  SDL_Log("Opcode id: %s \n", opcode->id);
  SDL_Log("Opcade prefixed: %b \n", opcode->prefixed);
}

void log_instruction(const struct Instruction *instruction) {
  if (instruction->immediate != NULL) {
    SDL_Log("Instruction immediate: %b", *instruction->immediate);
  }
  SDL_Log("Instruction mnemoni: %s", instruction->mnemonic);
  SDL_Log("Instruction bytes: %u", instruction->bytes);
  SDL_Log("Instruction cycle count: %u", instruction->cycle_count);
  for (uint8_t i = 0; i < instruction->cycle_count; i++) {
    SDL_Log("Instruction cycle %u: %u", i + 1, instruction->cycles[i]);
  }

  SDL_Log("Instruction operand count: %u", instruction->operand_count);
  for (uint8_t i = 0; i < instruction->operand_count; i++) {
    struct Operand operand = instruction->operands[i];
    SDL_Log("Operand bytes: %u", operand.bytes);
    SDL_Log("Operand immediate: %b", operand.immediate);
    SDL_Log("Operand immediate: %s", operand.name);
  }

  if (instruction->flags != NULL) {
    SDL_Log("Instruction flag H: %x", instruction->flags->H[0]);
    SDL_Log("Instruction flag C: %x", instruction->flags->C[0]);
    SDL_Log("Instruction flag N: %x", instruction->flags->N[0]);
    SDL_Log("Instruction flag Z: %x", instruction->flags->Z[0]);
  }

  log_opcode(instruction->opcode);
}

enum Conditions { Z, NZ, C, NC, NOT };

void jump(struct CPU *cpu, const enum Conditions *condition,
          const uint16_t n16) {
  if (condition == NULL) {
    cpu->registers.PC += n16;
    return;
  }
}

int run_instruction(struct CPU *cpu, cJSON *json, struct File *rom,
                    uint32_t *read_index) {
  struct Opcode opcode = {false, 0, NULL};

  uint8_t err = get_opcode(rom, &opcode, read_index);
  if (err > 0) {
    SDL_Log("opcode error");
    return -4;
  }

  if (opcode.prefixed == true) {
    json = cJSON_GetObjectItem(json, "cbprefixed");
  } else {
    json = cJSON_GetObjectItem(json, "unprefixed");
  }

  json = cJSON_GetObjectItem(json, opcode.id);

  struct Instruction instruction = {NULL, NULL, NULL, 0,      0,
                                    0,    NULL, NULL, &opcode};

  err = serialize_instruction(&instruction, json);
  if (err > 0) {
    SDL_Log("Serialize error");
  }

  log_instruction(&instruction);
  SDL_Log("###################### \n");

  switch (opcode.val) {
  case 0xC3:
    // JP NZ, a16
    break;

  default:
    break;
  }

  *read_index += instruction.bytes;

  return 0;
}

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
  struct CPU cpu = {0, 0, {0, 0, 0, 0, 0, 0, 0}};
  printf("%s \n", argv[1]);

  struct File file = read_file(argv[1]);
  if (file.data == NULL) {
    free(file.data);
    printf("Error with reading *.gb");

    return -5;
  }

  const struct File opcode = read_file("opcodes.json");
  if (opcode.data == NULL) {
    printf("Error with opcodes");
    free(file.data);
    free(opcode.data);
    return -6;
  }

  cJSON *json = cJSON_ParseWithLength((char *)opcode.data, opcode.size);
  if (json == NULL) {
    printf("Error with json");
    free(file.data);
    return -7;
  }

  SDL_Log("JSON loaded");

  int t = 0;
  while (t < 10) {
    run_instruction(&cpu, json, &file, &read_index);
    t++;
  }

  free(file.data);
  free(opcode.data);

  return 0;
}
