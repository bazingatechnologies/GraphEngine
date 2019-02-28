// Graph Engine
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//
#pragma once
#if (defined(__darwin__) || ((defined(__APPLE__) && defined(__MACH__))))
#define TRINITY_PLATFORM_DARWIN
#include "posix.h"
#endif
