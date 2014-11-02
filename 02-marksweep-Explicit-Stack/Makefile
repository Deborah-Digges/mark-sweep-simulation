program_NAME := marksweep
program_CPP_SRCS := $(wildcard *.cpp)
program_CPP_OBJS := ${program_CPP_SRCS:.cpp=.o}
CPPFLAGS +=  -g
program_INCLUDE_DIRS := .
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))

.PHONY: all clean distclean run

all: $(program_NAME)

$(program_NAME): $(program_CPP_OBJS)
	$(CXX) $(program_CPP_OBJS) -o $(program_NAME)

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_CPP_OBJS)

distclean: clean

run: all
	valgrind  --leak-check=yes ./$(program_NAME)




