find_package(LLVM REQUIRED)
find_package(BISON REQUIRED)
bison_target(parser parser.y ${CMAKE_CURRENT_SOURCE_DIR}/parser.cc 
        DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/parser.hh COMPILE_FLAGS --debug)
add_executable(sysu-parser main.cc ${BISON_parser_OUTPUT_SOURCE})
llvm_map_components_to_libnames(LLVM_LIBS core support)
target_link_libraries(sysu-parser ${LLVM_LIBS})
target_include_directories(sysu-parser PRIVATE ${LLVM_INCLUDE_DIRS} )
install(TARGETS sysu-parser)

