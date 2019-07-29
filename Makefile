BUILDDIR = build-Desktop_Qt_5_12_2_GCC_64bit-Debug/

all:
	$(MAKE) -C $(BUILDDIR) $@

initial:
	mkdir $(BUILDDIR)
	cd $(BUILDDIR)
	qmake-qt5 ..

%:
	$(MAKE) -C $(BUILDDIR) $@
