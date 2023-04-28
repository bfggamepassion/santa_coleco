path c:\sdcc\bin
@echo off
sdcc -mz80 -c --std-c99 --opt-code-speed charset.c
del charset.lst
del charset.sym
del charset.asm
sdcc -mz80 -c --std-c99 --opt-code-speed main.c
del main.lst
del main.sym
del main.asm
sdasz80 -o music.rel music.s
pause
