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

#ifndef FLUENTCONTAINER_IMPL_ALGO_H
#define FLUENTCONTAINER_IMPL_ALGO_H

#include "fluentcontainer.impl.fwd.h"
#include <algorithm>

namespace fluent {
namespace details {

// Contains algorithm wrappers for different container types
template<class C, int ST = ContainerTraits<C>::storageType>
struct AlgorithmPolicy;

template<class C>
struct AlgorithmPolicy<C, ContiguousStorageType>
{
    template<class I, class F>
    static void transform(const I &inputContainer, C &outputContainer, F &&callable)
    {
        std::transform(inputContainer.begin(), inputContainer.end(),
                       std::back_inserter(outputContainer), callable);
    }
    template<class F>
    static void inplace_transform(C &container, F &&callable)
    {
        std::transform(container.begin(), container.end(), container.begin(), callable);
    }
    template<class I>
    static void copy(const I &inputContainer, C &outputContainer)
    {
        std::copy(inputContainer.begin(), inputContainer.end(),
                  std::back_inserter(outputContainer));
    }
    template<class I, class F>
    static void copy_if(const I &inputContainer, C &outputContainer, F &&callable)
    {
        std::copy_if(inputContainer.begin(), inputContainer.end(),
                     std::back_inserter(outputContainer), callable);
    }
    template<class F>
    static void inplace_filter(C &container, F &&callable)
    {
        using V = typename C::value_type;
        auto it = std::remove_if(container.begin(), container.end(), [&callable](const V &value) {
            return !callable(value);
        });
        container.erase(it, container.end());
    }
};

template<class C>
struct AlgorithmPolicy<C, TreeBasedStorageType>
{
    template<class I, class F>
    static void transform(const I &inputContainer, C &outputContainer, F &&callable)
    {
        std::transform(inputContainer.begin(), inputContainer.end(),
                       std::inserter(outputContainer, outputContainer.begin()), callable);
    }
    template<class I>
    static void copy(const I &inputContainer, C &outputContainer)
    {
        std::copy(inputContainer.begin(), inputContainer.end(),
                  std::inserter(outputContainer, outputContainer.begin()));
    }
    template<class I, class F>
    static void copy_if(const I &inputContainer, C &outputContainer, F &&callable)
    {
        std::copy_if(inputContainer.begin(), inputContainer.end(),
                     std::inserter(outputContainer, outputContainer.begin()), callable);
    }
};


} // namespace details
} // namespace fluent

#endif // FLUENTCONTAINER_IMPL_ALGO_H
