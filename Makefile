# makefile
include config.mk


# common data
TARGET = kernel.elf
TARGET_32 = code32.elf
TARGET_64 = code64.elf

SRCS_64 = 
SRCS_32 = 
OBJS_64 = 
OBJS_32 = 
DATA_64 = 
DATA_32 = 
DEP_FILES_64 = 
DEP_FILES_32 = 


INCLUDE = .

#set each arch
ifeq ($(TARGET_ARCH), x86_64)
	CC = x86_64-elf-gcc
	OBJ_CPY = x86_64-elf-objcopy
	DATA_32 	= $(TARGET_64).o
	VPATH_64	= init arch/amd64
	VPATH_32	= arch/x86/boot
	CFLAGS_64	= -ffreestanding -fpic -fno-stack-protector -fshort-wchar -mno-red-zone -mgeneral-regs-only -mabi=ms -Wall -Wextra -Wpedantic -O3 
	CFLAGS_32	= -m32 -ffreestanding -fpic -fno-stack-protector -fshort-wchar -mno-red-zone -mgeneral-regs-only -mabi=ms -Wall -Wextra -Wpedantic -O3 
	LDFLAGS_64	= -nostdlib -shared -Wl,-T,amd64.lds -Wl,-Bsymbolic -Wl,-znocombreloc -lgcc
	LDFLAGS_32	= -nostdlib -shared -Wl,-T,x86.lds -Wl,-Bsymbolic -Wl,-znocombreloc -lgcc
	TARGET_32_FORMAT = elf32-x86-64
	TARGET_64_FORMAT = elf64-x86-64
	TARGET_FORMAT = elf64-x86-64
else
	$(error Unsupported TARGET_ARCH: $(TARGET_ARCH))
endif

#set each compress
ifeq ($(KERNEL_COMPRESS), NON)
	CFLAGS_32 += -D KERNEL_NONCOMPRESS
else
	$(error Unsupported KERNEL_COMPRESS: $(KERNEL_COMPRESS))
endif

#find all file
SRCS_64 += $(wildcard $(addsuffix /*.c, $(VPATH_64))) $(wildcard $(addsuffix /*.S, $(VPATH_64)))
SRCS_32 += $(wildcard $(addsuffix /*.c, $(VPATH_32))) $(wildcard $(addsuffix /*.S, $(VPATH_32)))
DEP_FILES_64 += $(patsubst %.S,%.d,$(patsubst %.c,%.d,$(SRCS_64)))
DEP_FILES_32 += $(patsubst %.S,%.d,$(patsubst %.c,%.d,$(SRCS_32)))

C_OBJS_64 += $(patsubst %.c,%.o,$(filter %.c, $(SRCS_64)))
C_OBJS_32 += $(patsubst %.c,%.o,$(filter %.c, $(SRCS_32)))
S_OBJS_64 += $(patsubst %.S,%.o,$(filter %.S, $(SRCS_64)))
S_OBJS_32 += $(patsubst %.S,%.o,$(filter %.S, $(SRCS_32)))

DIRS = obj $(addprefix obj/,$(sort $(dir $(C_OBJS_64)))) $(addprefix obj/,$(sort $(dir $(S_OBJS_64)))) $(addsuffix obj/,$(sort $(dir $(C_OBJS_32)))) $(addprefix obj/,$(sort $(dir $(S_OBJS_32)))) 



-include $(DEP_FILES_64)
-include $(DEP_FILES_32)


.PHONY: build
build: $(TARGET)

.PHONY: all
all: clean $(TARGET)

.PHONY: clean
clean:
	@echo "Cleaning up..."
	$(RM) $(TARGET) $(TARGET_32) $(TARGET_64)
	$(RM) $(TARGET_64).bin $(TARGET_64).o
	$(RM) -r obj/


$(TARGET): $(DIRS) $(TARGET_64) $(TARGET_32)
	$(OBJ_CPY) -I $(TARGET_32_FORMAT) -O $(TARGET_FORMAT) $(TARGET_32) $(TARGET)


$(TARGET_64): $(C_OBJS_64) $(S_OBJS_64) $(DATA_64)
	$(CC) $(LDFLAGS_64) -o $@ $(patsubst %,-I %,$(INCLUDE)) $(addprefix obj/, $(C_OBJS_64)) $(addprefix obj/, $(S_OBJS_64)) $(DATA_64)
	$(OBJ_CPY) -O binary $(TARGET_64) $(TARGET_64).bin
	$(OBJ_CPY) --readonly-text -I binary -O $(TARGET_64_FORMAT) $(TARGET_64).bin $(TARGET_64).o


$(TARGET_32): $(C_OBJS_32) $(S_OBJS_32) $(DATA_32)
	$(CC) $(LDFLAGS_32) -o $@ $(patsubst %,-I %,$(INCLUDE)) $(addprefix obj/, $(C_OBJS_32)) $(addprefix obj/, $(S_OBJS_32)) $(DATA_32)


$(C_OBJS_64): %.o: %.c
	$(CC) $(CFLAGS_64) -c $< -o obj/$@ -MMD -MP 

$(C_OBJS_32): %.o: %.c
	$(CC) $(CFLAGS_32) -c $< -o obj/$@ -MMD -MP 


$(S_OBJS_64): %.o: %.S
	$(CC) $(CFLAGS_64) -c $< -o obj/$@ -MMD -MP 

$(S_OBJS_32): %.o: %.S
	$(CC) $(CFLAGS_32) -c $< -o obj/$@ -MMD -MP 


$(DIRS):
	@echo "Creating directory: $@"
	@mkdir -p $@




	