#include "TaskTemplate.h"

#ifdef __TASK_TEMPLATED__
namespace TaskThreadLib
{
	namespace TaskTemplated
	{
		template<typename ...Args>
		TaskTemplate<Args...>::TaskTemplate(FuncTask&& functionCallback, Args&&..._args, bool renderTask)
		{
			TaskFunc = std::bind(functionCallback, _args...);
			isRenderTask = renderTask;
		}
	}
}
#endif // !__TASK_TEMPLATED__
