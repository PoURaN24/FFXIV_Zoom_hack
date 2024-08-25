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
