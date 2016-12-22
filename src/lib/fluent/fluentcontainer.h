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

#ifndef FLUENTCONTAINER_H
#define FLUENTCONTAINER_H

#include "fluentcontainer.impl.as.h"
#include "fluentcontainer.impl.transform.h"
#include "fluentcontainer.impl.filter.h"
#include "fluentcontainer.impl.reduce.h"

namespace fluent {

template<class I, class O>
class FluentToKeyValueConvertor
{
public:
    explicit FluentToKeyValueConvertor(I &&container)
        : m_container {std::move(container)}
    {
    }
    template<class KF, class VF>
    FluentContainer<O> map(KF &&keyCallable, VF &&valueCallable)
    {
        return FluentContainer<O>(std::move(details::TransformPolicy<I, O>::transform(m_container, keyCallable, valueCallable)));
    }
private:
    I m_container;
};

template<class I, class O>
class FluentToValueConvertor
{
public:
    explicit FluentToValueConvertor(I &&container)
        : m_container {std::move(container)}
    {
    }
    template<class F>
    FluentContainer<O> map(F &&callable)
    {
        return FluentContainer<O>(std::move(details::TransformPolicy<I, O>::transform(m_container, callable)));
    }
private:
    I m_container;
};

template<class C>
class FluentContainer
{
public:
    explicit FluentContainer(C &&container)
        : m_container {std::move(container)}
    {
    }
    FluentContainer(const FluentContainer &) = delete;
    FluentContainer(FluentContainer &&) = default;
    FluentContainer & operator=(const FluentContainer &) = delete;
    FluentContainer & operator=(FluentContainer &&) = default;
    template<class O>
    typename details::AsPolicy<C, O>::ReturnType as()
    {
        return details::AsPolicy<C, O>::as(m_container);
    }
    template<class F>
    FluentContainer<C> map(F &&callable)
    {
        static_assert(details::ContainerTraits<C>::containerType == details::ValueContainerType, "You can only map values on ValueContainerType");
        return FluentContainer<C>(std::move(details::TransformPolicy<C, C>::transform(m_container, callable)));
    }
    template<class KF, class VF>
    FluentContainer<C> map(KF &&keyCallable, VF &&valueCallable)
    {
        static_assert(details::ContainerTraits<C>::containerType == details::KeyValueContainerType, "You can only map keys and values on KeyValueContainerType");
        return FluentContainer<C>(std::move(details::TransformPolicy<C, C>::transform(m_container, keyCallable, valueCallable)));
    }
    template<class F, class R>
    R reduce(F &&callable, R initialValue)
    {
        return details::ReducePolicy<C>::reduce(m_container, callable, initialValue);
    }
    template<class F>
    FluentContainer<C> filter(F &&callable)
    {
        return FluentContainer<C>(std::move(details::FilterPolicy<C>::filter(m_container, callable)));
    }
    const C & get() const noexcept
    {
        return m_container;
    }
    const C & operator*() const noexcept
    {
        return m_container;
    }
    const C * operator->() const noexcept
    {
        return &m_container;
    }
private:
    C m_container;
};


template<class C>
static FluentContainer<C> from(C &&container)
{
    return FluentContainer<C>(std::move(container));
}

template<class C>
static FluentContainer<C> fromValue(const C &container)
{
    return FluentContainer<C>(C(container));
}

} // namespace fluent

#endif // FLUENTCONTAINER_H
