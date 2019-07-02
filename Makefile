# Set the location of riscv64-unknown-elf-* and openocd if it is not installed system-wide
BIN_DIR       ?= .
SDK_DIR       ?= $(abspath freedom-e-sdk)
BSP_DIR       := $(SDK_DIR)/bsp/sifive-hifive1
PATH          := $(BIN_DIR):$(PATH)
CROSS_COMPILE ?= riscv64-unknown-elf
CC            := $(CROSS_COMPILE)-gcc
AR            := $(CROSS_COMPILE)-ar
RANLIB        := $(CROSS_COMPILE)-ranlib
OBJDUMP       := $(CROSS_COMPILE)-objdump

TARGETS 	  := aes128tables aes128ctrbs chacha20 keccakf1600
ELFTARGETS    := $(foreach t,$(TARGETS),$(t)/$(t).elf)

.PHONY: all
all: $(ELFTARGETS)

# Required for libmetal.mk
CONFIGURATION     := debug
LINK_TARGET       := default
METAL_SOURCE_PATH := $(SDK_DIR)/freedom-metal

include $(BSP_DIR)/settings.mk
include $(SDK_DIR)/scripts/libmetal.mk

RISCV_CFLAGS  += -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
RISCV_CFLAGS  += -ffunction-sections -fdata-sections
RISCV_CFLAGS  += --specs=nano.specs
RISCV_LDFLAGS += -Wl,--gc-sections
RISCV_LDFLAGS += -nostartfiles
RISCV_LDFLAGS += -L$(sort $(dir $(abspath $(filter %.a,$^)))) -T$(abspath $(filter %.lds,$^))
RISCV_LDLIBS  += -Wl,--start-group -lc -lgcc -lmetal -lmetal-gloss -Wl,--end-group
CFLAGS        += $(RISCV_CFLAGS)
CFLAGS        += -O3 -fno-builtin-printf -std=gnu17 -Wall -Wextra -Wpedantic -Wvla -Wmissing-prototypes
CFLAGS        += -Iutil
LDFLAGS       += $(RISCV_LDFLAGS)
LDLIBS        += $(RISCV_LDLIBS)
METAL_DEPS    := $(BSP_DIR)/install/lib/$(CONFIGURATION)/libmetal.a \
	             $(BSP_DIR)/install/lib/$(CONFIGURATION)/libmetal-gloss.a \
				 $(BSP_DIR)/metal.$(LINK_TARGET).lds

#TODO Do this in a static pattern rule for $(ELFTARGETS)
aes128tables/aes128tables.elf: $(METAL_DEPS) $(wildcard aes128tables/*.c) $(wildcard aes128tables/*.h) $(wildcard aes128tables/*.S) $(wildcard util/*.S)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(filter %.c,$^) $(filter %.S,$^)

aes128ctrbs/aes128ctrbs.elf: $(METAL_DEPS) $(wildcard aes128ctrbs/*.c) $(wildcard aes128ctrbs/*.h) $(wildcard aes128ctrbs/*.S) $(wildcard util/*.S)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(filter %.c,$^) $(filter %.S,$^)

chacha20/chacha20.elf: $(METAL_DEPS) $(wildcard chacha20/*.c) $(wildcard chacha20/*.h) $(wildcard chacha20/*.S) $(wildcard util/*.S)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(filter %.c,$^) $(filter %.S,$^)

keccakf1600/keccakf1600.elf: $(METAL_DEPS) $(wildcard keccakf1600/*.c) $(wildcard keccakf1600/*.h) $(wildcard keccakf1600/*.S) $(wildcard util/*.S)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(filter %.c,$^) $(filter %.S,$^)

.PHONY: clean
clean:
	-rm -f $(ELFTARGETS)

.PHONY: upload
upload: $(PROGRAM_ELF)
	scripts/upload --elf $(PROGRAM_ELF) --openocd $(RISCV_OPENOCD) --gdb $(RISCV_GDB) --openocd-config bsp/$(TARGET)/openocd.cfg
