.SUFFIXES:

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

TOPDIR 		?= 	$(CURDIR)
include $(DEVKITARM)/3ds_rules

CTRPFLIB	?=	$(DEVKITPRO)/libctrpf

TARGET		:= 	Saltwater
PLGINFO 	:= 	saltwater.plgInfo

BUILD 		:= 	build
INCLUDES	:= 	include
SOURCES 	:= 	src src/Megamix src/external #src/Helpers

RELEASE		?= 0

FLATPAK 	:= 0
BARISTA_DIR	?= ../Barista/Barista

# Assume flatpak
ifeq ($(shell which citra 2> /dev/null || which citra-qt 2> /dev/null || true),)
	FLATPAK		:= 1
endif

ifeq ($(OS),Windows_NT)
	CITRA_DIR	:= $(APPDATA)/Citra/sdmc
else
	ifeq ($(shell uname -s),Linux)
		ifeq ($(FLATPAK),1)
			CITRA_DIR := ~/.var/app/org.citra_emu.citra/data/citra-emu/sdmc
		else
			CITRA_DIR := ~/.local/share/citra-emu/sdmc
		endif
	else
		ifeq ($(shell uname -s), macOS)
			CITRA_DIR := ~/Library/Application Support/Citra/sdmc
		else
			CITRA :=
		endif
	endif
endif

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH		:=	-march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

CFLAGS		:=	$(ARCH) -Os -W -mword-relocations -fomit-frame-pointer \
				-ffunction-sections -fno-strict-aliasing -flto

CFLAGS		+=	$(INCLUDE) -D__3DS__ -DCOMMIT="$(shell git show --format='%h' --no-patch || echo 'nogit')"

ifneq ($(RELEASE), 0)
CFLAGS		+= -DRELEASE
endif

CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++20 #-Wno-pointer-arith

ASFLAGS		:=	$(ARCH)
LDFLAGS		:= -T $(TOPDIR)/3gx.ld $(ARCH) -Os -Wl,--gc-sections

LIBS		:= -lctrpf -lctru
LIBDIRS		:= 	$(CTRPFLIB) $(CTRULIB) $(PORTLIBS)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES			:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES			:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

export LD 		:= 	$(CXX)
export OFILES	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I $(CURDIR)/$(dir) ) \
					$(foreach dir,$(LIBDIRS),-I $(dir)/include) \
					-I $(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L $(dir)/lib)

.PHONY: $(BUILD) clean all test

#---------------------------------------------------------------------------------
all: $(BUILD)

release: RELEASE = 1
release: clean $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $@ -f $(CURDIR)/Makefile RELEASE=$(RELEASE)

#---------------------------------------------------------------------------------
clean:
	@echo clean ... 
	@rm -fr $(BUILD) $(OUTPUT).3gx $(OUTPUT).elf

re: clean all

#---------------------------------------------------------------------------------

test: all
	@mkdir -p $(CITRA_DIR)/spicerack/bin/
	@cp $(OUTPUT).3gx $(CITRA_DIR)/spicerack/bin/
	@make -C $(BARISTA_DIR) test
	

#---------------------------------------------------------------------------------

else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).3gx : $(OUTPUT).elf

$(OUTPUT).elf: $(OFILES)

#---------------------------------------------------------------------------------
# you need a rule like this for each extension you use as binary data
#---------------------------------------------------------------------------------
%.bin.o	:	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.3gx: %.elf
#---------------------------------------------------------------------------------
	@echo creating $(notdir $@)
	@3gxtool -s $(word 1, $^) $(TOPDIR)/$(PLGINFO) $@

-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
