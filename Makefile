all: jam.z64

V=1
SOURCE_DIR=src
BUILD_DIR=build
include $(N64_INST)/include/n64.mk


SRC = main.c rdl.c
OBJS = $(SRC:%.c=$(BUILD_DIR)/%.o)
DEPS = $(SRC:%.c=$(BUILD_DIR)/%.d)

libdragon:
	$(MAKE) -C ./libdragon install

jam.z64: libdragon
jam.z64: N64_ROM_TITLE="Jam"
jam.z64: CFLAGS=-Wno-error

$(BUILD_DIR)/jam.elf: $(OBJS)

clean:
	rm -f $(BUILD_DIR)/* jam.z64

-include $(DEPS)

.PHONY: all clean libdragon
