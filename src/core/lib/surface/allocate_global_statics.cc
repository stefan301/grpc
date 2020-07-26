/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <grpc/support/port_platform.h>

#include "src/core/lib/debug/trace.h"
#include "src/core/lib/surface/allocate_global_statics.h"

static bool enable_leak_check( bool enable )
{
  int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

  if (enable)
    tmpFlag |= _CRTDBG_ALLOC_MEM_DF;
  else
    tmpFlag &= ~_CRTDBG_ALLOC_MEM_DF;

  // Set flag to the new value.
  int oldFlag = _CrtSetDbgFlag(tmpFlag);

  bool was_enabled = (oldFlag & _CRTDBG_ALLOC_MEM_DF) == _CRTDBG_ALLOC_MEM_DF;
  return was_enabled != enable;
}

// what if 2 Thread use LeakCheckDisabler?
// - the first will disable leak-check and will re-enable it (but maybe too early, while thread 2 still allocates memory) -> counter?
// what if another Thread allocates memory that is later a real leak -> will not be visible (at least not always, depending on the timing)) 

grpc_core::internal::LeakCheckDisabler::LeakCheckDisabler()
    : was_enabled(enable_leak_check(false))
{}

grpc_core::internal::LeakCheckDisabler::~LeakCheckDisabler() {
  if (was_enabled) enable_leak_check(true);
}
