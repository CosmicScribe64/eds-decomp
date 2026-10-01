@ sound_mixer_arm -- Konami sound driver: ARM PCM mixer (hand-written assembly).
@
@ Yu-Gi-Oh! The Eternal Duelist Soul (USA, AY5E)
@   ROM 0x0807EAD0 - 0x0807ECF8 (0x228 bytes, .text), between the Thumb sound
@   driver and the SDK SWI stubs (libagbsyscall, 0x0807ECF8).
@
@   0x0807EAD0  sub_0807EAD0  SoundMixAll      (proposed) mix both FIFOs
@   0x0807EAF0  sub_0807EAF0  SoundMixFifo     (proposed) clear + mix one FIFO
@   0x0807EC1C  sub_0807EC1C  SoundMixChannel  (proposed) inner mix loop; the
@               driver copies 0x38 words of it to IWRAM 0x03005A54 and
@               SoundMixFifo calls that copy, not this ROM image.
@
@ Hand-written: no agbcc ARM-mode patterns, a 10-register save in
@ SoundMixAll, values passed in r4-r8/ip/sl/sb, and SoundMixChannel returns
@ to a computed restart point through `bx r1`. See wiki/functions/sound-mixer.md.
@
@ Match status: MATCHING (assembled with arm-none-eabi-as -mcpu=arm7tdmi,
@ linked at 0x0807EAD0, compared byte-for-byte with baserom).

	.syntax unified
	.text

	.macro arm_func_start name:req
	.align 2, 0
	.global \name
	.arm
	.type \name, %function
	.endm

	.macro arm_func_end name:req
	.size \name, .-\name
	.endm

@ struct SoundPcmChannel (0x10 bytes; 6 at gUnk_030053AC, 3 per FIFO)
	.equ PCM_DATA,      0x0     @ u32  current sample pointer
	.equ PCM_REMAINING, 0x4     @ s32  samples left before the end/loop point
	.equ PCM_STEP,      0x8     @ u16  pitch step (0x1000 = one sample per byte)
	.equ PCM_FRAC,      0xA     @ u16  12-bit fractional position
	.equ PCM_SAMPLE_ID, 0xC     @ s16  sample id; bit 15 selects the bank
	.equ PCM_FLAGS,     0xE     @ u8   bit 7 active, bit 6 loop
	.equ PCM_VOLUME,    0xF     @ u8   0 = silent

	.equ FIFO_BUFFER_SIZE, 0x2C0    @ ring size used for wrap (each buffer has 0x320 bytes)
	.equ DMA_FILL32,       0x85000000   @ enable | 32-bit | fixed source

@ void SoundMixAll(void)
@ Mixes voices 0-2 into FIFO A (r1 = 0) and 3-5 into FIFO B (r1 = 4).
@ r7 walks gUnk_030053AC across both calls.
	arm_func_start sub_0807EAD0
sub_0807EAD0: @ 0x0807EAD0
	push {r4, r5, r6, r7, r8, sb, sl, fp, ip, lr}
	ldr r7, .Lvoices
	mov r1, #0
	bl sub_0807EAF0
	mov r1, #4
	bl sub_0807EAF0
	pop {r4, r5, r6, r7, r8, sb, sl, fp, ip, lr}
	bx lr
	arm_func_end sub_0807EAD0

@ SoundMixFifo(r1 = 0 for FIFO A | 4 for FIFO B, r7 = first voice)
@ Refills the part of the ring buffer the FIFO DMA consumed since the last
@ frame: [prev, cur), or [prev, end) + [0, cur) after a wrap. Each span is
@ cleared with a DMA0 32-bit fill from a zero word on the stack, then every
@ active voice with a nonzero volume is mixed into it by the IWRAM routine.
@ Returns with r7 advanced past the three voices.
	arm_func_start sub_0807EAF0
