	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 15, 0	sdk_version 15, 4
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ; -- Begin function set_viterbi27_polynomial
lCPI0_0:
	.long	0                               ; 0x0
	.long	2                               ; 0x2
	.long	4                               ; 0x4
	.long	6                               ; 0x6
lCPI0_1:
	.long	8                               ; 0x8
	.long	10                              ; 0xa
	.long	12                              ; 0xc
	.long	14                              ; 0xe
lCPI0_2:
	.long	16                              ; 0x10
	.long	18                              ; 0x12
	.long	20                              ; 0x14
	.long	22                              ; 0x16
lCPI0_3:
	.long	24                              ; 0x18
	.long	26                              ; 0x1a
	.long	28                              ; 0x1c
	.long	30                              ; 0x1e
lCPI0_4:
	.long	32                              ; 0x20
	.long	34                              ; 0x22
	.long	36                              ; 0x24
	.long	38                              ; 0x26
lCPI0_5:
	.long	40                              ; 0x28
	.long	42                              ; 0x2a
	.long	44                              ; 0x2c
	.long	46                              ; 0x2e
lCPI0_6:
	.long	48                              ; 0x30
	.long	50                              ; 0x32
	.long	52                              ; 0x34
	.long	54                              ; 0x36
lCPI0_7:
	.long	56                              ; 0x38
	.long	58                              ; 0x3a
	.long	60                              ; 0x3c
	.long	62                              ; 0x3e
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_set_viterbi27_polynomial
	.p2align	2
_set_viterbi27_polynomial:              ; @set_viterbi27_polynomial
	.cfi_startproc
; %bb.0:
	ldp	w9, w8, [x0]
	lsr	w10, w8, #31
	dup.4s	v0, w10
	cmp	w8, #0
	cneg	w8, w8, mi
	dup.4s	v1, w8
	lsr	w8, w9, #31
	dup.4s	v3, w8
	cmp	w9, #0
	cneg	w8, w9, mi
	dup.4s	v4, w8
Lloh0:
	adrp	x8, lCPI0_0@PAGE
Lloh1:
	ldr	q5, [x8, lCPI0_0@PAGEOFF]
	and.16b	v6, v4, v5
Lloh2:
	adrp	x8, lCPI0_1@PAGE
Lloh3:
	ldr	q7, [x8, lCPI0_1@PAGEOFF]
	and.16b	v16, v4, v7
Lloh4:
	adrp	x8, lCPI0_2@PAGE
Lloh5:
	ldr	q17, [x8, lCPI0_2@PAGEOFF]
	and.16b	v18, v4, v17
Lloh6:
	adrp	x8, lCPI0_3@PAGE
Lloh7:
	ldr	q19, [x8, lCPI0_3@PAGEOFF]
	and.16b	v2, v4, v19
	cnt.16b	v20, v2
	movi.16b	v2, #1
	movi.2d	v21, #0000000000000000
	udot.4s	v21, v2, v20
	cnt.16b	v18, v18
	movi.2d	v20, #0000000000000000
	udot.4s	v20, v2, v18
	cnt.16b	v16, v16
	movi.2d	v18, #0000000000000000
	udot.4s	v18, v2, v16
	cnt.16b	v6, v6
	movi.2d	v16, #0000000000000000
	udot.4s	v16, v2, v6
	bic.4s	v16, #2
	bic.4s	v18, #2
	bic.4s	v20, #2
	bic.4s	v21, #6
	cmeq.4s	v6, v3, v21
	cmeq.4s	v20, v3, v20
	uzp1.8h	v6, v20, v6
	cmeq.4s	v18, v3, v18
	cmeq.4s	v16, v3, v16
	uzp1.8h	v16, v16, v18
	uzp1.16b	v6, v16, v6
	and.16b	v5, v1, v5
	and.16b	v7, v1, v7
	and.16b	v16, v1, v17
	and.16b	v17, v1, v19
	cnt.16b	v17, v17
	movi.2d	v18, #0000000000000000
	udot.4s	v18, v2, v17
	cnt.16b	v16, v16
	movi.2d	v17, #0000000000000000
	udot.4s	v17, v2, v16
	cnt.16b	v7, v7
	movi.2d	v16, #0000000000000000
	udot.4s	v16, v2, v7
	cnt.16b	v5, v5
	movi.2d	v7, #0000000000000000
	udot.4s	v7, v2, v5
	bic.4s	v7, #2
	bic.4s	v16, #2
	bic.4s	v17, #2
	bic.4s	v18, #6
	cmeq.4s	v5, v0, v18
	cmeq.4s	v17, v0, v17
	uzp1.8h	v5, v17, v5
	cmeq.4s	v16, v0, v16
	cmeq.4s	v7, v0, v7
	uzp1.8h	v7, v7, v16
	uzp1.16b	v5, v7, v5
