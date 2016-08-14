#include <windows.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

size_t cmpxchg16b_size = 41;
char cmpxchg16b_r8[]  = { 0x41, 0x52, 0x41, 0x53, 0x4D, 0x8B, 0x10, 0x4D, 0x8B, 0x58, 0x08, 0x49, 0x39, 0xC2, 0x75, 0x0E, 0x49, 0x39, 0xD3, 0x75, 0x09, 0x49, 0x89, 0x18, 0x49, 0x89, 0x48, 0x08, 0xEB, 0x06, 0x4C, 0x89, 0xD0, 0x4C, 0x89, 0xDA, 0x41, 0x5B, 0x41, 0x5A, 0xC3 };
char cmpxchg16b_r9[]  = { 0x41, 0x52, 0x41, 0x53, 0x4D, 0x8B, 0x11, 0x4D, 0x8B, 0x59, 0x08, 0x49, 0x39, 0xC2, 0x75, 0x0E, 0x49, 0x39, 0xD3, 0x75, 0x09, 0x49, 0x89, 0x19, 0x49, 0x89, 0x49, 0x08, 0xEB, 0x06, 0x4C, 0x89, 0xD0, 0x4C, 0x89, 0xDA, 0x41, 0x5B, 0x41, 0x5A, 0xC3 };
char cmpxchg16b_r11[] = { 0x41, 0x50, 0x41, 0x51, 0x4D, 0x8B, 0x03, 0x4D, 0x8B, 0x4B, 0x08, 0x49, 0x39, 0xC0, 0x75, 0x0E, 0x49, 0x39, 0xD1, 0x75, 0x09, 0x49, 0x89, 0x1B, 0x49, 0x89, 0x4B, 0x08, 0xEB, 0x06, 0x4C, 0x89, 0xC0, 0x4C, 0x89, 0xCA, 0x41, 0x59, 0x41, 0x58, 0xC3 };

char* roundps_xmm3_xmm0_09;

struct CaveInfo {
    char*  payload;
    DWORD size;
    HANDLE hFile;
    HANDLE hMapping;
};

struct ProgramInfo {
    char* programData;
    HANDLE hFile;
    HANDLE hMapping;
};

struct ProgramInfo load_program(const char* file) {
    struct ProgramInfo programInfo;
    programInfo.hFile = CreateFile(file, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    assert(programInfo.hFile != INVALID_HANDLE_VALUE);
    programInfo.hMapping = CreateFileMapping(programInfo.hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
    assert(programInfo.hMapping);
    programInfo.programData = MapViewOfFile(programInfo.hMapping, FILE_MAP_WRITE, 0,0,0);
    assert(programInfo.programData);
    return programInfo;
}

void unload_program(struct ProgramInfo* programInfo) {
    UnmapViewOfFile(programInfo->programData);
    CloseHandle(programInfo->hMapping);
    CloseHandle(programInfo->hFile);
    programInfo->programData = NULL;
    programInfo->hMapping = NULL;
    programInfo->hFile = NULL;
}

struct CaveInfo load_cave(const char* file) {
    struct CaveInfo caveInfo;
    caveInfo.hFile = CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    assert(caveInfo.hFile != INVALID_HANDLE_VALUE);
    LARGE_INTEGER size;
    GetFileSizeEx(caveInfo.hFile, &size);
    assert(size.HighPart == 0);
    caveInfo.size = size.LowPart;
    caveInfo.hMapping = CreateFileMapping(caveInfo.hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    assert(caveInfo.hMapping);
    caveInfo.payload = MapViewOfFile(caveInfo.hMapping, FILE_MAP_READ, 0,0,0);
    assert(caveInfo.payload);
    return caveInfo;
}

void install_cave(const struct ProgramInfo* program, const struct CaveInfo* cave, size_t cave_offset) {
    memcpy(program->programData + cave_offset, cave->payload, cave->size);
}

void install_call(const struct ProgramInfo* program, size_t target_offset, size_t cave_offset) {
    int return_offset = target_offset + 5;
    int jump_offset = cave_offset - return_offset;
    char call_data[5] = {0xE8, 0x00, 0x00, 0x00, 0x00};
    *((int*)(call_data + 1)) = jump_offset;
    memcpy(program->programData + target_offset, call_data, 5);
}

int main(int argc, char const *argv[])
{
    struct ProgramInfo programInfo = load_program(argv[1]);
    struct CaveInfo caveInfo = load_cave("./roundps_xmm3_xmm0_09.bin");
    //install_cave(&programInfo, &caveInfo, 0xe8706f);

    install_call(&programInfo, 0x4B6, 0x550);
    //install_cave(0x2E6DDC, cmpxchg16b_r8, cmpxchg16b_size);
    // install_cave(0x2E704A, cmpxchg16b_r9, cmpxchg16b_size);
    // install_cave(0x2e7126, cmpxchg16b_r11, cmpxchg16b_size);

    // // r8
    // install_call(0x2e7247, 0x2E6DDC);
    // install_call(0x2e7658, 0x2E6DDC);

    // // r9
    // install_call(0x2e7287, 0x2E704A);
    // install_call(0x2e72cf, 0x2E704A);
    // install_call(0x2e731e, 0x2E704A);
    // install_call(0x2e743b, 0x2E704A);
    // install_call(0x2e7605, 0x2E704A);
  
    // // r11
    // install_call(0x2e75af, 0x2e7126);

    //unload_program(&programInfo);
    return 0;
}
