cmake_minimum_required (VERSION 2.6)

project(Dream-Gobang)

set(EGE_LIB_SOURCE_FILE
	ege_lib_src/color.cpp
	ege_lib_src/egecontrolbase.cpp
	ege_lib_src/egegapi.cpp
	ege_lib_src/image.cpp
	ege_lib_src/lpng/adler32.c
	ege_lib_src/lpng/compress.c
    ege_lib_src/lpng/crc32.c
	ege_lib_src/lpng/deflate.c
    ege_lib_src/graphics.cpp
	ege_lib_src/lpng/gzclose.c
	ege_lib_src/lpng/gzlib.c
    ege_lib_src/lpng/gzread.c
	ege_lib_src/lpng/gzwrite.c
	ege_lib_src/lpng/infback.c
    ege_lib_src/lpng/inffast.c
	ege_lib_src/lpng/inflate.c
    ege_lib_src/lpng/inftrees.c
    ege_lib_src/logo.cpp
	ege_lib_src/lpng/png.c
	ege_lib_src/lpng/pngerror.c
    ege_lib_src/lpng/pngget.c
	ege_lib_src/lpng/pngmem.c
    ege_lib_src/lpng/pngpread.c
	ege_lib_src/lpng/pngread.c
    ege_lib_src/lpng/pngrio.c
	ege_lib_src/lpng/pngrtran.c
    ege_lib_src/lpng/pngrutil.c
	ege_lib_src/lpng/pngset.c
    ege_lib_src/lpng/pngtrans.c
	ege_lib_src/lpng/pngwio.c
	ege_lib_src/lpng/pngwrite.c
    ege_lib_src/lpng/pngwtran.c
    ege_lib_src/lpng/pngwutil.c
	ege_lib_src/lpng/trees.c
    ege_lib_src/lpng/uncompr.c
	ege_lib_src/lpng/zutil.c
    ege_lib_src/math.cpp
    ege_lib_src/music.cpp
    ege_lib_src/random.cpp
    )

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
if(MINGW)
	link_libraries(gdiplus)
endif()

add_library(ege STATIC ${EGE_LIB_SOURCE_FILE})
if(MSVC)
	set_target_properties(ege PROPERTIES OUTPUT_NAME_DEBUG ege-debug)
    set_target_properties(ege PROPERTIES OUTPUT_NAME_RELEASE ege)
endif()
	
set(PROJECT_SOURCE_FILE
	Gobang.h
	Gobang.cpp
    Chess.h
    Chess.cpp
	ChessBoardView.h
	ChessBoardView.cpp
	ComputerPlayer.h
	ComputerPlayer.cpp
	WelcomeInterface.h
	WelcomeInterface.cpp
	SettingsInterface.h
	SettingsInterface.cpp
	PlayChess.h
	PlayChess.cpp
	NetSelectionInterface.h
	NetSelectionInterface.cpp
	NetSettingsInterface.h
	NetSettingsInterface.cpp
	NetPlayerInfoView.h
	NetPlayerInfoView.cpp
	NetWaitingInterface.h
	NetWaitingInterface.cpp
	NetPlayingInterface.h
	NetPlayingInterface.cpp
	ChessSaver.h
	ChessSaver.cpp
	Settings.h
	Settings.cpp
	GameMain.cpp
	tools/Image.h
	tools/Image.cpp
	tools/Music.h
	tools/Music.cpp
	tools/IniSimpleParser.h
	tools/IniSimpleParser.cpp
	tools/GradientAnimation.h
	tools/GradientAnimation.cpp
	tools/ThreadQueue.h
	widgets/BaseButton.h
	widgets/Button.h
	widgets/Button.cpp
	widgets/ImageButton.h
	widgets/ImageButton.cpp
	widgets/ImageTextButton.h
	widgets/ImageTextButton.cpp
	widgets/Dialog.h
	widgets/Dialog.cpp
	widgets/MessageDialog.h
	widgets/MessageDialog.cpp
	widgets/QuestionDialog.h
	widgets/QuestionDialog.cpp
	widgets/EditText.h
	widgets/EditText.cpp
	widgets/EditDialog.h
	widgets/EditDialog.cpp
	socket/ClientSocket.h
	socket/ClientSocket.cpp
	socket/ServerSocket.h
	socket/ServerSocket.cpp
	socket/ClientPlayer.h
	socket/ClientPlayer.cpp
	socket/ServerPlayer.h
	socket/ServerPlayer.cpp
    #Please put the new source files and header files here
    )

add_subdirectory(res)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/res)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
	
if(MINGW)
	link_libraries(ege gdi32 imm32 msimg32 ole32 oleaut32 winmm uuid wsock32)
	set(CMAKE_EXE_LINKER_FLAGS "-mwindows")
endif()


# Add resource files
if(WIN32)
    if(MINGW)
        set(CMAKE_RC_COMPILER_INIT windres)
        enable_language(RC)
        set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff -i <SOURCE> -o <OBJECT>")
		list(APPEND PROJECT_SOURCE_FILE res/Dream-Gobang.rc)
	elseif(MSVC)
		list(APPEND PROJECT_SOURCE_FILE res/Dream-Gobang.rc)
	endif()
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
add_executable (Dream-Gobang ${PROJECT_SOURCE_FILE})
target_link_libraries(Dream-Gobang ege)

set(INSTALL_RES
	${PROJECT_SOURCE_DIR}/res/cursor.ani
	${PROJECT_SOURCE_DIR}/res/font-llt.ttc
	${PROJECT_SOURCE_DIR}/res/font-yahei-mono.ttf
	${PROJECT_SOURCE_DIR}/res/button-click.wav
	${PROJECT_SOURCE_DIR}/res/game-bgm.wma
)

if(MINGW)
	install(FILES ${INSTALL_RES}  DESTINATION ${EXECUTABLE_OUTPUT_PATH}/res)
elseif(MSVC)
	install(FILES ${INSTALL_RES}  DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Debug/res)
	install(FILES ${INSTALL_RES}  DESTINATION ${EXECUTABLE_OUTPUT_PATH}/MinSizeRel/res)
	install(FILES ${INSTALL_RES}  DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release/res)
	install(FILES ${INSTALL_RES}  DESTINATION ${EXECUTABLE_OUTPUT_PATH}/RelWithDebInfo/res)
endif()

#project(Gobang-Terminal-Test)

#set(TERMINAL_TEST_SOURCE_FILE
#		Gobang.h
#		Chess.h
#		Chess.cpp
#		ComputerPlayer.h
#		ComputerPlayer.cpp
#		TerminalTestMain.cpp)
#if(MINGW)
#	set(CMAKE_EXE_LINKER_FLAGS "")
#endif()
#add_executable (Gobang-Terminal-Test ${TERMINAL_TEST_SOURCE_FILE})