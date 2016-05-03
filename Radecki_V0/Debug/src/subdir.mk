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
../src/devicenameservice.c \
../src/dwhcidevice.c \
../src/dwhciframeschednper.c \
../src/dwhciframeschednsplit.c \
../src/dwhciframeschedper.c \
../src/dwhciregister.c \
../src/dwhcirootport.c \
../src/dwhcixferstagedata.c \
../src/keymap.c \
../src/macaddress.c \
../src/main.c \
../src/periph.c \
../src/raspi.c \
../src/smsc951x.c \
../src/synchronize.c \
../src/usbconfigparser.c \
../src/usbdevice.c \
../src/usbdevicefactory.c \
../src/usbendpoint.c \
../src/usbgamepad.c \
../src/usbkeyboard.c \
../src/usbmassdevice.c \
../src/usbmouse.c \
../src/usbrequest.c \
../src/usbstandardhub.c \
../src/usbstring.c \
../src/uspilibrary.c \
../src/util.c 

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
./src/devicenameservice.o \
./src/dwhcidevice.o \
./src/dwhciframeschednper.o \
./src/dwhciframeschednsplit.o \
./src/dwhciframeschedper.o \
./src/dwhciregister.o \
./src/dwhcirootport.o \
./src/dwhcixferstagedata.o \
./src/keymap.o \
./src/macaddress.o \
./src/main.o \
./src/periph.o \
./src/raspi.o \
./src/smsc951x.o \
./src/synchronize.o \
./src/usbconfigparser.o \
./src/usbdevice.o \
./src/usbdevicefactory.o \
./src/usbendpoint.o \
./src/usbgamepad.o \
./src/usbkeyboard.o \
./src/usbmassdevice.o \
./src/usbmouse.o \
./src/usbrequest.o \
./src/usbstandardhub.o \
./src/usbstring.o \
./src/uspilibrary.o \
./src/util.o 

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
./src/devicenameservice.d \
./src/dwhcidevice.d \
./src/dwhciframeschednper.d \
./src/dwhciframeschednsplit.d \
./src/dwhciframeschedper.d \
./src/dwhciregister.d \
./src/dwhcirootport.d \
./src/dwhcixferstagedata.d \
./src/keymap.d \
./src/macaddress.d \
./src/main.d \
./src/periph.d \
./src/raspi.d \
./src/smsc951x.d \
./src/synchronize.d \
./src/usbconfigparser.d \
./src/usbdevice.d \
./src/usbdevicefactory.d \
./src/usbendpoint.d \
./src/usbgamepad.d \
./src/usbkeyboard.d \
./src/usbmassdevice.d \
./src/usbmouse.d \
./src/usbrequest.d \
./src/usbstandardhub.d \
./src/usbstring.d \
./src/uspilibrary.d \
./src/util.d 


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


