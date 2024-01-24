#include <gtest/gtest.h>
#include "../src/vertextree.hpp"

class VertexTreeTest : public testing::Test
{
protected:
    void SetUp() override
    {
        singleNodeTree.insert(v1);
        multipleNodeTree.insert(v1);
        multipleNodeTree.insert(v2);
        multipleNodeTree.insert(v3);
        multipleNodeTree.insert(v4);
    }

    VertexTree emptyTree;
    VertexTree singleNodeTree;
    VertexTree multipleNodeTree;
    const int c1[2] = {5, 5};
    const int c2[2] = {1, 8};
    const int c3[2] = {7, 7};
    const int c4[2] = {4, 3};
    const std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>(c1);
    const std::shared_ptr<Vertex> v2 = std::make_shared<Vertex>(c2);
    const std::shared_ptr<Vertex> v3 = std::make_shared<Vertex>(c3);
    const std::shared_ptr<Vertex> v4 = std::make_shared<Vertex>(c4);
};

// Test if a vertex is properly inserted into an empty tree
TEST_F(VertexTreeTest, TestInsertIntoEmptyTree)
{
    emptyTree.insert(v1);
    ASSERT_EQ(emptyTree.search(c1).get(), v1.get());
}

// Test if a vertex is properly inserted into a single-node tree
TEST_F(VertexTreeTest, TestInsertIntoSingleNodeTree)
{
    singleNodeTree.insert(v2);
    ASSERT_EQ(singleNodeTree.search(c2).get(), v2.get());
}

// Test if a vertex is properly inserted into a multi-node tree
TEST_F(VertexTreeTest, TestInsertIntoMultipleNodeTree)
{
    const int coords[2] = {6, 5};
    const std::shared_ptr<Vertex> vertex = std::make_shared<Vertex>(coords);

    multipleNodeTree.insert(vertex);
    ASSERT_EQ(multipleNodeTree.search(coords).get(), vertex.get());
}

// Test if a vertex is properly removed from a single-node tree
TEST_F(VertexTreeTest, TestRemoveFromSingleNodeTree)
{
    singleNodeTree.remove(v1->getId(), c1);
    ASSERT_EQ(singleNodeTree.search(c1), nullptr);
}

// Test if a leaf node is properly removed in a multi-node tree
TEST_F(VertexTreeTest, TestRemoveLeaf)
{
    multipleNodeTree.remove(v4->getId(), c4);
    EXPECT_EQ(multipleNodeTree.search(c1).get(), v1.get());
    EXPECT_EQ(multipleNodeTree.search(c2).get(), v2.get());
    EXPECT_EQ(multipleNodeTree.search(c3).get(), v3.get());
    ASSERT_EQ(multipleNodeTree.search(c4), nullptr);
}

// Test if a node with one child is properly removed in a multi-node tree
TEST_F(VertexTreeTest, TestRemoveOneChildNode)
{
    multipleNodeTree.remove(v2->getId(), c2);
    EXPECT_EQ(multipleNodeTree.search(c1).get(), v1.get());
    EXPECT_EQ(multipleNodeTree.search(c3).get(), v3.get());
    EXPECT_EQ(multipleNodeTree.search(c4).get(), v4.get());
    ASSERT_EQ(multipleNodeTree.search(c2), nullptr);
}

// Test if a node with two children is properly removed in a multi-node tree
TEST_F(VertexTreeTest, TestRemoveTwoChildrenNode)
{
    multipleNodeTree.remove(v1->getId(), c1);
    EXPECT_EQ(multipleNodeTree.search(c2).get(), v2.get());
    EXPECT_EQ(multipleNodeTree.search(c3).get(), v3.get());
    EXPECT_EQ(multipleNodeTree.search(c4).get(), v4.get());
    ASSERT_EQ(multipleNodeTree.search(c1), nullptr);
}

// Test if search returns no vertex if tree is empty
TEST_F(VertexTreeTest, TestSearchEmptyTree)
{
    ASSERT_EQ(emptyTree.search(c1), nullptr);
}

