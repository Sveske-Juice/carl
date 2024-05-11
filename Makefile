.PHONY: debug tests clean
ROOT_DIR:=$(shell pwd)

debug:
	mkdir -p build/ && \
	cd build/ && \
	cmake -DCMAKE_BUILD_TYPE=Debug .. && \
	ln -sf '$(ROOT_DIR)/build/compile_commands.json' '$(ROOT_DIR)/compile_commands.json' && \
	make carl

tests:
	mkdir -p build/ && \
	cd build/ && \
	cmake -DCMAKE_BUILD_TYPE=Debug .. && \
	ln -sf '$(ROOT_DIR)/build/compile_commands.json' '$(ROOT_DIR)/compile_commands.json' && \
	make tests

docs: clean_docs git-update
	doxygen Doxyfile

clean_docs:
	rm -rf docs/

git-update:
	git submodule sync
	git submodule update --init --recursive

clean:
	rm -rf build/
	rm -rf docs/
