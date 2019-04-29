#ifndef __TASK_TEMPLATED__
#define __TASK_TEMPLATED__

#include <functional>
#include "TaskBase.h"

namespace TaskThreadLib
{
	namespace TaskTemplated
	{
		template<typename ...Args>
		class TaskTemplate : public TaskThreadLib::TaskBaseSpace::TaskBase
		{
		public:
			using FuncTask = std::function<void(Args...)>;
			TaskTemplate() = default;
			TaskTemplate(FuncTask&& functionCallback, Args&&..._args, bool renderTask = false);
			virtual ~TaskTemplate() = default;

		private:
			virtual inline void Function() override { TaskFunc(); }

		protected:
			std::function<void()> TaskFunc;
		};
	}
}

#include "TaskTemplateimpl.h"

#endif // !__TASK_TEMPLATED__

