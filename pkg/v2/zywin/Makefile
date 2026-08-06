COMPILER = g++
CXXFLAGS = -std=c++17 -O2

BUILD = build
TARGET = $(BUILD)/zywin

all: $(TARGET)

$(TARGET): main.cpp
	mkdir -p $(BUILD)
	$(COMPILER) $(CXXFLAGS) $< -o $@

install: $(TARGET)
	install -Dm755 $(TARGET) zywin/usr/bin/zywin
	install -Dm755 $(TARGET) zywindow

clean:
	rm -rf $(BUILD)

.PHONY: all install clean