find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

INCLUDE_DIRECTORIES(
  ${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include 
  ${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS}
  ../motr/include
)

SET(test_SRC
  main.cpp
  version_test.cpp
  visitor_test.cpp
  calculus.cpp
)

ADD_EXECUTABLE(motr_tests ${test_SRC})
TARGET_LINK_LIBRARIES(motr_tests ${GTEST_LIBRARIES} ${GMOCK_LIBRARIES} motrlib)