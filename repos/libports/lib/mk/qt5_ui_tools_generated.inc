QT_DEFINES += -DQT_NO_LIBUDEV -DQT_NO_XCB -DQT_NO_XKBCOMMON -DQT_BUILD_UITOOLS_LIB -DQT_BUILDING_QT -DQT_NO_CAST_TO_ASCII -DQT_ASCII_CAST_WARNINGS -DQT_MOC_COMPAT -DQT_USE_QSTRINGBUILDER -DQT_DEPRECATED_WARNINGS -DQT_DISABLE_DEPRECATED_BEFORE=0x050000 -DQFORMINTERNAL_NAMESPACE -DQT_DESIGNER_STATIC -DQT_DESIGNER -DQT_NO_EXCEPTIONS -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB

QT_INCPATH += \
  qtbase/include \
  qtbase/include/QtCore \
  qtbase/include/QtGui \
  qtbase/include/QtWidgets \
  qtbase/mkspecs/genode-g++ \
  qttools/include \
  qttools/include/QtUiTools \
  qttools/include/QtUiTools/5.1.0 \
  qttools/include/QtUiTools/5.1.0/QtUiTools \
  qttools/src/designer/src/lib/uilib \
  qttools/src/designer/src/uitools \

QT_SOURCES += \
  quiloader.cpp \
  abstractformbuilder.cpp \
  formbuilder.cpp \
  ui4.cpp \
  properties.cpp \
  formbuilderextra.cpp \
  resourcebuilder.cpp \
  textbuilder.cpp \
  moc_quiloader.cpp \
  moc_properties_p.cpp

QT_VPATH += \
  qttools/src/designer/src/lib/uilib \
  qttools/src/designer/src/uitools \

# some source files need to be generated by moc from other source/header files before
# they get #included again by the original source file in the compiling stage

# source files generated from existing header files ('moc_%.cpp: %.h' rule in import-qt5.inc)
# extracted from 'compiler_moc_header_make_all' target

COMPILER_MOC_HEADER_MAKE_ALL_FILES = \
  moc_quiloader.cpp \
  moc_properties_p.cpp

# source files generated from existing source files ('%.moc: %.cpp' rule in import-qt5.inc)
# extracted from 'compiler_moc_source_make_all' rule

COMPILER_MOC_SOURCE_MAKE_ALL_FILES = \
  quiloader.moc

