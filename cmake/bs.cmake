# --------------------------------------------------------------------------------------------------
# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: (C) 2022 Jayesh Badwaik <j.badwaik@fz-juelich.de>
# --------------------------------------------------------------------------------------------------


function(target_source_directory TARGETNAME LINK_TYPE DIR_NAME)

  file(GLOB_RECURSE SOURCE_FILE_LIST LIST_DIRECTORIES false
    RELATIVE ${CMAKE_CURRENT_LIST_DIR} CONFIGURE_DEPENDS ${DIR_NAME}/*)

  foreach(SRC_FILE ${SOURCE_FILE_LIST})
    target_sources(${TARGETNAME} ${LINK_TYPE} ${SRC_FILE})
  endforeach()

endfunction()

function(target_header_directory TARGETNAME LINK_TYPE DIR_NAME)

  file(GLOB_RECURSE HEADER_FILE_LIST LIST_DIRECTORIES false
    RELATIVE ${CMAKE_CURRENT_LIST_DIR} CONFIGURE_DEPENDS ${DIR_NAME}/*)

  file(REAL_PATH ${DIR_NAME} ABS_DIR_NAME BASE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

  target_sources(
    ${TARGETNAME} ${LINK_TYPE} FILE_SET HEADERS BASE_DIRS ${ABS_DIR_NAME}
    FILES ${HEADER_FILE_LIST})
endfunction()



function(unit_test_directory DIR_NAME TARGET_LIB_LIST PREFIX LABELNAME)
  file(
    REAL_PATH
    ${DIR_NAME}
    ABS_DIR_NAME
    BASE_DIRECTORY
    ${CMAKE_CURRENT_LIST_DIR})
  file(GLOB SOURCE_FILE_LIST LIST_DIRECTORIES false
    RELATIVE ${ABS_DIR_NAME} CONFIGURE_DEPENDS ${DIR_NAME}/*)

  file(
    RELATIVE_PATH
    REL_DIR_NAME
    ${CMAKE_CURRENT_LIST_DIR}
    ${ABS_DIR_NAME}
    )


  foreach(SRC_FILE ${SOURCE_FILE_LIST})
    string(REGEX REPLACE "\\.[^.]*$" "" TARGETNAME ${SRC_FILE})
    string(REGEX REPLACE "/" "." TARGETNAME ${TARGETNAME})
    set(TARGETNAME "${PREFIX}.${REL_DIR_NAME}.${TARGETNAME}")

    string(REGEX REPLACE "\\.[^.]*$" "" TESTDIRNAME ${SRC_FILE})
    string(REGEX REPLACE "\\.[^.]*$" "" TESTDIRNAME ${TESTDIRNAME})


    add_executable(${TARGETNAME} ${ABS_DIR_NAME}/${SRC_FILE})
    add_test(
      NAME ${TARGETNAME}
      COMMAND ${TARGETNAME} ${TORA_PASS_ARGS}
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
      )
    foreach(TARGET_LIB ${TARGET_LIB_LIST})
      target_link_libraries(${TARGETNAME} PRIVATE ${TARGET_LIB})
    endforeach()
    if(TARGETNAME MATCHES ".fail.t$")
      set_tests_properties(${TARGETNAME} PROPERTIES WILL_FAIL TRUE)
    endif()

    if(TARGETNAME MATCHES "disabled.t$")
      set_tests_properties(${TARGETNAME} PROPERTIES DISABLED TRUE)
    endif()

    if(TARGETNAME MATCHES "disabled.fail.t$")
      set_tests_properties(${TARGETNAME} PROPERTIES DISABLED TRUE)
    endif()


    set_property(TEST ${TARGETNAME} PROPERTY LABELS ${LABELNAME})
  endforeach()
endfunction()
