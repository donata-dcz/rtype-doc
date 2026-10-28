include (GoogleTest)

function(rtype_add_test target)
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} PRIVATE RTypeWarnings GTest::gtest_main)
    gtest_discover_tests(${target})
endfunction()    

rtype_add_test(EngineTests Src/EngineTests.cpp)
target_link_libraries(EngineTests PRIVATE Engine)

rtype_add_test(ProtocolTests Src/ProtocolTests.cpp)
target_link_libraries(ProtocolTests PRIVATE Common RType)

rtype_add_test(PacmanTests Src/PacmanTests.cpp)
target_link_libraries(PacmanTests PRIVATE PacmanCore)
