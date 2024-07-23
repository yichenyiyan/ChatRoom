# 定义编译器  
CXX=g++  
  
# 定义编译选项  
CXXFLAGS=-Wall -g -std=c++11  
  
# 定义链接库  
LDLIBS=-lmysqlclient -lpthread  
  
# 定义源文件列表（使用wildcard函数获取所有.cc文件）  
SRCS=$(wildcard *.cc)  
  
# 定义目标文件名  
TARGET=server  
  
# 默认目标  
all: $(TARGET)  
  
# 链接规则  
$(TARGET): $(SRCS:.cc=.o)  
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)  
  
# 编译规则（通常隐式规则足够，但这里显式写出也无妨）  
%.o: %.cc  
	$(CXX) $(CXXFLAGS) -c $< -o $@  
  
# 清理编译生成的文件  
clean:  
	rm -f $(TARGET) $(SRCS:.cc=.o)  
  
# 伪目标声明  
.PHONY: all clean