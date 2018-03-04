TARGET     = btree.x

CC         = g++
GENERIC_F  = -Wall -Wextra -I. -Idoctest/doctest/

CFLAGS     = -O2 -std=c++11 $(GENERIC_F)
DEBUG_MODE = -DDEBUG -g

LINKER     = g++
LFLAGS     = $(GENERIC_F) -lm

SRCDIR     = src
OBJDIR     = build
BINDIR     = bin

SOURCES    = $(wildcard $(SRCDIR)/*.cc)
INCLUDES   = $(wildcard $(SRCDIR)/*.h)
OBJECTS    = $(SOURCES:$(SRCDIR)/%.cc=$(OBJDIR)/%.o)
rm         = rm -f

FIXED_ARGS = -d

.PHONY: format clear_screen test tests extend_cflags valgrind

# https://stackoverflow.com/a/3267187/ and https://stackoverflow.com/a/2714110/
test: tests
tests: | clean extend_cflags $(BINDIR)/$(TARGET) clear_screen
	$(BINDIR)/$(TARGET) $(ARGS) $(FIXED_ARGS)

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

valgrind: | clean extend_cflags $(BINDIR)/$(TARGET) clear_screen
	valgrind $(VALGR_ARGS) ./$(BINDIR)/$(TARGET) $(ARGS) $(FIXED_ARGS)

.PHONY: clean
clean:
	@$(rm) $(BINDIR)/$(TARGET) $(OBJECTS)
	@echo -e "Cleanup complete!\n"

clear_screen:
	@clear

extend_cflags:
	$(eval CFLAGS += $(DEBUG_MODE))

