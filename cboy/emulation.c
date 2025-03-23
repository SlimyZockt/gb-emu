#include "emulation.h"
#include "SDL3/SDL_log.h"
#include "cJSON.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void log_opcode(const struct Opcode *opcode) {
  SDL_Log("Opcode: \n");
  SDL_Log("  hex: %X \n", opcode->val);
  SDL_Log("  id: %s \n", opcode->id);
  SDL_Log("  prefixed: %b \n", opcode->prefixed);
}

void log_instruction(const struct Instruction *instruction) {

  SDL_Log("\n Instruction: \n");

  SDL_Log("  Opcode: \n");
  SDL_Log("    Hex: %X \n", instruction->opcode->val);
  SDL_Log("    Id: %s \n", instruction->opcode->id);
  SDL_Log("    Prefixed: %b \n", instruction->opcode->prefixed);
  if (instruction->immediate != NULL) {
    SDL_Log("  Immediate: %b", *instruction->immediate);
  }
  SDL_Log("  Mnemoni: %s", instruction->mnemonic);
  SDL_Log("  Bytes: %u", instruction->bytes);
  SDL_Log("  Cycle count: %u", instruction->cycle_count);
  for (uint8_t i = 0; i < instruction->cycle_count; i++) {
    SDL_Log("  Cycle %u: %u", i + 1, instruction->cycles[i]);
  }

  SDL_Log("  Operand count: %u", instruction->operand_count);
  for (uint8_t i = 0; i < instruction->operand_count; i++) {
    struct Operand operand = instruction->operands[i];
    SDL_Log("  Operand %s:", operand.name);
    if (operand.bytes != NULL) {
      SDL_Log("    Operand bytes: %u", *operand.bytes);
    }
    if (operand.immediate != NULL) {
      SDL_Log("    Operand immediate: %b", *operand.immediate);
    }
    if (operand.increment != NULL) {
      SDL_Log("    Operand increment: %u", *operand.increment);
    }
  }

  SDL_Log("  Flag:");
  if (instruction->flags != NULL) {
    SDL_Log("    H: %x", instruction->flags->H[0]);
    SDL_Log("    C: %x", instruction->flags->C[0]);
    SDL_Log("    N: %x", instruction->flags->N[0]);
    SDL_Log("    Z: %x", instruction->flags->Z[0]);
  }
}

