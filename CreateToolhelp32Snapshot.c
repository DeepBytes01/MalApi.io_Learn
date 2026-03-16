#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
int main(void) {
    /*
    CreateToolhelp32Snapshot();
    => is used to enumerate processes, 
    threads, and modules. This function is commonly used by malware 
    to enumerate processes before process injection. 
    */
    //! CreateToolhelp32Snapsoht(1, 2);
    //? param 1
    //! TH32CS_SNAPPROCESS	snapshot of all processes
    //! TH32CS_SNAPTHREAD	snapshot of all threads
    //! TH32CS_SNAPMODULE	snapshot of modules in a process
    //! TH32CS_SNAPHEAPLIST	snapshot of heaps
    //? param2
    //! 0 = enumerating all process
    HANDLE snapshot;
    PROCESSENTRY32 pe;
    //? param of PROCESSENTRY32
    //! pe.th32ProcessID
    //! pe.szExeFile
    //! pe.cntThreads
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pe.dwSize = sizeof(PROCESSENTRY32);
    if(Process32First(snapshot, &pe)) {
        do{
            printf("process: %s\n", pe.szExeFile);
        }while(Process32Next(snapshot, &pe));
    }
    CloseHandle(snapshot);        
    return 0;

}