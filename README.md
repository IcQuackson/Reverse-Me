<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
</head>
<body>
  <div class="center-text">
    <h1 align="center">
    	🔃 Reverse-Me 🔃
    </h1>
    <h3 align="center">
      <i>
    	  Introductory project to the discipline of reverse engineering.
      </i>
    </h3>
    <div align="center">
      <img alt="reverse" src="https://github.com/user-attachments/assets/7ac12667-1077-43b8-9ae7-29d18b0f28b0" width=250px/>
      <p>The goal of this project is to understand several binary files and then find a password to validate each of them.</p>
    </div>
  </div>




# Requirements:

1. **Analyze the files**:
   - Understand how the binaries work using whatever tool (gdb is sugested).

2. **Find the password**:
   - Use the tools to analyze each instruction and find the correct code for each program.

3. **Patch**:
   - Add for each program a patch allowing to validate each program with any password.

4. **Recreate**:
   - For each binary write a C program that should be a copy of the original.

# Setup Instructions

### Prerequisites
- `docker`

### Steps
1. **Start container**:
   ```bash
   cd src && sudo docker compose up
   ```
2. **Enter the linux machine**:
   ```bash
   sudo docker exec -it linux bash
   ```

# Reversing the binaries

## **Level 1**

2. **Analyze program behaviour**:
   ```bash
   bash$ cd /levels/level1
   bash$ ./level1
   Please enter key: hello
   Nope.
   ```
   - As we can see, the program asks for input, likely validates the code comparing it to a string variable and then returns a response indicating success or fail.
   - This gives us useful information because it means we can analyze the program instructions and find the variable that stores the string used for validation.
   - We then use gdb to analyze the program further:

   ```bash
   bash$ gdb ./level
   <GDB terminal output>
   gdb-peda$
   ```
   - As you can see we have installed [PEDA](https://github.com/longld/peda) which is a GDB plugin to add commands to support debugging and exploit development.
   - Let's see the whole assemble code for the main function so we can analyze all the instructions:

   ```bash
   gdb-peda$ disassemble main
   ```
  
  - We can clearly see a very interesting function being used which is `strcmp` and likely the 2 registers (`edx` and `ecx`)  being used to store the 2 function arguments:
  ```bash
  0x565e9237 <+119>:   mov    DWORD PTR [eax+0x4],edx
  0x565e923a <+122>:   mov    DWORD PTR [eax],ecx
  0x565e923c <+124>:   call   0x565e9040 <strcmp@plt>
  ```
  - Let's set a breakpoint at the `strcmp` call, run the program and look for useful information.
  ```bash
  gdb-peda$ b strcmp
  gdb-peda$ run
  ```

- Check out this part of the output:
```bash
ECX: 0xffc1267c ("hello")
EDX: 0xffc1266e ("__stack_check")
```

- These are the registers we were looking for! Looks like `__stack_check` might be what we wanted.
- Let's try that code:
```bash
Please enter key: __stack_check
Good job.
```
- We found the code!
- Now, to write a copy of the original program we can look at the `disassemble main` command output again.
- The program also uses `printf` and `scanf`.
- The program should look like:
```C
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
```

- If you want to be sure you can always use the `boomerang` to decompile the binary file.
```bash
bash$ boomerang-cli level1
```
- You should get something like:
```C
int main(int argc, char *argv[]);


/** address: 0x000011c0 */
int main(int argc, char *argv[])
{
    int eax; 		// r24
    char local0; 		// m[esp - 112]

    printf(pc + 0xe47);
    scanf(pc + 0xe5a);
    eax = strcmp(&local0, &eax);
    if (eax != 0) {
        printf(pc + 0xe68);
    }
    else {
        printf(pc + 0xe5d);
    }
    return 0;
}
```
- The notation m[esp - 112] indicates that the variable local0 resides at an offset of 112 bytes below the stack pointer (esp). So now we also have the buffer size for the `user_input` string.

