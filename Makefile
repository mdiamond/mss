CC = g++ -g -O0 -Wall
CFLAGS = -std=c++11
LDFLAGS = -lSDL2 -lSDL2_ttf
OBJCOMMAND = $(CC) $(CFLAGS) -o $@ -c
EXECOMMAND = $(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)
MAINFILEOBJ = obj/main.o obj/main_helpers.o
MISCFILEOBJ = obj/event_handler.o obj/function_forwarder.o \
			  obj/graphics_object_utils.o obj/image_processing.o \
			  obj/load_patch.o obj/populate_wavetables.o \
			  obj/save_patch.o obj/signal_processing.o \
			  obj/tests.o
MISCCLASSOBJ = obj/Timer.o
BASECLASSOBJ = obj/Graphics_Object.o obj/Module.o
MODULECLASSOBJ = obj/Adsr.o obj/Delay.o \
				 obj/Filter.o obj/Mixer.o obj/Multiplier.o \
				 obj/Noise.o obj/Oscillator.o obj/Output.o
GRAPHICCLASSOBJ = obj/Button.o obj/Input_Text_Box.o \
				  obj/Input_Toggle_Button.o obj/Page.o \
				  obj/Rect.o obj/Text.o \
				  obj/Text_Box.o obj/Toggle_Button.o \
				  obj/Waveform.o
OBJECTS = $(MAINFILEOBJ) $(MISCFILEOBJ) \
		  $(MISCCLASSOBJ) $(BASECLASSOBJ) \
		  $(MODULECLASSOBJ) $(GRAPHICCLASSOBJ)

all : synth

synth : $(OBJECTS)
	$(EXECOMMAND)

# Main
obj/main_helpers.o : main_helpers.cpp main_helpers.hpp
	$(OBJCOMMAND) main_helpers.cpp
obj/main.o : main.cpp main.hpp
	$(OBJCOMMAND) main.cpp 

# Miscellaneous files
obj/event_handler.o : event_handler.cpp event_handler.hpp
	$(OBJCOMMAND) event_handler.cpp
obj/function_forwarder.o : function_forwarder.cpp function_forwarder.hpp
	$(OBJCOMMAND) function_forwarder.cpp
obj/graphics_object_utils.o : graphics_object_utils.cpp graphics_object_utils.hpp
	$(OBJCOMMAND) graphics_object_utils.cpp
obj/image_processing.o : image_processing.cpp image_processing.hpp
	$(OBJCOMMAND) image_processing.cpp
obj/load_patch.o : load_patch.cpp load_patch.hpp
	$(OBJCOMMAND) load_patch.cpp
obj/populate_wavetables.o : populate_wavetables.cpp populate_wavetables.hpp
	$(OBJCOMMAND) populate_wavetables.cpp
obj/save_patch.o : save_patch.cpp save_patch.hpp
	$(OBJCOMMAND) save_patch.cpp
obj/signal_processing.o : signal_processing.cpp signal_processing.hpp
	$(OBJCOMMAND) signal_processing.cpp
obj/tests.o : tests.cpp tests.hpp
	$(OBJCOMMAND) tests.cpp

# Miscellaneous classes
obj/Timer.o : Timer.cpp Timer.hpp
	$(OBJCOMMAND) Timer.cpp

# Module classes
obj/Module.o : Module.cpp Module.hpp
	$(OBJCOMMAND) Module.cpp
obj/Adsr.o : Modules/Adsr.cpp Modules/Adsr.hpp
	$(OBJCOMMAND) Modules/Adsr.cpp
obj/Delay.o : Modules/Delay.cpp Modules/Delay.hpp
	$(OBJCOMMAND) Modules/Delay.cpp
obj/Filter.o : Modules/Filter.cpp Modules/Filter.hpp
	$(OBJCOMMAND) Modules/Filter.cpp
obj/Mixer.o : Modules/Mixer.cpp Modules/Mixer.hpp
	$(OBJCOMMAND) Modules/Mixer.cpp
obj/Multiplier.o : Modules/Multiplier.cpp Modules/Multiplier.hpp
	$(OBJCOMMAND) Modules/Multiplier.cpp
obj/Noise.o : Modules/Noise.cpp Modules/Noise.hpp
	$(OBJCOMMAND) Modules/Noise.cpp
obj/Oscillator.o : Modules/Oscillator.cpp Modules/Oscillator.hpp
	$(OBJCOMMAND) Modules/Oscillator.cpp
obj/Output.o : Modules/Output.cpp Modules/Output.hpp
	$(OBJCOMMAND) Modules/Output.cpp

# Graphics objects classes
obj/Graphics_Object.o : Graphics_Object.cpp Graphics_Object.hpp
	$(OBJCOMMAND) Graphics_Object.cpp
obj/Button.o : Graphics_Objects/Button.cpp Graphics_Objects/Button.hpp
	$(OBJCOMMAND) Graphics_Objects/Button.cpp
obj/Input_Text_Box.o : Graphics_Objects/Input_Text_Box.cpp Graphics_Objects/Input_Text_Box.hpp
	$(OBJCOMMAND) Graphics_Objects/Input_Text_Box.cpp
obj/Input_Toggle_Button.o : Graphics_Objects/Input_Toggle_Button.cpp Graphics_Objects/Input_Toggle_Button.hpp
	$(OBJCOMMAND) Graphics_Objects/Input_Toggle_Button.cpp
obj/Page.o : Graphics_Objects/Page.cpp Graphics_Objects/Page.hpp
	$(OBJCOMMAND) Graphics_Objects/Page.cpp
obj/Rect.o : Graphics_Objects/Rect.cpp Graphics_Objects/Rect.hpp
	$(OBJCOMMAND) Graphics_Objects/Rect.cpp
obj/Text.o : Graphics_Objects/Text.cpp Graphics_Objects/Text.hpp
	$(OBJCOMMAND) Graphics_Objects/Text.cpp
obj/Text_Box.o : Graphics_Objects/Text_Box.cpp Graphics_Objects/Text_Box.hpp
	$(OBJCOMMAND) Graphics_Objects/Text_Box.cpp
obj/Toggle_Button.o : Graphics_Objects/Toggle_Button.cpp Graphics_Objects/Toggle_Button.hpp
	$(OBJCOMMAND) Graphics_Objects/Toggle_Button.cpp
obj/Waveform.o : Graphics_Objects/Waveform.cpp Graphics_Objects/Waveform.hpp
	$(OBJCOMMAND) Graphics_Objects/Waveform.cpp

.PHONY: clean

# Clean
clean :
	rm -f synth $(OBJECTS)
