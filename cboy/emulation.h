#ifndef EMULATION_H
#define EMULATION_H

#include "cJSON.h"
#include <stdbool.h>
#include <stdint.h>

union Register {
  uint16_t full;
  uint8_t half[2];
};

struct Registers {
  uint8_t A;
  uint8_t F;
  union Register BC;
  union Register DE;
  union Register HL;
  uint16_t SP;
  uint16_t PC;

};

struct MemoryBus {
  uint8_t *memory;
};

struct CPU {
  struct Registers registers;
  struct MemoryBus bus;
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
    bool *immediate;
    uint8_t *bytes;
    uint8_t *increment;
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



enum Conditions { Z, NZ, C, NC, EMPTY};
struct DictionaryData {
  char *key;
  void *val;
};

struct Dictionary {
 uint32_t size;
 void *data;
};


/*void log_opcode(const struct Opcode *opcode);*/
/*void log_instruction(const struct Instruction *instruction);*/
/*uint8_t get_opcode(struct File *rom, struct Opcode *opcode, uint32_t *read_index);*/
/*int serialize_instruction(struct Instruction *instruction, cJSON *json);*/
/*void jump(struct CPU *cpu, const enum Conditions *condition, const uint16_t n16);*/
int cpu_step(struct CPU *cpu, cJSON *json, struct Dictionary *operads_lookup_table);


#endif