Lloh8:
	adrp	x8, lCPI0_4@PAGE
Lloh9:
	ldr	q7, [x8, lCPI0_4@PAGEOFF]
	and.16b	v16, v4, v7
Lloh10:
	adrp	x8, lCPI0_5@PAGE
Lloh11:
	ldr	q17, [x8, lCPI0_5@PAGEOFF]
	and.16b	v18, v4, v17
Lloh12:
	adrp	x8, lCPI0_6@PAGE
Lloh13:
	ldr	q19, [x8, lCPI0_6@PAGEOFF]
	and.16b	v20, v4, v19
Lloh14:
	adrp	x8, lCPI0_7@PAGE
Lloh15:
	ldr	q21, [x8, lCPI0_7@PAGEOFF]
	and.16b	v4, v4, v21
	cnt.16b	v4, v4
	movi.2d	v22, #0000000000000000
	udot.4s	v22, v2, v4
	cnt.16b	v4, v20
	movi.2d	v20, #0000000000000000
	udot.4s	v20, v2, v4
	cnt.16b	v4, v18
	movi.2d	v18, #0000000000000000
	udot.4s	v18, v2, v4
	cnt.16b	v4, v16
	movi.2d	v16, #0000000000000000
	udot.4s	v16, v2, v4
	movi.2d	v4, #0000000000000000
Lloh16:
	adrp	x8, __MergedGlobals@PAGE
