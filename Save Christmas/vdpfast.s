; vdpfast.s - copie rapide RAM/ROM -> VRAM pour le defilement des decors

        .module vdpfast
        .globl  _vdp_put_rect
        .area   _CODE

; void vdp_put_rect(unsigned vram_addr, void *src, byte n, byte w, byte h)
; Copie h lignes de n octets ; la source avance de w, la VRAM de 32 (table des noms)

_vdp_put_rect:
        push    ix
        ld      ix,#4
        add     ix,sp
        ld      e,0(ix)         ; de = adresse VRAM
        ld      d,1(ix)
        ld      l,2(ix)         ; hl = source
        ld      h,3(ix)
        ld      c,#0xBE
2$:     ld      a,e
        out     (0xBF),a
        ld      a,d
        or      #0x40
        out     (0xBF),a
        push    hl
        ld      b,4(ix)
1$:     outi
        nop
        jp      nz,1$
        pop     hl
        ld      a,5(ix)         ; source += w
        add     a,l
        ld      l,a
        jr      nc,3$
        inc     h
3$:     ld      a,#32           ; VRAM += 32
        add     a,e
        ld      e,a
        jr      nc,4$
        inc     d
4$:     dec     6(ix)
        jr      nz,2$
        pop     ix
        ret
