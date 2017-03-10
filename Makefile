ARCH := $(shell uname -m)
LEAPLIBDIR := LeapSDK/lib

ifeq ($(ARCH), x86_64)
	LIBDIR=$(LEAPLIBDIR)/x64
else
	LIBDIR=$(LEAPLIBDIR)/x86
endif

TARGET=hmi
SRCDIR=src
INCDIR=include
LEAPINCDIR=LeapSDK/include

#compiler
CC=g++
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

PTHREAD=-pthread

GTKLIB=`pkg-config --cflags --libs gtkmm-3.0`

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) $(GTKLIB) -I$(INCDIR) -I$(LEAPINCDIR) -pipe

# linker
LD=g++
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic
LIBS= -lsnap7 -Wl,-rpath,$(LIBDIR) $(LIBDIR)/libLeap.so
OBJS= main.o resources.o hmi.o leapmotion.o snap7.o s7server.o

.PHONY: all run clean

all: hmi

hmi: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

src/resources.cpp: glade/resources.xml glade/ui.glade
	cd glade && glib-compile-resources --target=../$(SRCDIR)/resources.cpp --generate-source resources.xml

%.o: $(SRCDIR)/%.cpp
	$(CC) -c $(CCFLAGS) $< -o $@

run:
	authbind ./hmi

clean:
	rm -f *.o
	rm -f $(SRCDIR)/resources.cpp
