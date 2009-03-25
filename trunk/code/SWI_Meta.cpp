#include "SWI_Meta.hpp"

void META_ImageEntryPoint()
{
	// When the 3DO loads and runs a CD's "launchme" file, that file
	// will perform it's self-relocation and zero-init code. Immediately
	// afterwards, it will call this code.
	// 
	// I haven't identified the purpose of the code. It reads the last
	// string on the stack ("$app\Launchme") and does something with it.
	//
	// It doesn't seem to effect the code, so who cares!
}