#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif

int main()
{
    SYSTEM_INFO sInfo{};
    GetSystemInfo(&sInfo);

    std::cout << std::hex << sInfo.lpMinimumApplicationAddress << std::endl;
    std::cout<< sInfo.lpMaximumApplicationAddress << std::endl;
    printf("Page Size: %d\n", sInfo.dwPageSize);
    printf("Allocation Granularity: %d\n", sInfo.dwAllocationGranularity);

    int addr = 0;
    bool flag = false;

    while (!flag)
    {
        std::cout << "What address whould you like? ";
        std::cin >> std::hex >> addr;
        std::cout << addr << std::endl;

        MEMORY_BASIC_INFORMATION sBinfo = {};
        VirtualQuery((void*)addr, &sBinfo, sizeof(sBinfo));
        flag = bool(sBinfo.State & MEM_FREE);
        if (!flag)
        {
            std::cout << "You'll never get this!!!\n";
        }
    }

    auto v = reinterpret_cast<int8_t*>(VirtualAlloc(
        (LPVOID)addr, 65536, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));

    if (!v)
    {
        DWORD error = GetLastError();
        std::cerr << "VirtualAlloc failed! Error: " << error << std::endl;

        switch (error)
        {
        case ERROR_INVALID_PARAMETER:
            std::cerr << "Error: Invalid parameter. Check alignment or size." << std::endl;
            break;
        case ERROR_INVALID_ADDRESS:
            std::cerr << "Error: Invalid address. The specified address is unavailable or invalid." << std::endl;
             break;
        case ERROR_NOT_ENOUGH_MEMORY:
            std::cerr << "Error: Not enough memory to complete the allocation." << std::endl;
             break;
        default:
            std::cout << "Error: not defined an error code!!!" << std::endl;
        }
        return -1;
    }

    *v = 0x66;
    std::cout << "Memory successfully allocated and set: " << std::hex << static_cast<int>(*v) << std::endl;

    std::cin.get();
    return 0;
}
