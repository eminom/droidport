
#include <android/log.h>
#include <jni.h>

#include "DroidWrapper.h"

#define LOG_TAG	"droidport"
#define LOGD(...)\
	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

extern "C" 
{

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	LOGD("Yes, I am here");
	LOGD("Want more ?");
	LOGD("Congrats to Musk's Falcon 9 !");
	DroidWrapper::setVM(vm);

	//Some external calling
	return JNI_VERSION_1_4;
}


}
