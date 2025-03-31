#include "SDL3/SDL_log.h"
#include <assert.h>
#include <cJSON.h>
#include <emulation.h>
#include <stdbool.h>
#include <stdlib.h>

void jump(const enum Conditions condition, struct CPU *cpu,
          const uint16_t n16) {
  if (condition == EMPTY) {
    cpu->registers.PC += n16;
    return;
  }
}

void push(struct CPU *cpu, uint16_t *val) {
  cpu->registers.SP += 0x02;
  cpu->bus.memory[cpu->registers.SP] = *val;
}

void rst(struct CPU *cpu, uint8_t id) {
  push(cpu, &cpu->registers.PC);
  SDL_Log("%d", id);
}

void load_r8(uint8_t *target, uint8_t val) { *target = val; }

void load(void *target, void *val) { *(uint16_t *)target = *(uint16_t *)val; }

void get_bytes(uint8_t *src, uint32_t offset, uint32_t x, uint8_t *dest) {
  if (offset + x > 0xFFFF * 8) {
    SDL_Log("Error: Requested range is out of bounds.\n");
    exit(EXIT_FAILURE);
  }
  memcpy(dest, src + offset, x);
}

uint8_t *get_r8(struct CPU *cpu, char *r8_name) {
  if (strncmp(r8_name, "A", 1) == 0) {
    return &cpu->registers.A;
  }
  if (strncmp(r8_name, "B", 1) == 0) {
    return &cpu->registers.BC.half[0];
  }
  if (strncmp(r8_name, "C", 1) == 0) {
    return &cpu->registers.BC.half[1];
  }
  if (strncmp(r8_name, "D", 1) == 0) {
    return &cpu->registers.DE.half[0];
  }
  if (strncmp(r8_name, "E", 1) == 0) {
    return &cpu->registers.DE.half[1];
  }
  if (strncmp(r8_name, "H", 1) == 0) {
    return &cpu->registers.HL.half[0];
  }
  if (strncmp(r8_name, "L", 1) == 0) {
    return &cpu->registers.HL.half[1];
  }

  // static_assert(true, "No r8 found");
  return NULL;
}

uint16_t *get_r16(struct CPU *cpu, char *r16_name) {
  if (strncmp(r16_name, "BC", 1) == 0) {
    return &cpu->registers.BC.full;
  }

  if (strncmp(r16_name, "DE", 1) == 0) {
    return &cpu->registers.DE.full;
  }

  if (strncmp(r16_name, "HL", 1) == 0) {
    return &cpu->registers.HL.full;
  }

  // static_assert(true, "No r16 found");
  return NULL;
}

void ld(struct Instruction *instruction, struct CPU *cpu) {
  assert(instruction->operand_count == 2);
  struct Operand operand1 = instruction->operands[0];
  struct Operand operand2 = instruction->operands[1];

  if (operand1.increment != NULL) {
    if (operand1.type == R_HL && operand2.type == R_A) {
      load(&cpu->registers.HL.full, &cpu->registers.A);
      cpu->registers.HL.full += *instruction->operands->increment;
      return;
    }
    assert(operand1.type == R_HL && operand2.type == R_A);
    load(&cpu->registers.A, &cpu->registers.HL.full);
    cpu->registers.HL.full += *instruction->operands->increment;
    return;
  }

  if (operand2.increment != NULL) {
    load(&cpu->registers.HL.full, &cpu->registers.A);
    cpu->registers.HL.full += *instruction->operands->increment;
    return;
  }

  if (operand1.type == r8 && operand2.type == r8) {
    load(get_r8(cpu, operand1.name), get_r8(cpu, operand2.name));
    return;
  }

  if (operand1.type == r8 && operand2.type == n8) {
    assert(operand2.bytes != NULL);
    load(get_r8(cpu, operand1.name),
         &cpu->bus.memory[cpu->registers.PC + *operand2.bytes]);
    return;
  }

  if (operand1.type == r16 && operand2.type == n16) {
    assert(operand2.bytes != NULL);
    load(get_r16(cpu, operand1.name),
         &cpu->bus.memory[cpu->registers.PC + *operand2.bytes]);
    return;
  }

  if (operand1.type == R_HL && operand2.type == r8) {
    load(&cpu->registers.HL.full, get_r8(cpu, operand2.name));
    return;
  }

  if (operand1.type == R_HL && operand2.type == n8) {
    assert(operand2.bytes != NULL);
    load(&cpu->registers.HL.full,
         &cpu->bus.memory[cpu->registers.PC + *operand2.bytes]);
    return;
  }

  if (operand1.type == r8 && operand2.type == R_HL) {
    assert(operand2.bytes != NULL);
    load(&cpu->bus.memory[cpu->registers.PC + *operand2.bytes],
         &cpu->registers.HL.full);
    return;
  }

  if (operand1.type == r16 && operand2.type == R_A) {
    load(get_r16(cpu, operand1.name), &cpu->registers.A);
    return;
  }

  if (operand1.type == n16 && operand2.type == R_A) {
    assert(operand1.bytes != NULL);
    load(&cpu->bus.memory[cpu->registers.PC + *operand1.bytes],
         &cpu->registers.A);
    return;
  }

  if (operand1.type == R_A && operand2.type == n16) {
    assert(operand1.bytes != NULL);
    load(&cpu->registers.A,
         &cpu->bus.memory[cpu->registers.PC + *operand1.bytes]);
    return;
  }

  assert(operand1.type == R_A && operand2.type == r16);
  load(&cpu->registers.A, get_r16(cpu, operand1.name));
  // }
}

void ldh(struct Instruction *instruction, struct CPU *cpu) {
  assert(instruction->operand_count == 2);
  struct Operand operand1 = instruction->operands[0];
  struct Operand operand2 = instruction->operands[1];

  if (operand1.type == n16 && operand2.type == R_A) {
    load(&cpu->bus.memory[8 * 0xFF00 + (8 * cpu->bus.memory[cpu->registers.PC +
                                                            *operand1.bytes])],
         &cpu->registers.A);
    return;
  }
  if (operand1.type == R_C && operand2.type == R_A) {
    load(&cpu->bus.memory[8 * 0xFF00 +
                          (8 * cpu->bus.memory[cpu->registers.BC.half[1]])],
         &cpu->registers.A);
    return;
  }
  if (operand1.type == R_C && operand2.type == n16) {
    load(&cpu->registers.A,
         &cpu->bus.memory[8 * 0xFF00 + (8 * cpu->bus.memory[cpu->registers.PC +
                                                            *operand2.bytes])]);
    return;
  }
  if (operand1.type == R_A && operand2.type == R_C) {
    load(&cpu->registers.A,
         &cpu->bus.memory[8 * 0xFF00 +
                          (8 * cpu->bus.memory[cpu->registers.BC.half[1]])]);
    return;
  }
}
