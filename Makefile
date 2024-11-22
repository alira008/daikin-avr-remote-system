MMCU=atmega1284
BAUD=8000000

SRC_DIR=src/
BUILD_DIR=build/
OBJ_DIR=$(BUILD_DIR)obj/

AVR_INCLUDE_DIR=/usr/local/Cellar/avr-gcc@9/9.4.0_1/avr/include
INCLUDES=-I$(AVR_INCLUDE_DIR)
SOURCES=$(wildcard $(SRC_DIR)*.c)
OBJS=$(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SOURCES))

# AVR Compiler
AVR_COMPILER=avr-gcc
COMPILER_FLAGS=-Wall -mmcu=$(MMCU) $(MMCUSECTION)

OBJ_COPY=avr-objcopy
OBJ_FLAGS=-j .text -j .data -O ihex

# AVR Programmer
AVR_PROGRAMMER=avrdude
AVR_PROGRAMMER_DEVICE=atmelice_isp
MMCUSECTION=-Wl,--undefined=_mmcu,--section-start=.mmcu=910000 
AVR_PROGRAMMER_VERBOSITY=#-v -v
HIGH_FUSE_DEFAULT=0xD9
HIGH_FUSE_WITH_JTAG=0x99
LOW_FUSE_DEFAULT=0x42

.PHONY: clean

all: $(BUILD_DIR)main.hex

verifyFuses: 
	$(AVR_PROGRAMMER) -c $(AVR_PROGRAMMER_DEVICE) -p $(MMCU) $(AVR_PROGRAMMER_VERBOSITY) -U hfuse:r:hfuse.txt:h -U lfuse:r:lfuse.txt:h 

defaultFuses: 
	$(AVR_PROGRAMMER) -c $(AVR_PROGRAMMER_DEVICE) -p $(MMCU) $(AVR_PROGRAMMER_VERBOSITY) -U hfuse:w:$(HIGH_FUSE_DEFAULT):m -U lfuse:w:$(LOW_FUSE_DEFAULT):m

fusesWithJTAG: 
	$(AVR_PROGRAMMER) -c $(AVR_PROGRAMMER_DEVICE) -p $(MMCU) $(AVR_PROGRAMMER_VERBOSITY) -U hfuse:w:$(HIGH_FUSE_WITH_JTAG):m -U lfuse:w:$(LOW_FUSE_DEFAULT):m

program: $(BUILD_DIR)main.hex
	$(AVR_PROGRAMMER) -c $(AVR_PROGRAMMER_DEVICE) -p $(MMCU) $(AVR_PROGRAMMER_VERBOSITY) -U flash:w:$<

$(BUILD_DIR)main.hex: $(OBJ_DIR)main.elf
	@$(OBJ_COPY) $(OBJ_FLAGS) $< $@

$(OBJ_DIR)main.elf: $(OBJS)
	@$(AVR_COMPILER) $(COMPILER_FLAGS) $(INCLUDES) -o $@ $(OBJS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(OBJ_DIR)
	@$(AVR_COMPILER) $(COMPILER_FLAGS) $(INCLUDES) -c -o $@ $<

$(OBJ_DIR): $(BUILD_DIR)
	@mkdir build/obj

$(BUILD_DIR):
	@mkdir build

clean:
	rm -r build
