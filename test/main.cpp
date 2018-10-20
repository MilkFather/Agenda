#include <functional>
#include "gtest/gtest.h"
#include "Date.hpp"
#include "Meeting.hpp"
#include "User.hpp"
#include "Storage.hpp"

using namespace std;

bool getAllM (const Meeting &) {
    return true;
}

bool getAllU (const User &) {
    return true;
}

bool filterUserTest (const User & u) {
    if (u.getName() == "Li")
        return true;
    else
        return false;
}

void updateUserTest (User & u) {
    u.setPassword("breakup");
}

TEST(Storage, basicIO) {
    Storage *s = Storage::getInstance().get();
    
    function<bool(const User &)> all = getAllU;
    EXPECT_EQ(s->queryUser(all).size(), 2);
    
    EXPECT_EQ(s->sync(), true);
}

TEST(Storage, AddQueryUpdateRemoveUser) {
    Storage *s = Storage::getInstance().get();
    
    function<bool(const User &)> all = getAllU;
    ASSERT_EQ(s->queryUser(all).size(), 2);
    User u("Li", "hate", "love@shi", "88877343376");
    // ADD
    s->createUser(u);
    ASSERT_EQ(s->queryUser(all).size(), 3);
    
    // QUERY
    function<bool(const User &)> filter = filterUserTest;
    ASSERT_EQ(s->queryUser(filter).size(), 1);
    EXPECT_EQ(s->queryUser(filter).front().getName(), "Li");
    EXPECT_EQ(s->queryUser(filter).front().getPassword(), "hate");
    EXPECT_EQ(s->queryUser(filter).front().getEmail(), "love@shi");
    EXPECT_EQ(s->queryUser(filter).front().getPhone(), "88877343376");
    
    // UPDATE
    function<void(User &)> update = updateUserTest;
    EXPECT_EQ(s->updateUser(filter, update), 1);
    
    ASSERT_EQ(s->queryUser(filter).size(), 1);
    EXPECT_EQ(s->queryUser(filter).front().getName(), "Li");
    EXPECT_EQ(s->queryUser(filter).front().getPassword(), "breakup");
    EXPECT_EQ(s->queryUser(filter).front().getEmail(), "love@shi");
    EXPECT_EQ(s->queryUser(filter).front().getPhone(), "88877343376");
    
    // DELETE
    s->deleteUser(filter);
    
    EXPECT_EQ(s->queryUser(filter).size(), 0);
    EXPECT_EQ(s->queryUser(all).size(), 2);
}
/*
TEST(Storage, AddQueryUpdateRemoveMeeeting) {
    Storage *s = Storage::getInstance().get();
    
    function<bool(const User &)> all = getAllM;
    ASSERT_EQ(s->queryUser(all).size(), 3);
    Meeting m("Li", "hate", "love@shi", "88877343376");
    // ADD
    s->createMeeting(m);
    ASSERT_EQ(s->queryUser(all).size(), 3);
    
    // QUERY
    function<bool(const User &)> filter = filterUserTest;
    ASSERT_EQ(s->queryUser(filter).size(), 1);
    EXPECT_EQ(s->queryUser(filter).front().getName(), "Li");
    EXPECT_EQ(s->queryUser(filter).front().getPassword(), "hate");
    EXPECT_EQ(s->queryUser(filter).front().getEmail(), "love@shi");
    EXPECT_EQ(s->queryUser(filter).front().getPhone(), "88877343376");
    
    // UPDATE
    function<void(User &)> update = updateUserTest;
    EXPECT_EQ(s->updateUser(filter, update), 1);
    
    ASSERT_EQ(s->queryUser(filter).size(), 1);
    EXPECT_EQ(s->queryUser(filter).front().getName(), "Li");
    EXPECT_EQ(s->queryUser(filter).front().getPassword(), "breakup");
    EXPECT_EQ(s->queryUser(filter).front().getEmail(), "love@shi");
    EXPECT_EQ(s->queryUser(filter).front().getPhone(), "88877343376");
    
    // DELETE
    s->deleteUser(filter);
    
    EXPECT_EQ(s->queryUser(filter).size(), 0);
    EXPECT_EQ(s->queryUser(all).size(), 2);
}
*/
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
