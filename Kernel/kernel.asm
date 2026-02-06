[BITS 64]
SECTION .text
global _start

_start:
	mov rcx, [rdi + 8]
	mov rdi, [rdi]

  mov rax, 0xFF18679AFF18679A
  shr rcx, 3

  cld
  rep stosq

_done:
  mov rax, 0x1234
  ret
