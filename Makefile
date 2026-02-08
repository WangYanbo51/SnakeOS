EFI_BUILD_PATH = ../Build/SnakeOSPkg/DEBUG_GCC5/X64/SnakeOS.efi
KERNEL_BIN = Kernel/kernel.elf
ISO_NAME = SnakeOS.iso
TMP_ISO = ./tmp_iso
EFI_IMG = efiboot.img

all: build kernel copy

kernel: $(wildcard Kernel/*.c)
	# x86_64-elf-gcc -O3 -ffreestanding -fno-stack-protector -mno-red-zone -fno-builtin -Wall -Wextra -nostdlib $^ -o Kernel/kernel.elf -e kernel_main
	clang -O2 $^ -fno-stack-protector -Wall -Wextra -nostdlib -o $(KERNEL_BIN) -e kernel_main
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

iso: all
	@echo "Creating ISO image..."
	mkdir -p $(TMP_ISO)/EFI/BOOT/
	cp $(EFI_BUILD_PATH) $(TMP_ISO)/EFI/BOOT/BOOTX64.EFI
	cp $(KERNEL_BIN) $(TMP_ISO)/kernel.elf
	rm -f $(EFI_IMG)
	dd if=/dev/zero of=$(EFI_IMG) bs=1M count=1
	mkfs.vfat -F 12 $(EFI_IMG)
	mmd -i $(EFI_IMG) ::/EFI ::/EFI/BOOT
	mcopy -i $(EFI_IMG) $(TMP_ISO)/EFI/BOOT/BOOTX64.EFI ::/EFI/BOOT/BOOTX64.EFI
	mcopy -i $(EFI_IMG) $(TMP_ISO)/kernel.elf ::/kernel.elf
	xorriso -as mkisofs \
		-R -J -V "SNAKEOS" \
		--graft-points \
		"boot.img=$(EFI_IMG)" \
		-e "boot.img" \
		-no-emul-boot \
		-isohybrid-gpt-basdat \
		-o $(ISO_NAME) \
		$(TMP_ISO)

copy:
	mkdir -p ~/EDKII/run-ovmf/hda/EFI/BOOT/
	cp ../Build/SnakeOSPkg/DEBUG_GCC5/X64/SnakeOS.efi ~/EDKII/run-ovmf/hda/EFI/BOOT/BOOTX64.efi
	cp Kernel/kernel.elf ~/EDKII/run-ovmf/hda/
	cp ../Build/SnakeOSPkg/DEBUG_GCC5/X64/SnakeOS.efi ./tmp_iso/EFI/BOOT/BOOTX64.efi
	cp Kernel/kernel.elf ./tmp_iso/

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
