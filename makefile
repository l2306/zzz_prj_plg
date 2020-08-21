target  =zzz_app

obj_dir = $(TMP_OBJ_DIR)
CC      = g++
INCLUDE = -I. -I./plugins
INCFLAGS=
CFLAGS  = -Wall -g -rdynamic -o  
LDFLAGS = -ldl
tgt     = $(BUILD_TODIR)/$(target)
src     = $(wildcard 3rd/tinyXML2/*.cpp 3rd/md5/*.cpp 0utils/*.cpp plg/*.cpp plugin/*.cpp *.cpp)
cpps    = $(notdir $(src))
objs    = $(patsubst %.cpp,$(obj_dir)/%.o,$(src))

$(tgt):$(objs)
	$(CC) $^ $(LDFLAGS) -std=c++11 $(CFLAGS) $@

$(obj_dir)/%.o:%.cpp
	cd $(obj_dir) && dirname $< | xargs mkdir -p
	$(CC) $< $(INCLUDE) -fPIC -std=c++11 -c -o $@

.PHONY:clean
clean:
	-rm -rf $(objs) $(tgt) $(TMP_OBJ_DIR)
	
mem:
	valgrind --leak-check=yes ./$(tgt)

