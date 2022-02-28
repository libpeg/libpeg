enable_testing()

include(GNUInstallDirs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/)

set(TESTCASES
    test_vm_0001
)


foreach (TESTCASE ${TESTCASES})
    add_executable("${TESTCASE}"
        tinytest.h
        ${TESTCASE}.c
    )

    target_link_libraries("${TESTCASE}" PUBLIC
        "${PROJECT_LIB}"
    )

    add_test(NAME "${TESTCASE}" COMMAND "./${TESTCASE}")

endforeach()

add_custom_target("check"
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS ${PROJECT_LIB} ${TESTCASES}
)
