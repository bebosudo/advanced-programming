TARGET     = btree.x

CC         = g++
GENERIC_F  = -Wall -Wextra -I. -Idoctest/doctest/

CFLAGS     = -O0 -std=c++11 $(GENERIC_F)
DEBUG_MODE = -DDEBUG

LINKER     = g++
LFLAGS     = $(GENERIC_F) -lm

SRCDIR     = src
OBJDIR     = build
BINDIR     = bin

SOURCES    = $(wildcard $(SRCDIR)/*.cc)
INCLUDES   = $(wildcard $(SRCDIR)/*.h)
OBJECTS    = $(SOURCES:$(SRCDIR)/%.cc=$(OBJDIR)/%.o)
rm         = rm -f

.PHONY: format clear_screen test tests extend_cflags valgrind_test

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

valgrind_test: $(BINDIR)/$(TARGET)
	valgrind ./$(BINDIR)/$(TARGET)

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"

clear_screen:
	@clear

extend_cflags:
	$(eval CFLAGS += $(DEBUG_MODE))

# https://stackoverflow.com/a/3267187/ and https://stackoverflow.com/a/2714110/
test: tests
tests: | remove extend_cflags $(BINDIR)/$(TARGET) clear_screen
	@$(BINDIR)/$(TARGET)
