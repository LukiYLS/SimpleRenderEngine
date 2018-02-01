#pragma once

#include <Windows.h>
#include <memory>
#define MAX_SEMAPHORE_COUNT 0x7fffffff
namespace SRE {

	class Mutex	
	{
	public:
		typedef std::shared_ptr<Mutex> ptr;
		Mutex()
		{
			InitializeCriticalSection(&cs);
		}

		~Mutex()
		{
			DeleteCriticalSection(&cs);
		}

		void lock()
		{
			EnterCriticalSection(&cs);
		}

		bool try_lock()
		{
			return TryEnterCriticalSection(&cs) ? true : false;
		}

		void unlock()
		{
			LeaveCriticalSection(&cs);
		}
	private:
		CRITICAL_SECTION cs;
	};

	class Condition
	{
	public:	
		typedef std::shared_ptr<Condition> ptr;
		Condition()
		{
			_event = CreateSemaphore(NULL, 0, MAX_SEMAPHORE_COUNT, NULL);
		}

		~Condition()
		{
			CloseHandle(_event);
		}

		void wait()
		{
			WaitForSingleObject(_event, INFINITE);
		}

		void notify_one()
		{
			ReleaseSemaphore(_event, 1, NULL);
		}

		void notify_all()
		{
			ReleaseSemaphore(_event, MAX_SEMAPHORE_COUNT, NULL);
		}
	private:
		HANDLE _event;
	};

	class Thread
	{
	public:
		typedef std::shared_ptr<Thread> ptr;
		Thread(HANDLE thread_handle)
			: _threadHandle(thread_handle)
		{

		}

		~Thread()
		{
			CloseHandle(_threadHandle);
		}

		void join()
		{
			WaitForSingleObject(_threadHandle, INFINITE);
		}
	private:
		HANDLE _threadHandle;
	};

	//////////////////////////////////////////////////////////////////////////

	//win32¶àÏß³Ì
	class ThreadService
	{
	public:
		static ThreadService* getSingleton();
		ThreadService();
		~ThreadService();
		Mutex::ptr createMutex();
		Condition::ptr createCondition();
		Thread::ptr createThread(void(*thread_start_function)(void *), void *parameter);
		void sleep(unsigned long milliseconds);
	private:
		static ThreadService* service;
	};

	Mutex::ptr ThreadService::createMutex()
	{
		return Mutex::ptr(new Mutex());
	}

	Condition::ptr ThreadService::createCondition()
	{
		return Condition::ptr(new Condition());
	}

	struct ThreadStartWarpper
	{
		void(*thread_start_function)(void *);
		void* parameter;
	};

	DWORD WINAPI thread_start_function_warpper(LPVOID lpThreadParameter)
	{
		ThreadStartWarpper* start_warpper = (ThreadStartWarpper*)lpThreadParameter;
		start_warpper->thread_start_function(start_warpper->parameter);
		delete start_warpper;
		return 0;
	}

	Thread::ptr ThreadService::createThread(void(*thread_start_function)(void *), void *parameter)
	{
		ThreadStartWarpper* start_warpper = new ThreadStartWarpper();
		start_warpper->thread_start_function = thread_start_function;
		start_warpper->parameter = parameter;

		HANDLE handle = CreateThread(NULL, 0, thread_start_function_warpper, start_warpper, 0, 0);
		return Thread::ptr(new Thread(handle));
	}

	void ThreadService::sleep(unsigned long milliseconds)
	{
		Sleep(milliseconds);
	}
}