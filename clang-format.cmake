file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)

add_custom_target(
        clangformat
        COMMAND /usr/bin/clang-format
        --style='{BasedOnStyle: llvm, ColumnLimit:     120}'
        -i
        ${ALL_SOURCE_FILES}
)

