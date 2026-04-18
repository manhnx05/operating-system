# VirtualBox Setup Guide for Quadratic OS

## Vấn đề
VirtualBox mặc định sử dụng UEFI boot, nhưng Quadratic OS sử dụng BIOS/Legacy boot (Multiboot).

## Giải pháp: Tắt EFI trong VirtualBox

### Bước 1: Tạo Virtual Machine mới

1. Mở VirtualBox
2. Click **New** (hoặc Machine → New)
3. Cấu hình:
   - **Name:** Quadratic OS
   - **Type:** Other
   - **Version:** Other/Unknown (32-bit)
   - **Memory:** 128 MB (tối thiểu) hoặc 256 MB
   - **Hard disk:** Do not add a virtual hard disk (không cần)

### Bước 2: Tắt EFI (QUAN TRỌNG!)

1. Chọn VM "Quadratic OS" vừa tạo
2. Click **Settings** (hoặc Machine → Settings)
3. Vào tab **System**
4. Trong tab **Motherboard**:
   - **UNCHECK** (bỏ chọn) "Enable EFI (special OSes only)"
   - Boot Order: Chỉ check "Optical"
   - Chipset: PIIX3 hoặc ICH9
   - Pointing Device: PS/2 Mouse

### Bước 3: Mount ISO

1. Vẫn trong **Settings**, vào tab **Storage**
2. Click vào **Empty** dưới "Controller: IDE"
3. Click icon đĩa CD bên phải
4. Chọn **Choose a disk file...**
5. Browse và chọn file `quadraticos.iso`
6. Click **OK**

### Bước 4: Khởi động

1. Click **Start** để boot VM
2. OS sẽ tự động boot vào Quadratic OS

## Cấu hình khuyến nghị

### System
- **Base Memory:** 128-256 MB
- **Processors:** 1 CPU
- **Enable PAE/NX:** Không cần
- **Hardware Clock in UTC Time:** Không quan trọng

### Display
- **Video Memory:** 16 MB
- **Graphics Controller:** VBoxVGA hoặc VMSVGA
- **Acceleration:** Không cần

### Storage
- **Controller:** IDE
- **Type:** CD/DVD
- **File:** quadraticos.iso

## Troubleshooting

### Lỗi: "No bootable medium found"
**Nguyên nhân:** EFI vẫn đang bật

**Giải pháp:**
1. Tắt VM
2. Settings → System → Motherboard
3. **UNCHECK** "Enable EFI"
4. Khởi động lại

### Lỗi: "FATAL: No bootable medium found"
**Nguyên nhân:** ISO chưa được mount

**Giải pháp:**
1. Settings → Storage
2. Chọn Controller: IDE
3. Mount quadraticos.iso
4. Đảm bảo Boot Order có "Optical" được check

### Lỗi: "Operating system not found"
**Nguyên nhân:** GRUB không tìm thấy kernel

**Giải pháp:**
1. Rebuild ISO: `docker run --rm -v ${PWD}:/root/env quadratic-os-builder make clean && make`
2. Mount lại ISO mới vào VM

## Kết quả mong đợi

Khi boot thành công, bạn sẽ thấy:

```
Quadratic OS Kernel Loaded!

Example equation: x^2 - 3x + 2 = 0
Two real roots: x1 = 2.00, x2 = 1.00


Enter your own equation (ax^2 + bx + c = 0):
a: _
```

## Alternative: Sử dụng QEMU (Đơn giản hơn)

Nếu VirtualBox vẫn gặp vấn đề, sử dụng QEMU:

```bash
# Windows
qemu-system-i386 -cdrom quadraticos.iso

# Linux/Mac
qemu-system-i386 -cdrom quadraticos.iso
```

QEMU không cần cấu hình gì, tự động nhận diện Multiboot.
