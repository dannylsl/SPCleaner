dirs = lib \
	   module \
	   test

MAKE=make

.PHONY: all clean $(dirs)
all: $(dirs)

clean: $(dirs)

$(dirs):
	$(MAKE) -C $@ $(MAKECMDGOALS)
