# RISC-V Crypto

## Content
This repository contains optimized assembly implementations of various cryptographic primitives. They are part of a research paper with the title 'Efficient Cryptography on the RISC-V Architecture'.

 * `aes128tables` contains a table-based AES-128 implementation. This should only be used if you are aware of the risks of cache-based timing attacks and you can guarantee that this is of no concern for your specific use case.
 * `aes128ctrbs` contains a bitsliced AES-128-CTR implementation.
 * `chacha20` contains an implementation of the ChaCha20 stream cipher.
 * `keccakf1600` contains an implementation of the Keccak-f[1600] permutation.

## Compilation instructions

This code uses SiFive's Freedom E SDK that is included as git submodule. That
SDK requires that you have the [RISC-V GNU toolchain](https://github.com/riscv/riscv-gnu-toolchain) and [RISCV OpenOCD](https://github.com/riscv/riscv-openocd).
For the GNU toolchain, make sure that multilib is enabled such that you can
cross-compile for 32-bit targets. Then put both tools in you `$PATH` or edit
the Makefile and add their locations to `BIN_DIR`. Once that is done, simply
calling `make` should be sufficient to build 4 binaries in their respective
directories. These binaries target the HiFive1 development board.

## Using the binaries

Connect the HiFive1 over USB. In one terminal, execute `./watch.sh` on Linux.
This will execute `screen` on `/dev/ttyUSB1`. In another terminal, execute
`./upload.sh <dir/binary.elf>`. This uses `riscv64-unknown-elf-gdb` and `openocd`
to flash the binary to the HiFive1 board.
