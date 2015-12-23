LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := droidport_shared

LOCAL_MODULE_FILENAME := libdroidport

LOCAL_SRC_FILES := \
../src/droidport.cc \
../src/DroidWrapper.cc 

LOCAL_LDLIBS := -llog

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../src 

include $(BUILD_SHARED_LIBRARY)