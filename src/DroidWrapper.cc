

#include "DroidWrapper.h"
#include <android/log.h>
#include <string.h>
#include <pthread.h>


#define LOG_TAG	"DroidWrapper"
#define LOGD(...)	\
	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

JavaVM* DroidWrapper::_vm = NULL;

JavaVM* DroidWrapper::getVM() {
	return _vm;
}

void DroidWrapper::setVM(JavaVM *vm) {
	_vm = vm;
}

