CXX = g++
# TARGET = test
CXXFLAGS = -Wall -Wextra -O2 -march=native -std=c++20 -fPIC #-fsanitize=address
# CXXFLAGS += $(shell root-config --cflags)

SRCDIR   = src
HEADDIR  = include
OBJDIR   = obj
BUILD	 = build

#SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
#INCLUDES := $(wildcard $(HEADDIR)/*.hpp)
#OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)


# LDFLAGS =  $(shell root-config --libs)
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lboost_system #-lQt6Widgets -lQt6Gui -lQt6Core 
# LDFLAGS += -lSpectrum -lRHTTP 
# LDFLAGS += -lCAENDigitizer

INCLUDE  := 			\
	-Iinclude/ \
	-I$(HEADDIR) \
	-I/usr/include/boost \
	#-I/usr/include/x86_64-linux-gnu/qt6 \
	#-I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
	#-I/usr/include/x86_64-linux-gnu/qt6/QtCore \
	#-I/usr/include/x86_64-linux-gnu/qt6/QtGui \

# $(BUILD)/$(TARGET): $(OBJECTS)
# $(TARGET): $(OBJECTS)
# 	@$(CXX) $(CXXFLAGS) $(INCLUDE) $(ROOTFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)
# 	@echo "Linking complete!"

# $(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
# 	@$(CXX) $(CXXFLAGS) $(INCLUDE) $(ROOTFLAGS) -c $< -o $@ $(LDFLAGS)
# 	@echo "Compiled "$<" successfully!"

all: SimpleClient SimpleServer SimpleExample

SimpleExample: $(SRCDIR)/SimpleExample.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(SRCDIR)/SimpleExample.cpp -o SimpleExample $(LDFLAGS)

SimpleClient: $(SRCDIR)/SimpleClient.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(SRCDIR)/SimpleClient.cpp -o SimpleClient $(LDFLAGS)

SimpleServer: $(SRCDIR)/SimpleServer.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(SRCDIR)/SimpleServer.cpp -o SimpleServer $(LDFLAGS)

clean:
	rm -rf $(OBJDIR)/*.o SimpleClient SimpleServer SimpleExample

debug: CXXFLAGS += -g

debug: SimpleExample SimpleServer SimpleClient
