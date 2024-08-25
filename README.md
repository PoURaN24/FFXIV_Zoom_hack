# FFXIV Zoom hack, aka Heap Memory Search and Replace Tool :D

This program is a Windows-based tool designed to search for a specific DWORD value within the heap memory of a target process and replace it with another DWORD value. The tool leverages the Windows API to access process memory and modify it as needed.

## Method

The program uses a specific method to locate and replace values in the process memory:

1. **Search for Specific Value**: The program locates memory regions (heaps) of size `1044480` bytes.
2. **Target Occurrences**: Within these regions, it searches for exactly three occurrences of a specific float value `20.0` (represented as `0x41A00000` in DWORD format).
3. **Replace Value**: If exactly three occurrences are found, each occurrence of `20.0` is replaced with `500.0` (represented as `0x43FA0000` in DWORD format).

This approach is designed to modify memory regions where the float value corresponds to zoom level of Final Fantasy XIV.

## Features

- **Heap Memory Inspection**: The program scans through the heap memory regions of a target process.
- **Targeted Search**: It searches for a specific DWORD value within memory regions of a particular size.
- **Conditional Replace**: If exactly three occurrences of the target value are found within a heap region, all of them are replaced with a new DWORD value.
- **Privilege Escalation**: The program attempts to enable the `SeDebugPrivilege` to allow access to processes that normally require elevated privileges.

## Prerequisites

- Windows OS
- Administrator privileges (for enabling `SeDebugPrivilege`)
- Visual Studio or any other C compiler for Windows

## Compilation

You can compile the program using a C compiler like Visual Studio. Here's how you can do it using the Visual Studio Developer Command Prompt:

```sh
cl heap.c advapi32.lib
```

Get the PID of the ffxiv process in windows command line:
```sh
tasklist | find /i "ffxiv"
```
Example output:
```sh
ffxiv_dx11.exe               18672 Console                    1  2,780,004 K
```

Now use the heap tool on that process:
```sh
heap 18672
```

An example output:
```sh
[+] SeDebugPrivilege successfully enabled.
[+] Opened process with PID 18672.
[+] Heap 0: Base Address: 0000029E59080000, Region Size: 1044480 bytes
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E591299A4
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E591299AC
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E59129A0C
        [+] Overwritten with NEW_VALUE 0x43FA0000 (500.0f) at address: 0000029E591299A4
        [+] Overwritten with NEW_VALUE 0x43FA0000 (500.0f) at address: 0000029E591299AC
        [+] Overwritten with NEW_VALUE 0x43FA0000 (500.0f) at address: 0000029E59129A0C
[+] Heap 1: Base Address: 0000029E5B8A0000, Region Size: 1044480 bytes
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8A148E
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8A4EFE
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8A53CE
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8AC98E
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8ACCB6
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8B4266
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8B7D9E
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8BB3EE
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8BFE3E
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8C4C26
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8C6846
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8C9CBE
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8CDDEE
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8D1A46
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8D5B4E
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8D98EE
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8E1686
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8E5736
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8E9766
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8ED3A6
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8F11FE
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8F517E
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8F8F9E
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B8FD7AE
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B90122E
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B9038F6
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B905376
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B9088FE
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B90C8A6
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B9148C6
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B914D56
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B91718E
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E5B971430
[-] Heap does not have exactly 3 occurrences, found 33 occurrences. No changes made.
[+] Heap 2: Base Address: 0000029ECAE38000, Region Size: 1044480 bytes
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE3C6BC
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE3C6F0
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE44844
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE44934
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE4C960
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE4C9A0
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE54164
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE54254
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE6B254
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE6B288
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE77166
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE82D94
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE82E84
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE8A4A8
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE8A4DC
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE921C4
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE922B4
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE996B0
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAE996F0
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEA08A4
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEA0994
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEA7D14
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEA7D48
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEAF59C
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEAF5DC
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEB6824
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEB6914
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEC5D24
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEC5D58
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAECD594
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAECD684
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAED4704
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAED47F4
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEDB8D4
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEDB9C4
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEE2FF4
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEE3028
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEEAF14
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEEAF54
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEF3194
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029ECAEF3284
[-] Heap does not have exactly 3 occurrences, found 41 occurrences. No changes made.
```

The successfull part of the tool happens here:
```sh
..
[+] Found TARGET_VALUE 0x41A00000 at address: 0000029E59129A0C
        [+] Overwritten with NEW_VALUE 0x43FA0000 (500.0f) at address: 0000029E591299A4
        [+] Overwritten with NEW_VALUE 0x43FA0000 (500.0f) at address: 0000029E591299AC
        [+] Overwritten with NEW_VALUE 0x43FA0000 (500.0f) at address: 0000029E59129A0C
[+] Heap 1: Base Address: 0000029E5B8A0000, Region Size: 1044480 bytes
..
```
