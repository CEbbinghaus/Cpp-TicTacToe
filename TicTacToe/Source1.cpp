#include <iostream>
#include <conio.h>

using namespace std;
enum keyCodes {
	kc_up = 72,
	kc_down = 80,
	kc_left = 75,
	kc_right = 77
};
int main12() {
	cout << "\033[1;31m[textgoeshere]\033[0m" << endl;
	system("pause");
	return 0;
}