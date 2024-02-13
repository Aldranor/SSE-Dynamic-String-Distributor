#pragma once
#include "../include/Globals.h"

class Processor
{
public:
	static Processor* GetSingleton()
	{
		static Processor menu;
		return &menu;
	}

	void RunConstTranslation();

private:

	template <typename T>
	void SetConstStrings(RE::TESForm* Form, RE::BSFixedString NewString, RE::BSFixedString T::* memberPtr);

	void SetGameSettingString(const std::string& a_name, const std::string& a_NewString);

	Processor() = default;
	~Processor() = default;
	Processor(const Processor&) = delete;
	Processor(Processor&&) = delete;
	Processor& operator=(const Processor&) = delete;
	Processor& operator=(Processor&&) = delete;
};