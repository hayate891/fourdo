AUTOMAKE_OPTIONS = foreign
SUBDIRS = filesystem

bin_PROGRAMS = 4do 
4do_SOURCES = ARMCPU.cpp BitMath.cpp Console.cpp DMAController.cpp FourDOApp.cpp FourDOCanvas.cpp ImageViewer.cpp ISOBrowser.cpp KernelFaker.cpp MainCanvas.cpp MainFrame.cpp SWIHandler.cpp

INCLUDES = -I./filesystem -I/usr/include/wx-2.8 -I/usr/lib/wx/include/gtk2-unicode-release-2.8 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -DGENRE_UNIX -pthread

LIBS = -pthread -Lfilesystem -lfilesystem -lwx_gtk2u_richtext-2.8 -lwx_gtk2u_aui-2.8 -lwx_gtk2u_xrc-2.8 -lwx_gtk2u_qa-2.8 -lwx_gtk2u_html-2.8 -lwx_gtk2u_adv-2.8 -lwx_gtk2u_core-2.8 -lwx_baseu_xml-2.8 -lwx_baseu_net-2.8 -lwx_baseu-2.8 -lwx_gtk2u_gl-2.8
