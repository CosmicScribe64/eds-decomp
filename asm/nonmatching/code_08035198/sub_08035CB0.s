	thumb_func_start sub_08035CB0
sub_08035CB0: @ 0x08035CB0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	mov r0, #4
	mov r1, r8
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08035D26
	mov r5, #0
	mov r2, #1
	mov sl, r2
	ldr r0, _08035D38 @ =0x00000D64
	mov r9, r0
_08035CD2:
	mov r4, #0
	add r7, r5, #1
	add r0, r5, #0
	mov r1, sl
	and r0, r1
	mov r6, r9
	mul r6, r0
_08035CE0:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _08035D3C @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08035CFE
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #1
	mov r3, #1
	bl sub_08018ED8
_08035CFE:
	add r4, #1
	cmp r4, #4
	ble _08035CE0
	add r5, r7, #0
	cmp r5, #1
	ble _08035CD2
	mov r0, #1
	mov r2, r8
	ldrb r2, [r2, #2]
	and r0, r2
	mov r1, #0x48
	cmp r0, #0
	beq _08035D1A
	ldr r1, _08035D40 @ =0x00008048
_08035D1A:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08035D26:
	mov r0, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08035D38: .4byte 0x00000D64
_08035D3C: .4byte 0x0201930C
_08035D40: .4byte 0x00008048
	thumb_func_end sub_08035CB0

