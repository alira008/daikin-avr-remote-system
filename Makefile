MMCU=atmega1284
BAUD=8000000
AVR_INCLUDE_DIR=/usr/local/Cellar/avr-gcc@9/9.4.0_1/avr/include

BUILD_DIR=build/
SRC_DIR=src/
SOURCES=$(wildcard $(SRC_DIR)*.c)
INCLUDES=-I$(AVR_INCLUDE_DIR)
OBJ_DIR=$(BUILD_DIR)obj/
OBJS=$(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SOURCES))

AVR=avr-gcc
AVR_PROGRAMMER=avrdude
AVR_PROGRAMMER_DEVICE=atmelice_isp
MMCUSECTION=-Wl,--undefined=_mmcu,--section-start=.mmcu=910000 
FLAGS=-Wall -mmcu=$(MMCU) $(MMCUSECTION)

OBJCOPY=avr-objcopy
OBJFLAGS=-j .text -j .data -O ihex

.PHONY: clean

all: $(BUILD_DIR)main.hex

$(BUILD_DIR)main.hex: $(OBJ_DIR)main.elf
	@$(OBJCOPY) $(OBJFLAGS) $< $@

$(OBJ_DIR)main.elf: $(OBJS)
	@$(AVR) $(FLAGS) $(INCLUDES) -o $@ $(OBJS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(OBJ_DIR)
	@$(AVR) $(FLAGS) $(INCLUDES) -c -o $@ $<

$(OBJ_DIR): $(BUILD_DIR)
	@mkdir build/obj

$(BUILD_DIR):
	@mkdir build

clean:
	rm -r build
