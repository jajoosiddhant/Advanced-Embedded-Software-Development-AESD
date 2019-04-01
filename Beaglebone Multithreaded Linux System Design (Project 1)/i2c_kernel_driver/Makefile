LINUX_DIR:=$(HOME)/APES/build/buildroot/output/build/linux-4796173fc58688055a99a1cef19a839174067220
COMPILE_DIR:=$(HOME)/APES/build/buildroot/output/host/bin/


obj-m := i2c_module.o

all:
	make ARCH=arm CROSS_COMPILE=$(COMPILE_DIR)arm-buildroot-linux-uclibcgnueabihf- -C $(LINUX_DIR) M=$(shell pwd) modules 

clean:
	make -C $(LINUX_DIR) M=$(shell pwd) clean

