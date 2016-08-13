global roundps_xmm3_xmm0_09
;section code use32 class=code
roundps_xmm3_xmm0_09:
  ; int 3
  ; nop
  ; sub     esp, 16                   ; allocate space for xmm4
;  ret
; movaps xmmword ptr [esp], xmm4    ; store xmm4
;;; construct 1.0f value in xmm3 ;;;
pxor        xmm3,xmm3    ; xmm3 <- zeroes
pcmpeqd     xmm3,xmm3    ; xmm3 <- ones
psrld       xmm3,19h     ; xmm3 >> 25
pslld       xmm3,17h     ; xmm3 << 23
;;; truncate xmm0 ;;;
movaps      xmm4, xmm0
cvttps2dq   xmm4,xmm4    ; truncate xmm3 floats to integers
cvtdq2ps    xmm4,xmm4    ; convert xmm3 integers to floats
cmpltps     xmm0,xmm4    ; xmm4 > xmm0

; 00E96863  movaps      xmmword ptr [ebp-20h],xmm0  
; 00E96867  movaps      xmm0,xmmword ptr [ebp-20h]  
; 00E9686B  movaps      xmmword ptr [v0],xmm0