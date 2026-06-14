
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

macro(xos_declare_project name)
    set(XOS_CURRENT_INTERFACE "${name}_inn_interface")
    set(XOS_CURRENT_CATCH_LIB "${name}_inn_catch_lib")

    MESSAGE(STATUS "declare XOS_CURRENT_INTERFACE: ${XOS_CURRENT_INTERFACE}\n")
    MESSAGE(STATUS "declare XOS_CURRENT_CATCH_LIB: ${XOS_CURRENT_CATCH_LIB}\n")

    add_library(${XOS_CURRENT_INTERFACE} INTERFACE)

    define_property(TARGET PROPERTY associated_libs 
        BRIEF_DOCS "haha"
        FULL_DOCS  "hehe"
    )
    set_target_properties(${XOS_CURRENT_INTERFACE} PROPERTIES 
        associated_libs ""
    )
    #
    # xos is a catchall CMake library for source files that can be
    # built purely with the include paths, defines, and other compiler
    # flags that come with ${XOS_CURRENT_INTERFACE}.
    #
    xos_library_named(${XOS_CURRENT_CATCH_LIB})
    target_sources(${XOS_CURRENT_CATCH_LIB} PRIVATE
      ${EMPTY_FILE}
    )
    set_target_properties(${XOS_CURRENT_CATCH_LIB} PROPERTIES 
                ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    set_property(GLOBAL APPEND PROPERTY XOS_CATCH_LIB ${XOS_CURRENT_CATCH_LIB})

    set_property(GLOBAL APPEND PROPERTY XOS_INTERFACE ${XOS_CURRENT_INTERFACE})

    set(OUTPUT_OBJLIBLIST_FILE ${name}_objliblist.txt)
    define_property(TARGET PROPERTY associated_objliblist_file 
        BRIEF_DOCS "haha"
        FULL_DOCS  "hehe"
    )
    set_target_properties(${XOS_CURRENT_INTERFACE} PROPERTIES 
        associated_objliblist_file "${OUTPUT_OBJLIBLIST_FILE}"
    )
endmacro()

macro(xos_print_current_project)
    get_target_property(defs ${XOS_CURRENT_INTERFACE} INTERFACE_COMPILE_DEFINITIONS)
    get_target_property(opts ${XOS_CURRENT_INTERFACE} INTERFACE_COMPILE_OPTIONS)
    get_target_property(incs ${XOS_CURRENT_INTERFACE} INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(srcs ${XOS_CURRENT_INTERFACE} INTERFACE_SOURCES)
    MESSAGE(STATUS "\nXOS_CURRENT_INTERFACE: ${XOS_CURRENT_INTERFACE}")
    MESSAGE(STATUS "defs: ${defs}")
    MESSAGE(STATUS "opts: ${opts}")
    MESSAGE(STATUS "incs: ${incs}")
    MESSAGE(STATUS "srcs: ${srcs}")
    get_target_property(defs ${XOS_CURRENT_CATCH_LIB} COMPILE_DEFINITIONS)
    get_target_property(opts ${XOS_CURRENT_CATCH_LIB} COMPILE_OPTIONS)
    get_target_property(incs ${XOS_CURRENT_CATCH_LIB} INCLUDE_DIRECTORIES)
    get_target_property(srcs ${XOS_CURRENT_CATCH_LIB} SOURCES)
    MESSAGE(STATUS "\nXOS_CURRENT_CATCH_LIB: ${XOS_CURRENT_CATCH_LIB}")
    MESSAGE(STATUS "defs: ${defs}")
    MESSAGE(STATUS "opts: ${opts}")
    MESSAGE(STATUS "incs: ${incs}")
    MESSAGE(STATUS "srcs: ${srcs}")
endmacro()

macro(xos_generate_objliblist_file)
    get_target_property(objliblist_file ${XOS_CURRENT_INTERFACE} associated_objliblist_file)
    set(OUTPUT_OBJLIBLIST_FILE ${CMAKE_CURRENT_LIST_DIR}/${objliblist_file})
    file(REMOVE ${OUTPUT_OBJLIBLIST_FILE})

    get_target_property(libs ${XOS_CURRENT_INTERFACE} associated_libs)
    if (${CONFIG_CMAKE_DEBUG})
        MESSAGE(STATUS "\n")
    endif()

    foreach(cur_lib ${libs})
        get_property(archive_dir TARGET ${cur_lib} PROPERTY ARCHIVE_OUTPUT_DIRECTORY)
        #
        # TODO: CHECK archive_dir
        #
        file(RELATIVE_PATH rel_dir ${CMAKE_CURRENT_BINARY_DIR} ${archive_dir})
        if ("${rel_dir}" STREQUAL "")
            set(rel_dir "./")
        endif()
        set(rel_dir_lib "${rel_dir}/${CMAKE_STATIC_LIBRARY_PREFIX}${cur_lib}${CMAKE_STATIC_LIBRARY_SUFFIX}")
      if (0)
        if ("${cur_lib}" MATCHES "catchall$")
            file(APPEND ${OUTPUT_OBJLIBLIST_FILE} "(${rel_dir_lib}")
        else()
            file(APPEND ${OUTPUT_OBJLIBLIST_FILE} ")${rel_dir_lib}")
        endif()
      else()
        file(APPEND ${OUTPUT_OBJLIBLIST_FILE} "./${rel_dir_lib}")
      endif()
        file(APPEND ${OUTPUT_OBJLIBLIST_FILE} "\n")
        if (${CONFIG_CMAKE_DEBUG})
            MESSAGE(STATUS "cur_lib     : ${cur_lib}")
            MESSAGE(STATUS "archive_dir : ${archive_dir}")
            MESSAGE(STATUS "cur_bin_dir : ${CMAKE_CURRENT_BINARY_DIR}")
            MESSAGE(STATUS "rel_dir     : ${rel_dir}")
            MESSAGE(STATUS "rel_dir_lib : ${rel_dir_lib}")
            MESSAGE(STATUS "\n")
        endif()
    endforeach()
endmacro()

# SPDX-License-Identifier: Apache-2.0

########################################################
# Table of contents
########################################################
# 1. Xos-aware extensions
# 1.1. xos_*
# 1.2. xos_library_*
# 1.2.1 xos_interface_library_*
# 1.3. generate_inc_*
# 1.4. board_*
# 1.5. Misc.
# 2. Kconfig-aware extensions
# 2.1 *_if_kconfig
# 2.2 Misc
# 3. CMake-generic extensions
# 3.1. *_ifdef
# 3.2. *_ifndef
# 3.3. *_option compiler compatibility checks
# 3.3.1 Toolchain integration
# 3.4. Debugging CMake
# 3.5. File system management

########################################################
# 1. Xos-aware extensions
########################################################
# 1.1. xos_*
#
# The following methods are for modifying the CMake library[0] called
# "xos". xos is a catch-all CMake library for source files that
# can be built purely with the include paths, defines, and other
# compiler flags that all xos source files use.
# [0] https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html
#
# Example usage:
# xos_sources(
#   random_esp32.c
#   utils.c
# )
#
# Is short for:
# target_sources(xos PRIVATE
#   ${CMAKE_CURRENT_SOURCE_DIR}/random_esp32.c
#   ${CMAKE_CURRENT_SOURCE_DIR}/utils.c
# )
#
# As a very high-level introduction here are two call graphs that are
# purposely minimalistic and incomplete.
#
#  xos_library_cc_option()
#           |
#           v
#  xos_library_compile_options()  -->  target_compile_options()
#
#
#  xos_cc_option()           --->  target_cc_option()
#                                          |
#                                          v
#  xos_cc_option_fallback()  --->  target_cc_option_fallback()
#                                          |
#                                          v
#  xos_compile_options()     --->  target_compile_options()
#


# https://cmake.org/cmake/help/latest/command/target_sources.html
function(xos_sources)
  foreach(arg ${ARGV})
    if(IS_DIRECTORY ${arg})
      message(FATAL_ERROR "xos_sources() was called on a directory")
    endif()
    target_sources(${XOS_CURRENT_CATCH_LIB} PRIVATE ${arg})
  endforeach()
endfunction()

# https://cmake.org/cmake/help/latest/command/target_include_directories.html
function(xos_include_directories)
  foreach(arg ${ARGV})
    if(IS_ABSOLUTE ${arg})
      set(path ${arg})
    else()
      set(path ${CMAKE_CURRENT_SOURCE_DIR}/${arg})
    endif()
    target_include_directories(${XOS_CURRENT_INTERFACE} INTERFACE ${path})
  endforeach()
endfunction()

# https://cmake.org/cmake/help/latest/command/target_include_directories.html
function(xos_system_include_directories)
  foreach(arg ${ARGV})
    if(IS_ABSOLUTE ${arg})
      set(path ${arg})
    else()
      set(path ${CMAKE_CURRENT_SOURCE_DIR}/${arg})
    endif()
    target_include_directories(${XOS_CURRENT_INTERFACE} SYSTEM INTERFACE ${path})
  endforeach()
endfunction()

# https://cmake.org/cmake/help/latest/command/target_compile_definitions.html
function(xos_compile_definitions)
  target_compile_definitions(${XOS_CURRENT_INTERFACE} INTERFACE ${ARGV})
endfunction()

# https://cmake.org/cmake/help/latest/command/target_compile_options.html
function(xos_compile_options)
  target_compile_options(${XOS_CURRENT_INTERFACE} INTERFACE ${ARGV})
endfunction()

# https://cmake.org/cmake/help/latest/command/target_link_libraries.html
function(xos_link_libraries)
  target_link_libraries(${XOS_CURRENT_INTERFACE} INTERFACE ${ARGV})
endfunction()

# See this file section 3.1. target_cc_option
function(xos_cc_option)
  foreach(arg ${ARGV})
    target_cc_option(${XOS_CURRENT_INTERFACE} INTERFACE ${arg})
  endforeach()
endfunction()

function(xos_cc_option_fallback option1 option2)
    target_cc_option_fallback(${XOS_CURRENT_INTERFACE} INTERFACE ${option1} ${option2})
endfunction()

function(xos_ld_options)
    target_ld_options(${XOS_CURRENT_INTERFACE} INTERFACE ${ARGV})
endfunction()

# Getter functions for extracting build information from
# ${XOS_CURRENT_INTERFACE}. Returning lists, and strings is supported, as is
# requesting specific categories of build information (defines,
# includes, options).
#
# The naming convention follows:
# xos_get_${build_information}_for_lang${format}(lang x [STRIP_PREFIX])
# Where
#  the argument 'x' is written with the result
# and
#  ${build_information} can be one of
#   - include_directories           # -I directories
#   - system_include_directories    # -isystem directories
#   - compile_definitions           # -D'efines
#   - compile_options               # misc. compiler flags
# and
#  ${format} can be
#   - the empty string '', signifying that it should be returned as a list
#   - _as_string signifying that it should be returned as a string
# and
#  ${lang} can be one of
#   - C
#   - CXX
#   - ASM
#
# STRIP_PREFIX
#
# By default the result will be returned ready to be passed directly
# to a compiler, e.g. prefixed with -D, or -I, but it is possible to
# omit this prefix by specifying 'STRIP_PREFIX' . This option has no
# effect for 'compile_options'.
#
# e.g.
# xos_get_include_directories_for_lang(ASM x)
# writes "-Isome_dir;-Isome/other/dir" to x

function(xos_get_include_directories_for_lang_as_string lang i)
  xos_get_include_directories_for_lang(${lang} list_of_flags ${ARGN})

  convert_list_of_flags_to_string_of_flags(list_of_flags str_of_flags)

  set(${i} ${str_of_flags} PARENT_SCOPE)
endfunction()

function(xos_get_system_include_directories_for_lang_as_string lang i)
  xos_get_system_include_directories_for_lang(${lang} list_of_flags ${ARGN})

  convert_list_of_flags_to_string_of_flags(list_of_flags str_of_flags)

  set(${i} ${str_of_flags} PARENT_SCOPE)
endfunction()

function(xos_get_compile_definitions_for_lang_as_string lang i)
  xos_get_compile_definitions_for_lang(${lang} list_of_flags ${ARGN})

  convert_list_of_flags_to_string_of_flags(list_of_flags str_of_flags)

  set(${i} ${str_of_flags} PARENT_SCOPE)
endfunction()

function(xos_get_compile_options_for_lang_as_string lang i)
  xos_get_compile_options_for_lang(${lang} list_of_flags)

  convert_list_of_flags_to_string_of_flags(list_of_flags str_of_flags)

  set(${i} ${str_of_flags} PARENT_SCOPE)
endfunction()

function(xos_get_include_directories_for_lang lang i)
  get_property_and_add_prefix(flags ${XOS_CURRENT_INTERFACE} INTERFACE_INCLUDE_DIRECTORIES
    "-I"
    ${ARGN}
    )

  process_flags(${lang} flags output_list)

  set(${i} ${output_list} PARENT_SCOPE)
endfunction()

function(xos_get_system_include_directories_for_lang lang i)
  get_property_and_add_prefix(flags ${XOS_CURRENT_INTERFACE} INTERFACE_SYSTEM_INCLUDE_DIRECTORIES
    "-isystem"
    ${ARGN}
    )

  process_flags(${lang} flags output_list)

  set(${i} ${output_list} PARENT_SCOPE)
endfunction()

function(xos_get_compile_definitions_for_lang lang i)
  get_property_and_add_prefix(flags ${XOS_CURRENT_INTERFACE} INTERFACE_COMPILE_DEFINITIONS
    "-D"
    ${ARGN}
    )

  process_flags(${lang} flags output_list)

  set(${i} ${output_list} PARENT_SCOPE)
endfunction()

function(xos_get_compile_options_for_lang lang i)
  get_property(flags TARGET ${XOS_CURRENT_INTERFACE} PROPERTY INTERFACE_COMPILE_OPTIONS)

  process_flags(${lang} flags output_list)

  set(${i} ${output_list} PARENT_SCOPE)
endfunction()

# This function writes a dict to it's output parameter
# 'return_dict'. The dict has information about the parsed arguments,
#
# Usage:
#   xos_get_parse_args(foo ${ARGN})
#   print(foo_STRIP_PREFIX) # foo_STRIP_PREFIX might be set to 1
function(xos_get_parse_args return_dict)
  foreach(x ${ARGN})
    if(x STREQUAL STRIP_PREFIX)
      set(${return_dict}_STRIP_PREFIX 1 PARENT_SCOPE)
    endif()
  endforeach()
endfunction()

function(process_flags lang input output)
  # The flags might contains compile language generator expressions that
  # look like this:
  # $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
  #
  # Flags that don't specify a language like this apply to all
  # languages.
  #
  # See COMPILE_LANGUAGE in
  # https://cmake.org/cmake/help/v3.3/manual/cmake-generator-expressions.7.html
  #
  # To deal with this, we apply a regex to extract the flag and also
  # to find out if the language matches.
  #
  # If this doesn't work out we might need to ban the use of
  # COMPILE_LANGUAGE and instead partition C, CXX, and ASM into
  # different libraries
  set(languages C CXX ASM)

  set(tmp_list "")

  foreach(flag ${${input}})
    set(is_compile_lang_generator_expression 0)
    foreach(l ${languages})
      if(flag MATCHES "<COMPILE_LANGUAGE:${l}>:([^>]+)>")
        set(is_compile_lang_generator_expression 1)
        if(${l} STREQUAL ${lang})
          list(APPEND tmp_list ${CMAKE_MATCH_1})
          break()
        endif()
      endif()
    endforeach()

    if(NOT is_compile_lang_generator_expression)
      list(APPEND tmp_list ${flag})
    endif()
  endforeach()

  set(${output} ${tmp_list} PARENT_SCOPE)
endfunction()

function(convert_list_of_flags_to_string_of_flags ptr_list_of_flags string_of_flags)
  # Convert the list to a string so we can do string replace
  # operations on it and replace the ";" list separators with a
  # whitespace so the flags are spaced out
  string(REPLACE ";"  " "  locally_scoped_string_of_flags "${${ptr_list_of_flags}}")

  # Set the output variable in the parent scope
  set(${string_of_flags} ${locally_scoped_string_of_flags} PARENT_SCOPE)
endfunction()

macro(get_property_and_add_prefix result target property prefix)
  xos_get_parse_args(args ${ARGN})

  if(args_STRIP_PREFIX)
    set(maybe_prefix "")
  else()
    set(maybe_prefix ${prefix})
  endif()

  get_property(target_property TARGET ${target} PROPERTY ${property})
  foreach(x ${target_property})
    list(APPEND ${result} ${maybe_prefix}${x})
  endforeach()
endmacro()

# 1.2 xos_library_*
#
# xos libraries use CMake's library concept and a set of
# assumptions about how xos code is organized to cut down on
# boilerplate code.
#
# A xos library can be constructed by the function xos_library
# or xos_library_named. The constructors create a CMake library
# with a name accessible through the variable XOS_CURRENT_LIBRARY.
#
# The variable XOS_CURRENT_LIBRARY should seldom be needed since
# the xos libraries have methods that modify the libraries. These
# methods have the signature: xos_library_<target-function>
#
# The methods are wrappers around the CMake target_* functions. See
# https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html for
# documentation on the underlying target_* functions.
#
# The methods modify the CMake target_* API to reduce boilerplate;
#  PRIVATE is assumed
#  The target is assumed to be XOS_CURRENT_LIBRARY
#
# When a flag that is given through the xos_* API conflicts with
# the xos_library_* API then precedence will be given to the
# xos_library_* API. In other words, local configuration overrides
# global configuration.

# Constructor with a directory-inferred name
macro(xos_library)
  xos_library_get_current_dir_lib_name(${XOS_BASE} lib_name)
  xos_library_named(${lib_name})
endmacro()

# Determines what the current directory's lib name would be according to the
# provided base and writes it to the argument "lib_name"
macro(xos_library_get_current_dir_lib_name base lib_name)
  # Remove the prefix (/home/sebo/xos/driver/serial/CMakeLists.txt => driver/serial/CMakeLists.txt)
  file(RELATIVE_PATH name ${base} ${CMAKE_CURRENT_LIST_FILE})

  # Remove the filename (driver/serial/CMakeLists.txt => driver/serial)
  get_filename_component(name ${name} DIRECTORY)

  # Replace / with __ (driver/serial => driver__serial)
  string(REGEX REPLACE "/" "__" name ${name})

  set(${lib_name} ${name})
endmacro()

# Constructor with an explicitly given name.
macro(xos_library_named name)
  # This is a macro because we need add_library() to be executed
  # within the scope of the caller.
  set(XOS_CURRENT_LIBRARY ${name})
  add_library(${name} STATIC "")

  xos_append_cmake_library(${name})

  target_link_libraries(${name} PUBLIC ${XOS_CURRENT_INTERFACE})
endmacro()

# Provides amend functionality to a xos library for out-of-tree usage.
#
# When called from a xos module, the corresponding xos library defined
# within xos will be looked up.
#
# Note, in order to ensure correct library when amending, the folder structure in the
# xos module must resemble the structure used in xos, as example:
#
# Example: to amend the xos library created in
# XOS_BASE/drivers/entropy/CMakeLists.txt
# add the following file:
# xos_MODULE/drivers/entropy/CMakeLists.txt
# with content:
# xos_library_amend()
# xos_libray_add_sources(...)
#
macro(xos_library_amend)
  # This is a macro because we need to ensure the XOS_CURRENT_LIBRARY and
  # following xos_library_* calls are executed within the scope of the
  # caller.
  if(NOT XOS_CURRENT_MODULE_DIR)
    message(FATAL_ERROR "Function only available for xos modules.")
  endif()

  xos_library_get_current_dir_lib_name(${XOS_CURRENT_MODULE_DIR} lib_name)

  set(XOS_CURRENT_LIBRARY ${lib_name})
endmacro()

function(xos_link_interface interface)
  target_link_libraries(${interface} INTERFACE ${XOS_CURRENT_INTERFACE})
endfunction()

#
# xos_library versions of normal CMake target_<func> functions
#
function(xos_library_sources source)
  target_sources(${XOS_CURRENT_LIBRARY} PRIVATE ${source} ${ARGN})
endfunction()

function(xos_library_include_directories)
  target_include_directories(${XOS_CURRENT_LIBRARY} PRIVATE ${ARGN})
endfunction()

function(xos_library_link_libraries item)
  target_link_libraries(${XOS_CURRENT_LIBRARY} PUBLIC ${item} ${ARGN})
endfunction()

function(xos_library_compile_definitions item)
  target_compile_definitions(${XOS_CURRENT_LIBRARY} PRIVATE ${item} ${ARGN})
endfunction()

function(xos_library_compile_options item)
  # The compiler is relied upon for sane behaviour when flags are in
  # conflict. Compilers generally give precedence to flags given late
  # on the command line. So to ensure that xos_library_* flags are
  # placed late on the command line we create a dummy interface
  # library and link with it to obtain the flags.
  #
  # Linking with a dummy interface library will place flags later on
  # the command line than the the flags from ${XOS_CURRENT_INTERFACE} because
  # ${XOS_CURRENT_INTERFACE} will be the first interface library that flags
  # are taken from.

  string(MD5 uniqueness ${item})
  set(lib_name options_interface_lib_${uniqueness})

  if (TARGET ${lib_name})
    # ${item} already added, ignoring duplicate just like CMake does
    return()
  endif()

  add_library(           ${lib_name} INTERFACE)
  target_compile_options(${lib_name} INTERFACE ${item} ${ARGN})

  target_link_libraries(${XOS_CURRENT_LIBRARY} PRIVATE ${lib_name})
endfunction()

function(xos_library_cc_option)
  foreach(option ${ARGV})
    string(MAKE_C_IDENTIFIER check${option} check)
    xos_check_compiler_flag(C ${option} ${check})

    if(${check})
      xos_library_compile_options(${option})
    endif()
  endforeach()
endfunction()

# Add the existing CMake library 'library' to the global list of
# xos CMake libraries. This is done automatically by the
# constructor but must called explicitly on CMake libraries that do
# not use a xos library constructor.
function(xos_append_cmake_library library)
  set_property(GLOBAL APPEND PROPERTY xos_LIBS ${library})
endfunction()

# Add the imported library 'library_name', located at 'library_path' to the
# global list of xos CMake libraries.
function(xos_library_import library_name library_path)
  add_library(${library_name} STATIC IMPORTED GLOBAL)
  set_target_properties(${library_name}
    PROPERTIES IMPORTED_LOCATION
    ${library_path}
    )
  xos_append_cmake_library(${library_name})
endfunction()

# Place the current xos library in the application memory partition.
#
# The partition argument is the name of the partition where the library shall
# be placed.
#
# Note: Ensure the given partition has been define using
#       K_APPMEM_PARTITION_DEFINE in source code.
function(xos_library_app_memory partition)
  set_property(TARGET xos_property_target
               APPEND PROPERTY COMPILE_OPTIONS
               "-l" $<TARGET_FILE_NAME:${XOS_CURRENT_LIBRARY}> "${partition}")
endfunction()

# 1.2.1 xos_interface_library_*
#
# A xos interface library is a thin wrapper over a CMake INTERFACE
# library. The most important responsibility of this abstraction is to
# ensure that when a user KConfig-enables a library then the header
# files of this library will be accessible to the 'app' library.
#
# This is done because when a user uses Kconfig to enable a library he
# expects to be able to include it's header files and call it's
# functions out-of-the box.
#
# A xos interface library should be used when there exists some
# build information (include directories, defines, compiler flags,
# etc.) that should be applied to a set of xos libraries and 'app'
# might be one of these libraries.
#
# xos libraries must explicitly call
# xos_library_link_libraries(<interface_library>) to use this build
# information. 'app' is treated as a special case for usability
# reasons; a Kconfig option (CONFIG_APP_LINK_WITH_<interface_library>)
# should exist for each interface_library and will determine if 'app'
# links with the interface_library.
#
# This API has a constructor like the xos_library API has, but it
# does not have wrappers over the other cmake target functions.
macro(xos_interface_library_named name)
  add_library(${name} INTERFACE)
  set_property(GLOBAL APPEND PROPERTY XOS_INTERFACE_LIBS ${name})
endmacro()

# 1.3 generate_inc_*

# These functions are useful if there is a need to generate a file
# that can be included into the application at build time. The file
# can also be compressed automatically when embedding it.
#
# See tests/application_development/gen_inc_file for an example of
# usage.
function(generate_inc_file
    source_file    # The source file to be converted to hex
    generated_file # The generated file
    )
  add_custom_command(
    OUTPUT ${generated_file}
    COMMAND
    ${PYTHON_EXECUTABLE}
    ${XOS_BASE}/scripts/file2hex.py
    ${ARGN} # Extra arguments are passed to file2hex.py
    --file ${source_file}
    > ${generated_file} # Does pipe redirection work on Windows?
    DEPENDS ${source_file}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endfunction()

function(generate_inc_file_for_gen_target
    target          # The cmake target that depends on the generated file
    source_file     # The source file to be converted to hex
    generated_file  # The generated file
    gen_target      # The generated file target we depend on
                    # Any additional arguments are passed on to file2hex.py
    )
  generate_inc_file(${source_file} ${generated_file} ${ARGN})

  # Ensure 'generated_file' is generated before 'target' by creating a
  # dependency between the two targets

  add_dependencies(${target} ${gen_target})
endfunction()

function(generate_inc_file_for_target
    target          # The cmake target that depends on the generated file
    source_file     # The source file to be converted to hex
    generated_file  # The generated file
                    # Any additional arguments are passed on to file2hex.py
    )
  # Ensure 'generated_file' is generated before 'target' by creating a
  # 'custom_target' for it and setting up a dependency between the two
  # targets

  # But first create a unique name for the custom target
  generate_unique_target_name_from_filename(${generated_file} generated_target_name)

  add_custom_target(${generated_target_name} DEPENDS ${generated_file})
  generate_inc_file_for_gen_target(${target} ${source_file} ${generated_file} ${generated_target_name} ${ARGN})
endfunction()

# 1.4. board_*
#
# This section is for extensions which control xos's board runners
# from the build system. The xos build system has targets for
# flashing and debugging supported boards. These are wrappers around a
# "runner" Python subpackage that is part of xos's "west" tool.
#
# This section provides glue between CMake and the Python code that
# manages the runners.

function(_board_check_runner_type type) # private helper
  if (NOT (("${type}" STREQUAL "FLASH") OR ("${type}" STREQUAL "DEBUG")))
    message(FATAL_ERROR "invalid type ${type}; should be FLASH or DEBUG")
  endif()
endfunction()

# This function sets the runner for the board unconditionally.  It's
# meant to be used from application CMakeLists.txt files.
#
# NOTE: Usually board_set_xxx_ifnset() is best in board.cmake files.
#       This lets the user set the runner at cmake time, or in their
#       own application's CMakeLists.txt.
#
# Usage:
#   board_set_runner(FLASH pyocd)
#
# This would set the board's flash runner to "pyocd".
#
# In general, "type" is FLASH or DEBUG, and "runner" is the name of a
# runner.
function(board_set_runner type runner)
  _board_check_runner_type(${type})
  if (DEFINED BOARD_${type}_RUNNER)
    message(STATUS "overriding ${type} runner ${BOARD_${type}_RUNNER}; it's now ${runner}")
  endif()
  set(BOARD_${type}_RUNNER ${runner} PARENT_SCOPE)
endfunction()

# This macro is like board_set_runner(), but will only make a change
# if that runner is currently not set.
#
# See also board_set_flasher_ifnset() and board_set_debugger_ifnset().
macro(board_set_runner_ifnset type runner)
  _board_check_runner_type(${type})
  # This is a macro because set_ifndef() works at parent scope.
  # If this were a function, that would be this function's scope,
  # which wouldn't work.
  set_ifndef(BOARD_${type}_RUNNER ${runner})
endmacro()

# A convenience macro for board_set_runner(FLASH ${runner}).
macro(board_set_flasher runner)
  board_set_runner(FLASH ${runner})
endmacro()

# A convenience macro for board_set_runner(DEBUG ${runner}).
macro(board_set_debugger runner)
  board_set_runner(DEBUG ${runner})
endmacro()

# A convenience macro for board_set_runner_ifnset(FLASH ${runner}).
macro(board_set_flasher_ifnset runner)
  board_set_runner_ifnset(FLASH ${runner})
endmacro()

# A convenience macro for board_set_runner_ifnset(DEBUG ${runner}).
macro(board_set_debugger_ifnset runner)
  board_set_runner_ifnset(DEBUG ${runner})
endmacro()

# This function is intended for board.cmake files and application
# CMakeLists.txt files.
#
# Usage from board.cmake files:
#   board_runner_args(runner "--some-arg=val1" "--another-arg=val2")
#
# The build system will then ensure the command line used to
# create the runner contains:
#   --some-arg=val1 --another-arg=val2
#
# Within application CMakeLists.txt files, ensure that all calls to
# board_runner_args() are part of a macro named app_set_runner_args(),
# like this, which is defined before including the boilerplate file:
#   macro(app_set_runner_args)
#     board_runner_args(runner "--some-app-setting=value")
#   endmacro()
#
# The build system tests for the existence of the macro and will
# invoke it at the appropriate time if it is defined.
#
# Any explicitly provided settings given by this function override
# defaults provided by the build system.
function(board_runner_args runner)
  string(MAKE_C_IDENTIFIER ${runner} runner_id)
  # Note the "_EXPLICIT_" here, and see below.
  set_property(GLOBAL APPEND PROPERTY BOARD_RUNNER_ARGS_EXPLICIT_${runner_id} ${ARGN})
endfunction()

# This function is intended for internal use by
# boards/common/runner.board.cmake files.
#
# Basic usage:
#   board_finalize_runner_args(runner)
#
# This ensures the build system captures all arguments added in any
# board_runner_args() calls, and otherwise finishes registering a
# runner for use.
#
# Extended usage:
#   board_runner_args(runner "--some-arg=default-value")
#
# This provides common or default values for arguments. These are
# placed before board_runner_args() calls, so they generally take
# precedence, except for arguments which can be given multiple times
# (use these with caution).
function(board_finalize_runner_args runner)
  # If the application provided a macro to add additional runner
  # arguments, handle them.
  if(COMMAND app_set_runner_args)
    app_set_runner_args()
  endif()

  # Retrieve the list of explicitly set arguments.
  string(MAKE_C_IDENTIFIER ${runner} runner_id)
  get_property(explicit GLOBAL PROPERTY "BOARD_RUNNER_ARGS_EXPLICIT_${runner_id}")

  # Note no _EXPLICIT_ here. This property contains the final list.
  set_property(GLOBAL APPEND PROPERTY BOARD_RUNNER_ARGS_${runner_id}
    # Default arguments from the common runner file come first.
    ${ARGN}
    # Arguments explicitly given with board_runner_args() come
    # last, so they take precedence.
    ${explicit}
    )

  # Add the finalized runner to the global property list.
  set_property(GLOBAL APPEND PROPERTY xos_RUNNERS ${runner})
endfunction()

# 1.5. Misc.

# xos_check_compiler_flag is a part of xos's toolchain
# infrastructure. It should be used when testing toolchain
# capabilities and it should normally be used in place of the
# functions:
#
# check_compiler_flag
# check_c_compiler_flag
# check_cxx_compiler_flag
#
# See check_compiler_flag() for API documentation as it has the same
# API.
#
# It is implemented as a wrapper on top of check_compiler_flag, which
# again wraps the CMake-builtin's check_c_compiler_flag and
# check_cxx_compiler_flag.
#
# It takes time to check for compatibility of flags against toolchains
# so we cache the capability test results in USER_CACHE_DIR (This
# caching comes in addition to the caching that CMake does in the
# build folder's CMakeCache.txt)
function(xos_check_compiler_flag lang option check)
  # Check if the option is covered by any hardcoded check before doing
  # an automated test.
  xos_check_compiler_flag_hardcoded(${lang} "${option}" check exists)
  if(exists)
    set(check ${check} PARENT_SCOPE)
    return()
  endif()

  # Locate the cache directory
  set_ifndef(
    xos_TOOLCHAIN_CAPABILITY_CACHE_DIR
    ${USER_CACHE_DIR}/ToolchainCapabilityDatabase
    )

  # The toolchain capability database/cache is maintained as a
  # directory of files. The filenames in the directory are keys, and
  # the file contents are the values in this key-value store.

  # We need to create a unique key wrt. testing the toolchain
  # capability. This key must include everything that can affect the
  # toolchain test.
  #
  # Also, to fit the key into a filename we calculate the MD5 sum of
  # the key.

  # The 'cacheformat' must be bumped if a bug in the caching mechanism
  # is detected and all old keys must be invalidated.
  set(cacheformat 3)

  set(key_string "")
  set(key_string "${key_string}${cacheformat}_")
  set(key_string "${key_string}${TOOLCHAIN_SIGNATURE}_")
  set(key_string "${key_string}${lang}_")
  set(key_string "${key_string}${option}_")
  set(key_string "${key_string}${CMAKE_REQUIRED_FLAGS}_")

  string(MD5 key ${key_string})

  # Check the cache
  set(key_path ${xos_TOOLCHAIN_CAPABILITY_CACHE_DIR}/${key})
  if(EXISTS ${key_path})
    file(READ
    ${key_path}   # File to be read
    key_value     # Output variable
    LIMIT 1       # Read at most 1 byte ('0' or '1')
    )

    set(${check} ${key_value} PARENT_SCOPE)
    return()
  endif()

  # Flags that start with -Wno-<warning> can not be tested by
  # check_compiler_flag, they will always pass, but -W<warning> can be
  # tested, so to test -Wno-<warning> flags we test -W<warning>
  # instead.
  if("${option}" MATCHES "-Wno-(.*)")
    set(possibly_translated_option -W${CMAKE_MATCH_1})
  else()
    set(possibly_translated_option ${option})
  endif()

  check_compiler_flag(${lang} "${possibly_translated_option}" inner_check)

  set(${check} ${inner_check} PARENT_SCOPE)

  # Populate the cache
  if(NOT (EXISTS ${key_path}))

    # This is racy. As often with race conditions, this one can easily be
    # made worse and demonstrated with a simple delay:
    #    execute_process(COMMAND "sleep" "5")
    # Delete the cache, add the sleep above and run sanitycheck with a
    # large number of JOBS. Once it's done look at the log.txt file
    # below and you will see that concurrent cmake processes created the
    # same files multiple times.

    # While there are a number of reasons why this race seems both very
    # unlikely and harmless, let's play it safe anyway and write to a
    # private, temporary file first. All modern filesystems seem to
    # support at least one atomic rename API and cmake's file(RENAME
    # ...) officially leverages that.
    string(RANDOM LENGTH 8 tempsuffix)

    file(
      WRITE
      "${key_path}_tmp_${tempsuffix}"
      ${inner_check}
      )
    file(
      RENAME
      "${key_path}_tmp_${tempsuffix}" "${key_path}"
      )

    # Populate a metadata file (only intended for trouble shooting)
    # with information about the hash, the toolchain capability
    # result, and the toolchain test.
    file(
      APPEND
      ${xos_TOOLCHAIN_CAPABILITY_CACHE_DIR}/log.txt
      "${inner_check} ${key} ${key_string}\n"
      )
  endif()
endfunction()

function(xos_check_compiler_flag_hardcoded lang option check exists)
  # Various flags that are not supported for CXX may not be testable
  # because they would produce a warning instead of an error during
  # the test.  Exclude them by toolchain-specific blacklist.
  if((${lang} STREQUAL CXX) AND ("${option}" IN_LIST CXX_EXCLUDED_OPTIONS))
    set(check 0 PARENT_SCOPE)
    set(exists 1 PARENT_SCOPE)
  else()
    # There does not exist a hardcoded check for this option.
    set(exists 0 PARENT_SCOPE)
  endif()
endfunction(xos_check_compiler_flag_hardcoded)

# xos_linker_sources(<location> [SORT_KEY <sort_key>] <files>)
#
# <files> is one or more .ld formatted files whose contents will be
#    copied/included verbatim into the given <location> in the global linker.ld.
#    Preprocessor directives work inside <files>. Relative paths are resolved
#    relative to the calling file, like xos_sources().
# <location> is one of
#    NOINIT       Inside the noinit output section.
#    RWDATA       Inside the data output section.
#    RODATA       Inside the rodata output section.
#    ROM_START    Inside the first output section of the image. This option is
#                 currently only available on ARM Cortex-M, ARM Cortex-R,
#                 x86, ARC, and openisa_rv32m1.
#    RAM_SECTIONS Inside the RAMABLE_REGION GROUP.
#    SECTIONS     Near the end of the file. Don't use this when linking into
#                 RAMABLE_REGION, use RAM_SECTIONS instead.
# <sort_key> is an optional key to sort by inside of each location. The key must
#    be alphanumeric, and the keys are sorted alphabetically. If no key is
#    given, the key 'default' is used. Keys are case-sensitive.
#
# Use NOINIT, RWDATA, and RODATA unless they don't work for your use case.
#
# When placing into NOINIT, RWDATA, RODATA, ROM_START, the contents of the files
# will be placed inside an output section, so assume the section definition is
# already present, e.g.:
#    _mysection_start = .;
#    KEEP(*(.mysection));
#    _mysection_end = .;
#    _mysection_size = ABSOLUTE(_mysection_end - _mysection_start);
#
# When placing into SECTIONS or RAM_SECTIONS, the files must instead define
# their own output sections to achieve the same thing:
#    SECTION_PROLOGUE(.mysection,,)
#    {
#        _mysection_start = .;
#        KEEP(*(.mysection))
#        _mysection_end = .;
#    } GROUP_LINK_IN(ROMABLE_REGION)
#    _mysection_size = _mysection_end - _mysection_start;
#
# Note about the above examples: If the first example was used with RODATA, and
# the second with SECTIONS, the two examples do the same thing from a user
# perspective.
#
# Friendly reminder: Beware of the different ways the location counter ('.')
# behaves inside vs. outside section definitions.
function(xos_linker_sources location)
  # Set up the paths to the destination files. These files are #included inside
  # the global linker.ld.
  set(snippet_base      "${__build_dir}/include/generated")
  set(sections_path     "${snippet_base}/snippets-sections.ld")
  set(ram_sections_path "${snippet_base}/snippets-ram-sections.ld")
  set(rom_start_path    "${snippet_base}/snippets-rom-start.ld")
  set(noinit_path       "${snippet_base}/snippets-noinit.ld")
  set(rwdata_path       "${snippet_base}/snippets-rwdata.ld")
  set(rodata_path       "${snippet_base}/snippets-rodata.ld")

  # Clear destination files if this is the first time the function is called.
  get_property(cleared GLOBAL PROPERTY snippet_files_cleared)
  if (NOT DEFINED cleared)
    file(WRITE ${sections_path} "")
    file(WRITE ${ram_sections_path} "")
    file(WRITE ${rom_start_path} "")
    file(WRITE ${noinit_path} "")
    file(WRITE ${rwdata_path} "")
    file(WRITE ${rodata_path} "")
    set_property(GLOBAL PROPERTY snippet_files_cleared true)
  endif()

  # Choose destination file, based on the <location> argument.
  if ("${location}" STREQUAL "SECTIONS")
    set(snippet_path "${sections_path}")
  elseif("${location}" STREQUAL "RAM_SECTIONS")
    set(snippet_path "${ram_sections_path}")
  elseif("${location}" STREQUAL "ROM_START")
    set(snippet_path "${rom_start_path}")
  elseif("${location}" STREQUAL "NOINIT")
    set(snippet_path "${noinit_path}")
  elseif("${location}" STREQUAL "RWDATA")
    set(snippet_path "${rwdata_path}")
  elseif("${location}" STREQUAL "RODATA")
    set(snippet_path "${rodata_path}")
  else()
    message(fatal_error "Must choose valid location for linker snippet.")
  endif()

  cmake_parse_arguments(L "" "SORT_KEY" "" ${ARGN})
  set(SORT_KEY default)
  if(DEFINED L_SORT_KEY)
    set(SORT_KEY ${L_SORT_KEY})
  endif()

  foreach(file IN ITEMS ${L_UNPARSED_ARGUMENTS})
    # Resolve path.
    if(IS_ABSOLUTE ${file})
      set(path ${file})
    else()
      set(path ${CMAKE_CURRENT_SOURCE_DIR}/${file})
    endif()

    if(IS_DIRECTORY ${path})
      message(FATAL_ERROR "xos_linker_sources() was called on a directory")
    endif()

    # Find the relative path to the linker file from the include folder.
    file(RELATIVE_PATH relpath ${XOS_BASE}/include ${path})

    # Create strings to be written into the file
    set (include_str "/* Sort key: \"${SORT_KEY}\" */#include \"${relpath}\"")

    # Add new line to existing lines, sort them, and write them back.
    file(STRINGS ${snippet_path} lines) # Get current lines (without newlines).
    list(APPEND lines ${include_str})
    list(SORT lines)
    string(REPLACE ";" "\n;" lines "${lines}") # Add newline to each line.
    file(WRITE ${snippet_path} ${lines} "\n")
  endforeach()
endfunction(xos_linker_sources)


# Helper function for CONFIG_CODE_DATA_RELOCATION
# Call this function with 2 arguments file and then memory location
function(xos_code_relocate file location)
  set_property(TARGET code_data_relocation_target
    APPEND PROPERTY COMPILE_DEFINITIONS
    "${location}:${CMAKE_CURRENT_SOURCE_DIR}/${file}")
endfunction()

# Usage:
#   check_dtc_flag("-Wtest" DTC_WARN_TEST)
#
# Writes 1 to the output variable 'ok' if
# the flag is supported, otherwise writes 0.
#
# using
function(check_dtc_flag flag ok)
  execute_process(
    COMMAND
    ${DTC} ${flag} -v
    ERROR_QUIET
    OUTPUT_QUIET
    RESULT_VARIABLE dtc_check_ret
  )
  if (dtc_check_ret EQUAL 0)
    set(${ok} 1 PARENT_SCOPE)
  else()
    set(${ok} 0 PARENT_SCOPE)
  endif()
endfunction()

########################################################
# 2. Kconfig-aware extensions
########################################################
#
# Kconfig is a configuration language developed for the Linux
# kernel. The below functions integrate CMake with Kconfig.
#
# 2.1 *_if_kconfig
#
# Functions for conditionally including directories and source files
# that have matching KConfig values.
#
# xos_library_sources_if_kconfig(fft.c)
# is the same as
# xos_library_sources_ifdef(CONFIG_FFT fft.c)
#
# add_subdirectory_if_kconfig(serial)
# is the same as
# add_subdirectory_ifdef(CONFIG_SERIAL serial)
function(add_subdirectory_if_kconfig dir)
  string(TOUPPER config_${dir} UPPER_CASE_CONFIG)
  add_subdirectory_ifdef(${UPPER_CASE_CONFIG} ${dir})
endfunction()

function(target_sources_if_kconfig target scope item)
  get_filename_component(item_basename ${item} NAME_WE)
  string(TOUPPER CONFIG_${item_basename} UPPER_CASE_CONFIG)
  target_sources_ifdef(${UPPER_CASE_CONFIG} ${target} ${scope} ${item})
endfunction()

function(xos_library_sources_if_kconfig item)
  get_filename_component(item_basename ${item} NAME_WE)
  string(TOUPPER CONFIG_${item_basename} UPPER_CASE_CONFIG)
  xos_library_sources_ifdef(${UPPER_CASE_CONFIG} ${item})
endfunction()

function(xos_sources_if_kconfig item)
  get_filename_component(item_basename ${item} NAME_WE)
  string(TOUPPER CONFIG_${item_basename} UPPER_CASE_CONFIG)
  xos_sources_ifdef(${UPPER_CASE_CONFIG} ${item})
endfunction()

# 2.2 Misc
#
# import_kconfig(<prefix> <kconfig_fragment> [<keys>])
#
# Parse a KConfig fragment (typically with extension .config) and
# introduce all the symbols that are prefixed with 'prefix' into the
# CMake namespace. List all created variable names in the 'keys'
# output variable if present.
function(import_kconfig prefix kconfig_fragment)
  # Parse the lines prefixed with 'prefix' in ${kconfig_fragment}
  file(
    STRINGS
    ${kconfig_fragment}
    DOT_CONFIG_LIST
    REGEX "^${prefix}"
    ENCODING "UTF-8"
  )

  foreach (CONFIG ${DOT_CONFIG_LIST})
    # CONFIG could look like: CONFIG_NET_BUF=y

    # Match the first part, the variable name
    string(REGEX MATCH "[^=]+" CONF_VARIABLE_NAME ${CONFIG})

    # Match the second part, variable value
    string(REGEX MATCH "=(.+$)" CONF_VARIABLE_VALUE ${CONFIG})
    # The variable name match we just did included the '=' symbol. To just get the
    # part on the RHS we use match group 1
    set(CONF_VARIABLE_VALUE ${CMAKE_MATCH_1})

    if("${CONF_VARIABLE_VALUE}" MATCHES "^\"(.*)\"$") # Is surrounded by quotes
      set(CONF_VARIABLE_VALUE ${CMAKE_MATCH_1})
    endif()

    set("${CONF_VARIABLE_NAME}" "${CONF_VARIABLE_VALUE}" PARENT_SCOPE)
    list(APPEND keys "${CONF_VARIABLE_NAME}")
  endforeach()

  foreach(outvar ${ARGN})
    set(${outvar} "${keys}" PARENT_SCOPE)
  endforeach()
endfunction()

########################################################
# 3. CMake-generic extensions
########################################################
#
# These functions extend the CMake API in a way that is not particular
# to xos. Primarily they work around limitations in the CMake
# language to allow cleaner build scripts.

# 3.1. *_ifdef
#
# Functions for conditionally executing CMake functions with oneliners
# e.g.
#
# if(CONFIG_FFT)
#   xos_library_source(
#     fft_32.c
#     fft_utils.c
#     )
# endif()
#
# Becomes
#
# xos_source_ifdef(
#   CONFIG_FFT
#   fft_32.c
#   fft_utils.c
#   )
#
# More Generally
# "<function-name>_ifdef(CONDITION args)"
# Becomes
# """
# if(CONDITION)
#   <function-name>(args)
# endif()
# """
#
# ifdef functions are added on an as-need basis. See
# https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html for
# a list of available functions.
function(add_subdirectory_ifdef feature_toggle dir)
  if(${${feature_toggle}})
    add_subdirectory(${dir})
  endif()
endfunction()

# 自定义的 add_subdirectory_ifexists function
function(add_subdirectory_ifexists dir)
    if (EXISTS "${dir}")
	message(STATUS "add_subdirectory_ifexists dir: ${dir}")
        add_subdirectory(${dir})
    endif()
endfunction()

function(add_subdirectory_ifdef_and_exists feature_toggle dir)
    if(${${feature_toggle}})
    if (EXISTS "${dir}")
	message(STATUS "add_subdirectory_ifdef_and_exists dir: ${dir}")
        add_subdirectory(${dir})
    endif()
    endif()
endfunction()

function(target_sources_ifdef feature_toggle target scope item)
  if(${${feature_toggle}})
    target_sources(${target} ${scope} ${item} ${ARGN})
  endif()
endfunction()

function(target_compile_definitions_ifdef feature_toggle target scope item)
  if(${${feature_toggle}})
    target_compile_definitions(${target} ${scope} ${item} ${ARGN})
  endif()
endfunction()

function(target_include_directories_ifdef feature_toggle target scope item)
  if(${${feature_toggle}})
    target_include_directories(${target} ${scope} ${item} ${ARGN})
  endif()
endfunction()

function(target_link_libraries_ifdef feature_toggle target item)
  if(${${feature_toggle}})
    target_link_libraries(${target} ${item} ${ARGN})
  endif()
endfunction()

function(add_compile_option_ifdef feature_toggle option)
  if(${${feature_toggle}})
    add_compile_options(${option})
  endif()
endfunction()

function(target_compile_option_ifdef feature_toggle target scope option)
  if(${feature_toggle})
    target_compile_options(${target} ${scope} ${option})
  endif()
endfunction()

function(target_cc_option_ifdef feature_toggle target scope option)
  if(${feature_toggle})
    target_cc_option(${target} ${scope} ${option})
  endif()
endfunction()

function(xos_library_sources_ifdef feature_toggle source)
  if(${${feature_toggle}})
    xos_library_sources(${source} ${ARGN})
  endif()
endfunction()

function(xos_library_sources_ifndef feature_toggle source)
  if(NOT ${feature_toggle})
    xos_library_sources(${source} ${ARGN})
  endif()
endfunction()

function(xos_sources_ifdef feature_toggle)
  if(${${feature_toggle}})
    xos_sources(${ARGN})
  endif()
endfunction()

function(xos_sources_ifndef feature_toggle)
   if(NOT ${feature_toggle})
    xos_sources(${ARGN})
  endif()
endfunction()

function(xos_cc_option_ifdef feature_toggle)
  if(${${feature_toggle}})
    xos_cc_option(${ARGN})
  endif()
endfunction()

function(xos_ld_option_ifdef feature_toggle)
  if(${${feature_toggle}})
    xos_ld_options(${ARGN})
  endif()
endfunction()

function(xos_link_libraries_ifdef feature_toggle)
  if(${${feature_toggle}})
    xos_link_libraries(${ARGN})
  endif()
endfunction()

function(xos_compile_options_ifdef feature_toggle)
  if(${${feature_toggle}})
    xos_compile_options(${ARGN})
  endif()
endfunction()

function(xos_compile_definitions_ifdef feature_toggle)
  if(${${feature_toggle}})
    xos_compile_definitions(${ARGN})
  endif()
endfunction()

function(xos_include_directories_ifdef feature_toggle)
  if(${${feature_toggle}})
    xos_include_directories(${ARGN})
  endif()
endfunction()

function(xos_library_compile_definitions_ifdef feature_toggle item)
  if(${${feature_toggle}})
    xos_library_compile_definitions(${item} ${ARGN})
  endif()
endfunction()

function(xos_library_compile_options_ifdef feature_toggle item)
  if(${${feature_toggle}})
    xos_library_compile_options(${item} ${ARGN})
  endif()
endfunction()

function(xos_link_interface_ifdef feature_toggle interface)
  if(${${feature_toggle}})
    target_link_libraries(${interface} INTERFACE ${XOS_CURRENT_INTERFACE})
  endif()
endfunction()

function(xos_library_link_libraries_ifdef feature_toggle item)
  if(${${feature_toggle}})
     xos_library_link_libraries(${item})
  endif()
endfunction()

function(xos_linker_sources_ifdef feature_toggle)
  if(${${feature_toggle}})
    xos_linker_sources(${ARGN})
  endif()
endfunction()

macro(list_append_ifdef feature_toggle list)
  if(${${feature_toggle}})
    list(APPEND ${list} ${ARGN})
  endif()
endmacro()

macro(list_append_ifexist list)
  # 判断传入的 list 是否有效
  if(NOT DEFINED ${list})
    message(FATAL_ERROR "The list parameter is not defined.")
  endif()

  # 判断目录是否存在，如果存在则添加到 list
  foreach(dir IN LISTS ARGN)
    if(EXISTS ${dir} AND IS_DIRECTORY ${dir})
      list(APPEND ${list} ${dir})
    endif()
  endforeach()
endmacro()

macro(list_append_ifnot_exist list dir1 dir2)
  # 如果 dir1 不存在，则将 dir2 添加到 list 中
  if(NOT EXISTS ${dir1})
    list(APPEND ${list} ${dir2})
  else()
    list(APPEND ${list} ${dir1})
  endif()
endmacro()


# 3.2. *_ifndef
# See 3.1 *_ifdef
function(set_ifndef variable value)
  if(NOT ${variable})
    set(${variable} ${value} ${ARGN} PARENT_SCOPE)
  endif()
endfunction()

function(target_cc_option_ifndef feature_toggle target scope option)
  if(NOT ${feature_toggle})
    target_cc_option(${target} ${scope} ${option})
  endif()
endfunction()

function(xos_cc_option_ifndef feature_toggle)
  if(NOT ${feature_toggle})
    xos_cc_option(${ARGN})
  endif()
endfunction()

function(xos_compile_options_ifndef feature_toggle)
  if(NOT ${feature_toggle})
    xos_compile_options(${ARGN})
  endif()
endfunction()

# 3.3. *_option Compiler-compatibility checks
#
# Utility functions for silently omitting compiler flags when the
# compiler lacks support. *_cc_option was ported from KBuild, see
# cc-option in
# https://www.kernel.org/doc/Documentation/kbuild/makefiles.txt

# Writes 1 to the output variable 'ok' for the language 'lang' if
# the flag is supported, otherwise writes 0.
#
# lang must be C or CXX
#
# TODO: Support ASM
#
# Usage:
#
# check_compiler_flag(C "-Wall" my_check)
# print(my_check) # my_check is now 1
function(check_compiler_flag lang option ok)
  if(NOT DEFINED CMAKE_REQUIRED_QUIET)
    set(CMAKE_REQUIRED_QUIET 1)
  endif()

  string(MAKE_C_IDENTIFIER
    check${option}_${lang}_${CMAKE_REQUIRED_FLAGS}
    ${ok}
    )

  if(${lang} STREQUAL C)
    check_c_compiler_flag("${option}" ${${ok}})
  else()
    check_cxx_compiler_flag("${option}" ${${ok}})
  endif()

  if(${${${ok}}})
    set(ret 1)
  else()
    set(ret 0)
  endif()

  set(${ok} ${ret} PARENT_SCOPE)
endfunction()

function(target_cc_option target scope option)
  target_cc_option_fallback(${target} ${scope} ${option} "")
endfunction()

# Support an optional second option for when the first option is not
# supported.
function(target_cc_option_fallback target scope option1 option2)
  if(CONFIG_CPLUSPLUS)
    foreach(lang C CXX)
      # For now, we assume that all flags that apply to C/CXX also
      # apply to ASM.
      xos_check_compiler_flag(${lang} ${option1} check)
      if(${check})
        target_compile_options(${target} ${scope}
          $<$<COMPILE_LANGUAGE:${lang}>:${option1}>
          $<$<COMPILE_LANGUAGE:ASM>:${option1}>
          )
      elseif(option2)
        target_compile_options(${target} ${scope}
          $<$<COMPILE_LANGUAGE:${lang}>:${option2}>
          $<$<COMPILE_LANGUAGE:ASM>:${option2}>
          )
      endif()
    endforeach()
  else()
    xos_check_compiler_flag(C ${option1} check)
    if(${check})
      target_compile_options(${target} ${scope} ${option1})
    elseif(option2)
      target_compile_options(${target} ${scope} ${option2})
    endif()
  endif()
endfunction()

function(target_ld_options target scope)
  foreach(option ${ARGN})
    string(MAKE_C_IDENTIFIER check${option} check)

    set(SAVED_CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS})
    set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} ${option}")
    xos_check_compiler_flag(C "" ${check})
    set(CMAKE_REQUIRED_FLAGS ${SAVED_CMAKE_REQUIRED_FLAGS})

    target_link_libraries_ifdef(${check} ${target} ${scope} ${option})
  endforeach()
endfunction()

# 3.3.1 Toolchain integration
#
# 'toolchain_parse_make_rule' is a function that parses the output of
# 'gcc -M'.
#
# The argument 'input_file' is in input parameter with the path to the
# file with the dependency information.
#
# The argument 'include_files' is an output parameter with the result
# of parsing the include files.
function(toolchain_parse_make_rule input_file include_files)
  file(READ ${input_file} input)

  # The file is formatted like this:
  # empty_file.o: misc/empty_file.c \
  # nrf52840_pca10056/nrf52840_pca10056.dts \
  # nrf52840_qiaa.dtsi

  # Get rid of the backslashes
  string(REPLACE "\\" ";" input_as_list ${input})

  # Pop the first line and treat it specially
  list(GET input_as_list 0 first_input_line)
  string(FIND ${first_input_line} ": " index)
  math(EXPR j "${index} + 2")
  string(SUBSTRING ${first_input_line} ${j} -1 first_include_file)
  list(REMOVE_AT input_as_list 0)

  list(APPEND result ${first_include_file})

  # Add the other lines
  list(APPEND result ${input_as_list})

  # Strip away the newlines and whitespaces
  list(TRANSFORM result STRIP)

  set(${include_files} ${result} PARENT_SCOPE)
endfunction()

# 3.4. Debugging CMake

# Usage:
#   print(BOARD)
#
# will print: "BOARD: nrf52_pca10040"
function(print arg)
  message(STATUS "${arg}: ${${arg}}")
endfunction()

# Usage:
#   assert(xos_TOOLCHAIN_VARIANT "xos_TOOLCHAIN_VARIANT not set.")
#
# will cause a FATAL_ERROR and print an error message if the first
# expression is false
macro(assert test comment)
  if(NOT ${test})
    message(FATAL_ERROR "Assertion failed: ${comment}")
  endif()
endmacro()

# Usage:
#   assert_not(OBSOLETE_VAR "OBSOLETE_VAR has been removed; use NEW_VAR instead")
#
# will cause a FATAL_ERROR and print an error message if the first
# expression is true
macro(assert_not test comment)
  if(${test})
    message(FATAL_ERROR "Assertion failed: ${comment}")
  endif()
endmacro()

# Usage:
#   assert_exists(CMAKE_READELF)
#
# will cause a FATAL_ERROR if there is no file or directory behind the
# variable
macro(assert_exists var)
  if(NOT EXISTS ${${var}})
    message(FATAL_ERROR "No such file or directory: ${var}: '${${var}}'")
  endif()
endmacro()

function(print_usage)
  message("see usage:")
  string(REPLACE ";" " " BOARD_ROOT_SPACE_SEPARATED "${BOARD_ROOT}")
  string(REPLACE ";" " " SHIELD_LIST_SPACE_SEPARATED "${SHIELD_LIST}")
  execute_process(
    COMMAND
    ${CMAKE_COMMAND}
    -DBOARD_ROOT_SPACE_SEPARATED=${BOARD_ROOT_SPACE_SEPARATED}
    -DSHIELD_LIST_SPACE_SEPARATED=${SHIELD_LIST_SPACE_SEPARATED}
    -P ${XOS_BASE}/cmake/usage/usage.cmake
    )
endfunction()

# 3.5. File system management
function(check_if_directory_is_writeable dir ok)
  execute_process(
    COMMAND
    ${PYTHON_EXECUTABLE}
    ${XOS_BASE}/scripts/dir_is_writeable.py
    ${dir}
    RESULT_VARIABLE ret
    )

  if("${ret}" STREQUAL "0")
    # The directory is write-able
    set(${ok} 1 PARENT_SCOPE)
  else()
    set(${ok} 0 PARENT_SCOPE)
  endif()
endfunction()

function(find_appropriate_cache_directory dir)
  set(env_suffix_LOCALAPPDATA   .cache)

  if(CMAKE_HOST_APPLE)
    # On macOS, ~/Library/Caches is the preferred cache directory.
    set(env_suffix_HOME Library/Caches)
  else()
    set(env_suffix_HOME .cache)
  endif()

  # Determine which env vars should be checked
  if(CMAKE_HOST_APPLE)
    set(dirs HOME)
  elseif(CMAKE_HOST_WIN32)
    set(dirs LOCALAPPDATA)
  else()
    # Assume Linux when we did not detect 'mac' or 'win'
    #
    # On Linux, freedesktop.org recommends using $XDG_CACHE_HOME if
    # that is defined and defaulting to $HOME/.cache otherwise.
    set(dirs
      XDG_CACHE_HOME
      HOME
      )
  endif()

  foreach(env_var ${dirs})
    if(DEFINED ENV{${env_var}})
      set(env_dir $ENV{${env_var}})

      set(test_user_dir ${env_dir}/${env_suffix_${env_var}})

      check_if_directory_is_writeable(${test_user_dir} ok)
      if(${ok})
        # The directory is write-able
        set(user_dir ${test_user_dir})
        break()
      else()
        # The directory was not writeable, keep looking for a suitable
        # directory
      endif()
    endif()
  endforeach()

  # Populate local_dir with a suitable directory for caching
  # files. Prefer a directory outside of the git repository because it
  # is good practice to have clean git repositories.
  if(DEFINED user_dir)
    # xos's cache files go in the "xos" subdirectory of the
    # user's cache directory.
    set(local_dir ${user_dir}/xos)
  else()
    set(local_dir ${XOS_BASE}/.cache)
  endif()

  set(${dir} ${local_dir} PARENT_SCOPE)
endfunction()

function(generate_unique_target_name_from_filename filename target_name)
  get_filename_component(basename ${filename} NAME)
  string(REPLACE "." "_" x ${basename})
  string(REPLACE "@" "_" x ${x})

  string(MD5 unique_chars ${filename})

  set(${target_name} gen_${x}_${unique_chars} PARENT_SCOPE)
endfunction()

function(add_subdir_includes input_includes all_inc_var)
    if(DEFINED ${input_includes})
        list(APPEND ${all_inc_var} ${${input_includes}})
    endif()
endfunction()


function(target_sources_if_kconfig target scope item)
  get_filename_component(item_basename ${item} NAME_WE)
  string(TOUPPER CONFIG_${item_basename} UPPER_CASE_CONFIG)
  target_sources_ifdef(${UPPER_CASE_CONFIG} ${target} ${scope} ${item})
endfunction()

function(library_sources_if_kconfig item)
  get_filename_component(item_basename ${item} NAME_WE)
  string(TOUPPER CONFIG_${item_basename} UPPER_CASE_CONFIG)
  xos_library_sources_ifdef(${UPPER_CASE_CONFIG} ${item})
endfunction()

function(sources_if_kconfig item)
  get_filename_component(item_basename ${item} NAME_WE)
  string(TOUPPER CONFIG_${item_basename} UPPER_CASE_CONFIG)
  xos_sources_ifdef(${UPPER_CASE_CONFIG} ${item})
endfunction()
