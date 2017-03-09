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

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -I$(INCDIR) -I$(LEAPINCDIR) -pipe

GTKLIB=`pkg-config --cflags --libs gtkmm-3.0`

# linker
LD=g++
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic
#LIBS= -lsnap7 -Llib${_arch}/ -Wl,-Bstatic -lLeap
#LIBS= -lsnap7 -lLeap
LIBS= -lsnap7 -Wl,-rpath,$(LIBDIR) $(LIBDIR)/libLeap.so
OBJS= main.o resources.o hmi.o leapmotion.o snap7.o s7server.o

.PHONY: all run clean

all: hmi

hmi: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

main.o: $(SRCDIR)/main.cpp
	$(CC) -c $(CCFLAGS) $(SRCDIR)/main.cpp $(GTKLIB) -o main.o

src/resources.cpp: glade/resources.xml glade/ui.glade
	cd glade && glib-compile-resources --target=../$(SRCDIR)/resources.cpp --generate-source resources.xml

resources.o: $(SRCDIR)/resources.cpp
	$(CC) -c $(CCFLAGS) $(SRCDIR)/resources.cpp $(GTKLIB) -o resources.o

hmi.o: $(SRCDIR)/hmi.cpp
	$(CC) -c $(CCFLAGS) $(SRCDIR)/hmi.cpp $(GTKLIB) -o hmi.o

leapmotion.o: $(SRCDIR)/leapmotion.cpp
	$(CC) -c $(CCFLAGS) $(SRCDIR)/leapmotion.cpp -o leapmotion.o

snap7.o: $(SRCDIR)/snap7.cpp
	$(CC) -c $(CCFLAGS) $(SRCDIR)/snap7.cpp -o snap7.o

s7server.o: $(SRCDIR)/s7server.cpp
	$(CC) -c $(CCFLAGS) $(SRCDIR)/s7server.cpp -o s7server.o

run:
	authbind ./hmi

clean:
	rm -f *.o
	rm -f $(SRCDIR)/resources.cpp
