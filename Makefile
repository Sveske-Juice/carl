ROOT_DIR:=$(shell pwd)

all:
	mkdir -p build/ && \
	cd build/ && \
	cmake .. && \
	ln -s '$(ROOT_DIR)/build/compile_commands.json' '$(ROOT_DIR)/compile_commands.json' # for auto completion; needs to be in root dir
