#pragma once

#include "../io/debug.h"

#include <cstddef>
#include <cstring>

template<size_t INITIAL_BYTES>
class ArenaAllocator {
    public:
        ArenaAllocator() {
            std::memset(bytes, 0, INITIAL_BYTES);
        }
        ~ArenaAllocator() {
            if (next != nullptr) {

                delete next;

            }
        }
        void *Alloc(size_t size) {
            if (size > INITIAL_BYTES) {

                Debug::Log(Debug::LogLevel::ERROR, "Cannot allocate %i bytes because maximum configured arena size is %s.", size, INITIAL_BYTES);
                return nullptr;

            }

            const size_t available = INITIAL_BYTES - ptr;

            if (available < size) {

                if (next == nullptr) {
                    next = new ArenaAllocator<INITIAL_BYTES>();
                }
                return next->Alloc(size);
            }

            void *p = (void*)(&bytes[ptr]);
            ptr += size;
            return p;
        }
        template<typename T>
        T *Alloc() {
            void *p = Alloc(sizeof(T));
            return (T*)p;
        }
        void Free() {
            if (next != nullptr) {
                delete next;
            }
            std::memset(bytes, 0, INITIAL_BYTES);
            ptr = 0;
        }
        template<typename T>
        T *AllocArray(size_t count) {
            void *p = Alloc(sizeof(T)*count);
            return (T*)p;
        }
    private:
        char bytes[INITIAL_BYTES];
        ArenaAllocator<INITIAL_BYTES> *next = nullptr;
        size_t ptr = 0;
};