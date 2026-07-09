set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Test)

function(add_lingmo_utils_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name}
        PRIVATE
            Lingmo::Utils
            Qt6::Test
    )
    target_compile_definitions(${name}
        PRIVATE
            DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data"
    )
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_lingmo_utils_test(tst_Logger)
add_lingmo_utils_test(tst_FileUtils)
add_lingmo_utils_test(tst_XdgUtils)
add_lingmo_utils_test(tst_Process)
