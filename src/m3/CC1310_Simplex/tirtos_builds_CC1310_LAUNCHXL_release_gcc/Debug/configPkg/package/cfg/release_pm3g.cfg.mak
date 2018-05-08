# invoke SourceDir generated makefile for release.pm3g
release.pm3g: .libraries,release.pm3g
.libraries,release.pm3g: package/cfg/release_pm3g.xdl
	$(MAKE) -f C:\Users\Malay\workspace_v8\tirtos_builds_CC1310_LAUNCHXL_release_gcc/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\Malay\workspace_v8\tirtos_builds_CC1310_LAUNCHXL_release_gcc/src/makefile.libs clean

