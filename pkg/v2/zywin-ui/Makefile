# COMPILER = g++
# QTFLAGSWEB := $(shell pkg-config --cflags --libs Qt6Widgets Qt6WebEngineWidgets)

# compile:
# 	$(COMPILER) main.cpp -o zywin-ui/usr/bin/zywin-ui $(QTFLAGSWEB)
# 	$(COMPILER) main.cpp -o init $(QTFLAGSWEB)

# clean:
# 	sudo rm -rf zywin-ui/usr/bin/zywin-ui
# 	sudo rm -rf init

COMPILER = g++

QTFLAGS := $(shell pkg-config --cflags --libs Qt6Widgets)

CXXFLAGS = -std=c++17

TARGET = zywin-ui/usr/bin/zywin-ui
INIT = init

compile:
	mkdir -p zywin-ui/usr/bin
	$(COMPILER) $(CXXFLAGS) main.cpp -o $(TARGET) $(QTFLAGS)
	$(COMPILER) $(CXXFLAGS) main.cpp -o $(INIT) $(QTFLAGS)

clean:
	rm -f $(TARGET)
	rm -f $(INIT)