	thumb_func_start JudgeRockPaperScissors
JudgeRockPaperScissors: @ 0x08028930
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	add r2, r1, #0
	cmp r1, #1
	beq _08028962
	cmp r1, #1
	bgt _08028948
	cmp r1, #0
	beq _0802894E
	b _08028998
_08028948:
	cmp r2, #2
	beq _0802897A
	b _08028998
_0802894E:
	cmp r0, #1
	beq _0802898E
	cmp r0, #1
	bgt _0802895C
	cmp r0, #0
	beq _08028976
	b _08028998
_0802895C:
	cmp r0, #2
	bne _08028998
	b _08028992
_08028962:
	cmp r0, #1
	beq _08028976
	cmp r0, #1
	bgt _08028970
	cmp r0, #0
	beq _08028992
	b _08028998
_08028970:
	cmp r0, #2
	beq _0802898E
	b _08028998
_08028976:
	mov r0, #2
	b _08028998
_0802897A:
	cmp r0, #1
	beq _08028992
	cmp r0, #1
	bgt _08028988
	cmp r0, #0
	beq _0802898E
	b _08028998
_08028988:
	cmp r0, #2
	beq _08028996
	b _08028998
_0802898E:
	mov r0, #1
	b _08028998
_08028992:
	mov r0, #0
	b _08028998
_08028996:
	mov r0, #2
_08028998:
	bx lr
	thumb_func_end JudgeRockPaperScissors
	.align 2, 0

