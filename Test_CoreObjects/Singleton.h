#ifndef SINGLETON_H
#define SINGLETON_H

#include "Exception.h"

/**
 Singleton : class with an unique occurency
*/
template<typename T>
class Singleton
{
	static T* ms_singleton;
	protected:

		Singleton()
		{
			ASSERT(!ms_singleton);
			ms_singleton = static_cast<T*>(this);
		}

		~Singleton()
		{
			ASSERT(ms_singleton);
			ms_singleton=0;
		}

	public:
		static inline void Create()
		{
			if(ms_singleton)return;
			new T();
		}
		static inline T& GetSingleton()
		{
			ASSERT(ms_singleton);
			return *ms_singleton;
		}
		static inline T* GetSingletonPtr()
		{
			ASSERT(ms_singleton);
			return ms_singleton;
		}
		static inline void Destroy()
		{
			if(ms_singleton)delete ms_singleton;
			ms_singleton=0;
		}
		static inline bool IsValid()
		{
			return ( ms_singleton != 0 );
		}
};

template <typename T> T* Singleton <T>::ms_singleton = 0;

#endif