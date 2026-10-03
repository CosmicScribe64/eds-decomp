	thumb_func_start sub_080288DC
sub_080288DC: @ 0x080288DC
	push {r4, lr}
	sub sp, #0x24
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov r3, #0x40
	mov r4, #0x20
	ldr r2, _08028924 @ =0x0808270C
	lsl r1, r0, #1
	add r1, r1, r2
	ldrh r1, [r1]
	str r3, [sp, #0]
	str r4, [sp, #4]
	mov r2, #4
	str r2, [sp, #8]
	ldr r2, _08028928 @ =0x08082710
	add r0, r0, r2
	ldrb r0, [r0]
	str r0, [sp, #0xC]
	mov r0, #0x80
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	str r0, [sp, #0x1C]
	ldr r0, _0802892C @ =0x02020310
	str r0, [sp, #0x20]
	mov r0, #0
	mov r2, #0x58
	mov r3, #0x64
	bl OamListAddSprite
	add sp, #0x24
	pop {r4}
	pop {r0}
	bx r0
_08028924: .4byte gUnk_0808270C
_08028928: .4byte gUnk_08082710
_0802892C: .4byte 0x02020310
	thumb_func_end sub_080288DC

