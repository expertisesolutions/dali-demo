SET(EXAMPLES_SRC_DIR ${ROOT_SRC_DIR}/examples)

# Get all subdirectory names from EXAMPLES_SRC_DIR
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()
SUBDIRLIST(SUBDIRS ${EXAMPLES_SRC_DIR})

FOREACH(EXAMPLE ${SUBDIRS})
  FILE(GLOB SRCS "${EXAMPLES_SRC_DIR}/${EXAMPLE}/*.cpp")
  SET(SRCS ${SRCS} "${ROOT_SRC_DIR}/shared/resources-location.cpp")
  IF(SHARED)
    ADD_LIBRARY(${EXAMPLE}.example SHARED ${SRCS})
  ELSE()
    ADD_EXECUTABLE(${EXAMPLE}.example ${SRCS})
  ENDIF()
  TARGET_LINK_LIBRARIES(${EXAMPLE}.example ${REQUIRED_LIBS})
  INSTALL(TARGETS ${EXAMPLE}.example DESTINATION ${BINDIR})
ENDFOREACH(EXAMPLE)
