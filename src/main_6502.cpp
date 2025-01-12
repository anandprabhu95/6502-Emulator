#include "m6502.h"

int main()
{
    m6502::Mem mem;
    m6502::CPU cpu;
    cpu.Reset(mem);
    // Test Program - Start
    mem[0xFFFC] = m6502::CPU::INS_JSR;
    mem[0xFFFD] = 0x42;
    mem[0xFFFE] = 0x42;
    mem[0x4242] = m6502::CPU::INS_LDA_IM;
    mem[0x4243] = 0x84;
    cpu.Execute(9, mem);
    // Test Program - End
    return 0;
}