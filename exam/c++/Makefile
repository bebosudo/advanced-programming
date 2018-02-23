TARGET     = btree.x

CC         = g++

CFLAGS     = -O0 -std=c++11 -Wall -Wextra -I. -Idoctest/doctest/
DEBUG_MODE = -DDEBUG

LINKER     = g++
LFLAGS     = -Wall -I. -lm

SRCDIR     = src
OBJDIR     = build
BINDIR     = bin

SOURCES    = $(wildcard $(SRCDIR)/*.cc)
INCLUDES   = $(wildcard $(SRCDIR)/*.h)
OBJECTS    = $(SOURCES:$(SRCDIR)/%.cc=$(OBJDIR)/%.o)
rm         = rm -f

.PHONY: format clear_screen tests

$(BINDIR)/$(TARGET): $(OBJECTS) $(INCLUDES)
	$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo -e "Linking complete!\n"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc $(INCLUDES)
	@mkdir -p $(OBJDIR) $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo -e "Compiled "$<" successfully!\n"

format: $(SOURCES)
	@clang-format -style=file -i $^ 2>/dev/null || echo "Install clang-format to format sources."
	@echo "Formatting done!"

test: $(BINDIR)/$(TARGET)
	valgrind ./$(BINDIR)/$(TARGET)

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET) $(OBJECTS)
	@echo "Executable removed!"

clear_screen:
	clear

# https://stackoverflow.com/a/3267187/ and https://stackoverflow.com/a/2714110/
tests: clean clear_screen
tests: CFLAGS += $(DEBUG_MODE)
tests: $(BINDIR)/$(TARGET)
	@$(BINDIR)/$(TARGET)

