BUILDDIR = build-Desktop-Debug/

all:
	$(MAKE) -C $(BUILDDIR) $@

initial:
	mkdir build-Desktop-Debug
	cd build-Desktop-Debug
	qmake-qt5 ..

%:
	$(MAKE) -C $(BUILDDIR) $@
