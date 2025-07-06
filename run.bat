

where qemu-system-x86_64 >nul 2>nul
if %ERRORLEVEL% == 0 (
    qemu-system-x86_64 -machine q35 -cpu qemu64 -m 256 -smp 2 -drive format=raw,file=os.iso -d cpu_reset,int,guest_errors -D qemu.log -monitor stdio
)