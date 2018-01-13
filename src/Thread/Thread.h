#pragma once

#include <Windows.h>
#include <memory>
#define MAX_SEMAPHORE_COUNT 0x7fffffff
namespace SRE {

	class mutex	
	{
	public:
		typedef std::shared_ptr<mutex> ptr;
		mutex()
		{
			InitializeCriticalSection(&cs);
		}

		~mutex()
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

	class condition
	{
	public:	
		typedef std::shared_ptr<condition> ptr;
		condition()
		{
			_event = CreateSemaphore(NULL, 0, MAX_SEMAPHORE_COUNT, NULL);
		}

		~condition()
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

	class thread
	{
	public:
		typedef std::shared_ptr<thread> ptr;
		thread(HANDLE thread_handle_)
			: _thread_handle(thread_handle_)
		{

		}

		~thread()
		{
			CloseHandle(_thread_handle);
		}

		void join()
		{
			WaitForSingleObject(_thread_handle, INFINITE);
		}
	private:
		HANDLE _thread_handle;
	};

	//////////////////////////////////////////////////////////////////////////


	class thread_service
	{
	public:
		static thread_service* getSingleton();
		thread_service();
		~thread_service();
		mutex::ptr create_mutex();
		condition::ptr create_condition();
		thread::ptr create_thread(void(*thread_start_function)(void *), void *parameter);
		void sleep(unsigned long milliseconds);
	private:
		static thread_service* service;
	};

	mutex::ptr thread_service::create_mutex()
	{
		return mutex::ptr(new mutex());
	}

	condition::ptr thread_service::create_condition()
	{
		return condition::ptr(new condition());
	}

	struct thread_start_warpper
	{
		void(*thread_start_function)(void *);
		void* parameter;
	};

	DWORD WINAPI thread_start_function_warpper(LPVOID lpThreadParameter)
	{
		thread_start_warpper* start_warpper = (thread_start_warpper*)lpThreadParameter;
		start_warpper->thread_start_function(start_warpper->parameter);
		delete start_warpper;
		return 0;
	}

	thread::ptr thread_service::create_thread(void(*thread_start_function)(void *), void *parameter)
	{
		thread_start_warpper* start_warpper = new thread_start_warpper();
		start_warpper->thread_start_function = thread_start_function;
		start_warpper->parameter = parameter;

		HANDLE handle = CreateThread(NULL, 0, thread_start_function_warpper, start_warpper, 0, 0);
		return thread::ptr(new thread(handle));
	}

	void thread_service::sleep(unsigned long milliseconds)
	{
		Sleep(milliseconds);
	}
}