cmd_../rohith//include/asm/.install := perl scripts/headers_install.pl /home/android-build/imx6/ankaa/kk-4.4.2/kernel_imx/arch/arm/include/asm ../rohith//include/asm arm a.out.h auxvec.h bitsperlong.h byteorder.h errno.h fcntl.h hwcap.h ioctl.h ioctls.h ipcbuf.h mman.h msgbuf.h param.h poll.h posix_types.h ptrace.h resource.h sembuf.h setup.h shmbuf.h sigcontext.h siginfo.h signal.h socket.h sockios.h stat.h statfs.h swab.h termbits.h termios.h types.h unistd.h; perl scripts/headers_install.pl /home/android-build/imx6/ankaa/kk-4.4.2/kernel_imx/arch/arm/include/asm ../rohith//include/asm arm ; for F in ; do echo "\#include <asm-generic/$$F>" > ../rohith//include/asm/$$F; done; touch ../rohith//include/asm/.install
