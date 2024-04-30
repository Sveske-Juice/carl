ROOT_DIR:=$(shell pwd)

all:
	mkdir -p build/ && \
	cd build/ && \
	cmake .. && \
	ln -sf '$(ROOT_DIR)/build/compile_commands.json' '$(ROOT_DIR)/compile_commands.json' && \
	make
