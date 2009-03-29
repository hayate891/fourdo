AUTOMAKE_OPTIONS = foreign
SUBDIRS = filesystem

bin_PROGRAMS = 4do 
4do_SOURCES = ARMCPU.cpp BitMath.cpp Console.cpp DMAController.cpp FourDOApp.cpp FourDOCanvas.cpp ImageViewer.cpp ISOBrowser.cpp KernelFaker.cpp MainCanvas.cpp MainFrame.cpp SWIHandler.cpp

AM_CPPFLAGS = -I./filesystem -I/usr/include/wx-2.8 -I/usr/lib/wx/include/gtk2-ansi-release-2.8 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -DGENRE_UNIX -pthread

AM_LDFLAGS = -pthread -Lfilesystem -lfilesystem -lwx_gtk2_richtext-2.8 -lwx_gtk2_aui-2.8 -lwx_gtk2_xrc-2.8 -lwx_gtk2_qa-2.8 -lwx_gtk2_html-2.8 -lwx_gtk2_adv-2.8 -lwx_gtk2_core-2.8 -lwx_base_xml-2.8 -lwx_base_net-2.8 -lwx_base-2.8 -lwx_gtk2_gl-2.8
