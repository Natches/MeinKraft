#pragma once
#define TOSTRING_HELPER(x) #x
#define TOSTRING(x) TOSTRING_HELPER(x)
#define _WARNING(desc, code) __pragma(message(__FILE__ "(" TOSTRING(__LINE__) ") : Warning " TOSTRING(code) " : " desc "\n Found in :" __FUNCSIG__))

#define _WARNING_CODE1_ CTN1
#define _WARNING_MSG1_ "\"duplicationCount\" template variable is under or equal to 0, the function will does nothing from this point.\nUse the variant that doesn't need the \"duplicationCount\" template instead."

//TODO : warning no assert when it will be here, cause static_assert is not friendly
#define _ASSERT_MESSAGE_CTN1 __FILE__"(" TOSTRING(__LINE__) ") : Warning " TOSTRING(_WARNING_CODE1_) " : " _WARNING_MSG1_ "\n Found in :" __FUNCSIG__