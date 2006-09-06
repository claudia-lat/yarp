
INCLUDE(UsePkgConfig)

SET(YARP_DEFINES_ACCUM "-DYARP_PRESENT")

# on windows, we have to tell ace how it was compiled
IF (WIN32 AND NOT CYGWIN)
  SET(YARP_DEFINES_ACCUM ${YARP_DEFINES_ACCUM} -DWIN32 -D_WINDOWS)
ELSE (WIN32 AND NOT CYGWIN)
  # flush out warnings
#  ADD_DEFINITIONS(-Wall)
  # don't add to YARP_DEFINES_ACCUM since clients may not want it
ENDIF (WIN32 AND NOT CYGWIN)

## check if we are on cygwin
IF(WIN32 AND CYGWIN)
  SET(YARP_DEFINES_ACCUM ${YARP_DEFINES_ACCUM} -DCYGWIN)
ENDIF(WIN32 AND CYGWIN)

# check endianness
IF(EXISTS "${CMAKE_ROOT}/Modules/TestBigEndian.cmake")
    INCLUDE(TestBigEndian)
    TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
    IF(${IS_BIG_ENDIAN})
	SET(YARP_DEFINES_ACCUM ${YARP_DEFINES_ACCUM} -DYARP_BIG_ENDIAN)
    ELSE(${IS_BIG_ENDIAN})
	SET(YARP_DEFINES_ACCUM ${YARP_DEFINES_ACCUM} -DYARP_LITTLE_ENDIAN)
    ENDIF(${IS_BIG_ENDIAN})
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/TestBigEndian.cmake")

FIND_PACKAGE(Ace REQUIRED)

IF (CREATE_GUIS)
  FIND_PACKAGE(GtkPlus)
  IF (NOT GtkPlus_FOUND)
	MESSAGE(ERROR "gtk+ not found, won't compile dependent tools")
  ENDIF (NOT GtkPlus_FOUND)
ENDIF (CREATE_GUIS)

FIND_PACKAGE(Yarp1)

IF (ACE_DEBUG_FOUND)
 LINK_LIBRARIES(optimized ${ACE_LIBRARY} debug ${ACE_DEBUG_LIBRARY})
 SET(ACE_LIB optimized ${ACE_LIBRARY} debug ${ACE_DEBUG_LIBRARY} CACHE INTERNAL "libraries")
ELSE (ACE_DEBUG_FOUND)
 LINK_LIBRARIES(${ACE_LIBRARY})
 SET(ACE_LIB ${ACE_LIBRARY} CACHE INTERNAL "libraries")
ENDIF (ACE_DEBUG_FOUND)

SET(YARP_DEFINES "${YARP_DEFINES_ACCUM}" CACHE STRING "Definitions needed when compiling with YARP")
MARK_AS_ADVANCED(YARP_DEFINES)
ADD_DEFINITIONS(${YARP_DEFINES})

INCLUDE_DIRECTORIES(${ACE_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${ACE_INCLUDE_CONFIG_DIR})

# msvc problems...
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib ${CMAKE_BINARY_DIR}/lib/Debug ${CMAKE_BINARY_DIR}/lib/Release)
# again, can't do this on windows without a lot of mess
#SET(BUILD_SHARED_LIBS ON)

