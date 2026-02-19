include(CMakeFindDependencyMacro)
find_dependency(pugixml CONFIG)
if(ON)
    find_dependency(nowide CONFIG)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/OpenXLSXTargets.cmake")
