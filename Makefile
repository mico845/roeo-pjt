# 顶层 Makefile

# ==========================
# 编译配置
# ==========================
TARGET      	 = 	roeo
CC				 = 	gcc 
CFLAGS			 = 	-Wall -Wextra -O0 -g
LDFLAGS			 = 	
LDLIBS			 = 	$(ROEO_LIB)
BUILD_DIR    	 = 	build
AR               = 	ar
ARFLAGS          = 	rcs


# ==========================
# 头文件和源文件
# ==========================
# ROEO库路径
ROEO_HOME    := roeo-lib

INCLUDES 	 	 = 	$(ROEO_INCLUDES)

SRC_DIRS     	:= 	.
SRCS 			:= 	$(wildcard $(addsuffix /*.c, $(SRC_DIRS)))
OBJS 			:= 	$(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))


# ==========================
# 规则定义
# ==========================

# 链接
.PHONY: all

all : $(TARGET)
	@echo "Build complete: $(TARGET)"

# 引入roeo库规则
include $(ROEO_HOME)/Makefile

$(TARGET): $(ROEO_LIB) $(OBJS) 
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# 编译
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(ROEO_LIB)


print:
	@echo "CC: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo "LDLIBS: $(LDLIBS)"
	@echo "BUILD_DIR: $(BUILD_DIR)"
	@echo "AR: $(AR)"
	@echo "ARFLAGS: $(ARFLAGS)"
	@echo "INCLUDES: $(INCLUDES)"
	@echo "ROEO_ROOT: $(ROEO_OBJS)"
