#include <gtest/gtest.h>
#include "../src/walltree.hpp"

class WallTreeTest : public testing::Test
{
protected:
    void SetUp() override
    {
        singleNodeTree.insert(w1);
        multipleNodeTree.insert(w3);
        multipleNodeTree.insert(w2);
        multipleNodeTree.insert(w4);
        multipleNodeTree.insert(w1);
    }

    WallTree emptyTree;
    WallTree singleNodeTree;
    WallTree multipleNodeTree;
    std::shared_ptr<Wall> w1 = std::make_shared<Wall>();
    std::shared_ptr<Wall> w2 = std::make_shared<Wall>();
    std::shared_ptr<Wall> w3 = std::make_shared<Wall>();
    std::shared_ptr<Wall> w4 = std::make_shared<Wall>();
    std::shared_ptr<Wall> w5 = std::make_shared<Wall>();
};

// Test if a wall is properly inserted into an empty tree
TEST_F(WallTreeTest, TestInsertIntoEmptyTree)
{
    emptyTree.insert(w1);
    ASSERT_EQ(emptyTree.search(w1->getId())->getId(), w1->getId());
}

// Test if a wall is properly inserted into a single-node tree
TEST_F(WallTreeTest, TestInsertIntoSingleNodeTree)
{
    singleNodeTree.insert(w2);
    ASSERT_EQ(singleNodeTree.search(w2->getId())->getId(), w2->getId());
}

// Test if a wall is properly inserted into a multi-node tree
TEST_F(WallTreeTest, TestInsertIntoMultipleNodeTree)
{
    const std::shared_ptr<Wall> wall = std::make_shared<Wall>();

    multipleNodeTree.insert(wall);
    ASSERT_EQ(multipleNodeTree.search(wall->getId())->getId(), wall->getId());
}

// Test if a wall is properly removed from a single-node tree
TEST_F(WallTreeTest, TestRemoveFromSingleNodeTree)
{
    singleNodeTree.remove(w1->getId());
    ASSERT_EQ(singleNodeTree.search(w1->getId()), nullptr);
}

// Test if a leaf node is properly removed in a multi-node tree
TEST_F(WallTreeTest, TestRemoveLeaf)
{
    multipleNodeTree.remove(w1->getId());
    EXPECT_EQ(multipleNodeTree.search(w2->getId())->getId(), w2->getId());
    EXPECT_EQ(multipleNodeTree.search(w3->getId())->getId(), w3->getId());
    EXPECT_EQ(multipleNodeTree.search(w4->getId())->getId(), w4->getId());
    ASSERT_EQ(multipleNodeTree.search(w1->getId()), nullptr);
}

// Test if a node with one child is properly removed in a multi-node tree
TEST_F(WallTreeTest, TestRemoveOneChildNode)
{
    multipleNodeTree.remove(w2->getId());
    EXPECT_EQ(multipleNodeTree.search(w1->getId())->getId(), w1->getId());
    EXPECT_EQ(multipleNodeTree.search(w3->getId())->getId(), w3->getId());
    EXPECT_EQ(multipleNodeTree.search(w4->getId())->getId(), w4->getId());
    ASSERT_EQ(multipleNodeTree.search(w2->getId()), nullptr);
}

// Test if a node with two children is properly removed in a multi-node tree
TEST_F(WallTreeTest, TestRemoveTwoChildrenNode)
{
    multipleNodeTree.remove(w3->getId());
    EXPECT_EQ(multipleNodeTree.search(w1->getId())->getId(), w1->getId());
    EXPECT_EQ(multipleNodeTree.search(w2->getId())->getId(), w2->getId());
    EXPECT_EQ(multipleNodeTree.search(w4->getId())->getId(), w4->getId());
    ASSERT_EQ(multipleNodeTree.search(w3->getId()), nullptr);
}

// Test if search returns no vertex if tree is empty
TEST_F(WallTreeTest, TestSearchEmptyTree)
{
    ASSERT_EQ(emptyTree.search(w1->getId()), nullptr);
}

// Test if search returns the proper wall for a single-node tree given wall id
TEST_F(WallTreeTest, TestSingleNodeTreeSearchCorrectId)
{
    ASSERT_EQ(singleNodeTree.search(w1->getId())->getId(), w1->getId());
}

// Test if search returns no vertex for a single-node tree given wrong wall id
TEST_F(WallTreeTest, TestSingleNodeTreeSearchIncorrectId)
{
    ASSERT_EQ(singleNodeTree.search(w2->getId()), nullptr);
}

// Test if search returns the proper vertex for a multi-node tree given wall id
TEST_F(WallTreeTest, TestMultipleNodeTreeSearchCorrectId)
{
    ASSERT_EQ(multipleNodeTree.search(w2->getId())->getId(), w2->getId());
}

// Test if search returns no vertex for a multi-node tree given wrong wall id
TEST_F(WallTreeTest, TestMultipleNodeTreeSearchIncorrectId)
{
    ASSERT_EQ(multipleNodeTree.search(8), nullptr);
}