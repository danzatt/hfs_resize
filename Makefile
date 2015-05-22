include theos/makefiles/common.mk

TOOL_NAME = hfs_resize
hfs_resize_FILES = main.c

clean_all: clean
	rm *.deb *~

include $(THEOS_MAKE_PATH)/tool.mk
