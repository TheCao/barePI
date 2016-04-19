################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FrameBuffer.c \
../src/GPIO.c \
../src/Mailbox.c \
../src/SystemTimer.c \
../src/armtimer.c \
../src/bcm2835.c \
../src/main.c \
../src/periph.c \
../src/raspi.c \
../src/rpi-mailbox-interface.c \
../src/rpi-mailbox.c 

S_UPPER_SRCS += \
../src/Asetup.S \
../src/Avectors.S 

OBJS += \
./src/Asetup.o \
./src/Avectors.o \
./src/FrameBuffer.o \
./src/GPIO.o \
./src/Mailbox.o \
./src/SystemTimer.o \
./src/armtimer.o \
./src/bcm2835.o \
./src/main.o \
./src/periph.o \
./src/raspi.o \
./src/rpi-mailbox-interface.o \
./src/rpi-mailbox.o 

S_UPPER_DEPS += \
./src/Asetup.d \
./src/Avectors.d 

C_DEPS += \
./src/FrameBuffer.d \
./src/GPIO.d \
./src/Mailbox.d \
./src/SystemTimer.d \
./src/armtimer.d \
./src/bcm2835.d \
./src/main.d \
./src/periph.d \
./src/raspi.d \
./src/rpi-mailbox-interface.d \
./src/rpi-mailbox.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-a7 -mfloat-abi=softfp -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-a7 -mfloat-abi=softfp -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


