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

#ifndef FLUENTCONTAINER_IMPL_AS_H
#define FLUENTCONTAINER_IMPL_AS_H

#include "fluentcontainer.impl.fwd.h"
#include "fluentcontainer.impl.copy.h"

namespace fluent {
namespace details {

template<class I, class O,
         int IT = ContainerTraits<I>::containerType,
         int OT = ContainerTraits<O>::containerType>
struct AsPolicy;

template<class I, class O>
struct AsPolicy<I, O, ValueContainerType, ValueContainerType>
{
    using ReturnType = FluentContainer<O>;
    static ReturnType as(I &container)
    {
        return FluentContainer<O>(details::CopyPolicy<I, O>::copy(container));
    }
};

template<class I, class O>
struct AsPolicy<I, O, KeyValueContainerType, KeyValueContainerType>
{
    using ReturnType = FluentContainer<O>;
    static ReturnType as(I &container)
    {
        return ReturnType(details::CopyPolicy<I, O>::copy(container));
    }
};

template<class I, class O>
struct AsPolicy<I, O, ValueContainerType, KeyValueContainerType>
{
    using ReturnType = FluentToKeyValueConvertor<I, O>;
    static ReturnType as(I &container)
    {
        return ReturnType(std::move(container));
    }
};

template<class I, class O>
struct AsPolicy<I, O, KeyValueContainerType, ValueContainerType>
{
    using ReturnType = FluentToValueConvertor<I, O>;
    static ReturnType as(I &container)
    {
        return ReturnType(std::move(container));
    }
};


} // namespace details
} // namespace fluent

#endif // FLUENTCONTAINER_IMPL_AS_H
