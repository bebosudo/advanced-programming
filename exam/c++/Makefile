TARGET   = btree.x

CC       = g++
CFLAGS   = -O0 -std=c++11 -Wall -Wextra -I. -Idoctest/doctest/

LINKER   = g++
LFLAGS   = -Wall -I. -lm

SRCDIR   = src
OBJDIR   = build
BINDIR   = bin

SOURCES  = $(wildcard $(SRCDIR)/*.cc)
INCLUDES = $(wildcard $(SRCDIR)/*.h)
OBJECTS  = $(SOURCES:$(SRCDIR)/%.cc=$(OBJDIR)/%.o)
rm       = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS) $(INCLUDES)
	$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

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

