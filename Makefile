REPO_URL = https://github.com/catchorg/Catch2
LIB_DIR = ./lib
BUILD_DIR = ./cmake-build-release

all: setup


setup:
	@echo $(LIB_DIR)/Catch2;
	@if [ ! -d  "$(LIB_DIR)/Catch2" ]; then \
		echo "Cloning the repository into $(LIB_DIR)..."; \
		mkdir -p $(LIB_DIR); \
		git clone $(REPO_URL) $(LIB_DIR)/Catch2; \
	else \
		echo "Repository already exists at $(LIB_DIR)"; \
	fi

build:
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		echo "Creating a $(BUILD_DIR)..."; \
		mkdir $(BUILD_DIR); \
	fi

	(cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .)

clean:
	@echo "Removing Catch2 from $(LIB_DIR)"
	@rm -rf $(LIB_DIR)/Catch2

