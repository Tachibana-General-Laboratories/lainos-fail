# Initial process execs /init.

#include "syscall.h"
#include "traps.h"


# execve(init, argv, envp)
.globl start
start:
  pushl $envp
  pushl $argv
  pushl $init
  pushl $0  // where caller pc would be
  movl $SYS_execve, %eax
  int $T_SYSCALL

# for(;;) exit();
exit:
  movl $SYS_exit, %eax
  int $T_SYSCALL
  jmp exit

# char init[] = "/init\0";
init:
  .string "/init\0"

# char *argv[] = { init, 0 };
.p2align 2
argv:
  .long init
  .long 0

.p2align 2
envp:
  .long 0
  .long 0

