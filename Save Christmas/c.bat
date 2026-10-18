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
sdcc -mz80 -c --std-c99 --opt-code-speed title.c
del title.lst
del title.sym
del title.asm
sdcc -mz80 -c --std-c99 --opt-code-speed scroll.c
del scroll.lst
del scroll.sym
del scroll.asm
sdasz80 -o music.rel music.s
sdasz80 -o vdpfast.rel vdpfast.s
sdasz80 -o xmas.rel xmas.s
pause
