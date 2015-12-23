

#include "DroidWrapper.h"
#include <android/log.h>
#include <string.h>
#include <pthread.h>


#define LOG_TAG	"DroidWrapper"

#define LOGD(...)	\
	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#define LOGE(...)	\
	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


static pthread_key_t g_key;

static JavaVM* _vm = nullptr; // compiler speicifed in Application.mk
static jobject _loader = nullptr;
static jmethodID _loadMethodID = nullptr;

static jclass _getClassID(const char *className) {
    if (nullptr == className) {
        return nullptr;
    }
    JNIEnv* env = DroidWrapper::getEnv();
    jstring clsname = env->NewStringUTF(className);
    jclass clz = (jclass) env->CallObjectMethod(_loader,
           _loadMethodID,
           clsname);
    if (nullptr == clz) {
        LOGE("Classloader failed to find class of %s", className);
        env->ExceptionClear();
    }
    env->DeleteLocalRef(clsname);
    return clz;
}

static void _detachCurrentThread(void *a){
	DroidWrapper::getVM()->DetachCurrentThread();
}

JavaVM* DroidWrapper::getVM() {
	pthread_t thisthread = pthread_self();
	LOGD("DroidWrapper::getVM(): pthread_self() = %ld", thisthread);
	return _vm;
}

void DroidWrapper::setVM(JavaVM *vm) {
	pthread_t thisthread = pthread_self();
	LOGD("DroidWrapper::setVM(%p): pthread_self() = %ld", vm, thisthread);
	_vm = vm;

	//
	pthread_key_create(&g_key, _detachCurrentThread);
}

static JNIEnv* _cacheEnv(JavaVM *jvm){
	JNIEnv *env = nullptr;
	jint ret = jvm->GetEnv((void**)&env, JNI_VERSION_1_4);
	switch(ret){
	case JNI_OK:
		pthread_setspecific(g_key, env);
		return env;
	case JNI_EDETACHED:
		if (jvm->AttachCurrentThread(&env, nullptr) < 0 ) {
			LOGE("Failed to get the environment using AttachCurrentThread()");
			return nullptr;
		} else {
			pthread_setspecific(g_key, env);
			return env;
		}
	case JNI_EVERSION:
		LOGE("JNI interface version 1.4 not supported");
		//thru
	default:
		LOGE("Failed to get the environment using GetEnv()");
		return nullptr;
	}
}

JNIEnv* DroidWrapper::getEnv(){
	JNIEnv *_env = (JNIEnv*)pthread_getspecific(g_key);
	if (nullptr==_env)
		_env = _cacheEnv(_vm);
	return _env;
}

bool
DroidWrapper::getMethodInfo(DroidMethodCache& mi, 
	const char *className,
	const char *methodName,
	const char *sigCode)
{
	if( ! className ||
		! methodName ||
		! sigCode )
		return false;

	JNIEnv *env = DroidWrapper::getEnv();
	if(!env)
		return false;

	jclass clsid = _getClassID(className);
	if( ! clsid ){
		LOGE("Failed to find class <%s>", className);
		env->ExceptionClear();
		return false;
	}

	jmethodID mid = env->GetMethodID(clsid, methodName, sigCode);
	if (!mid){
		LOGE("Failed to find method %s for <%s>", methodName, className);
		env->ExceptionClear();
		return false;
	}

	mi.env = env;
	mi.classID = clsid;
	mi.methodID = mid;
	return true;
}

bool 
DroidWrapper::getMethodInfoDefault(DroidMethodCache& mi
	, const char *className
	, const char *methodName
	, const char *sigCode){
	if( nullptr == className ||
		nullptr == methodName ||
		nullptr == sigCode ){
		return false;
	}

	JNIEnv *env = DroidWrapper::getEnv();
	if(!env)
		return false;

	jclass clzid = env->FindClass(className);
	if (!clzid) {
		LOGE("Failed to find class %s", className);
		env->ExceptionClear();
		return false;
	}

	jmethodID methodID = env->GetMethodID(clzid, methodName, sigCode);
	if(!methodID){
		LOGE("Failed to find method %s for class<%s>", methodName, className);
		env->ExceptionClear();
		return false;
	}

	mi.env = env;
	mi.classID = clzid;
	mi.methodID = methodID;
	return true;
}

// o derives from Context(such as an Acvitity)
bool DroidWrapper::attachClassLoaderFromObj(jobject ooo){
	// Activity.getClassLoader()
	// cache loadClass(method)
	DroidMethodCache getClassLoader;
	if ( ! getMethodInfoDefault(getClassLoader, 
			"android/content/Context",
			"getClassLoader",
			"()Ljava/lang/ClassLoader;")){
		return false;
	}
	auto env = getEnv();
	jobject loader = env->CallObjectMethod(ooo, getClassLoader.methodID);
	if( nullptr == loader){
		LOGE("Calls failed");
		env->ExceptionClear();
		return false;
	}
	DroidMethodCache loadClass;
	if( ! getMethodInfoDefault(loadClass, 
			"java/lang/ClassLoader",
			"loadClass",
			"(Ljava/lang/String;)Ljava/lang/Class;")){
		LOGE("No method loadClass for ClassLoader");
		return false;
	}
	// _loader must not be set before
	_loader = env->NewGlobalRef(loader);
	_loadMethodID = loadClass.methodID;
	return true;
}

bool DroidWrapper::getStaticMethodInfo(DroidMethodCache &mi,
                                    const char *className, 
                                    const char *methodName,
                                    const char *paramCode) {
    if ((nullptr == className) ||
        (nullptr == methodName) ||
        (nullptr == paramCode)) {
        return false;
    }

    JNIEnv *env = getEnv();
    if (!env) {
        LOGE("Failed to get JNIEnv");
        return false;
    }
        
    jclass classID = _getClassID(className);
    if (! classID) {
        LOGE("Failed to find class %s", className);
        env->ExceptionClear();
        return false;
    }

    jmethodID methodID = env->GetStaticMethodID(classID, methodName, paramCode);
    if (! methodID) {
        LOGE("Failed to find static method id of %s in class<%s>", methodName, className);
        env->ExceptionClear();
        return false;
    }
    
    mi.env = env;    
    mi.classID = classID;
    mi.methodID = methodID;
    return true;
}