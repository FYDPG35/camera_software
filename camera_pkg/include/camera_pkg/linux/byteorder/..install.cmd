cmd_../rohith//include/linux/byteorder/.install := perl scripts/headers_install.pl /home/android-build/imx6/ankaa/kk-4.4.2/kernel_imx/include/linux/byteorder ../rohith//include/linux/byteorder arm big_endian.h little_endian.h; perl scripts/headers_install.pl /home/android-build/imx6/ankaa/kk-4.4.2/kernel_imx/include/linux/byteorder ../rohith//include/linux/byteorder arm ; for F in ; do echo "\#include <asm-generic/$$F>" > ../rohith//include/linux/byteorder/$$F; done; touch ../rohith//include/linux/byteorder/.install
