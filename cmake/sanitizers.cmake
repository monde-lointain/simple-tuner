#########################
# Code Sanitizer Module #
#########################
# This module provides a USE_SANITIZER option which can enable code sanitizers with GCC and Clang.
# Include this module before you define your targets.

set(USE_SANITIZER
    "" CACHE STRING
    "Compile with a sanitizer. Options are: Address, Memory, Leak, Undefined, Thread, or combinations like Address,Undefined"
)

# Convert input to lowercase for simpler matching
string(TOLOWER "${USE_SANITIZER}" SANITIZER_LOWER)

# Split multiple sanitizers separated by commas
string(REPLACE "," ";" SANITIZER_LIST "${SANITIZER_LOWER}")

# List of valid sanitizer names
set(VALID_SANITIZERS address thread undefined leak memory)

# Track if we have applied at least one sanitizer
set(SANITIZER_APPLIED FALSE)

foreach(s IN LISTS SANITIZER_LIST)
    string(STRIP "${s}" s)  # remove whitespace
    if(NOT s IN_LIST VALID_SANITIZERS)
        message(FATAL_ERROR "Unsupported value of USE_SANITIZER: '${s}'")
    endif()

    # Apply compiler and linker options
    add_compile_options(-fsanitize=${s})
    add_link_options(-fsanitize=${s})
    set(SANITIZER_APPLIED TRUE)
endforeach()

# If nothing applied, assume no sanitizer
if(NOT SANITIZER_APPLIED)
    message(STATUS "No sanitizer applied (USE_SANITIZER is empty or None)")
endif()

# Update possible values for cmake-gui/ccmake
set_property(CACHE USE_SANITIZER
    PROPERTY STRINGS
        "None" "Address" "Thread" "Undefined" "Leak" "Memory"
        "Address,Thread" "Address,Undefined" "Address,Memory" "Address,Leak"
        "Thread,Undefined" "Thread,Memory" "Thread,Leak"
        "Undefined,Memory" "Undefined,Leak"
        "Memory,Leak"
        "Address,Thread,Undefined" "Address,Thread,Memory" "Address,Thread,Leak"
        "Address,Undefined,Memory" "Address,Undefined,Leak"
        "Address,Memory,Leak"
        "Thread,Undefined,Memory" "Thread,Undefined,Leak" "Thread,Memory,Leak"
        "Undefined,Memory,Leak"
        "Address,Thread,Undefined,Memory" "Address,Thread,Undefined,Leak"
        "Address,Thread,Memory,Leak" "Address,Undefined,Memory,Leak"
        "Thread,Undefined,Memory,Leak"
        "Address,Thread,Undefined,Memory,Leak"
)

