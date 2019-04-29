#pragma once

namespace ExampleNamespace
{
	class ExampleClass
	{
#pragma region Non-Static Attributes
	public:
		int m_variableInt;

	private:
		bool m_variableBool;
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
	public:
		ExampleClass() {}

	private:
		~ExampleClass() {}

	public:
		void				MethodsWithNoParameter() const {}
		inline const int	MethodThatReturnInt() {}

	private:
		void				MethodWithParameter(const double& value) {}
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
	public:
		inline int GetVariableInt() const { return m_variableInt; }
#pragma endregion
	};

	enum E_EXAMPLE_ENUM
	{
		ONE,
		TWO,
		THREE,
		COUNT
	};
} //namespace ExampleNamespace

//Only Regions
/*
#pragma region Non-Static Attributes
#pragma endregion
#pragma region Static Attributes
#pragma endregion
#pragma region Non-Static Methods
#pragma endregion
#pragma region Static Methods
#pragma endregion
#pragma region Getters & Setters
#pragma endregion
*/