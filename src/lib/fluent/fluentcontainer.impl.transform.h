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

#ifndef FLUENTCONTAINER_IMPL_MAP_H
#define FLUENTCONTAINER_IMPL_MAP_H

#include "fluentcontainer.impl.fwd.h"
#include "fluentcontainer.impl.algo.h"

namespace fluent {
namespace details {

template<class I, class O,
         int IT = ContainerTraits<I>::containerType,
         int OT = ContainerTraits<O>::containerType,
         int TPT = ContainerAlgorithmTraits<O>::transformPolicyType>
struct TransformPolicy;

template<class I, class O>
struct TransformPolicy<I, O, ValueContainerType, ValueContainerType, DefaultTransformPolicyType>
{
    template<class F>
    static O transform(const I &container, F &&callable)
    {
        O returned {};
        AlgorithmPolicy<O>::transform(container, returned, callable);
        return returned;
    }
};

template<class I, class O>
struct TransformPolicy<I, O, ValueContainerType, KeyValueContainerType, DefaultTransformPolicyType>
{
    template<class KF, class VF>
    static O transform(const I &container, KF &&keyCallable, VF &&valueCallable)
    {
        O returned {};
        using V = typename I::value_type;
        AlgorithmPolicy<O>::transform(container, returned, [&keyCallable, &valueCallable](const V &value) {
            using R = typename O::value_type;
            return R {keyCallable(value), valueCallable(value)};
        });
        return returned;
    }
};

template<class I, class O>
struct TransformPolicy<I, O, KeyValueContainerType, KeyValueContainerType, DefaultTransformPolicyType>
{
    template<class KF, class VF>
    static O transform(const I &container, KF &&keyCallable, VF &&valueCallable)
    {
        O returned {};

        using V = typename I::value_type;
        AlgorithmPolicy<O>::transform(container, returned, [&keyCallable, &valueCallable](const V &value) {
            using R = typename O::value_type;
            return R {keyCallable(value.first, value.second), valueCallable(value.first, value.second)};
        });
        return returned;
    }
};

template<class I, class O, int TPT>
struct TransformPolicy<I, O, KeyValueContainerType, ValueContainerType, TPT>
{
    template<class F>
    static O transform(I &container, F &&callable)
    {
        O returned {};

        using V = typename I::value_type;
        AlgorithmPolicy<O>::transform(container, returned, [&callable](const V &value) {
            return callable(value.first, value.second);
        });
        return returned;
    }
};

template<class C>
struct TransformPolicy<C, C, ValueContainerType, ValueContainerType, InPlaceTransformPolicyType>
{
    template<class F>
    static C & transform(C &container, F &&callable)
    {
        AlgorithmPolicy<C>::inplace_transform(container, callable);
        return container;
    }
};

} // namespace details
} // namespace fluent

#endif // FLUENTCONTAINER_IMPL_MAP_H
