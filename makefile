# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -Iinclude

# Source files
SRC = src/mygit.cpp src/init.cpp src/hash_object.cpp src/cat_file.cpp src/write_tree.cpp src/ls_tree.cpp src/add_files.cpp src/commit.cpp src/log.cpp src/checkout.cpp

# Output executable
TARGET = mygit

# Libraries (OpenSSL and zlib)
LIBS = -lssl -lcrypto -lz

# Default rule to build the executable
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

# Rule to clean up generated files
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean
