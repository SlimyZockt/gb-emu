#include "SDL3/SDL_log.h"
#include <assert.h>
#include <cJSON.h>
#include <emulation.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

bool is_condition_set(const enum OperandType condition, struct CPU *cpu) {
  switch (condition) {
  case C:
    return ((cpu->registers.F << 4) & 1) == 1;
  case NC:
    return ((cpu->registers.F << 4) & 1) == 0;
  case Z:
    return ((cpu->registers.F << 7) & 1) == 1;
  case NZ:
    return ((cpu->registers.F << 7) & 1) == 0;
  default:
    assert(true);
    return true;
  }
}

void jump(const enum OperandType condition, struct CPU *cpu,
          const uint16_t n16) {
  if (condition == NONE || is_condition_set(condition, cpu)) {
    cpu->registers.PC = n16;
    return;
  }
}

void jump_relative(const enum OperandType condition, struct CPU *cpu,
                   const void *n16) {
  if (condition == NONE || is_condition_set(condition, cpu)) {
    cpu->registers.PC += *(int8_t *)n16;
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

void load(void *target, void *val) { *(char *)target = *(char *)val; }

void *get_reg(struct CPU *cpu, enum OperandType reg_type) {
  switch (reg_type) {
  case R_A:
    return &cpu->registers.A;
  case R_B:
    return &cpu->registers.BC.half[0];
  case R_C:
    return &cpu->registers.BC.half[1];
  case R_D:
    return &cpu->registers.DE.half[0];
  case R_E:
    return &cpu->registers.DE.half[1];
  case R_H:
    return &cpu->registers.HL.half[0];
  case R_L:
    return &cpu->registers.HL.half[1];
  case R_BC:
    return &cpu->registers.BC.full;
  case R_DE:
    return &cpu->registers.DE.full;
  case R_HL:
    return &cpu->registers.HL.full;
  default:
    return NULL;
  }

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
    load(get_reg(cpu, operand1.type), get_reg(cpu, operand2.type));
    return;
  }

  if (operand1.type == r8 && operand2.type == n8) {
    assert(operand2.bytes != NULL);
    load(get_reg(cpu, operand1.type),
         &cpu->bus.memory[cpu->registers.PC + *operand2.bytes]);
    return;
  }

  if (operand1.type == r16 && operand2.type == n16) {
    assert(operand2.bytes != NULL);
    load(get_reg(cpu, operand1.type),
         &cpu->bus.memory[cpu->registers.PC + *operand2.bytes]);
    return;
  }

  if (operand1.type == R_HL && operand2.type == r8) {
    load(&cpu->registers.HL.full, get_reg(cpu, operand2.type));
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
    load(get_reg(cpu, operand1.type), &cpu->registers.A);
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
  load(&cpu->registers.A, get_reg(cpu, operand1.type));
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

void jp(struct Instruction *instruction, struct CPU *cpu) {
  struct Operand operand1 = instruction->operands[0];

  if (instruction->operand_count == 1) {

    switch (operand1.type) {
    case R_HL:
      return jump(NONE, cpu, cpu->bus.memory[cpu->registers.PC]);
    case n16:
      return jump(NONE, cpu, cpu->bus.memory[cpu->registers.PC]);
    default:
      return;
    }
    return;
  }

  assert(instruction->operand_count == 2);
  jump(operand1.type, cpu, cpu->bus.memory[cpu->registers.PC]);
  return;
}

void jr(struct Instruction *instruction, struct CPU *cpu) {
  struct Operand operand1 = instruction->operands[0];

  if (instruction->operand_count == 1) {
    return jump_relative(NONE, cpu, &cpu->bus.memory[cpu->registers.PC]);
  }

  assert(instruction->operand_count == 2);
  jump_relative(operand1.type, cpu, &cpu->bus.memory[cpu->registers.PC]);
  return;
}

void call(struct Instruction *instruction, struct CPU *cpu) {
  struct Operand operand1 = instruction->operands[0];
  cpu->registers.SP += 0x2;

  cpu->bus.memory[cpu->registers.SP] = 3;
}
