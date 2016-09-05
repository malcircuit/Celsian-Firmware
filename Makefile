PROJECT_NAME := celsian_r2

export OUTPUT_FILENAME
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) ) 

NRF_SDK_ROOT := /Users/matt/NRF52/nRF5_SDK_11.0.0_89a8197
PROJECT_ROOT := /Users/matt/git/Celsian-Firmware

TEMPLATE_PATH = $(NRF_SDK_ROOT)/components/toolchain/gcc
ifeq ($(OS),Windows_NT)
include $(TEMPLATE_PATH)/Makefile.windows
else
include $(TEMPLATE_PATH)/Makefile.posix
endif

MK := mkdir
RM := rm -rf

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO := 
else
NO_ECHO := @
endif

# Toolchain commands
CC              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc'
AS              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as'
AR              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar' -r
LD              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld'
NM              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm'
OBJDUMP         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump'
OBJCOPY         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy'
SIZE            := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size'

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

#source common to all targets
C_SOURCE_FILES += \
$(NRF_SDK_ROOT)/components/toolchain/system_nrf52.c \
$(PROJECT_ROOT)/main.c \
$(PROJECT_ROOT)/debug.c \
$(PROJECT_ROOT)/util.c \
$(PROJECT_ROOT)/flash.c \
$(PROJECT_ROOT)/leds.c \
$(PROJECT_ROOT)/ble_util.c \
$(PROJECT_ROOT)/sensors/twi_util.c \
$(PROJECT_ROOT)/sensors/SHT20.c \
$(PROJECT_ROOT)/sensors/FXOS8700CQ.c \
$(PROJECT_ROOT)/sensors/MPL3115A2.c \
$(PROJECT_ROOT)/sensors/VEML6075.c \
$(PROJECT_ROOT)/sensors/sensors.c \
$(NRF_SDK_ROOT)/components/libraries/fstorage/fstorage.c \
$(NRF_SDK_ROOT)/components/libraries/timer/app_timer.c \
$(NRF_SDK_ROOT)/components/libraries/twi/app_twi.c \
$(NRF_SDK_ROOT)/components/libraries/uart/app_uart.c \
$(NRF_SDK_ROOT)/components/libraries/util/app_util_platform.c \
$(NRF_SDK_ROOT)/components/libraries/util/app_error.c \
$(NRF_SDK_ROOT)/components/drivers_nrf/clock/nrf_drv_clock.c \
$(NRF_SDK_ROOT)/components/drivers_nrf/common/nrf_drv_common.c \
$(NRF_SDK_ROOT)/components/drivers_nrf/delay/nrf_delay.c \
$(NRF_SDK_ROOT)/components/drivers_nrf/gpiote/nrf_drv_gpiote.c \
$(NRF_SDK_ROOT)/components/drivers_nrf/pstorage/pstorage.c \
$(NRF_SDK_ROOT)/components/drivers_nrf/spi_master/nrf_drv_spi.c \
$(NRF_SDK_ROOT)/components/drivers_nrf/twi_master/nrf_drv_twi.c \
$(NRF_SDK_ROOT)/components/drivers_nrf/uart/nrf_drv_uart.c \
$(NRF_SDK_ROOT)/components/ble/common/ble_advdata.c \
$(NRF_SDK_ROOT)/components/ble/common/ble_conn_params.c \
$(NRF_SDK_ROOT)/components/ble/common/ble_srv_common.c \
$(NRF_SDK_ROOT)/components/ble/ble_advertising/ble_advertising.c \
$(NRF_SDK_ROOT)/components/softdevice/common/softdevice_handler/softdevice_handler.c \

#assembly files common to all targets
ASM_SOURCE_FILES  = $(TEMPLATE_PATH)/gcc_startup_nrf52.s

