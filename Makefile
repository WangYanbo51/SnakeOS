all: build kernel copy

kernel: $(wildcard Kernel/*.c)
	# x86_64-elf-gcc -O3 -ffreestanding -fno-stack-protector -mno-red-zone -fno-builtin -Wall -Wextra -nostdlib $^ -o Kernel/kernel.elf -e kernel_main
	clang -O3 $^ -fno-stack-protector -Wall -Wextra -nostdlib -o Kernel/kernel.elf -e kernel_main
	# objcopy -R .note.gnu.property -R .note.ABI-tag Kernel/kernel.elf Kernel/kernel.bin -O binary
	# nasm kernel.asm -f bin -o kernel.bin
	# x86_64-elf-gcc -O3 \
	#        -ffreestanding \
	#        -Wall -Wextra \
	#        -nostdlib \
	#        -fno-pie \
	#        -mcmodel=kernel \
	# 	   -e kernel_main \
	#        $^ -o Kernel/kernel.elf
build:
	build

setup:
	cd .. && . ./edksetup.sh BaseTools

# iso:
# 	mkdir -p tmp_iso/EFI/BOOT/
# 	cp ../Build/SnakeOSPkg/DEBUG_GCC5/X64/SnakeOS.efi tmp_iso/EFI/BOOT/BOOTX64.efi
# 	cp Kernel/kernel.elf tmp_iso/
# 	dd if=/dev/zero of=efiboot.img bs=1K count=64
# 	mkfs.vfat -F 32 efiboot.img
# 	mcopy -i efiboot.img -s tmp_iso/* ::/
# 	xorriso -as mkisofs \
# 		-R -f \
# 		-o SnakeOS.iso \
# 		--graft-points \
# 		"EFI/BOOT/BOOTX64.EFI=tmp_iso/EFI/BOOT/BOOTX64.efi" \
# 		"kernel.elf=tmp_iso/kernel.elf" \
# 		"boot.img=efiboot.img" \
# 		-hide "boot.img" \
# 		-hide-joliet "boot.img" \
# 		-e "boot.img" \
# 		-no-emul-boot

copy:
	cp ../Build/SnakeOSPkg/DEBUG_GCC5/X64/SnakeOS.efi ~/EDKII/run-ovmf/hda/EFI/BOOT/BOOTX64.efi
	cp Kernel/kernel.elf ~/EDKII/run-ovmf/hda/

run:
	cd ../../run-ovmf && qemu-system-x86_64 -pflash bios.bin -hda fat:rw:hda -net none

bear: clean
	rm -rf compile_commands.json
	bear -- make
	sed -i -z 's/      "-maccumulate-outgoing-args",\n//g' compile_commands.json

clean:
	rm -rf ../Build/*
	rm -rf Kernel/kernel.bin Kernel/kernel.o Kernel/kernel.elf
	rm -rf SnakeOS.iso efiboot.img

.PHONY: all
