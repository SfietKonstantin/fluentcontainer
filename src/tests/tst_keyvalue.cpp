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
#include <fluent/fluentcontainer.map.h>
#include "testdata.h"

using namespace testing;

class KeyValueTest: public Test
{
public:
    explicit KeyValueTest()
    {
        fill(mapValues);
    }
protected:
    static void fill(std::map<int, TestData> &container)
    {
        container.emplace(1, TestData {1, "Value 1"});
        container.emplace(2, TestData {2, "Value 2"});
        container.emplace(3, TestData {3, "Value 3"});
        container.emplace(4, TestData {4, "Value 4"});
        container.emplace(5, TestData {5, "Value 5"});
    }
    static void verify(const std::map<int, TestData> &values)
    {
        std::map<int, TestData> reference {};
        fill(reference);
        EXPECT_EQ(values, reference);
    }
    static void verifyMapped(const std::map<int, TestData> &values)
    {
        std::map<int, TestData> reference {};
        reference.emplace(2, TestData {1, "Value 1"});
        reference.emplace(4, TestData {0, "Value 2"});
        reference.emplace(6, TestData {1, "Value 3"});
        reference.emplace(8, TestData {0, "Value 4"});
        reference.emplace(10, TestData {1, "Value 5"});
        EXPECT_EQ(values, reference);
    }
    static int mapDataKey(int, const TestData &item)
    {
        return item.key() * 2;
    }
    static TestData mapDataValue(int, const TestData &item)
    {
        return TestData {item.key() % 2, item.value()};
    }
    static std::string reduceData(const std::string &input, int, const TestData &item)
    {
        std::ostringstream ss;
        ss << input << item.key() << item.value();
        return ss.str();
    }
    std::map<int, TestData> mapValues {};
};

TEST_F(KeyValueTest, Constructor)
{
    auto result {fluent::from(std::move(mapValues))};
    verify(*result);
}

TEST_F(KeyValueTest, As)
{
    auto result {fluent::from(std::move(mapValues)).as<std::map<int, TestData>>()};
    verify(*result);
}


TEST_F(KeyValueTest, Map)
{
    auto result {fluent::from(std::move(mapValues)).map(mapDataKey, mapDataValue)};
    verifyMapped(*result);
}

TEST_F(KeyValueTest, Reduce)
{
    std::string result {fluent::from(std::move(mapValues)).reduce(reduceData, std::string {})};
    EXPECT_EQ(result, "1Value 12Value 23Value 34Value 45Value 5");
}

/*
TEST_F(KeyValueTest, Filter)
{
    // implem of filter with set requires copy_if, so we cannot do it with non-copiable classes
    std::set<int> values {1, 2, 3, 4, 5};
    auto result {fluent::fromValue(values).filter([](int item) { return item % 2 == 0;})};
    std::set<int> reference {2, 4};
    EXPECT_EQ(*result, reference);
}

TEST_F(KeyValueTest, AsWithCopy1)
{
    // implem of as to different classes requires copy, so we cannot do it with non-copiable classes
    std::set<int> values {1, 2, 3, 4, 5};
    auto result {fluent::fromValue(values).as<std::vector<int>>()};
    std::vector<int> reference {1, 2, 3, 4, 5};
    EXPECT_EQ(*result, reference);
}

TEST_F(KeyValueTest, AsWithCopy2)
{
    std::vector<int> values {4, 3, 5, 2, 1};
    auto result {fluent::fromValue(values).as<std::set<int>>()};
    std::set<int> reference {1, 2, 3, 4, 5};
    EXPECT_EQ(*result, reference);
}

*/
