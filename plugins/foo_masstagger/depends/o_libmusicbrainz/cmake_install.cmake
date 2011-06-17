# Install script for directory: D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/libmusicbrainz3")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "C:/Program Files (x86)/libmusicbrainz3/include/musicbrainz3" TYPE FILE FILES
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/artist.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/artistalias.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/defines.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/disc.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/entity.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/factory.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/filters.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/includes.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/iwebservice.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/label.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/labelalias.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/lists.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/mbxmlparser.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/mb_c.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/metadata.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/model.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/musicbrainz.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/query.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/relation.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/release.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/releaseevent.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/results.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/tag.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/track.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/user.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/utils.h"
    "D:/Foobar/plugins/foo_masstagger/depends/libmusicbrainz-3.0.2/include/musicbrainz3/webservice.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "C:/Program Files (x86)/libmusicbrainz3/lib/pkgconfig" TYPE FILE FILES "D:/Foobar/plugins/foo_masstagger/depends/o_libmusicbrainz/libmusicbrainz3.pc")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("D:/Foobar/plugins/foo_masstagger/depends/o_libmusicbrainz/src/cmake_install.cmake")
  INCLUDE("D:/Foobar/plugins/foo_masstagger/depends/o_libmusicbrainz/examples/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "D:/Foobar/plugins/foo_masstagger/depends/o_libmusicbrainz/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "D:/Foobar/plugins/foo_masstagger/depends/o_libmusicbrainz/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
