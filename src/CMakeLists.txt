include(FindPkgConfig)

# add /usr/loca/lib/pkgconfig (necessary on macOS)
set( ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/local/lib/pkgconfig" )

pkg_check_modules (LATERO_GRAPHICS latero-graphics)
link_directories(${LATERO_GRAPHICS_LIBRARY_DIRS})
include_directories(${LATERO_GRAPHICS_INCLUDE_DIRS})

set(demo_SHARED_SRCS
	generatorhandle.h generatorhandle.cpp
	demo.h config.h mainwindow.h mainwindow.cpp main.cpp
)
SOURCE_GROUP(shared FILES ${demo_SHARED_SRCS})

set(demo_VECTOR_SRCS
	vector/demo.cpp vector/demo.h
)
SOURCE_GROUP(vector FILES ${demo_VECTOR_SRCS})


set(demo_TEXTURE_SRCS
	texture/demo.cpp texture/demo.h
)
SOURCE_GROUP(texture FILES ${demo_TEXTURE_SRCS})


set(demo_SCHOOLBOOK_SRCS
	schoolbook/demo.cpp schoolbook/demo.h
)
SOURCE_GROUP(schoolbook FILES ${demo_SCHOOLBOOK_SRCS})

set(demo_IMG_SRCS
	img/settings.h img/demo.h img/cardtable.h img/cardset.h img/card.h img/cardcollection.h
	img/cardcollection.cpp img/card.cpp img/cardset.cpp img/cardtable.cpp img/demo.cpp
)
SOURCE_GROUP(img FILES ${demo_IMG_SRCS})

set(demo_ICON_SRCS
	icon/demo.h icon/demo.cpp icon/generator.h icon/generator.cpp icon/toolbar.h icon/toolbar.cpp
)
SOURCE_GROUP(icon FILES ${demo_ICON_SRCS})

set(demo_BRAILLE_SRCS
	braille/braillecell.cpp braille/braillegen.cpp braille/braillegenwidget.h braille/braillestringwidget.h
	braille/dotgraph.cpp braille/dotwidget.h braille/modulatorwidget.h braille/virtualsurfacewidget.h
	braille/braillecell.h braille/braillegenfwd.h braille/braillestring.cpp braille/demo.cpp
	braille/dotgraph.h braille/modulator.cpp braille/positiongraph.cpp
	braille/braillecellwidget.cpp braille/braillegen.h braille/braillestring.h
	braille/demo.h braille/dot.h braille/modulator.h braille/positiongraph.h
	braille/braillecellwidget.h braille/braillegenwidget.cpp braille/braillestringwidget.cpp braille/dot.cpp
	braille/dotwidget.cpp braille/modulatorwidget.cpp braille/virtualsurfacewidget.cpp
)
SOURCE_GROUP(braille FILES ${demo_BRAILLE_SRCS})

set(demo_SRCS
	${demo_SHARED_SRCS}
	${demo_VECTOR_SRCS}
	${demo_TEXTURE_SRCS}
	${demo_SCHOOLBOOK_SRCS}
	${demo_IMG_SRCS}
	${demo_ICON_SRCS}
	${demo_BRAILLE_SRCS})

add_executable(latero-demo ${demo_SRCS})
target_link_libraries(latero-demo ${LATERO_GRAPHICS_LIBRARIES} boost_program_options-mt)
install_programs(/bin latero-demo)

# generate config.h
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)
