VERSION := 1.2.1-r3
ARCH    := $(shell dpkg --print-architecture 2>/dev/null || echo amd64)

CXX      ?= g++
CXXFLAGS := -Wall -Wextra -Wconversion -Wformat -Wformat-security -std=c++17 -O2
SECFLAGS := -fstack-protector-strong -fPIE -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2
LDFLAGS  := -pie -Wl,-z,relro,-z,now

BUILD := build
BIN   := $(BUILD)/zylearn
PKG   := $(BUILD)/pkg
DEB   := $(BUILD)/zylearn_$(VERSION)-1_$(ARCH).deb

.PHONY: all compile install uninstall deb clean

all compile: $(BIN)

$(BIN): main.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $(SECFLAGS) main.cpp -o $@ $(LDFLAGS)

# Run as: sudo make install   (compiling never needs root)
install: $(BIN)
	install -Dm755 $(BIN) $(DESTDIR)/usr/bin/zylearn
	install -d $(DESTDIR)/usr/share/zylearn/lessons
	install -m644 lessons/*.txt lessons/*.check $(DESTDIR)/usr/share/zylearn/lessons/

uninstall:
	rm -f $(DESTDIR)/usr/bin/zylearn
	rm -rf $(DESTDIR)/usr/share/zylearn

deb: $(BIN)
	rm -rf $(PKG)
	$(MAKE) install DESTDIR=$(CURDIR)/$(PKG)
	install -d $(PKG)/DEBIAN
	sed -e 's/@VERSION@/$(VERSION)/' -e 's/@ARCH@/$(ARCH)/' packaging/control.in > $(PKG)/DEBIAN/control
	dpkg-deb --root-owner-group --build $(PKG) zylearn.deb
	@echo "Built zylearn.deb"

clean:
	rm -rf $(BUILD)
