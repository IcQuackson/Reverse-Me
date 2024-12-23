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

### Analyze program behavior:
```bash
bash$ cd /levels/level1
bash$ ./level1
Please enter key: hello
Nope.
```
- The program likely validates the input by comparing it to a stored string. Using `gdb`, we analyze further:

```bash
bash$ gdb ./level1
<gdb output>
gdb-peda$
```

- Disassembling the `main` function reveals a `strcmp` call with two arguments in registers `ecx` and `edx`:

```bash
gdb-peda$ disassemble main
<disassembled output>
```

- Setting a breakpoint at `strcmp` and running the program reveals:
```bash
ECX: 0xffc1267c ("hello")
EDX: 0xffc1266e ("__stack_check")
```
- The correct code is `__stack_check`:
```bash
Please enter key: __stack_check
Good job.
```

### Recreating the program
Here is the recreated C program:
```c
#include <stdio.h>
#include <string.h>

int main() {
    char user_input[112];

    printf("Please enter key: ");
    scanf("%s", user_input);

    if (strcmp(user_input, "__stack_check") == 0) {
        printf("Good job.\n");
    } else {
        printf("Nope.\n");
    }
    return 0;
}
```

### Patching the binary
To allow validation with any password:
1. Open the binary using `radare2`, identify the instruction and replace it to bypasse verification.
	```bash
	cp level1 level1_patched // copy
 	r2 -w level1_patched
 	aa // analyze instructions
	/a cmp eax, 0 // find nearest instruction of that pattern
	s <address> // go to instruction
	wx 31c090 // xor eax, eax ; nop // overwrite instruction by clearing register used for checking
 	qyy // save and quit
	```
2. Test.
```bash
Please enter key: anything
Good job.
```
3. Generate patch.
	```bash
	bsdiff level1 level1_patched level1.patch
	```
4. Get patched binary with the original and patch file.
	```bash
	bspatch level1 level1_patched level1.patch
	```

## **Level 2**

### Analyze program behavior:

- Using `gdb-peda`, we find that the input passcode must start with `"00"` and is followed by a sequence of groups of three numbers. The program validates these groups against a string stored in memory, "delabere".
- By analyzing the instructions in `gdb` and the decompiled code using `retdec`, we confirm that the passcode `00101108097098101114101` satisfies these conditions, as each group corresponds to a character in "delabere".

### Patching the binary
1. Using `radare2`, locate the validation logic:
  ```bash
  r2 ./level2
  aaa
  s main // go to main
  pdf // show every instruction of main
  s <address> // go to adress with an instruction of 5 bytes
  wa jmp <address> // address where ok() will be called
  ```

2. Test.
```bash
Please enter key: anything
Good job.
```

3. Generate patch.
	```bash
	bsdiff level1 level1_patched level1.patch
	```
 
4. Get patched binary with the original and patch file.
	```bash
	bspatch level1 level1_patched level1.patch
	```

## **Level 3**

### Analyze program behavior:
- Run the binary:
```bash
bash$ cd /levels/level3
bash$ ./level3
Enter key: key123
Invalid key.
```
- Using `gdb` and `radare2`, we determine that the input key must start with `"42"`. The rest of the string is processed in groups of three characters, each interpreted as an integer. These integers are stored in a buffer that is later matched against the string `********`.
- By analyzing the instructions and logic using `gdb` and `radare2`, we derive the key `42042042042042042042042`, where each group corresponds to the required values.

### Patching the binary
1. Using `radare2`, locate the validation logic. We bypass verifications using a jmp:
  ```bash
  $> r2 ./level2
  $> aaa
  $> s main // go to main
  $> pdf // show every instruction of main
  $> s 0x00001369
  $> pdi 2
  0x00001369           b832000000  mov eax, 0x32
  0x0000136e                 39c8  cmp eax, ecx
  $> wa jmp 0x000014a7
  INFO: Written 5 byte(s) (jmp 0x000014a7) = wx e939010000 @ 0x00001369
  $> pdi 2
  0x00001369           e939010000  jmp 0x14a7
  0x0000136e                 39c8  cmp eax, ecx
  $> qyy
  
  ```

2. Test.
```bash
Please enter key: anything
Good job.
```

3. Generate patch.
	```bash
	bsdiff level1 level1_patched level1.patch
	```

3. Get patched binary with the original and patch file.
	```bash
	bspatch level1 level1_patched level1.patch
	```