// Test if search returns the proper vertex for a single-node tree given vertex coords
TEST_F(VertexTreeTest, TestSingleNodeTreeSearchCorrectCoords)
{
    ASSERT_EQ(singleNodeTree.search(c1).get(), v1.get());
}

// Test if search returns no vertex for a single-node tree given coords are not exact
TEST_F(VertexTreeTest, TestSingleNodeTreeSearchIncorrectCoords)
{
    ASSERT_EQ(singleNodeTree.search(c2), nullptr);
}

// Test if search returns the proper vertex for a multi-node tree given vertex coords
TEST_F(VertexTreeTest, TestMultipleNodeTreeSearchCorrectCoords)
{
    ASSERT_EQ(multipleNodeTree.search(c3).get(), v3.get());
}

// Test if search returns no vertex for a multi-node tree given coords are not exact
TEST_F(VertexTreeTest, TestMultipleNodeTreeSearchIncorrectCoords)
{
    const int coords[2] = {9, 9};
    ASSERT_EQ(multipleNodeTree.search(coords), nullptr);
}

// Test if proximitySearch returns no vertex given tree is empty
TEST_F(VertexTreeTest, TestNeighbourEmptyTree)
{
    ASSERT_EQ(emptyTree.proximitySearch(c1, 0), nullptr);
}

// Test if proximitySearch returns the proper vertex for a single-node tree given coords are exact and dist is 0
TEST_F(VertexTreeTest, TestSingleNodeTreeNeighbourZeroDist)
{
    ASSERT_EQ(singleNodeTree.proximitySearch(c1, 0).get(), v1.get());
}

// Test if proximitySearch returns no vertex for a single-node tree given coords are not exact and dist is 0
TEST_F(VertexTreeTest, TestSingleNodeTreeNoNeighbourZeroDist)
{
    ASSERT_EQ(singleNodeTree.proximitySearch(c2, 0), nullptr);
}

// Test if proximitySearch returns the proper vertex for a single-node tree given vertex is within range of dist
TEST_F(VertexTreeTest, TestSingleNodeTreeNeighbourInRange)
{
    const int coords[2] = {8, 9};
    ASSERT_EQ(singleNodeTree.proximitySearch(coords, 5).get(), v1.get());
}

// Test if proximitySearch returns no vertex for a single-node tree given vertex is not within range of dist
TEST_F(VertexTreeTest, TestSingleNodeTreeNoNeighbourInRange)
{
    const int coords[2] = {9, 9};
    ASSERT_EQ(singleNodeTree.proximitySearch(coords, 5), nullptr);
}

// Test if proximitySearch returns the proper vertex for a multi-node tree given coords are exact and dist is 0
TEST_F(VertexTreeTest, TestMultipleNodeTreeNeighbourZeroDist)
{
    ASSERT_EQ(multipleNodeTree.proximitySearch(c3, 0).get(), v3.get());
}

// Test if proximitySearch returns no vertex for a multi-node tree given coords are not exact and dist is 0
TEST_F(VertexTreeTest, TestMultipleNodeTreeNoNeighbourZeroDist)
{
    const int coords[2] = {10, 10};
    ASSERT_EQ(multipleNodeTree.proximitySearch(coords, 0), nullptr);
}

// Test if proximitySearch returns the proper vertex for a multi-node tree given vertex is within range of dist
TEST_F(VertexTreeTest, TestMultipleNodeTreeNeighbourInRange)
{
    const int coords[2] = {8, 8};
    ASSERT_EQ(multipleNodeTree.proximitySearch(coords, 2).get(), v3.get());
}

// Test if proximitySearch returns no vertex for a multi-node tree given vertex is not within range of dist
TEST_F(VertexTreeTest, TestMultipleNodeTreeNoNeighbourInRange)
{
    const int coords[2] = {12, 12};
    ASSERT_EQ(multipleNodeTree.proximitySearch(coords, 2), nullptr);
}