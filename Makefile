# COLORS
BLACK   := $(shell tput -Txterm setaf 0)
RED     := $(shell tput -Txterm setaf 1)
GREEN   := $(shell tput -Txterm setaf 2)
YELLOW  := $(shell tput -Txterm setaf 3)
BLUE    := $(shell tput -Txterm setaf 4)
MAGENTA := $(shell tput -Txterm setaf 5)
CYAN    := $(shell tput -Txterm setaf 6)
WHITE   := $(shell tput -Txterm setaf 7)
RESET   := $(shell tput -Txterm sgr0)

TARGET_MAX_CHAR_NUM=20

TARGET ?= main
SRC_DIRS ?= src

CXX ?= g++

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS  := src
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS += $(INC_FLAGS) -MMD -MP -DIMGUI_IMPL_OPENGL_LOADER_GLAD -g -O2 -std=c++11 -Wall -Wextra
LDLIBS += -lglfw -ldl -lm

DOCX := doxygen
DOCSRC := Doxyfile
DOC_DIRS  := doc

.PHONY: clean all help doc main

all: help


## Build the project
main: $(OBJS)
	@printf "\n\n\n########## BUILDING THE PROJECT ##########\n\n\n"
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)
	@printf "\n\n########## DONE ##########\n\n\n"


## Build the documentation
doc:
	@printf "\n\n\n########## BUILDING THE DOCUMENTATION ##########\n\n\n"
	$(DOCX) $(DOCSRC)
	@printf "\n\n########## DONE ##########\n\n\n"


## Clean the project
clean:
	@printf "\n\n\n########## CLEANING THE PROJECT ##########\n\n\n"
	$(RM) $(TARGET) $(OBJS) $(DEPS) 
	$(RM) -r $(DOC_DIRS)
	@printf "\n\n########## DONE ##########\n\n\n"


## Show help
help:
	@echo ''
	@echo 'Usage:'
	@echo '  ${YELLOW}make${RESET} ${GREEN}<target>${RESET}'
	@echo ''
	@echo 'Targets:'
	@awk '/^[a-zA-Z\-_0-9]+:/ { \
			helpMessage = match(lastLine, /^## (.*)/); \
			if (helpMessage) { \
					helpCommand = substr($$1, 0, index($$1, ":")-1); \
					helpMessage = substr(lastLine, RSTART + 3, RLENGTH); \
					printf "  ${YELLOW}%-$(TARGET_MAX_CHAR_NUM)s${RESET} ${GREEN}%s${RESET}\n", helpCommand, helpMessage; \
			} \
	} \
	{ lastLine = $$0 }' $(MAKEFILE_LIST)
	@echo ''


-include $(DEPS)