file(GLOB LIB_SOURCES "*.cc")

# --- ADDED: Link the pre-compiled solution object ---
set(NET_IFACE_OBJ "network_interface.cc.o")

# 1. Safety Check: Stop build if the student deleted the .o file
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${NET_IFACE_OBJ}")
    message(FATAL_ERROR "Missing required file: src/${NET_IFACE_OBJ}\n"
            "This file contains the solution for PA1 required for PA2.\n"
            "Please restore it from the starter code.")
endif()

# 2. Tell CMake this is an existing object file (not C++ source code)
set_source_files_properties(${NET_IFACE_OBJ} PROPERTIES 
    EXTERNAL_OBJECT true 
    GENERATED true 
    LANGUAGE CXX
)

# 3. Add the object file to the list of sources for all libraries
list(APPEND LIB_SOURCES ${NET_IFACE_OBJ})
# ----------------------------------------------------

add_library(comp_net_debug STATIC ${LIB_SOURCES})

add_library(comp_net_sanitized EXCLUDE_FROM_ALL STATIC ${LIB_SOURCES})
target_compile_options(comp_net_sanitized PUBLIC ${SANITIZING_FLAGS})

add_library(comp_net_optimized EXCLUDE_FROM_ALL STATIC ${LIB_SOURCES})
target_compile_options(comp_net_optimized PUBLIC "-O2")

macro(add_app exec_name)
  add_executable("${exec_name}" "${exec_name}.cc")
  target_link_libraries("${exec_name}" comp_net_debug)
  target_link_libraries("${exec_name}" util_debug)
endmacro(add_app)

add_app(webget)