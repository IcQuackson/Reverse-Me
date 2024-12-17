#include <stdio.h>
#include <string.h>


int main()
{
	char user_input[112];

	printf("Please enter key: ");
    scanf("%s", user_input);

	if (strcmp(user_input, "__stack_check") == 0)
	{
		printf("Good job.\n");
	}
	else
	{
		printf("Nope.\n");	
	}
}
