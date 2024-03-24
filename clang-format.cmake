file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h *.hpp)

add_custom_target(
        clangformat
        COMMAND /usr/bin/clang-format
        -style=LLVM
        -i
        ${ALL_SOURCE_FILES}
)

