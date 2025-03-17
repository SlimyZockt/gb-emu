#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CLOCK_SPEED 4194304
#define WRAM 8192
#define VRAM 8192
#define SCREEN_X 160
#define SCREEN_Y 160

struct CPU {
  uint16_t cycleCount;
  uint8_t instruction;
  struct Reqisters {
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
  long size;
};

enum Instruction {
  NOOP = 0x0,
  STOP = 0x1,
  JR_NZ_e8 = 0x2,
  JR_NC_e8 = 0x3,
  LD_B_B = 0x4,
  LD_D_B = 0x5,
};

struct InstructionData {
  uint8_t size;
  uint8_t t_states;
  void *data;
  enum Instruction type;
};

struct InstructionData read_instruction(struct File *file,
                                        uint32_t *read_index) {
  char *data = file->data;

  while (true) {
    uint8_t high = (uint8_t)data[*read_index] >> 4;
    uint8_t low = (uint8_t)data[*read_index] & 0x0F;
    printf("%X", high);
    printf("%X", low);
    /*for (int j = 7; j >= 0; j--) {*/
    /*  printf("%d", (data[i] >> j) & 1);*/
    /*}*/
  }

  struct InstructionData instruction = {0, 0, malloc(sizeof(uint16_t))};

  return instruction;
};

void do_instruction(struct CPU *cpu) {
  switch (cpu->instruction) {}
}

void run_cpu(struct CPU *cpu, struct File *file) {

  u_int8_t *instructions = malloc(WRAM);
  uint32_t read_index = 0;

  while (false) {
    read_instruction(file, &read_index);
    do_instruction(cpu);

    sleep(1 / CLOCK_SPEED);
    cpu->cycleCount += 1;
  }
}

struct File read_rom(char *path) {
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    printf("File not found.");
    exit(EXIT_FAILURE);
  }
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  void *data = calloc(1, file_size);
  fread(data, 1, file_size, file);
  fclose(file);
  struct File f = {data, file_size};
  return f;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Worng Argument\n");
    return 1;
  }

  printf("%s \n", argv[1]);
  struct CPU cpu = {0, 0, {0, 0, 0, 0, 0, 0, 0}};
  struct File file = read_rom(argv[1]);
  run_cpu(&cpu, &file);
  return 0;
}
