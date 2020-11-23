TARGET ?= kicdev

ifneq ($(KERNELRELEASE),)
    ccflags-y := -std=gnu99 -DENABLE_DEBUG -DMODULE_NAME=\"$(TARGET)\"
    $(TARGET)-objs := drv.o fops.o
    obj-m := $(TARGET).o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

all default: modules

.PHONY: rebuild
rebuild: clean all

.PHONY: modules clean
modules clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) $@
endif

load:
	sudo insmod $(PWD)/$(TARGET).ko
	x-terminal-emulator -e $(open_debug_terminal)
	@echo "Module $(TARGET) loaded... see below info:"
	@echo "  To get OS/Kernel/Module/IRQ information run: make info"
	@echo "  To unload module run: make unload"
	@echo "  To rebuild module run: make rebuild"

unload:
	sudo rmmod $(TARGET)
	@echo "Module $(TARGET) unloaded"


info:
	@echo "---------------------- OS Info ----------------------------------"
	lsb_release -a
	@echo "---------------------- Kernel Info ------------------------------"
	cat /proc/version
	@echo "---------------------- Module $(TARGET) Info -----------------------"
	modinfo $(PWD)/$(TARGET).ko
	@echo "---------------------- IRQ Info ---------------------------------"
	cat /sys/module/$(TARGET)/parameters/irq
	x-terminal-emulator -e $(open_irq_terminal)
	@echo "-----------------------------------------------------------------"
    # To get memory section of module ran objdump -h $(PWD)/$(TARGET).ko

# Open new terminal for debuging - dmesg
define open_debug_terminal =
watch -n 1 "dmesg | grep $(TARGET) | tail -20" 
endef

# Open new terminal for irq information - only for target module
define open_irq_terminal =
watch -d -n 1 'cat /proc/interrupts | grep $(TARGET)'
endef
