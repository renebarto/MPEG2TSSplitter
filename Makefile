MD := mkdir
RM := rm

CXX := c++
CXXFLAGS := -O2 -fPIC -std=c++11 -Wall -Wextra -Wno-switch -I source
LDFLAGS :=

BUILDDIR := build

SOURCES_MPEG2TS_SPLITTER := source/main.cpp source/CommandLineOptionsParser.cpp
SOURCE_DIRS_MPEG2TS_SPLITTER := $(sort $(dir $(SOURCES_MPEG2TS_SPLITTER)))
OBJS_MPEG2TS_SPLITTER = $(patsubst %.cpp,$(BUILDDIR)/%.o, $(notdir $(SOURCES_MPEG2TS_SPLITTER)))

SOURCES_TOOLS := \
	source/tools/BitBuffer.cpp \
	source/tools/CommandLineOption.cpp \
	source/tools/CommandLineOptionGroup.cpp \
	source/tools/CommandLineParser.cpp \
	source/tools/Console.cpp \
	source/tools/DeserializationImpl.cpp \
	source/tools/SerializationImpl.cpp \
	source/tools/Stopwatch.cpp \
	source/tools/Util.cpp
SOURCE_DIRS_TOOLS := $(sort $(dir $(SOURCES_TOOLS)))
OBJS_TOOLS = $(patsubst %.cpp,$(BUILDDIR)/%.o, $(SOURCES_TOOLS))

SOURCES_MEDIA := \
	source/media/AudioStreamHandler.cpp \
	source/media/Logging.cpp \
	source/media/MPEG2Splitter.cpp \
	source/media/PAT.cpp \
	source/media/PATHandler.cpp \
	source/media/PESPacket.cpp \
	source/media/PMT.cpp \
	source/media/PMTHandler.cpp \
	source/media/ProgramDescriptor.cpp \
	source/media/StreamInfo.cpp \
	source/media/StreamWriter.cpp \
	source/media/TransportStream.cpp \
	source/media/TSPacket.cpp \
	source/media/VideoStreamHandler.cpp
SOURCE_DIRS_MEDIA := $(sort $(dir $(SOURCES_MEDIA)))
OBJS_MEDIA = $(patsubst %.cpp,$(BUILDDIR)/%.o, $(SOURCES_MEDIA))

SOURCES_TEST := \
	tests/BitBufferTest.cpp \
	tests/main.cpp \
	tests/MPEG2SplitterTest.cpp
SOURCE_DIRS_TEST := $(sort $(dir $(SOURCES_TEST)))
OBJS_TEST = $(patsubst %.cpp,$(BUILDDIR)/%.o, $(SOURCES_TEST))

all : directories mpeg2ts-splitter tools media mpeg2ts-splitter-test

directories : $(BUILDDIR) $(BUILDDIR)/source $(BUILDDIR)/test $(BUILDDIR)/source/tools $(BUILDDIR)/source/media

$(BUILDDIR) :
	$(MD) -p $@

$(BUILDDIR)/source :
	$(MD) -p $@

$(BUILDDIR)/test :
	$(MD) -p $@

$(BUILDDIR)/source/tools :
	$(MD) -p $@

$(BUILDDIR)/source/media :
	$(MD) -p $@

mpeg2ts-splitter : $(OBJS_MPEG2TS_SPLITTER) tools media
	$(CXX) -o $@ $(OBJS_MPEG2TS_SPLITTER) -L$(BUILDDIR) -lmedia -ltools

mpeg2ts-splitter-test : $(OBJS_TEST) tools media
	$(CXX) -o $@ $(OBJS_TEST) -L$(BUILDDIR) -lmedia -ltools

tools : $(OBJS_TOOLS)
	rm -f $(BUILDDIR)/libtools.a
	$(AR) qc $(BUILDDIR)/libtools.a $^

media : $(OBJS_MEDIA)
	rm -f $(BUILDDIR)/libmedia.a
	$(AR) qc $(BUILDDIR)/libmedia.a $^

vpath %.cpp $(SOURCE_DIRS_MPEG2TS_SPLITTER)

$(OBJS_MPEG2TS_SPLITTER): $(BUILDDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJS_TOOLS): $(BUILDDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJS_MEDIA): $(BUILDDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJS_TEST): $(BUILDDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean :
	rm -rf $(BUILDDIR)

