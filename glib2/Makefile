all:
	(cd ext/glib2 && $(MAKE))
	$(MAKE) -f Makefile.lib

install:
	(cd ext/glib2 && $(MAKE) install)
	$(MAKE) -f Makefile.lib install

site-install:
	(cd ext/glib2 && $(MAKE) site-install)
	$(MAKE) -f Makefile.lib site-install

clean:
	(cd ext/glib2 && $(MAKE) clean)
	$(MAKE) -f Makefile.lib clean

distclean:
	(cd ext/glib2 && $(MAKE) distclean)
	$(MAKE) -f Makefile.lib distclean
	@rm -f Makefile.lib
