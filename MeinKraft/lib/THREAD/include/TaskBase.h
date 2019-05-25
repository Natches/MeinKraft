#pragma once

namespace TaskThreadLib
{
	namespace TaskBaseSpace
	{
		class TaskBase
		{

		public:
			TaskBase() = default;
			virtual ~TaskBase() = default;

			bool IsRenderTask()const { return isRenderTask; }

			virtual inline void Function() = 0;

		protected:
			bool isRenderTask;

		};
	}
}

