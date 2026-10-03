	thumb_func_start OpponentSelect_DrawSelectionRing
OpponentSelect_DrawSelectionRing: @ 0x08003020
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r5, r0, #0
	add r6, r1, #0
	lsl r0, r6, #0x10
	mov r8, r0
	add r0, r5, #0
	mov r1, r8
	orr r0, r1
	mov r1, #0x81
	lsl r1, r1, #7
	mov r2, #0x80
	lsl r2, r2, #1
	mov r3, #0
	bl AddSprite8bppFlip
	add r4, r6, #0
	add r4, #0x10
	lsl r4, r4, #0x10
	add r0, r5, #0
	orr r0, r4
	mov r7, #0x90
	lsl r7, r7, #1
	mov r1, #0x40
	add r2, r7, #0
	mov r3, #0
	bl AddSprite8bppFlip
	mov r0, #0x20
	add r0, r0, r5
	mov r9, r0
	mov r1, r8
	orr r0, r1
	mov r8, r0
	mov r0, #0x80
	lsl r0, r0, #5
	mov sl, r0
	mov r0, r8
	mov r1, #0x81
	lsl r1, r1, #7
	mov r2, #0x80
	lsl r2, r2, #1
	mov r3, sl
	bl AddSprite8bppFlip
	mov r1, #0x30
	add r1, r1, r5
	mov r8, r1
	orr r4, r1
	add r0, r4, #0
	mov r1, #0x40
	add r2, r7, #0
	mov r3, sl
	bl AddSprite8bppFlip
	add r4, r6, #0
	add r4, #0x30
	lsl r4, r4, #0x10
	add r0, r5, #0
	orr r0, r4
	mov r1, #0x80
	lsl r1, r1, #6
	mov sl, r1
	mov r1, #0x81
	lsl r1, r1, #7
	mov r2, #0x80
	lsl r2, r2, #1
	mov r3, sl
	bl AddSprite8bppFlip
	add r6, #0x20
	lsl r6, r6, #0x10
	orr r5, r6
	add r0, r5, #0
	mov r1, #0x40
	add r2, r7, #0
	mov r3, sl
	bl AddSprite8bppFlip
	mov r0, r9
	orr r0, r4
	mov r9, r0
	mov r4, #0xC0
	lsl r4, r4, #6
	mov r1, #0x81
	lsl r1, r1, #7
	mov r2, #0x80
	lsl r2, r2, #1
	add r3, r4, #0
	bl AddSprite8bppFlip
	mov r1, r8
	orr r1, r6
	mov r8, r1
	mov r0, r8
	mov r1, #0x40
	add r2, r7, #0
	add r3, r4, #0
	bl AddSprite8bppFlip
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end OpponentSelect_DrawSelectionRing

