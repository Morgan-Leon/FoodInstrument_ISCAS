#-------------------------------------------------
#
# Project created by QtCreator 2012-09-09T17:51:15
#
#-------------------------------------------------
QT      += sql
QT       += core gui

TARGET = FoodInstrument
TEMPLATE = app


SOURCES += main.cpp \ 
    foodtest/sample_infoinput.cpp \
    foodtest/foodtest_dlg.cpp \
    global/externdata.cpp \
    main_dlg/main_dlg.cpp \
    serialcom/serialcom.cpp \
    spectralscan/spectralscansettings_dlg.cpp \
    spectralscan/spectralscan_dlg.cpp \
    systemsettings/systemsettings_dlg.cpp \
    spectralscan/spectralscandata_dlg.cpp \
    dingliangtest/quantifymeasurement_dlg.cpp \
    dingliangtest/quantifyinfoinput_dlg.cpp \
    timesweep/timesweepsrt_dlg.cpp \
    timesweep/timesweep_dlg.cpp \
    dingliangtest/dingtest_dlg.cpp \
    serialcom/serialprint.cpp \
    VirtualKeyboard/inputwidget.cpp \
    VirtualKeyboard/inframe.cpp \
    serialcom/seriallinux/qextserialbase.cpp \
    serialcom/seriallinux/posix_qextserialport.cpp

HEADERS += \
    foodtest/sample_infoinput.h \
    foodtest/foodtest_dlg.h \
    global/externdata.h \
    main_dlg/main_dlg.h \
    serialcom/serialcom.h \
    spectralscan/spectralscansettings_dlg.h \
    spectralscan/spectralscan_dlg.h \
    systemsettings/systemsettings_dlg.h \
    spectralscan/spectralscandata_dlg.h \
    dingliangtest/quantifymeasurement_dlg.h \
    dingliangtest/quantifyinfoinput_dlg.h \
    timesweep/timesweepsrt_dlg.h \
    timesweep/timesweep_dlg.h \
    dingliangtest/dingtest_dlg.h \
    serialcom/serialprint.h \
    VirtualKeyboard/inputwidget.h \
    VirtualKeyboard/inframe.h \
    VirtualKeyboard/database.h \
    serialcom/seriallinux/qextserialbase.h \
    serialcom/seriallinux/posix_qextserialport.h

FORMS += \
    foodtest/sample_infoinput.ui \
    foodtest/foodtest_dlg.ui \
    main_dlg/main_dlg.ui \
    spectralscan/spectralscansettings_dlg.ui \
    spectralscan/spectralscan_dlg.ui \
    systemsettings/systemsettings_dlg.ui \
    spectralscan/spectralscandata_dlg.ui \
    dingliangtest/quantifymeasurement_dlg.ui \
    dingliangtest/quantifyinfoinput_dlg.ui \
    timesweep/timesweepsrt_dlg.ui \
    timesweep/timesweep_dlg.ui \
    dingliangtest/dingtest_dlg.ui \
    VirtualKeyboard/inputwidget.ui

RESOURCES += \
    resources/resources.qrc
