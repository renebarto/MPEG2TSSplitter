MD := mkdir
RM := rm

CXX := c++
CXXFLAGS := -g -fPIC -std=c++11 -Wall -Wextra -Wno-switch -I .
LDFLAGS :=

BUILDDIR := build

SOURCES_MPEG2TS_SPLITTER := main.cpp CommandLineOptionsParser.cpp
SOURCE_DIRS_MPEG2TS_SPLITTER := $(sort $(dir $(SOURCES_MPEG2TS_SPLITTER)))
OBJS_MPEG2TS_SPLITTER = $(patsubst %.cpp,$(BUILDDIR)/%.o, $(notdir $(SOURCES_MPEG2TS_SPLITTER)))

SOURCES_TOOLS := \
	tools/BitBuffer.cpp \
	tools/Util.cpp \
	tools/CommandLineParser.cpp \
	tools/CommandLineOption.cpp \
	tools/CommandLineOptionGroup.cpp \
	tools/Console.cpp \
	tools/DeserializationImpl.cpp \
	tools/SerializationImpl.cpp
SOURCE_DIRS_TOOLS := $(sort $(dir $(SOURCES_TOOLS)))
OBJS_TOOLS = $(patsubst %.cpp,$(BUILDDIR)/%.o, $(SOURCES_TOOLS))

SOURCES_MEDIA := \
	media/MPEG2Splitter.cpp \
	media/TransportStream.cpp \
	media/TSPacket.cpp \
	media/PESPacket.cpp \
	media/PAT.cpp \
	media/PMT.cpp \
	media/ProgramDescriptor.cpp \
	media/StreamInfo.cpp \
	media/StreamWriter.cpp
SOURCE_DIRS_MEDIA := $(sort $(dir $(SOURCES_MEDIA)))
OBJS_MEDIA = $(patsubst %.cpp,$(BUILDDIR)/%.o, $(SOURCES_MEDIA))

all : directories mpeg2ts-splitter tools media

directories : $(BUILDDIR) $(BUILDDIR)/tools $(BUILDDIR)/media

$(BUILDDIR) :
	$(MD) -p $@

$(BUILDDIR)/tools :
	$(MD) -p $@

$(BUILDDIR)/media :
	$(MD) -p $@

mpeg2ts-splitter : $(OBJS_MPEG2TS_SPLITTER) tools media
	$(CXX) -o $@ $(OBJS_MPEG2TS_SPLITTER) -L$(BUILDDIR) -lmedia -ltools

tools : $(OBJS_TOOLS)
	rm -f $(BUILDDIR)/libtools.a
	$(AR) qc $(BUILDDIR)/libtools.a $^

media : $(OBJS_MEDIA)
	rm -f $(BUILDDIR)/libmedia.a
	$(AR) qc $(BUILDDIR)/libmedia.a $^

vpath %.cpp $(SOURCE_DIRS_MPEG2TS_SPLITTER)

$(OBJS_MPEG2TS_SPLITTER): $(BUILDDIR)/%.o: %.cpp
	$(info $$< is [$<])
	$(info $$@ is [$@])
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJS_TOOLS): $(BUILDDIR)/%.o: %.cpp
	$(info $$< is [$<])
	$(info $$@ is [$@])
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJS_MEDIA): $(BUILDDIR)/%.o: %.cpp
	$(info $$< is [$<])
	$(info $$@ is [$@])
	$(CXX) $(CXXFLAGS) -c -o $@ $<


clean :
	rm -rf $(BUILDDIR)

