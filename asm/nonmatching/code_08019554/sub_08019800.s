	thumb_func_start sub_08019800
sub_08019800: @ 0x08019800
	push {lr}
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r2, #0x74
	cmp r0, #0
	beq _0801980E
	ldr r2, _0801981C @ =0x00008074
_0801980E:
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	pop {r0}
	bx r0
_0801981C: .4byte 0x00008074
	thumb_func_end sub_08019800

