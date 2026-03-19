#include <stdio.h>
#include <stdint.h>

uint8_t mem[256] = {0};
uint8_t reg[4] = {0};
uint8_t pc = 0, zf = 0, running = 1;
int ciclo = 0;

void fetch(uint8_t *op, uint8_t *a, uint8_t *b) {
    *op = mem[pc]; *a = mem[pc+1]; *b = mem[pc+2];
    pc += 3;
}

void decode_execute(uint8_t op, uint8_t a, uint8_t b) {
    switch (op) {
        case 0x01: reg[a] = mem[b]; break;
        case 0x02: mem[b] = reg[a]; break;
        case 0x03: reg[a] = reg[a] + reg[b]; break;
        case 0x04: reg[a] = reg[a] - reg[b]; break;
        case 0x05: reg[a] = b; break;
        case 0x06: zf = (reg[a] == reg[b]) ? 1 : 0; break;
        case 0x07: pc = a; break;
        case 0x08: if (zf) pc = a; break;
        case 0x09: if (!zf) pc = a; break;
        case 0x0A: running = 0; break;
    }
}

void trace(uint8_t op, uint8_t a, uint8_t b) {
    const char *nomes[] = {"","LOAD","STORE","ADD", "SUB","MOV","CMP","JMP","JZ","JNZ","HALT"};

    printf("Ciclo %d: %-5s %d,%d | R0=%3d R1=%3d" " R2=%3d R3=%3d | PC=%3d ZF=%d\n", ciclo, nomes[op], a, b, reg[0], reg[1], reg[2], reg[3], pc, zf);
}

int main() {
    mem[0x10] = 4; mem[0x11] = 7; mem[0x12] = 2; mem[0x13] = 9; 
    mem[0x14] = 6; mem[0x15] = 1; mem[0x16] = 8; mem[0x17] = 3;

    mem[0x20] = 0;

    mem[0x00] = 0x07; mem[0x01] = 0x22; mem[0x02] = 0x00;

    int pos = 0x22;

    mem[pos++] = 0x05; mem[pos++] = 0x01; mem[pos++] = 0x02;
    mem[pos++] = 0x05; mem[pos++] = 0x02; mem[pos++] = 0x00;
    mem[pos++] = 0x05; mem[pos++] = 0x03; mem[pos++] = 0x01;
    
    int loop = pos;
    
    mem[pos++] = 0x01; mem[pos++] = 0x00; mem[pos++] = 0x10;
    
    int ret = pos;
    
    mem[pos++] = 0x06; mem[pos++] = 0x00; mem[pos++] = 0x02;
    int jp = pos;
    mem[pos++] = 0x08; mem[pos++] = 0x00; mem[pos++] = 0x00;
    
    mem[pos++] = 0x06; mem[pos++] = 0x00; mem[pos++] = 0x03;
    int jimp = pos;
    mem[pos++] = 0x08; mem[pos++] = 0x00; mem[pos++] = 0x00;
    
    mem[pos++] = 0x04; mem[pos++] = 0x00; mem[pos++] = 0x01;
    mem[pos++] = 0x07; mem[pos++] = ret; mem[pos++] = 0x00;
    
    mem[jp + 1] = pos;
    mem[pos++] = 0x01; mem[pos++] = 0x00; mem[pos++] = 0x20;
    mem[pos++] = 0x03; mem[pos++] = 0x00; mem[pos++] = 0x03;
    mem[pos++] = 0x02; mem[pos++] = 0x00; mem[pos++] = 0x20;
    
    mem[jimp + 1] = pos;
    
    mem[pos++] = 0x01; mem[pos++] = 0x00; mem[pos++] = loop + 2;
    mem[pos++] = 0x03; mem[pos++] = 0x00; mem[pos++] = 0x03;
    mem[pos++] = 0x02; mem[pos++] = 0x00; mem[pos++] = loop + 2;
    
    mem[pos++] = 0x05; mem[pos++] = 0x01; mem[pos++] = 0x18;
    mem[pos++] = 0x06; mem[pos++] = 0x00; mem[pos++] = 0x01;
    int jdone = pos;
    mem[pos++] = 0x08; mem[pos++] = 0x00; mem[pos++] = 0x00;
    
    mem[pos++] = 0x05; mem[pos++] = 0x01; mem[pos++] = 0x02;
    mem[pos++] = 0x07; mem[pos++] = loop; mem[pos++] = 0x00;
    
    mem[jdone + 1] = pos;
    mem[pos++] = 0x0A; mem[pos++] = 0x00; mem[pos++] = 0x00;

    while (running && pc < 256) {
        uint8_t op, a, b;
        ciclo++;
        fetch(&op, &a, &b);
        decode_execute(op, a, b);
        trace(op, a, b);
    }

    printf("\n\nTotal de pares encontrados (mem[0x20]): %d\n", mem[0x20]);
    
    return 0;
}
