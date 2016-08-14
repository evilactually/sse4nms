global roundps_xmm3_xmm0_09

roundps_xmm3_xmm0_09:
  sub     esp, 16
  movaps oword [dword esp], xmm4
  
  sub     esp, 16
  movaps oword [dword esp], xmm5

  ;;; construct 1.0f value in xmm3 ;;;
  pxor        xmm3, xmm3    ; xmm3 <- zeroes
  pcmpeqd     xmm3, xmm3    ; xmm3 <- ones
  psrld       xmm3, 19h     ; xmm3 >> 25
  pslld       xmm3, 17h     ; xmm3 << 23
  
  ;;; truncate xmm0 and store in xmm4 ;;;
  movaps      xmm4, xmm0
  cvttps2dq   xmm4, xmm4    ; truncate xmm3 floats to integers
  cvtdq2ps    xmm4, xmm4    ; convert xmm3 integers to floats
  
  ;;; create conditional mask in xmm5 ;;;
  movaps      xmm5, xmm0
  cmpltps     xmm5, xmm4    ; xmm5 bits are on if xmm4 > xmm0

  andps       xmm3, xmm5   ; apply xmm5 mask to xmm3
  subps       xmm4, xmm3   ; turn truncated value into floored in xmm4
  movaps      xmm3, xmm4   ; store final result in xmm3

  movaps xmm5, oword [dword esp]
  add    esp, 16
  
  movaps xmm4, oword [dword esp]
  add    esp, 16
  
  ret
  
; 00E96863  movaps      xmmword ptr [ebp-20h],xmm0  
; 00E96867  movaps      xmm0,xmmword ptr [ebp-20h]  
; 00E9686B  movaps      xmmword ptr [v0],xmm0