#ifndef __FINAL_TASK__
#define __FINAL_TASK__

#include <functional>
#include "TaskTemplate.h"
#include "TaskBase.h"

namespace TaskThreadLib
{
	namespace TaskTemplatedInHerited
	{
		template<typename ...Args>
		class FinalTask final : public TaskThreadLib::TaskTemplated::TaskTemplate<Args...>, public TaskThreadLib::TaskBaseSpace::TaskBase
		{
		public:
			FinalTask(std::function<void(void*)> functionCallback, void* _args,  bool renderTask = false);
			~FinalTask() = default;
			virtual TaskTemplated::TaskTemplate<void(void*)>::FuncTask Function() const override
			{
				return TaskFunc;
			}

		private:
			FinalTask() = delete;
		};
	}
}
#include"../src/FinalTask.cpp"
#endif // __TASK__