Lloh17:
	add	x8, x8, __MergedGlobals@PAGEOFF
	mvn.16b	v6, v6
	mvn.16b	v5, v5
	bic.4s	v16, #2
	bic.4s	v18, #6
	bic.4s	v20, #6
	bic.4s	v22, #6
	cmeq.4s	v22, v3, v22
	cmeq.4s	v20, v3, v20
	uzp1.8h	v20, v20, v22
	cmeq.4s	v18, v3, v18
	cmeq.4s	v3, v3, v16
	uzp1.8h	v3, v3, v18
	uzp1.16b	v3, v3, v20
	mvn.16b	v3, v3
	stp	q6, q3, [x8, #64]
	and.16b	v3, v1, v7
	and.16b	v6, v1, v17
	and.16b	v7, v1, v19
	and.16b	v1, v1, v21
	cnt.16b	v1, v1
	movi.2d	v16, #0000000000000000
	udot.4s	v16, v2, v1
	cnt.16b	v1, v7
	movi.2d	v7, #0000000000000000
	udot.4s	v7, v2, v1
	cnt.16b	v1, v6
	movi.2d	v6, #0000000000000000
	udot.4s	v6, v2, v1
	cnt.16b	v1, v3
	udot.4s	v4, v2, v1
	bic.4s	v4, #2
	bic.4s	v6, #6
	bic.4s	v7, #6
	bic.4s	v16, #6
	cmeq.4s	v1, v0, v16
	cmeq.4s	v2, v0, v7
	uzp1.8h	v1, v2, v1
	cmeq.4s	v2, v0, v6
	cmeq.4s	v0, v0, v4
	uzp1.8h	v0, v0, v2
	uzp1.16b	v0, v0, v1
	mvn.16b	v0, v0
	stp	q5, q0, [x8, #96]
	ldr	w9, [x8]
	add	w9, w9, #1
	str	w9, [x8]
	ret
	.loh AdrpAdd	Lloh16, Lloh17
	.loh AdrpAdrp	Lloh14, Lloh16
	.loh AdrpLdr	Lloh14, Lloh15
	.loh AdrpAdrp	Lloh12, Lloh14
	.loh AdrpLdr	Lloh12, Lloh13
	.loh AdrpAdrp	Lloh10, Lloh12
	.loh AdrpLdr	Lloh10, Lloh11
	.loh AdrpAdrp	Lloh8, Lloh10
	.loh AdrpLdr	Lloh8, Lloh9
	.loh AdrpAdrp	Lloh6, Lloh8
	.loh AdrpLdr	Lloh6, Lloh7
	.loh AdrpAdrp	Lloh4, Lloh6
	.loh AdrpLdr	Lloh4, Lloh5
	.loh AdrpAdrp	Lloh2, Lloh4
	.loh AdrpLdr	Lloh2, Lloh3
	.loh AdrpAdrp	Lloh0, Lloh2
	.loh AdrpLdr	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3, 0x0                          ; -- Begin function create_viterbi27
lCPI1_0:
	.long	109                             ; 0x6d
	.long	79                              ; 0x4f
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_create_viterbi27
	.p2align	2
_create_viterbi27:                      ; @create_viterbi27
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #48
	stp	x20, x19, [sp, #16]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	add	x29, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
Lloh18:
	adrp	x8, __MergedGlobals@PAGE
Lloh19:
	ldr	w8, [x8, __MergedGlobals@PAGEOFF]
	cbnz	w8, LBB1_2
; %bb.1:
Lloh20:
	adrp	x8, __MergedGlobals@PAGE
Lloh21:
	add	x8, x8, __MergedGlobals@PAGEOFF
	movi.2d	v0, #0x00ffffffff0000
	movi.2d	v1, #0xff0000ff00ffff00
	movi.2d	v2, #0xffff00000000ffff
	stp	q0, q2, [x8, #64]
	stp	q1, q1, [x8, #96]
	mov	w9, #1                          ; =0x1
	str	w9, [x8]
LBB1_2:
	add	x0, sp, #8
	mov	w1, #64                         ; =0x40
	mov	w2, #576                        ; =0x240
	bl	_posix_memalign
	cbnz	w0, LBB1_5
; %bb.3:
	ldr	x8, [sp, #8]
Lloh22:
	adrp	x9, lCPI1_0@PAGE
Lloh23:
	ldr	d0, [x9, lCPI1_0@PAGEOFF]
	str	d0, [x8, #544]
	sbfiz	x9, x19, #4, #32
	add	x0, x8, #536
	add	x2, x9, #96
	mov	w1, #64                         ; =0x40
	bl	_posix_memalign
	cbnz	w0, LBB1_6
; %bb.4:
	ldr	x19, [sp, #8]
	add	x20, x19, #256
Lloh24:
	adrp	x1, l_.memset_pattern@PAGE
Lloh25:
	add	x1, x1, l_.memset_pattern@PAGEOFF
	mov	x0, x19
	mov	w2, #256                        ; =0x100
	bl	_memset_pattern16
	str	x19, [x19, #520]
	str	x20, [x19, #528]
	ldr	x8, [x19, #536]
	str	x8, [x19, #512]
	str	wzr, [x19]
	ldr	x0, [sp, #8]
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
LBB1_5:
Lloh26:
	adrp	x0, l_.str@PAGE
Lloh27:
	add	x0, x0, l_.str@PAGEOFF
	bl	_perror
	mov	x0, #0                          ; =0x0
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
LBB1_6:
	add	x0, sp, #8
	bl	_create_viterbi27.cold.1
	mov	x0, #0                          ; =0x0
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
	.loh AdrpLdr	Lloh18, Lloh19
	.loh AdrpAdd	Lloh20, Lloh21
	.loh AdrpLdr	Lloh22, Lloh23
	.loh AdrpAdd	Lloh24, Lloh25
	.loh AdrpAdd	Lloh26, Lloh27
	.cfi_endproc
                                        ; -- End function
	.globl	_init_viterbi27                 ; -- Begin function init_viterbi27
	.p2align	2
_init_viterbi27:                        ; @init_viterbi27
	.cfi_startproc
; %bb.0:
	cbz	x0, LBB2_3
; %bb.1:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x20, x1
	mov	x19, x0
Lloh28:
	adrp	x1, l_.memset_pattern@PAGE
Lloh29:
	add	x1, x1, l_.memset_pattern@PAGEOFF
	mov	w2, #256                        ; =0x100
	bl	_memset_pattern16
	str	x19, [x19, #520]
	add	x8, x19, #256
	str	x8, [x19, #528]
	ldr	x8, [x19, #536]
	str	x8, [x19, #512]
	cmp	w20, #63
	b.hi	LBB2_4
; %bb.2:
	mov	w0, #0                          ; =0x0
	str	wzr, [x19, w20, uxtw #2]
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
LBB2_3:
	mov	w0, #-1                         ; =0xffffffff
	ret
LBB2_4:
	mov	w0, #0                          ; =0x0
	str	wzr, [x19]
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh28, Lloh29
	.cfi_endproc
                                        ; -- End function
	.globl	_update_viterbi27_blk           ; -- Begin function update_viterbi27_blk
	.p2align	2
_update_viterbi27_blk:                  ; @update_viterbi27_blk
	.cfi_startproc
; %bb.0:
	stp	d11, d10, [sp, #-112]!          ; 16-byte Folded Spill
	stp	d9, d8, [sp, #16]               ; 16-byte Folded Spill
	stp	x26, x25, [sp, #32]             ; 16-byte Folded Spill
	stp	x24, x23, [sp, #48]             ; 16-byte Folded Spill
	stp	x22, x21, [sp, #64]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #80]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #96]             ; 16-byte Folded Spill
	add	x29, sp, #96
	sub	x9, sp, #80
	and	sp, x9, #0xffffffffffffffc0
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	.cfi_offset b8, -88
	.cfi_offset b9, -96
	.cfi_offset b10, -104
	.cfi_offset b11, -112
Lloh30:
	adrp	x8, ___stack_chk_guard@GOTPAGE
Lloh31:
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
Lloh32:
	ldr	x8, [x8]
	str	x8, [sp, #72]
	mov	w8, #-1                         ; =0xffffffff
	cbz	x0, LBB3_14
; %bb.1:
	cbz	x1, LBB3_14
; %bb.2:
	ldr	x9, [x0, #512]
	cmp	w2, #1
	b.lt	LBB3_13
; %bb.3:
	mov	w8, #0                          ; =0x0
	ldr	x17, [x0, #520]
	ldr	x12, [x0, #528]
	mov	x10, sp
	orr	x11, x10, #0x20
Lloh33:
	adrp	x14, __MergedGlobals@PAGE+64
Lloh34:
	add	x14, x14, __MergedGlobals@PAGEOFF+64
	add	x13, x10, #64
	ldp	q0, q1, [x14]
	ldp	q2, q3, [x14, #32]
	orr	x14, x10, #0x1
Lloh35:
	adrp	x15, __MergedGlobals@PAGE+96
Lloh36:
	add	x15, x15, __MergedGlobals@PAGEOFF+96
	mov	w16, #510                       ; =0x1fe
	movi.16b	v4, #1
	dup.4s	v5, w16
	b	LBB3_5
LBB3_4:                                 ;   in Loop: Header=BB3_5 Depth=1
	add	x1, x1, #2
	add	x9, x9, #16
	ldr	x12, [x0, #520]
	ldr	x17, [x0, #528]
	str	x17, [x0, #520]
	str	x12, [x0, #528]
	add	w8, w8, #1
	cmp	w8, w2
	b.eq	LBB3_13
LBB3_5:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB3_10 Depth 2
	ldrb	w3, [x1]
	ldrb	w4, [x1, #1]
	add	x7, x12, #256
	add	x6, x17, #256
	cmp	x12, x6
	ccmp	x17, x7, #2, lo
	cset	w5, lo
	cmp	x10, x6
	ccmp	x17, x13, #2, lo
	cset	w6, lo
	cmp	x10, x7
	ccmp	x12, x13, #2, lo
	b.lo	LBB3_9
; %bb.6:                                ;   in Loop: Header=BB3_5 Depth=1
	tbnz	w5, #0, LBB3_9
; %bb.7:                                ;   in Loop: Header=BB3_5 Depth=1
	cbnz	w6, LBB3_9
; %bb.8:                                ;   in Loop: Header=BB3_5 Depth=1
	dup.16b	v6, w4
	dup.16b	v7, w3
	eor.16b	v16, v0, v7
	eor.16b	v17, v2, v6
	uaddl.8h	v18, v17, v16
	uaddl2.8h	v16, v17, v16
	ldp	q19, q17, [x17, #32]
	usubw2.4s	v20, v5, v16
	usubw.4s	v21, v5, v16
	usubw2.4s	v22, v5, v18
	ldp	q24, q23, [x17]
	usubw.4s	v25, v5, v18
	uaddw.4s	v26, v24, v18
	uaddw2.4s	v27, v23, v18
	ldp	q29, q28, [x17, #128]
	uaddw.4s	v30, v19, v16
	uaddw2.4s	v31, v17, v16
	add.4s	v8, v25, v29
	ldp	q10, q9, [x17, #160]
	add.4s	v19, v21, v19
	add.4s	v21, v21, v10
	add.4s	v17, v20, v17
	add.4s	v20, v20, v9
	uaddw2.4s	v9, v9, v16
	uaddw.4s	v16, v10, v16
	add.4s	v10, v22, v28
	add.4s	v22, v22, v23
	uaddw2.4s	v23, v28, v18
	add.4s	v24, v25, v24
	uaddw.4s	v18, v29, v18
	cmhi.4s	v25, v31, v20
	cmhi.4s	v28, v30, v21
	uzp1.8h	v25, v28, v25
	cmhi.4s	v28, v27, v10
	cmhi.4s	v29, v26, v8
	uzp1.8h	v28, v29, v28
	uzp1.16b	v25, v28, v25
	umin.4s	v28, v31, v20
	umin.4s	v29, v17, v9
	cmhi.4s	v17, v17, v9
	umin.4s	v20, v30, v21
	umin.4s	v21, v19, v16
	umin.4s	v30, v27, v10
	umin.4s	v31, v22, v23
	umin.4s	v26, v26, v8
	umin.4s	v27, v24, v18
	cmhi.4s	v16, v19, v16
	and.16b	v8, v25, v4
	uzp1.8h	v16, v16, v17
	cmhi.4s	v17, v22, v23
	mov	x3, x12
	st2.4s	{ v26, v27 }, [x3], #32
	cmhi.4s	v18, v24, v18
	uzp1.8h	v17, v18, v17
	st2.4s	{ v30, v31 }, [x3]
	uzp1.16b	v16, v17, v16
	add	x3, x12, #64
	st2.4s	{ v20, v21 }, [x3]
	and.16b	v9, v16, v4
	add	x3, x12, #96
	st2.4s	{ v28, v29 }, [x3]
	st2.16b	{ v8, v9 }, [x10]
	ldp	q18, q16, [x17, #96]
	ldp	q26, q25, [x17, #64]
	ldp	q19, q17, [x17, #224]
	ldp	q27, q24, [x17, #192]
	eor.16b	v7, v1, v7
	eor.16b	v6, v3, v6
	uaddl.8h	v28, v6, v7
	uaddl2.8h	v20, v6, v7
	usubw2.4s	v22, v5, v20
	usubw.4s	v29, v5, v20
	usubw2.4s	v30, v5, v28
	usubw.4s	v31, v5, v28
	uaddw.4s	v6, v26, v28
	uaddw2.4s	v7, v25, v28
	add.4s	v21, v31, v27
	add.4s	v23, v30, v24
	add.4s	v25, v30, v25
	add.4s	v26, v31, v26
	umin.4s	v30, v6, v21
	uaddw.4s	v27, v27, v28
	umin.4s	v31, v26, v27
	add	x17, x12, #128
	st2.4s	{ v30, v31 }, [x17]
	umin.4s	v30, v7, v23
	uaddw2.4s	v24, v24, v28
	umin.4s	v31, v25, v24
	add	x17, x12, #160
	st2.4s	{ v30, v31 }, [x17]
	uaddw.4s	v28, v18, v20
	add.4s	v30, v29, v19
	add.4s	v18, v29, v18
	umin.4s	v8, v28, v30
	uaddw.4s	v19, v19, v20
	umin.4s	v9, v18, v19
	add	x17, x12, #192
	st2.4s	{ v8, v9 }, [x17]
	uaddw2.4s	v29, v16, v20
	add.4s	v31, v22, v17
	add.4s	v16, v22, v16
	uaddw2.4s	v17, v17, v20
	umin.4s	v8, v29, v31
	umin.4s	v9, v16, v17
	add	x17, x12, #224
	st2.4s	{ v8, v9 }, [x17]
	cmhi.4s	v20, v29, v31
	cmhi.4s	v22, v28, v30
	uzp1.8h	v20, v22, v20
	cmhi.4s	v7, v7, v23
	cmhi.4s	v6, v6, v21
	uzp1.8h	v6, v6, v7
	uzp1.16b	v6, v6, v20
	cmhi.4s	v7, v16, v17
	cmhi.4s	v16, v18, v19
	uzp1.8h	v7, v16, v7
	cmhi.4s	v16, v25, v24
	cmhi.4s	v17, v26, v27
	uzp1.8h	v16, v17, v16
	uzp1.16b	v7, v16, v7
	and.16b	v16, v6, v4
	and.16b	v17, v7, v4
	st2.16b	{ v16, v17 }, [x11]
	b	LBB3_11
LBB3_9:                                 ;   in Loop: Header=BB3_5 Depth=1
	add	x17, x17, #128
	add	x5, x12, #4
	mov	x6, x14
	mov	x7, x15
	mov	w19, #32                        ; =0x20
LBB3_10:                                ;   Parent Loop BB3_5 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldur	w20, [x17, #-128]
	ldr	w21, [x17], #4
	ldurb	w22, [x7, #-32]
	ldrb	w23, [x7], #1
	eor	w22, w22, w3
	eor	w23, w23, w4
	add	w22, w23, w22
	sub	w23, w16, w22
	add	w24, w22, w20
	add	w25, w23, w21
	cmp	w24, w25
	cset	w26, hi
	csel	w24, w24, w25, lo
	add	w20, w23, w20
	add	w21, w22, w21
	cmp	w20, w21
	cset	w22, hi
	csel	w20, w20, w21, lo
	stur	w24, [x5, #-4]
	sturb	w26, [x6, #-1]
	str	w20, [x5], #8
	strb	w22, [x6], #2
	subs	x19, x19, #1
	b.ne	LBB3_10
LBB3_11:                                ;   in Loop: Header=BB3_5 Depth=1
	ldrb	w17, [sp]
	cmp	w17, #0
	cset	w17, ne
	ldrb	w3, [sp, #1]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #1
	ldrb	w3, [sp, #2]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #2
	ldrb	w3, [sp, #3]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #3
	ldrb	w3, [sp, #4]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #4
	ldrb	w3, [sp, #5]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #5
	ldrb	w3, [sp, #6]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #6
	ldrb	w3, [sp, #7]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #7
	ldrb	w3, [sp, #8]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #8
	ldrb	w3, [sp, #9]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #9
	ldrb	w3, [sp, #10]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #10
	ldrb	w3, [sp, #11]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #11
	ldrb	w3, [sp, #12]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #12
	ldrb	w3, [sp, #13]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #13
	ldrb	w3, [sp, #14]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #14
	ldrb	w3, [sp, #15]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #15
	ldrb	w3, [sp, #16]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #16
	ldrb	w3, [sp, #17]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #17
	ldrb	w3, [sp, #18]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #18
	ldrb	w3, [sp, #19]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #19
	ldrb	w3, [sp, #20]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #20
	ldrb	w3, [sp, #21]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #21
	ldrb	w3, [sp, #22]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #22
	ldrb	w3, [sp, #23]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #23
	ldrb	w3, [sp, #24]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #24
	ldrb	w3, [sp, #25]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #25
	ldrb	w3, [sp, #26]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #26
	ldrb	w3, [sp, #27]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #27
	ldrb	w3, [sp, #28]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #28
	ldrb	w3, [sp, #29]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #29
	ldrb	w3, [sp, #30]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #30
	ldrb	w3, [sp, #31]
	cmp	w3, #0
	cset	w3, ne
	orr	x17, x17, x3, lsl #31
	ldrb	w3, [sp, #32]
	cmp	w3, #0
	cset	w3, ne
	ldrb	w4, [sp, #33]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #1
	ldrb	w4, [sp, #34]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #2
	ldrb	w4, [sp, #35]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #3
	ldrb	w4, [sp, #36]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #4
	ldrb	w4, [sp, #37]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #5
	ldrb	w4, [sp, #38]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #6
	ldrb	w4, [sp, #39]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #7
	ldrb	w4, [sp, #40]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #8
	ldrb	w4, [sp, #41]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #9
	ldrb	w4, [sp, #42]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #10
	ldrb	w4, [sp, #43]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #11
	ldrb	w4, [sp, #44]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #12
	ldrb	w4, [sp, #45]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #13
	ldrb	w4, [sp, #46]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #14
	ldrb	w4, [sp, #47]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #15
	ldrb	w4, [sp, #48]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #16
	ldrb	w4, [sp, #49]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #17
	ldrb	w4, [sp, #50]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #18
	ldrb	w4, [sp, #51]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #19
	ldrb	w4, [sp, #52]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #20
	ldrb	w4, [sp, #53]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #21
	ldrb	w4, [sp, #54]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #22
	ldrb	w4, [sp, #55]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #23
	ldrb	w4, [sp, #56]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #24
	ldrb	w4, [sp, #57]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #25
	ldrb	w4, [sp, #58]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #26
	ldrb	w4, [sp, #59]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #27
	ldrb	w4, [sp, #60]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #28
	ldrb	w4, [sp, #61]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #29
	ldrb	w4, [sp, #62]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #30
	ldrb	w4, [sp, #63]
	cmp	w4, #0
	cset	w4, ne
	orr	x3, x3, x4, lsl #31
	stp	x17, x3, [x9]
	ldp	q7, q17, [x12, #64]
	ldp	q16, q18, [x12]
	ldp	q19, q20, [x12, #96]
	ldp	q21, q22, [x12, #32]
	ldr	d6, [x12, #128]
	fmov	w17, s6
	mov.s	w3, v6[1]
	ldur	q23, [x12, #168]
	ldur	q24, [x12, #136]
	ldur	q25, [x12, #184]
	ldur	q26, [x12, #152]
	ldur	q27, [x12, #216]
	ldur	q28, [x12, #200]
	ldur	q29, [x12, #232]
	ldp	w4, w5, [x12, #248]
	umin.4s	v20, v22, v20
	umin.4s	v17, v18, v17
	umin.4s	v17, v17, v20
	umin.4s	v18, v21, v19
	umin.4s	v7, v16, v7
	umin.4s	v7, v7, v18
	umin.4s	v7, v7, v17
	umin.4s	v17, v26, v25
	umin.4s	v18, v24, v23
	umin.4s	v17, v18, v17
	umin.4s	v7, v7, v17
	umin.4s	v17, v28, v27
	umin.4s	v7, v7, v17
	umin.4s	v7, v7, v29
	uminv.4s	s7, v7
	fmov	w6, s7
	cmp	w6, w4
	csel	w4, w6, w4, lo
	cmp	w5, w17
	csel	w17, w5, w17, lo
	cmp	w4, w17
	csel	w17, w4, w17, lo
	cmp	w17, w3
	csel	w17, w17, w3, lo
	cbz	w17, LBB3_4
; %bb.12:                               ;   in Loop: Header=BB3_5 Depth=1
	add	x3, x12, #4
	ld1.s	{ v16 }[1], [x3]
	ldr	d7, [x12, #8]
	mov.d	v16[1], v7[0]
	dup.4s	v7, w17
	sub.4s	v16, v16, v7
	ldp	q17, q18, [x12, #16]
	sub.4s	v17, v17, v7
	stp	q16, q17, [x12]
	sub.4s	v16, v18, v7
	ldp	q17, q18, [x12, #48]
	sub.4s	v17, v17, v7
	stp	q16, q17, [x12, #32]
	sub.4s	v16, v18, v7
	ldp	q17, q18, [x12, #80]
	sub.4s	v17, v17, v7
	stp	q16, q17, [x12, #64]
	sub.4s	v16, v18, v7
	ldr	d17, [x12, #136]
	mov.d	v6[1], v17[0]
	ldr	q17, [x12, #112]
	sub.4s	v17, v17, v7
	stp	q16, q17, [x12, #96]
	sub.4s	v6, v6, v7
	ldp	q16, q17, [x12, #144]
	sub.4s	v16, v16, v7
	stp	q6, q16, [x12, #128]
	sub.4s	v6, v17, v7
	ldp	q16, q17, [x12, #176]
	sub.4s	v16, v16, v7
	stp	q6, q16, [x12, #160]
	sub.4s	v6, v17, v7
	ldp	q16, q17, [x12, #208]
	sub.4s	v16, v16, v7
	stp	q6, q16, [x12, #192]
	sub.4s	v6, v17, v7
	ldr	q16, [x12, #240]
	sub.4s	v7, v16, v7
	stp	q6, q7, [x12, #224]
	b	LBB3_4
LBB3_13:
	mov	w8, #0                          ; =0x0
	str	x9, [x0, #512]
LBB3_14:
	ldr	x9, [sp, #72]
Lloh37:
	adrp	x10, ___stack_chk_guard@GOTPAGE
Lloh38:
	ldr	x10, [x10, ___stack_chk_guard@GOTPAGEOFF]
Lloh39:
	ldr	x10, [x10]
	cmp	x10, x9
	b.ne	LBB3_16
; %bb.15:
	mov	x0, x8
	sub	sp, x29, #96
	ldp	x29, x30, [sp, #96]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #80]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #64]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #48]             ; 16-byte Folded Reload
	ldp	x26, x25, [sp, #32]             ; 16-byte Folded Reload
	ldp	d9, d8, [sp, #16]               ; 16-byte Folded Reload
	ldp	d11, d10, [sp], #112            ; 16-byte Folded Reload
	ret
LBB3_16:
	bl	___stack_chk_fail
	.loh AdrpLdrGotLdr	Lloh30, Lloh31, Lloh32
	.loh AdrpAdd	Lloh35, Lloh36
	.loh AdrpAdd	Lloh33, Lloh34
	.loh AdrpLdrGotLdr	Lloh37, Lloh38, Lloh39
	.cfi_endproc
                                        ; -- End function
	.globl	_chainback_viterbi27            ; -- Begin function chainback_viterbi27
	.p2align	2
_chainback_viterbi27:                   ; @chainback_viterbi27
	.cfi_startproc
; %bb.0:
	cbz	w2, LBB4_3
; %bb.1:
	ldr	x8, [x0, #536]
	ubfiz	w9, w3, #2, #6
	mov	w10, w2
	sub	x10, x10, #1
LBB4_2:                                 ; =>This Inner Loop Header: Depth=1
	add	x11, x8, w10, uxtw #4
	lsr	w12, w9, #7
	add	x11, x11, w12, uxtw #3
	ldr	x11, [x11, #96]
	ubfx	w12, w9, #2, #5
	lsr	x11, x11, x12
	ubfiz	w11, w11, #7, #1
	orr	w9, w11, w9, lsr #1
	ubfx	x11, x10, #3, #29
	strb	w9, [x1, x11]
	mov	w11, w10
	sub	x10, x10, #1
	cbnz	x11, LBB4_2
LBB4_3:
	mov	w0, #0                          ; =0x0
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_delete_viterbi27               ; -- Begin function delete_viterbi27
	.p2align	2
_delete_viterbi27:                      ; @delete_viterbi27
	.cfi_startproc
; %bb.0:
	cbz	x0, LBB5_2
; %bb.1:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	ldr	x8, [x0, #536]
	mov	x19, x0
	mov	x0, x8
	bl	_free
	mov	x0, x19
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	b	_free
LBB5_2:
	ret
	.cfi_endproc
                                        ; -- End function
	.p2align	2                               ; -- Begin function create_viterbi27.cold.1
_create_viterbi27.cold.1:               ; @create_viterbi27.cold.1
	.cfi_startproc
; %bb.0:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
Lloh40:
	adrp	x0, l_.str.1@PAGE
Lloh41:
	add	x0, x0, l_.str.1@PAGEOFF
	bl	_perror
	ldr	x0, [x19]
	bl	_free
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
	.loh AdrpAdd	Lloh40, Lloh41
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"posix_memalign for v27_state failed"

l_.str.1:                               ; @.str.1
	.asciz	"posix_memalign for decisions buffer failed"

	.section	__TEXT,__literal16,16byte_literals
	.p2align	4, 0x0                          ; @.memset_pattern
l_.memset_pattern:
	.long	131070                          ; 0x1fffe
	.long	131070                          ; 0x1fffe
	.long	131070                          ; 0x1fffe
	.long	131070                          ; 0x1fffe

.zerofill __DATA,__bss,__MergedGlobals,128,6 ; @_MergedGlobals
.subsections_via_symbols
