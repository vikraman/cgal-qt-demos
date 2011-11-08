
project (cgal-qt-demos)

cmake_minimum_required(VERSION 2.6.2)
if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" VERSION_GREATER 2.6)
  if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER 2.8.3)
    cmake_policy(VERSION 2.8.4)
  else()
    cmake_policy(VERSION 2.6)
  endif()
endif()

find_package(CGAL COMPONENTS Qt4)
include(${CGAL_USE_FILE})

find_package(Qt4)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set( QT_USE_QTXML TRUE )
set( QT_USE_QTMAIN TRUE )
set( QT_USE_QTSCRIPT TRUE )
set( QT_USE_QTOPENGL TRUE )

if ( CGAL_FOUND AND CGAL_Qt4_FOUND AND QT4_FOUND )

    set( main_SOURCES main.cpp mainwindow.cpp )
    set( main_HEADERS mainwindow.h )
    set( main_FORMS mainwindow.ui )
    set( main_RESOURCES mainwindow.qrc icons/View.qrc icons/File.qrc
        icons/Input.qrc mainwindow.qrc resources/CGAL.qrc)

    qt4_wrap_cpp(main_HEADERS_MOC ${main_HEADERS})
    qt4_wrap_ui(main_FORMS_HEADERS ${main_FORMS})
    qt4_add_resources(main_RESOURCES_RCC ${main_RESOURCES})

    add_executable(main ${main_SOURCES} ${main_HEADERS_MOC}
        ${main_FORMS_HEADERS} ${main_RESOURCES_RCC})

    target_link_libraries( main ${QT_LIBRARIES} )
    target_link_libraries( main ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})

else()

    message(STATUS "This app requires CGAL and Qt4, and will not be compiled.")

endif()
