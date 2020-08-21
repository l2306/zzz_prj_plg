target	= plg_2nd

VPATH    =../
obj_dir  =$(TMP_OBJ_DIR)/$(target)
CC		 = g++
INCLUDE	 = -I. -I../plg
INCFLAGS =
#CFLAGS=-g -w
CFLAGS  = -Wall -shared -o 
LDFLAGS	=
tgt	    =$(BUILD_TODIR)/plg/$(target).so
src		=$(wildcard *.cpp ../plg/*.cpp)
cpps	=$(notdir $(src))
objs	=$(patsubst %.cpp,$(obj_dir)/%.o,$(src))

$(tgt):$(objs)
	$(CC) $^ $(dbg_or_not) $(LDFLAGS) $(CFLAGS) $@

$(obj_dir)/%.o:%.cpp
	mkdir -p $(obj_dir)
	cd $(obj_dir) && dirname $< | xargs mkdir -p
	$(CC) $< -c $(dbg_or_not) $(INCLUDE) -fPIC -o $@

.PHONY:clean
clean:
	-rm -f $(objs) $(tgt)
	
mem:
	valgrind --leak-check=yes ./$(target)
