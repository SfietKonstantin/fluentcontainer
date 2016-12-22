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

#include <gtest/gtest.h>
#include <deque>
#include <sstream>
#include <fluent/fluentcontainer.h>
#include <fluent/fluentcontainer.vector.h>
#include "testdata.h"

// Dummy implementation for deque
namespace fluent {
namespace details {

template<class ...T>
struct ContainerTraits<std::deque<T...>>
{
    static const ContainerType containerType = ValueContainerType;
    static const ContainerStorageType storageType = ContiguousStorageType;
};

} // namespace details
} // namespace fluent

using namespace testing;

class ContiguousTest: public Test
{
public:
    explicit ContiguousTest()
    {
        fill(vectorValues);
        fill(dequeValues);
    }
protected:
    template<class Container>
    static void fill(Container &container)
    {
        container.emplace_back(1, "Value 1");
        container.emplace_back(2, "Value 2");
        container.emplace_back(3, "Value 3");
        container.emplace_back(4, "Value 4");
        container.emplace_back(5, "Value 5");
    }
    template<class Container>
    static void verify(const Container &values)
    {
        Container reference {};
        fill(reference);
        EXPECT_EQ(values, reference);
    }
    template<class Container>
    static void verifyFiltered(const Container &values)
    {
        Container reference {};
        reference.emplace_back(2, "Value 2");
        reference.emplace_back(4, "Value 4");
        EXPECT_EQ(values, reference);
    }
    template<class Container>
    static void verifyMapped(const Container &values)
    {
        Container reference {};
        reference.emplace_back(2, "Value 1");
        reference.emplace_back(4, "Value 2");
        reference.emplace_back(6, "Value 3");
        reference.emplace_back(8, "Value 4");
        reference.emplace_back(10, "Value 5");
        EXPECT_EQ(values, reference);
    }
    static TestData mapData(const TestData &item)
    {
        return TestData {item.key() * 2, item.value()};
    }
    static std::string reduceData(const std::string &input, const TestData &item)
    {
        std::ostringstream ss;
        ss << input << item.key() << item.value();
        return ss.str();
    }
    static bool filterData(const TestData &item)
    {
        return item.key() % 2 == 0;
    }
    TestData::Vector vectorValues {};
    std::deque<TestData> dequeValues {};
};

TEST_F(ContiguousTest, VectorConstructor)
{
    auto result {fluent::from(std::move(vectorValues))};
    verify(*result);
}

TEST_F(ContiguousTest, VectorAs)
{
    auto result {fluent::from(std::move(vectorValues)).as<TestData::Vector>()};
    verify(*result);
}

TEST_F(ContiguousTest, VectorMap)
{
    auto result {fluent::from(std::move(vectorValues)).map(mapData)};
    verifyMapped(*result);
}

TEST_F(ContiguousTest, VectorReduce)
{
    std::string result {fluent::from(std::move(vectorValues)).reduce(reduceData, std::string {})};
    EXPECT_EQ(result, "1Value 12Value 23Value 34Value 45Value 5");
}

TEST_F(ContiguousTest, VectorFilter)
{
    auto result {fluent::from(std::move(vectorValues)).filter(filterData)};
    verifyFiltered(*result);
}

TEST_F(ContiguousTest, DequeConstructor)
{
    auto result {fluent::from(std::move(dequeValues))};
    verify(*result);
}

TEST_F(ContiguousTest, DequeAs)
{
    auto result {fluent::from(std::move(dequeValues)).as<std::deque<TestData>>()};
    verify(*result);
}

TEST_F(ContiguousTest, DequeMap)
{
    auto result {fluent::from(std::move(dequeValues)).map(mapData)};
    verifyMapped(*result);
}

TEST_F(ContiguousTest, DequeReduce)
{
    std::string result {fluent::from(std::move(dequeValues)).reduce(reduceData, std::string {})};
    EXPECT_EQ(result, "1Value 12Value 23Value 34Value 45Value 5");
}

TEST_F(ContiguousTest, DequeFilter)
{
    // non-inplace implem of filter requires copy_if, so we cannot do it with non-copiable classes
    std::deque<int> values {1, 2, 3, 4, 5};
    auto result {fluent::fromValue(values).filter([](int item) { return item % 2 == 0;})};
    std::deque<int> reference {2, 4};
    EXPECT_EQ(*result, reference);
}

TEST_F(ContiguousTest, AsWithCopy1)
{
    // implem of as to different classes requires copy, so we cannot do it with non-copiable classes
    std::deque<int> values {1, 2, 3, 4, 5};
    auto result {fluent::fromValue(values).as<std::vector<int>>()};
    std::vector<int> reference {1, 2, 3, 4, 5};
    EXPECT_EQ(*result, reference);
}

TEST_F(ContiguousTest, AsWithCopy2)
{
    std::vector<int> values {1, 2, 3, 4, 5};
    auto result {fluent::fromValue(values).as<std::deque<int>>()};
    std::deque<int> reference {1, 2, 3, 4, 5};
    EXPECT_EQ(*result, reference);
}
