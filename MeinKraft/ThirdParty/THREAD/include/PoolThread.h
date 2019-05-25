#pragma once

#ifdef __RENDER_THREAD__
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#endif

#include <vector>
#include <thread>
#include <memory>
#include <atomic>
#include <condition_variable>
#include "TaskBase.h"
#include "TaskTemplate.h"
#include "ThreadObject.h"

namespace ThreadLib
{
	class PoolThread final
	{
		std::mutex Locker;
		std::mutex RenderLocker;
	private:
		using Thread = ThreadObjectSpace::ThreadObject;
		using TaskObject = TaskThreadLib::TaskBaseSpace::TaskBase;
#ifdef __RENDER_THREAD__
		using RenderThread = RenderThreadObjectSpace::RenderThreadObject;
		friend class RenderThreadObjectSpace::RenderThreadObject;
#endif
		friend class ThreadObjectSpace::ThreadObject;

	public:
		static PoolThread& getInstance();

		bool GimmeRenderTask(unsigned short int maxSize, std::atomic<TrivialStack<PoolThread::TaskObject*>*>& taskList);

		void AddTask(TaskObject* task);
		void JoinAllThread(bool cancelAllTask = true);

		~PoolThread() = default;
		void GiveTask();
		void ChangeMaxWaitingQueueThread(unsigned short int nb);
		void ChangeMaxWaitingQueueRenderThread(unsigned short int nb);

		void FinishAllTaskThenContinue();

	private:
		void JoinAllThreadAfterAllTaskComplete();
		bool GimmeTask(unsigned short int maxSize, std::atomic<TrivialStack<PoolThread::TaskObject*>*>& taskList);
		void GiveRenderTask();

		PoolThread();
		PoolThread(const PoolThread& _poolThread) = delete;
		PoolThread(const PoolThread&& _poolThread) = delete;
		void operator=(const PoolThread& _poolThread) = delete;
		void operator=(const PoolThread&& _poolThread) = delete;

	private:
		std::atomic<TrivialStack<TaskObject*>*>	taskList;
		std::vector<Thread*>		threadPool;
#ifdef __RENDER_THREAD__
		std::atomic<TrivialStack<TaskObject*>*>	renderTaskList;
		std::vector<RenderThread*>			renderThreadPool;
#endif
		std::condition_variable cv;
		std::mutex WaitMutex;
		unsigned short int maxThread;
	};
}