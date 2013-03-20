#pragma once

#include <jni.h>

namespace pb
{
	class Jni
	{
	public:
		static JNIEnv* GetJniEnv();
	};
}
