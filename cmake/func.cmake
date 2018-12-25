
FUNCTION(GenOrganizedTree VSPREFORDER SOURCEDIR)
    SET(FTYPES "")
    FOREACH(FTYPE IN ITEMS ${ARGN})
        LIST(APPEND FTYPES ${FTYPE})
    ENDFOREACH()
    FILE(GLOB_RECURSE source_list RELATIVE
        "${SOURCEDIR}" ${FTYPES}
        )
    foreach(source IN LISTS source_list)
        get_filename_component(source_path "${source}" PATH)
        string(REPLACE "/" "\\" vs_source_path "${VSPREFORDER}/${source_path}")
        source_group("${vs_source_path}" FILES "${source}")
    endforeach()
ENDFUNCTION()

# absolute path version
FUNCTION(GenOrganizedTree2 VSPREFORDER SOURCEDIR)
    SET(source_lists "")
    FOREACH(FTYPE IN ITEMS ${ARGN})
        FILE(GLOB_RECURSE source_list ${SOURCEDIR}/${FTYPE})
        LIST(APPEND source_lists ${source_list})
    ENDFOREACH()
    foreach(source IN LISTS source_lists)
        get_filename_component(source_path "${source}" PATH)
        string(REPLACE ${SOURCEDIR} ${VSPREFORDER} relate_path ${source_path})
        string(REPLACE "/" "\\" vs_source_path "${relate_path}")
        source_group("${vs_source_path}" FILES "${source}")
    endforeach()
ENDFUNCTION()

FUNCTION(LinkIce projname)
    IF(WIN32)
        target_link_libraries(${projname} debug Iced)
        target_link_libraries(${projname} debug Glacier2d)
        target_link_libraries(${projname} debug IceUtild)
        target_link_libraries(${projname} optimized Ice)
        target_link_libraries(${projname} optimized Glacier2)
        target_link_libraries(${projname} optimized IceUtil)
    ELSE()
        target_link_libraries(${projname} Ice Glacier2 IceUtil)
    ENDIF()
ENDFUNCTION()

# install libraries
FUNCTION(install_libraries) # install_libraries lib1 lib2 lib3 ...
	INSTALL(TARGETS ${ARGN}
		RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin
		LIBRARY DESTINATION ${CMAKE_PREFIX_PATH}/lib
		ARCHIVE DESTINATION ${CMAKE_PREFIX_PATH}/lib
		)
ENDFUNCTION()


# fetch sub module
function(FetchSubModule git_path)
	message(STATUS "git_path: ${git_path}")
    if (EXISTS ${git_path}/.git)
        message(STATUS "Submodule update")

        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY "${git_path}"
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)

        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}£¬ for pb repository, please checkout submodules")
        else()
            message(STATUS "git submodule update --init success, with ${GIT_SUBMOD_RESULT} submodules")
            #add the protobuf as sub-projuect
            add_subdirectory(${git_path}/cmake)
        endif()
    else()
        message(FATAL_ERROR "${CMAKE_BUILD_PATH} cannot find the ${git_path}/.git !")
    endif()
endfunction()

