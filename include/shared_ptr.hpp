#pragma once

#define SYNC_NONE 1
#define SYNC_ATOMICS 2
#define SYNC_CRITICAL_SECTIONS 3

#if (SYNC_METHOD == SYNC_NONE)
#pragma message("SYNC_METHOD == SYNC_NONE")
#include "shared_ptr_sync_none.hpp"
using namespace sync_none;
#elif (SYNC_METHOD == SYNC_ATOMICS)
#pragma message("SYNC_METHOD == SYNC_ATOMICS")
#include "shared_ptr_sync_atomics.hpp"
using namespace sync_atomics;
#elif (SYNC_METHOD == SYNC_CRITICAL_SECTIONS)
#pragma message("SYNC_METHOD == SYNC_CRITICAL_SECTIONS")
#include "shared_ptr_sync_critical_sections.hpp"
using namespace sync_critical_sections;
#else
#error "SYNC_METHOD is non of [SYNC_NONE, SYNC_ATOMICS, SYNC_CRITICAL_SECTIONS]"
#endif