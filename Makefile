.SILENT:

# Paths
SRCDIR		= src
INCDIR		= include
BUILDDIR	= build
OBJDIR		= $(BUILDDIR)/obj
LIBDIR		= $(BUILDDIR)/lib

# Install paths
PREFIX		?= /usr/local
INCLUDEDIR	= $(PREFIX)/include/safeinput
LIBINSTALL	= $(PREFIX)/lib

# Tools
CC		= gcc
AR		= ar
ARFLAGS	= rcs
CFLAGS	= -Wall -Wextra -Wpedantic -Werror -std=gnu99 -O3 -march=native -flto -I$(INCDIR)

# Files
SRC		= $(SRCDIR)/safeinput.c
OBJ		= $(OBJDIR)/safeinput.o
LIB		= $(LIBDIR)/libsafeinput.a
HEADER	= $(INCDIR)/safeinput/safeinput.h


# Default target
all: $(LIB)
	@echo "[+] Buildfiles dropped in /build"

# Compile .o
$(OBJ): $(SRC)
	@mkdir -p $(OBJDIR)
	@if $(CC) $(CFLAGS) -c $< -o $@; then \
		echo "[+] Compiled $<"; \
	else \
		echo "[!] Compiler error"; \
		exit 1; \
	fi

# Archive .a
$(LIB): $(OBJ)
	@mkdir -p $(LIBDIR)
	@$(AR) $(ARFLAGS) $@ $^

# Clean
clean:
	@rm -rf $(BUILDDIR)
	@echo "[+] Cleaned build artifacts"

# Install
install: $(LIB)
	@install -d $(DESTDIR)$(INCLUDEDIR)
	@install -m 644 $(HEADER) $(DESTDIR)$(INCLUDEDIR)
	@install -d $(DESTDIR)$(LIBINSTALL)
	@install -m 644 $(LIB) $(DESTDIR)$(LIBINSTALL)
	@echo "[+] Installed to $(DESTDIR)$(PREFIX)"

# Uninstall
uninstall:
	@rm -f $(DESTDIR)$(INCLUDEDIR)/safeinput.h
	@rm -f $(DESTDIR)$(LIBINSTALL)/libsafeinput.a
	@echo "[+] Uninstalled from $(DESTDIR)$(PREFIX)"

.PHONY: all clean install uninstall
