CC=gcc
CFLAGS=-std=c11 -g -Wall -Wextra -Werror -D DEBUG
SRCDIR=src
BUILDDIR=build
TARGET=bin/run
SRCEXT=c
SOURCES=$(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS=$(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
INC=-I include

$(TARGET): $(OBJECTS)
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB) -O3

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(@D)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $< -save-temps -O3

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

