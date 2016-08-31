#DEBUG=-DDEBUG

INSTALL_PREFIX=/usr/local
BIN_TARGET_DIR=/bin
LIB_TARGET_DIR=/lib/libtipu

ifeq (y,$(findstring y,$(CROSS)))
	SYSROOT=/opt/b2qt/1.7/sysroots/armv7at2hf-vfp-neon-poky-linux-gnueabi
	CROSS_PATH=/opt/b2qt/1.7/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/
	CROSS_PREFIX=arm-poky-linux-gnueabi
	CROSS_FLAGS=-march=armv7-a -marm -mthumb-interwork -mfloat-abi=hard -mfpu=neon --sysroot=$(SYSROOT)
	CC=$(CROSS_PATH)$(CROSS_PREFIX)-gcc $(CROSS_FLAGS)
	LD=$(CROSS_PATH)$(CROSS_PREFIX)-ld
endif

CFLAGS=$(DEBUG) -pedantic -Wall -fPIC -g -I.
LDFLAGS=-shared

SRC=*.c *.h
LIB_A=libtipu.a
LIB_S=libtipu.so.1.0.1
LIBS_S=libtipu.so.1 libtipu.so.1.0.1
LIBS=$(LIB_A) $(LIBS_S)
OBJS=libtipu.o tipu.o
BIN=tipu
BINS=tipu tipu-shared tipu-static
TAGS=tags

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.a: %.o
	$(AR) $(ARFLAGS) $@ $<

%.so.1.0.1: %.o
	$(LD) $(LDFLAGS) -soname $(subst .so.1.0.1,.so.1,$@) -o $@ $<

%.so.1: %.so.1.0.1
	ln -sf $< $@

%.so: %.so.1
	ln -sf $< $@

.PHONY: all
all: $(LIBS) $(BIN)

$(BIN): $(BIN).o $(LIB_S) $(LIB_A)
	$(CC) $(BIN).o $(LIB_S) -o $@-shared
	$(CC) $(BIN).o $(LIB_A) -o $@-static
	ln -sf $@-shared $@

.PHONY: tags
tags: $(SRC)
	ctags -f $(TAGS) $(SRC)

.PHONY: install
install: $(BIN) $(LIBS)
	cp -a $(BINS) $(INSTALL_PREFIX)$(BIN_TARGET_DIR)
	install -m 755 -d $(INSTALL_PREFIX)$(LIB_TARGET_DIR)
	cp -a $(LIBS) $(INSTALL_PREFIX)$(LIB_TARGET_DIR)

.PHONY: uninstall
uninstall:
	rm -f $(foreach BIN,$(BINS),$(INSTALL_PREFIX)$(BIN_TARGET_DIR)/$(BIN))
	rm -f $(foreach LIB,$(LIBS),$(INSTALL_PREFIX)$(LIB_TARGET_DIR)/$(LIB))
	rmdir $(INSTALL_PREFIX)$(LIB_TARGET_DIR)

.PHONY: clean
clean:
	rm -f $(TAGS) $(LIBS) $(OBJS) $(BINS)
