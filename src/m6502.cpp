#include "m6502.h"

void m6502::CPU::Execute(u32 Cycles, Mem& memory)
{
    while (Cycles > 0)
    {
        Byte Ins = FetchByte(Cycles, memory);
        switch (Ins)
        {
        case INS_LDA_IM:
        {
            Byte Value = FetchByte(Cycles, memory);
            A = Value;
            LDASetStatus();
        } break;
        case INS_LDA_ZP:
        {
            Byte ZeroPageAddr = FetchByte(Cycles, memory);
            A = ReadByte(Cycles, ZeroPageAddr, memory);
            LDASetStatus();
        } break;
        case INS_LDA_ZPX:
        {
            Byte ZeroPageAddr = FetchByte(Cycles, memory);
            ZeroPageAddr += X;
            Cycles--;
            A = ReadByte(Cycles, ZeroPageAddr, memory);
            LDASetStatus();
        } break;
        case INS_JSR:
        {
            Word SubRtnAddr = FetchWord(Cycles, memory);
            WriteWord(PC - 1, SP, Cycles, memory);
            Cycles--;
            PC = SubRtnAddr;
            Cycles--;
        } break;
        default:
        {
            printf("Instruction not handled %d", Ins);
        } break;
        }
    }
}