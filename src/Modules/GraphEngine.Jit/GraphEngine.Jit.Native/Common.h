#pragma once

#include <iostream>
#include <asmjit.h>
#include "TypeSystem.h"
#include "CellAccessor.h"
#include "Trinity.h"
using namespace asmjit;

#define print(x) std::wcout << x << std::endl
#define debug(x) std::wcout << #x << " = " << (x) << std::endl

class ErrHandler : public ErrorHandler
{
public:
    bool handleError(asmjit::Error err, const char* message, CodeEmitter* origin)
    {
        debug(message);
        return false;
    }
};

asmjit::TypeId::Id GetTypeId(IN TypeDescriptor* const type);

char* tsl_getstring(int32_t* trinity_string_ptr);
char* tsl_getu8string(int32_t* trinity_string_ptr);
void* tsl_copy(char* ptr, int32_t size);
void* tsl_copy_dynamic(int32_t* ptr);
void tsl_assign(CellAccessor* accessor, char* dst, char* src, int32_t size_dst, int32_t size_src);
void tsl_setstring(CellAccessor* accessor, int32_t* p, u16char* str);
void tsl_setu8string(CellAccessor* accessor, int32_t* p, char* trinity_string_ptr);
uint64_t tsl_hash(void* ptr, int32_t len);
int32_t tsl_newaccessor(CellAccessor* ptr, int32_t len, uint16_t type, uint8_t is_cell);

template<bool preserve_content>
void* tsl_resize(CellAccessor* accessor, int32_t offset, int32_t delta)
{
    char* cellPtr;

    if (accessor->malloced)
    {
        if (preserve_content)
        {
            // TODO reservation
            if (delta > 0)
            {
                cellPtr = (char*)realloc((void*)accessor->cellPtr, accessor->size + delta);
                if (cellPtr == nullptr) throw;
                memmove(
                    cellPtr + offset + delta,
                    cellPtr + offset,
                    (uint64_t)(accessor->size - offset));
            }
            else
            {
                cellPtr = (char*)accessor->cellPtr;
                memmove(
                    cellPtr + offset,
                    cellPtr + offset - delta,
                    (uint64_t)(accessor->size - offset + delta));
                cellPtr = (char*)realloc((void*)accessor->cellPtr, accessor->size + delta);
                if (cellPtr == nullptr) throw;
            }
        }
        else
        {
            cellPtr = (char*)realloc((void*)accessor->cellPtr, accessor->size + delta);
            if (cellPtr == nullptr) throw;
        }
    }
    else { ::CResizeCell(accessor->cellId, accessor->entryIndex, offset, delta, cellPtr); }

    accessor->cellPtr = reinterpret_cast<int64_t>(cellPtr);
    accessor->size += delta;

    return cellPtr + offset;
}
