# Makefile

SOURCES += ./src/main.cpp

# SOURCES += ./src/bitree.cpp
SOURCES += ./src/graph.cpp

INCLUDE_PATH += ./include

BINDIR   = ./bin
BUILDDIR = ./build
OBJDIR   = $(BUILDDIR)/obj
EXECUTABLE = main

CC     = g++
CFLAGS = -I. -I$(INCLUDE_PATH)

OBJS = $(patsubst ./src/%.cpp,$(OBJDIR)/src/%.o,$(SOURCES))


all: $(BINDIR) $(BUILDDIR) $(OBJDIR) $(EXECUTABLE)

# 创建必要构建目录
$(BUILDDIR):
	@mkdir -p $(BUILDDIR)
	@echo "Build directory created at $(BUILDDIR)"
$(BINDIR):
	@mkdir -p $(BINDIR)
	@echo "Binary directory created at $(BINDIR)"

# 创建对象文件目录
$(OBJDIR):
	@mkdir -p $(OBJDIR)
	@echo "Object directory created at $(OBJDIR)"	

# 规则：编译源文件为对象文件
$(OBJDIR)/src/%.o: ./src/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# 规则：链接对象文件生成可执行文件
$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(BINDIR)/$@


# 查看汇编
code: $(EXECUTABLE)
	@echo "====== Generating assembly code ======"
	@$(CC) -S $(CFLAGS) $(SOURCES) -o $(BUILDDIR)/$(EXECUTABLE).s
	@echo "Assembly code generated at $(BUILDDIR)/$(EXECUTABLE).s"


# 伪目标：运行可执行文件
run: $(EXECUTABLE)
	@echo "====== Running $(EXECUTABLE) ======"
	@$(BINDIR)/$(EXECUTABLE)


# 伪目标：清理构建目录和可执行文件
clean:
	@rm -rf $(BUILDDIR)
	@echo "Cleaned up build directories and executable"


# 伪目标：重新构建并运行
crun: clean run
