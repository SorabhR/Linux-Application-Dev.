#include <stdlib.h>

int main()
{
	int return_value;

	//return_value = system("ls -l /");
	return_value = system("ps -e");
	return return_value;
}
