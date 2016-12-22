/*
 * Copyright (C) 2016 Lucien XU <sfietkonstantin@free.fr>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * The names of its contributors may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#ifndef FLUENTCONTAINER_IMPL_FILTER
#define FLUENTCONTAINER_IMPL_FILTER

#include "fluentcontainer.impl.algo.h"

namespace fluent {
namespace details {

template<class C, int CT = ContainerTraits<C>::containerType,
         int FPT = ContainerAlgorithmTraits<C>::filterPolicyType>
struct FilterPolicy;

template<class C>
struct FilterPolicy<C, ValueContainerType, DefaultFilterPolicyType>
{
    template<class F>
    static C filter(const C &container, F callable)
    {
        C returned {};
        AlgorithmPolicy<C>::copy_if(container, returned, callable);
        return returned;
    }
};

template<class C>
struct FilterPolicy<C, ValueContainerType, InPlaceFilterPolicyType>
{
    template<class F>
    static C & filter(C &container, F callable)
    {
        AlgorithmPolicy<C>::inplace_filter(container, callable);
        return container;
    }
};

template<class C>
struct FilterPolicy<C, KeyValueContainerType, DefaultFilterPolicyType>
{
    template<class F>
    static C filter(const C &container, F callable)
    {
        C returned {};

        using V = typename C::value_type;
        AlgorithmPolicy<C>::copy_if(container, returned, [&callable](const V &value) {
            return callable(value.first, value.second);
        });
        return returned;
    }
};

} // namespace details
} // namespace fluent

#endif // FLUENTCONTAINER_IMPL_FILTER