#includes common to all targets
INC_PATHS += -I$(NRF_SDK_ROOT)/components/toolchain/gcc
INC_PATHS += -I$(NRF_SDK_ROOT)/components/toolchain
INC_PATHS += -I$(NRF_SDK_ROOT)/components/device
INC_PATHS += -I$(NRF_SDK_ROOT)/components/toolchain/CMSIS/Include
INC_PATHS += -I$(NRF_SDK_ROOT)/components/libraries/util
INC_PATHS += -I$(NRF_SDK_ROOT)/components/libraries/twi
INC_PATHS += -I$(NRF_SDK_ROOT)/components/libraries/uart
INC_PATHS += -I$(NRF_SDK_ROOT)/components/libraries/fstorage
INC_PATHS += -I$(NRF_SDK_ROOT)/components/libraries/fstorage/config
INC_PATHS += -I$(NRF_SDK_ROOT)/components/libraries/experimental_section_vars
INC_PATHS += -I$(NRF_SDK_ROOT)/components/libraries/timer
INC_PATHS += -I$(NRF_SDK_ROOT)/components/drivers_nrf/common
INC_PATHS += -I$(NRF_SDK_ROOT)/components/drivers_nrf/twi_master
INC_PATHS += -I$(NRF_SDK_ROOT)/components/drivers_nrf/hal
INC_PATHS += -I$(NRF_SDK_ROOT)/components/drivers_nrf/delay
INC_PATHS += -I$(NRF_SDK_ROOT)/components/drivers_nrf/uart
INC_PATHS += -I$(NRF_SDK_ROOT)/components/drivers_nrf/spi_master
INC_PATHS += -I$(NRF_SDK_ROOT)/components/drivers_nrf/gpiote
INC_PATHS += -I$(NRF_SDK_ROOT)/components/drivers_nrf/pstorage
INC_PATHS += -I$(NRF_SDK_ROOT)/components/drivers_nrf/clock
INC_PATHS += -I$(NRF_SDK_ROOT)/components/softdevice/s132/headers
INC_PATHS += -I$(NRF_SDK_ROOT)/components/softdevice/s132/headers/nrf52
INC_PATHS += -I$(NRF_SDK_ROOT)/components/softdevice/common/softdevice_handler
INC_PATHS += -I$(NRF_SDK_ROOT)/components/ble/common
INC_PATHS += -I$(NRF_SDK_ROOT)/components/ble/device_manager
INC_PATHS += -I$(NRF_SDK_ROOT)/components/ble/ble_advertising
INC_PATHS += -I$(PROJECT_ROOT)
INC_PATHS += -I$(PROJECT_ROOT)/sensors

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

#flags common to all targets
CFLAGS  = -DSOFTDEVICE_PRESENT
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DS132
CFLAGS += -DNRF52_PAN_12
CFLAGS += -DNRF52_PAN_15
CFLAGS += -DNRF52_PAN_58
CFLAGS += -DNRF52_PAN_20
CFLAGS += -DNRF52_PAN_54
CFLAGS += -DNRF52_PAN_31
CFLAGS += -DNRF52_PAN_30
CFLAGS += -DNRF52_PAN_51
CFLAGS += -DNRF52_PAN_36
CFLAGS += -DNRF52_PAN_53
CFLAGS += -DNRF52_PAN_64
CFLAGS += -DNRF52_PAN_55
CFLAGS += -DNRF52_PAN_62
CFLAGS += -DNRF52_PAN_63
CFLAGS += -DNRF52
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -Wall -Werror -O0 -g3
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums 
# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DNRF52_PAN_12
ASMFLAGS += -DNRF52_PAN_15
ASMFLAGS += -DNRF52_PAN_58
ASMFLAGS += -DNRF52_PAN_20
ASMFLAGS += -DNRF52_PAN_54
ASMFLAGS += -DNRF52_PAN_31
ASMFLAGS += -DNRF52_PAN_30
ASMFLAGS += -DNRF52_PAN_51
ASMFLAGS += -DNRF52_PAN_36
ASMFLAGS += -DNRF52_PAN_53
ASMFLAGS += -DNRF52_PAN_64
ASMFLAGS += -DNRF52_PAN_55
ASMFLAGS += -DNRF52_PAN_62
ASMFLAGS += -DNRF52_PAN_63
ASMFLAGS += -DNRF52

#default target - first one defined
default: clean nrf52832_xxaa

#building all targets
all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e nrf52832_xxaa

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	nrf52832_xxaa

C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) )

vpath %.c $(C_PATHS)
vpath %.s $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

nrf52832_xxaa: OUTPUT_FILENAME := nrf52832_xxaa
nrf52832_xxaa: LINKER_SCRIPT=celsian_ble_r2_gcc_nrf52.ld
#nrf52832_xxaa: LINKER_SCRIPT=celsian_r2_gcc_nrf52.ld

nrf52832_xxaa: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -lm -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Assembly file: $(notdir $<)
	$(NO_ECHO)$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<
# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -lm -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
## Create binary .bin file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
genhex: 
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex
echosize:
	-@echo ''
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ''

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o
	
flash: nrf52832_xxaa
	@echo Flashing: $(OUTPUT_BINARY_DIRECTORY)/$<.hex
	JLinkExe -Device NRF52 -if swd -speed 1000 -CommanderScript flash.jlink
	
flash_sd:
	@echo Flashing SoftDevice s132 2.0.0
	JLinkExe -Device NRF52 -if swd -speed 1000 -CommanderScript flash_sd.jlink
	
erase:
	@echo Erasing NRF52
	JLinkExe -Device NRF52 -if swd -speed 1000 -CommanderScript erase.jlink
	
unlock:
	@echo Unlocking BMD-300
	JLinkExe -Device NRF52 -if swd -speed 1000 -CommanderScript unlock_bmd_300.jlink
	
disable_nfc:
	@echo Disabling NRF52 NFC functionality
	JLinkExe -Device NRF52 -if swd -speed 1000 -CommanderScript disable_nfc.jlink

## Flash softdevice