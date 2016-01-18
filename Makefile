dirs = lib module test

SUBDIRS := $(dirs)
clean_dirs:=$(addprefix _clean_, $(SUBDIRS))

.PHNOY: all $(SUBDIRS) clean

all: $(SUBDIRS)
$(SUBDIRS):
	make -C lib;
	make -C module;
	make -C test;


$(clean_dirs):
	$(MAKE) -C $(patsubst _clean_%,%,$@) clean
clean: $(clean_dirs)



