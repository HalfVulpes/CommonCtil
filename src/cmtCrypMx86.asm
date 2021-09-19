;cmtCryp assembly support
;i386 version for masm
.model flat
.code

;cx dx
;void cmtRealRand(cmtUint64* buf, cmtUint64 len)
@cmtRealRand@8 proc
	push rbx
	mov ebx, ecx
	xor ecx, ecx

	;ebx = buf
	;edx = len
	;ecx = r
	cRR_GenLoopStart:
	cmp ecx, edx
	jae cRR_End
	;生成随机数
	cRR_GenRand:
	rdrand eax
	;检查结果，失败重来
	jnc cRR_GenRand
	;写入数组
	mov dword ptr [ebx + ecx*4], eax
	;i++
	inc ecx
	jmp cRR_GenLoopStart
	
	cRR_End:
	pop rbx
	ret
@cmtRealRand@8 endp

end