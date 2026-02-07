# SnakeOS
![Github stars](https://img.shields.io/github/stars/WangYanbo51/SnakeOS.svg)

一个简单的OS，包含UEFI Loader和ELF内核。

# 内容
- [使用方法](#使用方法)
  - [准备](#准备)
  - [构建](#构建)
  - [编译](#编译)
  - [在虚拟机中运行](#在虚拟机中运行)

# 使用方法
### 准备
  * EDKII环境
  * linux下使用 clang（或x86_64-elf-gcc）

### 构建
```
cd edk2
git clone https://github.com/WangYanbo51/SnakeOS.git
mv SnakeOS SnakeOSPkg
cd Conf
vim target.txt
```
在文件中的TARGET_PLATFORM条目下加上
```
ACTIVE_PLATFORM         = SnakeOSPkg/SnakeOSPkg.dsc
```
并确保TARGET_ARCH条目为X64
```
TARGET_ARCH           = X64
```

### 编译
修改Makefile中的目录（makefile文件内容混乱致歉）
然后运行
```
cd edk2/SnakeOS
make setup
build
make kernel
```

### 在虚拟机中运行
需提前准备虚拟机UEFI启动所需要的bios.bin文件
将target.txt文件中的TARGET_PLATFORM条目下添加
```
ACTIVE_PLATFORM       = OvmfPkg/OvmfPkgX64.dsc
```
在edk2目录下
```
. ./edksetup.sh BaseTools
build
```
等待编译完成后，从Build/OvmfX64/DEBUG_GCC5/FV中取出OVMF.fd文件
```
cd Build/OvmfX64/DEBUG_GCC5/FV  # 此处应依照你的对应编译模式和编译工具链替换目录名
cp OVMF.fd /path/to/edk2/../run-ovmf/bios.bin # 此处应该修改为你的QEMU工作目录
```
拷贝make编译文件，在某一目录（hda）下建立以下文件结构：
```
hda
├── EFI
│   └── BOOT
│       └── BOOTX64.efi
└── kernel.elf
```
运行
```
qemu-system-x86_64 -pflash bios.bin -hda fat:rw:hda -net none
```
