cmd_../rohith//include/drm/.install := perl scripts/headers_install.pl /home/android-build/imx6/ankaa/kk-4.4.2/kernel_imx/include/drm ../rohith//include/drm arm drm.h drm_mode.h drm_sarea.h i810_drm.h i915_drm.h mga_drm.h nouveau_drm.h r128_drm.h radeon_drm.h savage_drm.h sis_drm.h via_drm.h vmwgfx_drm.h; perl scripts/headers_install.pl /home/android-build/imx6/ankaa/kk-4.4.2/kernel_imx/include/drm ../rohith//include/drm arm ; for F in ; do echo "\#include <asm-generic/$$F>" > ../rohith//include/drm/$$F; done; touch ../rohith//include/drm/.install
