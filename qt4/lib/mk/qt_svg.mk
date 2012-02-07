include $(REP_DIR)/lib/import/import-qt_svg.mk

SHARED_LIB = yes

# extracted from src/svg/Makefile
QT_DEFINES += -DQT_BUILD_SVG_LIB -DQT_NO_USING_NAMESPACE -DQT_NO_CAST_TO_ASCII -DQT_ASCII_CAST_WARNINGS -DQT_MOC_COMPAT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB

# extracted from src/svg/Makefile

SRC_CC = \
         qsvggraphics.cpp \
         qsvghandler.cpp \
         qsvgnode.cpp \
         qsvgstructure.cpp \
         qsvgstyle.cpp \
         qsvgfont.cpp \
         qsvgtinydocument.cpp \
         qsvgrenderer.cpp \
         qsvgwidget.cpp \
         qgraphicssvgitem.cpp \
         qsvggenerator.cpp \
         moc_qsvgwidget.cpp

# some source files need to be generated by moc from other source/header files before
# they get #included again by the original source file in the compiling stage

# source files generated from header files ("moc_%.cpp: %.h" rule in spec-qt4.mk)
# extracted from "compiler_moc_header_make_all" target
COMPILER_MOC_HEADER_MAKE_ALL_FILES = \
                                     moc_qsvgrenderer.cpp \
                                     moc_qsvgwidget.cpp \
                                     moc_qgraphicssvgitem.cpp

$(SRC_CC:.cpp=.o): $(COMPILER_MOC_HEADER_MAKE_ALL_FILES)

# source files generated from source files ("%.moc: %.cpp" rule in spec-qt4.mk)
# extracted from "compiler_moc_source_make_all" target
COMPILER_MOC_SOURCE_MAKE_ALL_FILES = \

$(SRC_CC:.cpp=.o): $(COMPILER_MOC_SOURCE_MAKE_ALL_FILES)

INC_DIR += $(REP_DIR)/src/lib/qt4/mkspecs/qws/genode-x86-g++ \
           $(REP_DIR)/include/qt4 \
           $(REP_DIR)/contrib/$(QT4)/include \
           $(REP_DIR)/include/qt4/QtCore \
           $(REP_DIR)/contrib/$(QT4)/include/QtCore \
           $(REP_DIR)/include/qt4/QtCore/private \
           $(REP_DIR)/contrib/$(QT4)/include/QtCore/private \
           $(REP_DIR)/include/qt4/QtGui \
           $(REP_DIR)/contrib/$(QT4)/include/QtGui \
           $(REP_DIR)/include/qt4/QtGui/private \
           $(REP_DIR)/contrib/$(QT4)/include/QtGui/private \
           $(REP_DIR)/include/qt4/QtSvg \
           $(REP_DIR)/contrib/$(QT4)/include/QtSvg \
           $(REP_DIR)/include/qt4/QtSvg/private \
           $(REP_DIR)/contrib/$(QT4)/include/QtSvg/private \
           $(REP_DIR)/src/lib/qt4/src/corelib/global \
           $(REP_DIR)/contrib/$(QT4)/src/3rdparty/harfbuzz/src

LIBS += qt_core qt_gui zlib libc

vpath % $(REP_DIR)/include/qt4/QtSvg
vpath % $(REP_DIR)/include/qt4/QtSvg/private

vpath % $(REP_DIR)/src/lib/qt4/src/svg

vpath % $(REP_DIR)/contrib/$(QT4)/src/svg
