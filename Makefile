CPP := g++

OUT := build
SRC := src

# flags to pass to the compile stage of C files
CFLAGS := -I./include/ -Wall

# flags to pass to the compile stage of C++ files
CPPFLAGS := -I./include/ -Wall

CPPFILES := $(subst $(SRC)/,$(empty),$(wildcard $(SRC)/*.cpp))
CPPOBJS := $(foreach file, $(CPPFILES), $(OUT)/$(file:.cpp=.o))

CFILES := $(subst $(SRC)/,$(empty),$(wildcard $(SRC)/*.c))
COBJS := $(foreach file, $(CFILES), $(OUT)/$(file:.c=.o))

LINKFLAGS := -L./lib -l:libglfw3.a -lGdi32 -l:libassimp.a -l:libzlibstatic.a -lComdlg32 -lComctl32

all: mdlviewer

mdlviewer: $(CPPOBJS) $(COBJS)
	$(CPP) $(CPPOBJS) $(COBJS) -o mdlviewer $(LINKFLAGS) 

$(OUT)/%.o: $(SRC)/%.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@


$(OUT)/%.o: $(SRC)/%.c
	$(CPP) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OUT)/*.o mdlviewer
