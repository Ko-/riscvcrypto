#!/bin/sh
GDB_PORT=3333
SDK_DIR="freedom-e-sdk"
OPENOCD=$(which openocd)
RISCV_GDB=$(which riscv64-unknown-elf-gdb)

if [ -z "$1" ]; then
    echo "$0 <binary>"
    exit 0
fi

if [ ! -f "$1" ]; then
    echo "$1 is not a valid file"
    exit 1
fi

if [ ! -x "$OPENOCD" ]; then
    echo "OpenOCD does not seem to exist. Please add that to your PATH variable."
    exit 1
fi

if [ ! -x "$RISCV_GDB" ]; then
    echo "risv64-unknown-elf-gdb does not seem to exist. Please add that to your PATH variable."
    exit 1
fi

${SDK_DIR}/scripts/upload --elf "$1" --openocd "$OPENOCD" --gdb "$RISCV_GDB" --openocd-config "$SDK_DIR/bsp/sifive-hifive1/openocd.cfg" &&
    echo "Successfully uploaded $1"
