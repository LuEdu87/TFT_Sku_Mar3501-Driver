################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
LCD_TFT_004_Tests_LCD_OK_TCHOK_SD_OK_TEXT_OK.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
LCD_TFT_004_Tests_LCD_OK_TCHOK_SD_OK_TEXT_OK.list \

OBJCOPY_BIN += \
LCD_TFT_004_Tests_LCD_OK_TCHOK_SD_OK_TEXT_OK.bin \


# All Target
all: LCD_TFT_004_Tests_LCD_OK_TCHOK_SD_OK_TEXT_OK.elf secondary-outputs

# Tool invocations
LCD_TFT_004_Tests_LCD_OK_TCHOK_SD_OK_TEXT_OK.elf: $(OBJS) $(USER_OBJS) C:\Users\El_an\Embedded-C\RepositorioLCDShield\LCD_TFT_004_Tests_LCD_OK_TCHOK_SD_OK_TEXT_OK\STM32F446RETX_FLASH.ld
	arm-none-eabi-gcc -o "LCD_TFT_004_Tests_LCD_OK_TCHOK_SD_OK_TEXT_OK.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\El_an\Embedded-C\RepositorioLCDShield\LCD_TFT_004_Tests_LCD_OK_TCHOK_SD_OK_TEXT_OK\STM32F446RETX_FLASH.ld" -Wl,-Map="LCD_TFT_004_Tests_LCD_OK_TCHOK_SD_OK_TEXT_OK.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

LCD_TFT_004_Tests_LCD_OK_TCHOK_SD_OK_TEXT_OK.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "LCD_TFT_004_Tests_LCD_OK_TCHOK_SD_OK_TEXT_OK.list"
	@echo 'Finished building: $@'
	@echo ' '

LCD_TFT_004_Tests_LCD_OK_TCHOK_SD_OK_TEXT_OK.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "LCD_TFT_004_Tests_LCD_OK_TCHOK_SD_OK_TEXT_OK.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
