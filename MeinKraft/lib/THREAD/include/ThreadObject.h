#ifndef __THREAD_OBJECT__
#define __THREAD_OBJECT__

#ifdef __RENDER_THREAD__
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#endif

#include <iostream>
#include <thread>
#include <atomic>
#include <condition_variable>
#include "TrivialStack.h"
#include "TaskBase.h"

namespace ThreadObjectSpace
{
	class ThreadObject
	{
		using TaskObject = TaskThreadLib::TaskBaseSpace::TaskBase;
	public:
		ThreadObject(unsigned short int _maxWaitSize = 10u);
		virtual ~ThreadObject();

		inline bool AreYouAvailable()const { return bool((task.load(std::memory_order_acquire)->size() ^ maxWaitSize) > 0); }

		void AddTask(TaskObject* _task);
		inline std::atomic<TrivialStack<TaskObject*>*>& getTask() { return task; }
		inline void NotifyMe() { cv.notify_all(); }

		inline void Join() { join = true; NotifyMe(); thread.join(); }
		inline bool HasJoin() { return join; }
		inline void ChangeMaxWaitSize(unsigned short int nb) { maxWaitSize = nb; }

		virtual void Init();
	private:
		bool StealTask();
	protected:
		virtual void ThreadFunc();

	protected:
		std::thread	thread;
		std::atomic<TrivialStack<TaskObject*>*> task;
		std::condition_variable cv;
		std::mutex WaitMutex;
		unsigned short int maxWaitSize;
		bool join;
	};
}
namespace RenderThreadObjectSpace
{
#ifdef __RENDER_THREAD__
	class RenderThreadObject final : public ThreadObjectSpace::ThreadObject
	{
		using TaskObject = TaskThreadLib::TaskBaseSpace::TaskBase;
	public:
		RenderThreadObject(unsigned short int _maxWaitSize = 2u);
		~RenderThreadObject();
		virtual void Init() override;

	private:
		virtual void ThreadFunc() override;

	private:
		GLFWwindow* privateThreadContext;
	};
#endif // __RENDER_THREAD__
}

#endif // __THREAD_OBJECT__
