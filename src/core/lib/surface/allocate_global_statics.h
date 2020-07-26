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

#ifndef GRPC_CORE_LIB_SURFACE_ALLOCATE_GLOBAL_STATICS_H
#define GRPC_CORE_LIB_SURFACE_ALLOCATE_GLOBAL_STATICS_H

#include <grpc/support/port_platform.h>

namespace grpc_core {

namespace internal {
class LeakCheckDisabler {
 public:
  LeakCheckDisabler();
  LeakCheckDisabler(const LeakCheckDisabler&) = delete;
  LeakCheckDisabler& operator=(const LeakCheckDisabler&) = delete;
  ~LeakCheckDisabler();

 private:
  bool was_enabled;
};

}  // namespace internal

inline void* malloc_global_static(size_t size) {
  grpc_core::internal::LeakCheckDisabler d;
  return malloc(size);
}

template <typename T>
T* new_global_static() {
  grpc_core::internal::LeakCheckDisabler d;
  return new T();
}

}  // namespace grpc_core

#endif /* GRPC_CORE_LIB_SURFACE_ALLOCATE_GLOBAL_STATICS_H */
