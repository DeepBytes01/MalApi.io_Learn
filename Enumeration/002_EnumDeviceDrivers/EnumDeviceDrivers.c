#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#define ARRAY_SIZE 1024

int main(void) {
    LPVOID drivers[ARRAY_SIZE];
    DWORD cbNeeded;
    /*
    | Parameter     | Meaning                                     |
    | ------------- | ------------------------------------------- |
    | `lpImageBase` | Array that will store driver base addresses |
    | `cb`          | Size of the array                           |
    | `lpcbNeeded`  | How many bytes were needed                  |
    */
   //! EnumDeviceDrivers() = returns base address so we need GetDeviceDriverBaseNameA()
   //? = GetDeviceDriverBaseNameA() takes a driver base address and converts it into the driver name.
    /*//! Prototype
    DWORD GetDeviceDriverBaseNameA(
        LPVOID ImageBase, //! Address from EnumDeviceDrivers() | index also
        LPSTR  lpBaseName, //! Buffer to store the name
        DWORD  nSize //! size of the buffer lpBaseName
    );
   */
    if(EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded)) {
        int count = cbNeeded/sizeof(drivers[0]);
        printf("Loaded drivers: %d\n", count);
        for(int i = 0; i < count; i++) {
            char driverName[ARRAY_SIZE];
            if(GetDeviceDriverBaseNameA(drivers[i], driverName, sizeof(driverName))){
                    printf("%d: %s\n", i, driverName);
                    if(strcmp(driverName, "WdNisDrv.sys") == 0 || strcmp(driverName, "WdFilter.sys") == 0 || strcmp(driverName, "avguard.sys") == 0 || strcmp(driverName, "crowdstrike.sys") == 0 || strcmp(driverName, "edr.sys") == 0 || strcmp(driverName, "sentinel.sys") == 0){
                        printf("!!!!!!!Active: %s\n", driverName);
                    }
            }
        }
    }else{
        printf("EnumDeviceDrivers failed: %lu\n", GetLastError());
    }    
    return 0;
}
// compile: x86_64-w64-mingw32-gcc enum_drivers.c -o enum_drivers.exe -lpsapi