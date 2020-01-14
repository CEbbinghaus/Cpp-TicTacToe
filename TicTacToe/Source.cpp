#include "Process.h"

Process p;

//Main function that the operating system calls
int main() {

	#ifdef DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	p.init();
	return 0;
}