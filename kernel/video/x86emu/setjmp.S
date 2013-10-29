    .globl	setjmp
setjmp:
    movl	4(%esp),%ecx
    movl	4(%ebp), %edx
    movl	%edx, 0(%ecx)
    movl	%ebx, 4(%ecx)

    	addl	$4,%ebp
    	movl	%ebp, 8(%ecx)
    	subl	$4,%ebp

    	movl	(%ebp),%edx
    	movl	%edx,12(%ecx)
    movl	%esi,16(%ecx)
    movl	%edi,20(%ecx)
    movl	%eax,24(%ecx)
    xorl	%eax,%eax
    ret

    .globl	longjmp
longjmp:

    movl	4(%esp),%edx
    movl	8(%esp),%eax
    movl	0(%edx),%ecx
    movl	4(%edx),%ebx
    movl	8(%edx),%esp
    movl	12(%edx),%ebp
    movl	16(%edx),%esi
    movl	20(%edx),%edi
    testl	%eax,%eax
    jnz	1f
    incl	%eax
1:	movl	%ecx,0(%esp)
    ret
