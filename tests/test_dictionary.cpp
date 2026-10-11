#include <gtest/gtest.h>
#include "structures/HashTableDictionary.hpp"
#include <stdexcept>

TEST(Dictionary, EmptyHasZeroCount) {
    HashTableDictionary<uint32_t, int> d;
    EXPECT_EQ(d.GetCount(), 0);
}

TEST(Dictionary, CapacityIsPreserved) {
    HashTableDictionary<uint32_t, int> d(25);
    EXPECT_EQ(d.GetCapacity(), 25);
}

TEST(Dictionary, AddIncreasesCount) {
    HashTableDictionary<uint32_t, int> d;
    d.Add(1, 100);
    d.Add(2, 200);
    d.Add(3, 300);
    EXPECT_EQ(d.GetCount(), 3);
}

TEST(Dictionary, GetReturnsAddedValue) {
    HashTableDictionary<uint32_t, int> d;
    d.Add(42, 999);
    EXPECT_EQ(d.Get(42), 999);
}

TEST(Dictionary, ContainsKeyTrueForExisting) {
    HashTableDictionary<uint32_t, int> d;
    d.Add(7, 70);
    EXPECT_TRUE(d.ContainsKey(7));
}

TEST(Dictionary, ContainsKeyFalseForMissing) {
    HashTableDictionary<uint32_t, int> d;
    d.Add(7, 70);
    EXPECT_FALSE(d.ContainsKey(8));
}

TEST(Dictionary, GetThrowsForMissing) {
    HashTableDictionary<uint32_t, int> d;
    d.Add(1, 10);
    EXPECT_THROW(d.Get(2), std::runtime_error);
}

TEST(Dictionary, AddSameKeyOverwrites) {
    HashTableDictionary<uint32_t, int> d;
    d.Add(5, 50);
    d.Add(5, 500);
    EXPECT_EQ(d.Get(5), 500);
    EXPECT_EQ(d.GetCount(), 1);
}

TEST(Dictionary, RemoveDecreasesCount) {
    HashTableDictionary<uint32_t, int> d;
    d.Add(1, 10);
    d.Add(2, 20);
    d.Remove(1);
    EXPECT_EQ(d.GetCount(), 1);
    EXPECT_FALSE(d.ContainsKey(1));
    EXPECT_TRUE(d.ContainsKey(2));
}

TEST(Dictionary, RemoveThrowsForMissing) {
    HashTableDictionary<uint32_t, int> d;
    d.Add(1, 10);
    EXPECT_THROW(d.Remove(99), std::runtime_error);
}

TEST(Dictionary, HandlesCollisions) {
    // capacity = 1 → все ключи попадают в один бакет
    HashTableDictionary<uint32_t, int> d(1);
    d.Add(10, 100);
    d.Add(20, 200);
    d.Add(30, 300);

    EXPECT_EQ(d.GetCount(), 3);
    EXPECT_EQ(d.Get(10), 100);
    EXPECT_EQ(d.Get(20), 200);
    EXPECT_EQ(d.Get(30), 300);
}

TEST(Dictionary, CollisionRemoveKeepsOthers) {
    HashTableDictionary<uint32_t, int> d(1);
    d.Add(10, 100);
    d.Add(20, 200);
    d.Add(30, 300);

    d.Remove(20);

    EXPECT_EQ(d.GetCount(), 2);
    EXPECT_FALSE(d.ContainsKey(20));
    EXPECT_EQ(d.Get(10), 100);
    EXPECT_EQ(d.Get(30), 300);
}

TEST(Dictionary, ManyElementsStress) {
    HashTableDictionary<uint32_t, int> d(10);
    for (uint32_t i = 0; i < 1000; ++i) {
        d.Add(i, (int)(i * 2));
    }
    EXPECT_EQ(d.GetCount(), 1000);

    for (uint32_t i = 0; i < 1000; ++i) {
        EXPECT_TRUE(d.ContainsKey(i));
        EXPECT_EQ(d.Get(i), (int)(i * 2));
    }
}

TEST(Dictionary, StressWithRemovals) {
    HashTableDictionary<uint32_t, int> d(10);
    for (uint32_t i = 0; i < 100; ++i) {
        d.Add(i, (int)i);
    }
    // удаляем чётные
    for (uint32_t i = 0; i < 100; i += 2) {
        d.Remove(i);
    }
    EXPECT_EQ(d.GetCount(), 50);

    for (uint32_t i = 0; i < 100; ++i) {
        if (i % 2 == 0) {
            EXPECT_FALSE(d.ContainsKey(i));
        } else {
            EXPECT_TRUE(d.ContainsKey(i));
            EXPECT_EQ(d.Get(i), (int)i);
        }
    }
}

TEST(Dictionary, StoresTicTacToeStates) {
    // Реалистичный сценарий: состояние поля → оценка
    HashTableDictionary<uint32_t, int> cache(25);

    cache.Add(0u,        0);   // пустое поле
    cache.Add(0b010000u, 1);   // X в клетке 1
    cache.Add(0b100000u, 2);   // O в клетке 1

    EXPECT_EQ(cache.Get(0u),        0);
    EXPECT_EQ(cache.Get(0b010000u), 1);
    EXPECT_EQ(cache.Get(0b100000u), 2);
    EXPECT_EQ(cache.GetCount(), 3);
}