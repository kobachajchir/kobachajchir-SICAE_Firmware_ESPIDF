# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

# Define the source files for the project
# Define the source files for the project
file(GLOB_RECURSE app_sources "${CMAKE_SOURCE_DIR}/src/*.*")

# Register the component with IDF
idf_component_register(SRCS ${app_sources})
