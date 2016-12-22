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
#include <fluent/fluentcontainer.h>
#include <fluent/fluentcontainer.vector.h>
#include <fluent/fluentcontainer.map.h>

static int identity(int value)
{
    return value;
}

static int square(int value)
{
    return value * value;
}

static int sum(int first, int second)
{
    return first + second;
}

TEST(Converter, VectorToMap)
{
    std::vector<int> values {1, 2, 3, 4, 5};
    auto result {fluent::from(std::move(values)).as<std::map<int, int>>().map(identity, square)};
    std::map<int, int> reference { {1, 1}, {2, 4}, {3, 9}, {4, 16}, {5, 25} };
    EXPECT_EQ(*result, reference);
}

TEST(Converter, MapToVector)
{
    std::map<int, int> values { {1, 1}, {2, 4}, {3, 9}, {4, 16}, {5, 25} };
    auto result {fluent::from(std::move(values)).as<std::vector<int>>().map(sum)};
    std::vector<int> reference {2, 6, 12, 20, 30};
    EXPECT_EQ(*result, reference);
}

TEST(Converter, Full)
{
    int value {
        fluent::from(std::vector<int> {1, 2, 3, 4, 5})
                .as<std::map<int, int>>()
                .map(identity, square)
                .as<std::vector<int>>()
                .map(sum)
                .reduce(sum, 0)
    };
    EXPECT_EQ(value, 70);
}
