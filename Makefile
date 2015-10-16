CC = g++ -g -Wall
CFLAGS = -std=c++11
LDFLAGS = -lSDL2 -lSDL2_ttf
OBJFLAGS = $(CC) $(CFLAGS) -o $@ -c
EXEFLAGS = $(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)
MAINOBJ = obj/main.o obj/main_helpers.o
MISCFILEOBJ = obj/event_handler.o obj/function_forwarder.o obj/image_processing.o obj/populate_wavetables.o obj/signal_processing.o obj/tests.o
MISCCLASSOBJ = obj/Timer.o
BASECLASSOBJ = obj/Graphics_Object.o obj/Module.o
MODULEOBJ =  obj/Oscillator.o obj/Output.o obj/VCA.o
GRAPHICOBJ =  obj/Button.o obj/Page.o obj/Rect.o obj/Text.o obj/Text_Box.o obj/Toggle_Button.o obj/Waveform.o
OBJECTS = $(MAINOBJ) $(MISCFILEOBJ) $(MISCCLASSOBJ) $(BASECLASSOBJ) $(MODULEOBJ) $(GRAPHICOBJ)

all : synth

synth : $(OBJECTS)
	$(EXEFLAGS)

# Main
obj/main_helpers.o : main_helpers.cpp main_helpers.hpp
	$(OBJFLAGS) main_helpers.cpp
obj/main.o : main.cpp main.hpp
	$(OBJFLAGS) main.cpp 

# Miscellaneous files
obj/event_handler.o : event_handler.cpp event_handler.hpp
	$(OBJFLAGS) event_handler.cpp
obj/function_forwarder.o : function_forwarder.cpp function_forwarder.hpp
	$(OBJFLAGS) function_forwarder.cpp
obj/image_processing.o : image_processing.cpp image_processing.hpp
	$(OBJFLAGS) image_processing.cpp
obj/populate_wavetables.o : populate_wavetables.cpp populate_wavetables.hpp
	$(OBJFLAGS) populate_wavetables.cpp
obj/signal_processing.o : signal_processing.cpp signal_processing.hpp
	$(OBJFLAGS) signal_processing.cpp
obj/tests.o : tests.cpp tests.hpp
	$(OBJFLAGS) tests.cpp

# Miscellaneous classes
obj/Timer.o : Timer.cpp Timer.hpp
	$(OBJFLAGS) Timer.cpp

# Module classes
obj/Module.o : Module.cpp Module.hpp
	$(OBJFLAGS) Module.cpp
obj/Oscillator.o : Modules/Oscillator.cpp Modules/Oscillator.hpp
	$(OBJFLAGS) Modules/Oscillator.cpp
obj/Output.o : Modules/Output.cpp Modules/Output.hpp
	$(OBJFLAGS) Modules/Output.cpp
obj/VCA.o : Modules/VCA.cpp Modules/VCA.hpp
	$(OBJFLAGS) Modules/VCA.cpp

# Graphics objects classes
obj/Graphics_Object.o : Graphics_Object.cpp Graphics_Object.hpp
	$(OBJFLAGS) Graphics_Object.cpp
obj/Button.o : Graphics_Objects/Button.cpp Graphics_Objects/Button.hpp
	$(OBJFLAGS) Graphics_Objects/Button.cpp
obj/Page.o : Graphics_Objects/Page.cpp Graphics_Objects/Page.hpp
	$(OBJFLAGS) Graphics_Objects/Page.cpp
obj/Rect.o : Graphics_Objects/Rect.cpp Graphics_Objects/Rect.hpp
	$(OBJFLAGS) Graphics_Objects/Rect.cpp
obj/Text.o : Graphics_Objects/Text.cpp Graphics_Objects/Text.hpp
	$(OBJFLAGS) Graphics_Objects/Text.cpp
obj/Text_Box.o : Graphics_Objects/Text_Box.cpp Graphics_Objects/Text_Box.hpp
	$(OBJFLAGS) Graphics_Objects/Text_Box.cpp
obj/Toggle_Button.o : Graphics_Objects/Toggle_Button.cpp Graphics_Objects/Toggle_Button.hpp
	$(OBJFLAGS) Graphics_Objects/Toggle_Button.cpp
obj/Waveform.o : Graphics_Objects/Waveform.cpp Graphics_Objects/Waveform.hpp
	$(OBJFLAGS) Graphics_Objects/Waveform.cpp

.PHONY: clean

# Clean
clean :
	rm -f synth $(OBJECTS)
