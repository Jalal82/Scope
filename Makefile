CCX  = g++
CFLAGS  = -g -Wall -std=c++17

TARGET = main
LIB = Lib/Src/usb.cpp Lib/Src/graphics.cpp  Lib/Src/ui.cpp
INC = Lib/Inc
OUT = Scope

LINKER = -lws2_32 -lSDL2 -lSDL2_gfx -lSDL2_ttf -lfftw3

all : $(TARGET)
$(TARGET) : $(TARGET).cpp
	$(CCX) $(TARGET).cpp -I $(INC) $(LIB)  $(LINKER)  $(CFLAGS)  -o $(OUT) 

clean:
	del $(OUT).exe