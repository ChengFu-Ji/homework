#COMMANDS
MAKE = make
CMAKE = cmake
MKDIR = mkdir
CD = cd
MV = mv

#DIRS
SERVER_DIR      = server
CLIENT_DIR      = whiteboard
BUILD_DIR       = build
CMAKE_BUILD_DIR = $(BUILD_DIR)/whiteboardBuild

SOURCE          = $(SERVER_DIR) $(CMAKE_BUILD_DIR)

#OUTPUT FILES 
FILES   = socketServer WhiteboardClient 
TARGETS = $(join $(addsuffix /, $(SOURCE)), $(FILES))

#BUILD TARGETS and MOVE TO BUILD_DIR
all: CMAKE_BUILD
	@for i in $(SOURCE); do ($(CD) $$i && $(MAKE)); done
	@for j in $(TARGETS); do if [ -e $$j ]; then($(MV) $$j $(BUILD_DIR)); fi done

#use CMAKE command BUILD to generate Makefile in CMAKE_BUILD_DIR 
.PHONY: CMAKE_BUILD
CMAKE_BUILD: $(CMAKE_BUILD_DIR)
	$(CMAKE) -S $(CLIENT_DIR) -B $^;

#stores generated files from CMAKE command
$(CMAKE_BUILD_DIR): $(BUILD_DIR)
	$(MKDIR) $@ 

#stores executeable files and CMAKE_BUILD_DIR
$(BUILD_DIR): 
	$(MKDIR) $@

#delete all executeable files
.PHONY: clean
clean:
	-@rm $(addprefix $(BUILD_DIR)/, $(SOURCE)) 

#delete BUILD_DIR
.PHONY: allclean
allclean:
	-@rm -rf $(BUILD_DIR)