sub_0807EAF0: @ 0x0807EAF0
	stmdb sp!, {lr}
	sub sp, sp, #0xc
	ldr r6, .Ldma_pos
	ldrh r2, [r6, r1]!          @ r2 = cur = DMA position of this FIFO
	ldrh r3, [r6, #2]           @ r3 = prev
	strh r2, [r6, #2]           @ prev = cur
	mov sb, #0
	str sb, [sp, #8]            @ zero word for the DMA fill
	ldr r6, .Lbuffers
	mov r0, #0xc8
	mul r1, r0, r1
	add sl, r6, r1              @ sl = buffer base (A + r1 * 0xC8)
	add r6, sl, r3              @ r6 = buffer + prev
	subs r4, r2, r3             @ r4 = cur - prev
	bgt .Lclear_span
	@ wrapped: the second span is [0, cur), the first [prev, end)
	movs sb, r2
	beq .Lfirst_span
	ldr r2, .Ldma0
	add r0, sp, #8
	str r0, [r2]                @ DMA0SAD = &zero
	str sl, [r2, #4]            @ DMA0DAD = buffer
	mov r0, #DMA_FILL32
	orr r0, r0, sb, lsr #2
	str r0, [r2, #8]            @ DMA0CNT = fill cur / 4 words
	ldr r0, [r2, #8]
.Lwait_wrap_fill:
	ldr r0, [r2, #8]
	lsls r0, r0, #1
	bhs .Lwait_wrap_fill
.Lfirst_span:
	rsb r4, r3, #FIFO_BUFFER_SIZE
.Lclear_span:
	ldr r2, .Ldma0
	add r0, sp, #8
	str r0, [r2]
	str r6, [r2, #4]
	mov r0, #DMA_FILL32
	orr r0, r0, r4, lsr #2
	str r0, [r2, #8]
	str r4, [sp]                @ save first span length
	str r6, [sp, #4]            @ and destination for each voice
.Lwait_fill:
	ldr r0, [r2, #8]
	lsls r0, r0, #1
	bhs .Lwait_fill
	mov fp, #3
.Lvoice_loop:
	ldrh r1, [r7, #PCM_FLAGS]
	lsrs r1, r1, #8             @ C = active flag, r1 = volume
	blo .Lnext_voice
	beq .Lnext_voice
	add r8, r1, #1              @ r8 = volume + 1
	ldr r5, [r7, #PCM_DATA]
	ldr ip, [r7, #PCM_REMAINING]
	ldrh r2, [r7, #PCM_STEP]
	ldrh r3, [r7, #PCM_FRAC]
	ldr r0, .Lmix_channel
	mov lr, pc
	bx r0                       @ mix the first span (r4 bytes at r6)
	mov r6, sl
	mov r4, sb
	ldr r0, .Lmix_channel_wrap
	mov lr, pc
	bx r0                       @ mix the wrapped span (sb bytes at the base)
	str r5, [r7, #PCM_DATA]
	str ip, [r7, #PCM_REMAINING]
	strh r3, [r7, #PCM_FRAC]
	ldr r4, [sp]
	ldr r6, [sp, #4]
.Lnext_voice:
	add r7, r7, #0x10
	subs fp, fp, #1
	bne .Lvoice_loop
	add sp, sp, #0xc
	ldm sp!, {pc}
.Lvoices:
	.4byte gUnk_030053AC        @ struct SoundPcmChannel[6]
.Ldma_pos:
	.4byte gUnk_0300540C        @ u16 cur/prev DMA positions per FIFO
.Lbuffers:
	.4byte gUnk_03005414        @ FIFO A buffer; FIFO B at +0x320
.Ldma0:
	.4byte 0x040000B0           @ REG_DMA0SAD
.Lmix_channel:
	.4byte gUnk_03005A54        @ IWRAM copy of sub_0807EC1C
.Lmix_channel_wrap:
	.4byte gUnk_03005A54
	arm_func_end sub_0807EAF0

@ SoundMixChannel: runs from its IWRAM copy at 0x03005A54.
@ In:  r4 = bytes to mix, r5 = sample pointer, r6 = destination,
@      ip = samples remaining, r2 = step, r3 = 12-bit fraction,
@      r7 = voice, r8 = volume + 1.
@ Out: r5, ip and r3 updated; the voice is stopped (flags = 0) at the end of
@      a non-looping sample. Each output byte gets += (sample * r8) >> 4,
@      without saturation. A step of 0x800 takes a path that writes every
@      sample twice.
	arm_func_start sub_0807EC1C
sub_0807EC1C: @ 0x0807EC1C
	stmdb sp!, {lr}
	cmp r2, #0x800
	beq .Lhalf_speed
.Lrestart:
	mov r0, #0
.Lload_sample:
	ldrsb r1, [r5, r0]!         @ advance by r0 whole samples and read
	mul r1, r8, r1
	asr r1, r1, #4
.Lmix_byte:
	subs r4, r4, #1
	bmi .Lreturn
	ldrsb r0, [r6]
	add r0, r0, r1
	strb r0, [r6], #1
	add r3, r3, r2
	lsrs r0, r3, #0xc           @ whole samples to advance
	beq .Lmix_byte
	lsl r3, r3, #0x14
	lsr r3, r3, #0x14
	subs ip, ip, r0
	bgt .Lload_sample
	adr r1, .Lrestart           @ continue at the restart after a loop reload
.Lsample_end:
	ldrb r0, [r7, #PCM_FLAGS]
	lsrs r0, r0, #7             @ C = loop flag (bit 6)
	bhs .Lloop_sample
	mov r0, #0
	strb r0, [r7, #PCM_FLAGS]   @ stop the voice
.Lreturn:
	ldm sp!, {pc}
.Lloop_sample:
	ldr r5, .Lsample_bank0
	ldrsh r0, [r7, #PCM_SAMPLE_ID]
	lsls r0, r0, #0x11          @ C = bit 15 of the sample id
	ldrhs r5, .Lsample_bank1
	add r5, r5, r0, lsr #15     @ table + (id & 0x7FFF) * 4
	ldr r5, [r5]                @ sample header
	ldr ip, [r5, #4]
	ldr r0, [r5, #8]
	sub ip, ip, r0              @ remaining = length - loop start
	add r5, r5, #0xc
	add r5, r5, r0              @ data = header + 0xC + loop start
	bx r1
.Lhalf_speed:
	subs r4, r4, #2
	bmi .Lreturn
	ldrsb r1, [r5], #1
	mul r1, r8, r1
	asr r1, r1, #4
	ldrsb r0, [r6]
	add r0, r0, r1
	strb r0, [r6], #1
	ldrsb r0, [r6]
	add r0, r0, r1
	strb r0, [r6], #1
	subs ip, ip, #1
	bgt .Lhalf_speed
	adr r1, .Lhalf_speed
	b .Lsample_end
.Lsample_bank0:
	.4byte gUnk_0811B420        @ sample headers, id bit 15 clear
.Lsample_bank1:
	.4byte gUnk_08088A20        @ sample headers, id bit 15 set
	arm_func_end sub_0807EC1C
