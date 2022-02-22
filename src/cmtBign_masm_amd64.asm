;cmtBign assembly support
;amd64 version for masm

.code

;cx dx r8 r9
;cx->in
;dx->out
;void cmtBignNegSoft64(cmtBigNum* in, cmtBigNum* out)
cmtBignNegSoft64 proc
	;save context
	push rbp
	mov rbp, rsp
	push rbx ;[rbp-8h]
	;parameter
	mov rbx, rdx;out=rbx

	;NegLoop prepare
	xor rsi, rsi
	xor rdi, rdi
	stc
	cBNS64_NegLoopStart:
	;rax = in->data[rsi] //read
		;check offset
		mov rax, qword ptr [rcx+8] ;rax = in->size
		cmp rsi, rax
		je cBNS64_NegLoopEnd ;in out of range
	mov rax, qword ptr [rcx] ;rax = in->data
	mov rax, qword ptr [rax+rsi*8] ;rax = in->data[rsi]
	;rsi++
	inc rsi
	;rax = ~rax + carry //按位取反再加一
	not rax ;~rax
	adc rax, 0 ;rax += CF
	;out->data[rdi] = rax //write
		;check offset
		mov rdx, qword ptr [rbx+8] ;rdx = out->size
		cmp rdi, rdx
		je cBNS64_NegLoopEnd ;in out of range
	mov rdx, qword ptr [rbx] ;rdx = out->data
	mov qword ptr [rdx+rdi*8], rax ;out->data[rdi] = rax
	;rdi++
	inc rdi
	;loop tail jump
	jmp cBNS64_NegLoopStart
	cBNS64_NegLoopEnd:

	;restore context
	;lea rsp, [rbp-8h] //rsp not change (no local var)
	pop rbx
	mov rsp, rbp
	pop rbp
	ret
cmtBignNegSoft64 endp

end