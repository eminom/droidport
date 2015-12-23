


#ifndef _DROID_HELPER_DEF__
#define _DROID_HELPER_DEF__

#include <jni.h>

typedef struct tagDroidMethodCache{
	JNIEnv *env;
	jclass classID;
	jmethodID methodID;
}DroidMethodCache;

class DroidWrapper
{
private:
	DroidWrapper();
	~DroidWrapper();

public:
	static void setVM(JavaVM *javaVM);
	static JavaVM* getVM();
	static JNIEnv* getEnv();

	static bool getMethodInfo(DroidMethodCache&
		, const char *className
		, const char *methodName
		, const char *sigCode);
	
	static bool getStaticMethodInfo(DroidMethodCache&
		, const char *className
		, const char *methodName
		, const char *sigCode);

	static bool attachClassLoaderFromObj(jobject ooo);

private:
	static bool getMethodInfoDefault(DroidMethodCache&
		, const char *className
		, const char *methodName
		, const char *sigCode);
};


#endif