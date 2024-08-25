#include <windows.h>
#include <stdio.h>

// Define global variables
DWORD TARGET_VALUE = 0x41A00000;  // The DWORD value we are searching for
DWORD NEW_VALUE = 0x43FA0000;     // The DWORD value representing 500.0f
SIZE_T TARGET_REGION_SIZE = 1044480;  // The size of the memory region we are interested in

int SearchAndReplaceValue(HANDLE hProcess, PVOID baseAddress, SIZE_T regionSize) {
    BYTE *buffer = (BYTE*)malloc(regionSize);  // Allocate buffer dynamically
    SIZE_T bytesRead;
    int occurrenceCount = 0;

    // Ensure baseAddress is not null
    if (baseAddress == NULL || regionSize == 0) {
        printf("[-] Invalid base address or region size.\n");
        free(buffer);
        return 0;
    }

    // Read the memory region into a buffer
    if (ReadProcessMemory(hProcess, baseAddress, buffer, regionSize, &bytesRead)) {
        // Search for the target value in the memory dump
        for (SIZE_T i = 0; i <= bytesRead - sizeof(DWORD); i++) {
            DWORD value = *(DWORD*)&buffer[i];
            if (value == TARGET_VALUE) {
                occurrenceCount++;
                PVOID targetAddress = (PVOID)((SIZE_T)baseAddress + i);
                printf("[+] Found TARGET_VALUE 0x%08X at address: %p\n", TARGET_VALUE, targetAddress);
            }
        }

        // If exactly 3 occurrences are found, replace them with the new value
        if (occurrenceCount == 3) {
            for (SIZE_T i = 0; i <= bytesRead - sizeof(DWORD); i++) {
                DWORD value = *(DWORD*)&buffer[i];
                if (value == TARGET_VALUE) {
                    PVOID targetAddress = (PVOID)((SIZE_T)baseAddress + i);
                    if (WriteProcessMemory(hProcess, targetAddress, &NEW_VALUE, sizeof(DWORD), NULL)) {
                        printf("\t[+] Overwritten with NEW_VALUE 0x%08X (500.0f) at address: %p\n", NEW_VALUE, targetAddress);
                    } else {
                        printf("\t[-] Failed to overwrite value at address: %p\n", targetAddress);
                    }
                }
            }
        } else {
            printf("[-] Heap does not have exactly 3 occurrences, found %d occurrences. No changes made.\n", occurrenceCount);
        }
    } else {
        printf("[-] Failed to read memory at base address: %p\n", baseAddress);
    }

    free(buffer);  // Free the allocated buffer
    return occurrenceCount;  // Return the count of occurrences found
}

void ListHeaps(HANDLE hProcess) {
    SYSTEM_INFO sysInfo;
    MEMORY_BASIC_INFORMATION memInfo;
    PVOID address = NULL;
    int heapCount = 0;

    // Get system information
    GetSystemInfo(&sysInfo);

    // Walk through the entire memory range of the process
    while (address < sysInfo.lpMaximumApplicationAddress) {
        // Query the memory information
        if (VirtualQueryEx(hProcess, address, &memInfo, sizeof(memInfo)) == sizeof(memInfo)) {
            // Filter only memory regions that are private, committed, read/write, and of size 1044480 bytes
            if (memInfo.State == MEM_COMMIT && memInfo.Type == MEM_PRIVATE && memInfo.Protect == PAGE_READWRITE && memInfo.RegionSize == TARGET_REGION_SIZE) {
                printf("[+] Heap %d: Base Address: %p, Region Size: %zu bytes\n", heapCount, memInfo.BaseAddress, memInfo.RegionSize);

                // Search and replace the target value in this memory region
                SearchAndReplaceValue(hProcess, memInfo.BaseAddress, memInfo.RegionSize);

                heapCount++;
            }
        }

        // Move to the next region
        address = (PVOID)((SIZE_T)memInfo.BaseAddress + memInfo.RegionSize);

        // Ensure we do not loop back to 0x00000000
        if (address == NULL) {
            break;
        }
    }
}

BOOL EnableDebugPrivilege() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID luid;

    // Open the current process token with necessary access rights
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        printf("[-] OpenProcessToken failed. Error code: %lu\n", GetLastError());
        return FALSE;
    }

    // Retrieve the LUID for SeDebugPrivilege
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
        printf("[-] LookupPrivilegeValue failed. Error code: %lu\n", GetLastError());
        CloseHandle(hToken);
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // Enable the privilege
    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
        printf("[-] AdjustTokenPrivileges failed. Error code: %lu\n", GetLastError());
        CloseHandle(hToken);
        return FALSE;
    }

    // Check if the adjustment succeeded
    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
        printf("[-] The token does not have the specified privilege.\n");
        CloseHandle(hToken);
        return FALSE;
    }

    printf("[+] SeDebugPrivilege successfully enabled.\n");
    CloseHandle(hToken);
    return TRUE;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <PID>\n", argv[0]);
        return 1;
    }

    // Enable the SE_DEBUG_NAME privilege
    EnableDebugPrivilege();

    // Get the process ID from command line arguments
    DWORD pid = (DWORD)atoi(argv[1]);

    // Open the process with sufficient rights
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (hProcess == NULL) {
        // Retrieve and print the last error code
        DWORD error = GetLastError();
        printf("[-] Failed to open process with PID %d. Error code: %lu\n", pid, error);
        return 1;
    } else {
        printf("[+] Opened process with PID %d.\n", pid);
    }

    // List the heaps and search for the target value in all qualifying heap regions
    ListHeaps(hProcess);

    // Close the process handle
    CloseHandle(hProcess);
    return 0;
}
