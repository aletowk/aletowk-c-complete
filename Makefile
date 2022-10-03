# Output file basename
OUTPUT	= App
# Output directories
BIN		= bin
OBJ		= obj

#-------------------------------------------------------------------------------
#		Tools
#-------------------------------------------------------------------------------
# Doxygen tools

DOC 	= doc
HTML 	= $(DOC)/html/
DOXYFILE= $(DOC)/Doxyfile 
BROWSER	= firefox

# Compilation tools
CC		= gcc
SIZE	= size
STRIP	= strip
OBJCOPY	= objcopy
DEBUGGER= gdb

# Flags
INCLUDES	= -Isrc/
INCLUDES   += -Isrc/common/
INCLUDES   += -Isrc/queue/
INCLUDES   += -Isrc/state_manager/
INCLUDES   += -Isrc/tcp_server/
INCLUDES   += -Isrc/static-allocation/
INCLUDES   += -Isrc/ram_storage/
INCLUDES   += -Isrc/packet_store_sdcard/
INCLUDES   += -Isrc/cut_tm/
INCLUDES   += -Isrc/binary_search/

OPTIMIZATION	= -O0
DEF_THREAD = -D_REENTRANT
CFLAGS	= $(OPTIMIZATION) -Wall -Wextra -g3 $(DEF_THREAD) 
LDFLAGS = $(OPTIMIZATION) -Wall -Wextra -g3 $(DEF_THREAD)
LDLIBS = -lpthread
#-------------------------------------------------------------------------------
#		FILES
#-------------------------------------------------------------------------------

# Directories where source files can be found
VPATH    = src/
VPATH   += src/common/
VPATH   += src/queue/
VPATH   += src/state_manager/
VPATH   += src/tcp_server/
VPATH   += src/static-allocation/
VPATH   += src/ram_storage/
VPATH   += src/packet_store_sdcard/
VPATH   += src/cut_tm/
VPATH   += src/binary_search/


# Objects build from C source files
C_OBJECTS	 = main.o
C_OBJECTS	+= common.o
C_OBJECTS	+= queue.o
C_OBJECTS	+= state_manager.o
C_OBJECTS	+= state_survival.o
C_OBJECTS	+= state_mission.o
C_OBJECTS	+= tcp_server.o
C_OBJECTS	+= static_allocation.o
C_OBJECTS	+= ram_storage.o
C_OBJECTS	+= packet_store_sdcard.o
C_OBJECTS	+= cut_tm.o
C_OBJECTS	+= binary_search.o

#Append OBJ and BIN directories to output filename
OUTPUT_BIN	= $(BIN)/$(OUTPUT).bin

#-------------------------------------------------------------------------------
#		Rules
#-------------------------------------------------------------------------------
all: $(BIN) $(OBJ) $(OUTPUT)

$(BIN) $(OBJ):
	mkdir $@

C_OBJECTS_OBJ = $(addprefix $(OBJ)/, $(C_OBJECTS))

$(OUTPUT): $(C_OBJECTS_OBJ) $(LDLIBS)
	@echo "Compilation complete"
	@$(CC) $(LDFLAGS) -o $(OUTPUT_BIN) $^
	@echo "Linking complete"
	@echo "Following, the size of objects and binary"
	@$(SIZE) $(C_OBJECTS_OBJ) $(OUTPUT_BIN)

$(C_OBJECTS_OBJ): $(OBJ)/%.o: %.c Makefile $(OBJ) $(BIN)
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<


debug: $(OUTPUT)
	@echo "Starting debug session of $(OUTPUT_BIN)"
	@$(DEBUGGER) $(OUTPUT_BIN)

.PHONY: clean run doc print_doc

run:
	@echo "Running the program $(OUTPUT_BIN):"
	@echo ""
	@./$(OUTPUT_BIN)

clean:
	rm -f $(BIN)/*.bin $(OBJ)/*.o -r $(HTML)

doc:
	doxygen $(DOXYFILE)

print_doc:
	$(BROWSER) $(HTML)/index.html