uint8_t get_opcode(uint8_t *rom, struct Opcode *opcode, uint16_t *pc) {
  char *opcode_str = calloc(1, 4);
  if (opcode_str == NULL) {
    SDL_Log("malloc error");
    return -2;
  }
  opcode_str[0] = '0';
  opcode_str[1] = 'x';

  char byte = (rom + *pc)[0];

  char *t = malloc(2);
  if (t == NULL) {
    SDL_Log("malloc error");
    return -3;
  }
  sprintf(t, "%02X", (uint8_t)byte);

  strncat(opcode_str, t, 2);
  opcode->val = byte;
  opcode->id = opcode_str;

  if (opcode->val == 0xCB) {
    free(opcode_str);
    opcode->id = NULL;
    opcode->prefixed = true;
    *pc += 1;
    uint8_t err = get_opcode(rom, opcode, pc);

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
      uint8_t *bytes = NULL;
      if (j_bytes != NULL) {
        uint8_t b = cJSON_GetNumberValue(j_immediate);
        bytes = &b;
      }

      cJSON *j_increment = cJSON_GetObjectItem(operands, "increment");
      uint8_t *increment = NULL;
      if (j_increment != NULL) {
        uint8_t i = cJSON_GetNumberValue(j_immediate);
        increment = &i;
      }

      cJSON *j_decrement = cJSON_GetObjectItem(operands, "decrement");
      if (j_decrement != NULL) {
        uint8_t d = -cJSON_GetNumberValue(j_immediate);
        increment = &d;
      }

      struct Operand operand = {name, &immediate, bytes, increment};

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

void jump(struct CPU *cpu, const enum Conditions condition,
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

void rst(struct CPU *cpu, uint8_t id) { push(cpu, &cpu->registers.PC); }

void load_r8(uint8_t *target, uint8_t val) {
  // \n
  *target = val;
}

void load(void *target, void *val) {
  // \n
  *(uint16_t *)target = *(uint16_t *)val;
}

void get_bytes(uint8_t *src, size_t offset, size_t x, uint8_t *dest) {
  if (offset + x > 0xFFFF * 8) {
    fprintf(stderr, "Error: Requested range is out of bounds.\n");
    exit(EXIT_FAILURE);
  }
  memcpy(dest, src + offset, x);
}

void *dict_get_val(struct Dictionary *dictionary, char *id, size_t id_size) {
  struct DictionaryData *dict = dictionary->data;
  for (size_t i = 0; i < dictionary->size; i++) {
    if (strncmp(dict[i].key, id, id_size) == 1) {
      return dict[i].val;
    }
  }

  return NULL;
}

enum OperandTypes { r8, r16, n8, n16, e8, u3, cc, vec };
struct OperandTable {
  char *id;
  enum OperandTypes operand;
};

const static uint32_t operand_count = 7;
static struct OperandTable operands_table[] = {
    {"A", r8}, {"B", r8}, {"C", r8}, {"D", r8}, {"E", r8}, {"H", r8}, {"L", r8},
};

char *get_operand_id(const enum OperandTypes operand) {
  for (uint32_t i = 0; i < operand_count; i++) {
    if (operands_table[i].operand == operand) {
      return operands_table[i].id;
    }
  }

  return NULL;
}

bool eq_operand(struct Instruction *instruction,
                const enum OperandTypes operands[], uint8_t operand_count) {
  if (instruction->operand_count != operand_count)
    return false;

  for (uint32_t i = 0; i < operand_count; i++) {
    char *dest = instruction->operands[i].name;
    uint32_t dest_size = strlen(dest);
    char *target = get_operand_id(operands[i]);
    uint32_t target_size = strlen(target);

    if (dest_size != target_size)
      return false;

    if (strncmp(dest, target, target_size) == 0) {
      return false;
    }
  }

  return true;
}

int cpu_step(struct CPU *cpu, cJSON *json, struct Dictionary *operads_lookup) {
  struct Opcode opcode = {false, 0, NULL};

  /*uint8_t *rom = malloc(0x3FFF * 8);*/
  /*get_bytes(cpu->bus.memory, 0x0, 0x3FFF * 8, rom);*/

  uint8_t err = get_opcode(cpu->bus.memory, &opcode, &cpu->registers.PC);
  if (err > 0) {
    SDL_LogError(0, "opcode error");
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

  if (strncmp(instruction.mnemonic, "LD", 2) == 1) {
    assert(instruction.operand_count == 2);
    char *target_name = instruction.operands[0].name;
    size_t target_len = strlen(target_name);
    char *val_name = instruction.operands[1].name;
    size_t val_len = strlen(val_name);

    enum OperandTypes operand[] = {r8, r8};
    bool t = eq_operand(&instruction, operand, 2);

    if (val_len == 2 &&
        (strncmp(val_name, "n8", 2) == 1 || strncmp(val_name, "n16", 2) == 1)) {
      load(dict_get_val(operads_lookup, target_name, target_len),
           &cpu->bus.memory[cpu->registers.PC + 1]);
    } else if (instruction.operands[0].increment != NULL) {
      load(&cpu->registers.HL.full, &cpu->registers.A);
      cpu->registers.HL.full += *instruction.operands->increment;
    } else if (instruction.operands[1].increment != NULL) {
      load(&cpu->registers.A, &cpu->registers.HL.full);
      cpu->registers.HL.full += *instruction.operands->increment;
    } else {

      load(dict_get_val(operads_lookup, target_name, target_len),
           dict_get_val(operads_lookup, val_name, val_len));
    }
  }

  if (strncmp(instruction.mnemonic, "LD", 2) == 1) {
    char *target_name = instruction.operands[0].name;
    size_t target_len = strlen(target_name);
    char *val_name = instruction.operands[1].name;
    size_t val_len = strlen(val_name);
    if (instruction.operand_count == 1 && strncmp(val_name, "n16", 2) == 1) {
      jump(cpu, NULL, cpu->bus.memory[cpu->registers.PC + 1]);
    }
    if (instruction.operand_count == 2 && strncmp(val_name, "n16", 2) == 1) {
      jump(cpu, NULL, cpu->bus.memory[cpu->registers.PC + 2]);
    }
  }
  switch (instruction.opcode->val) {
  case 0x00:
    // noop
    break;
  case 0x01:
    // STOP
    break;
  case 0xE9:
    jump(cpu, NULL, cpu->registers.HL.full);
    break;
  efault:
    break;
  }
  cpu->registers.PC += instruction.bytes;

  return 0;
}
