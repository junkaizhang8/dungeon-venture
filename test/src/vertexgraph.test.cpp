#include <gtest/gtest.h>
#include "../src/vertexgraph.hpp"

class VertexGraphTest : public testing::Test
{
protected:
    void SetUp() override
    {
        w1.setStartVertex(v1);
        w1.setEndVertex(v2);
        w2.setStartVertex(v3);
        w2.setEndVertex(v1);
        w3.setStartVertex(v3);
        w3.setEndVertex(v2);
        w4.setStartVertex(v3);
        w4.setEndVertex(v4);
        w5.setStartVertex(v5);
        w5.setEndVertex(v6);
        nonEmptyGraph.insertMapping(w1);
        nonEmptyGraph.insertMapping(w2);
        nonEmptyGraph.insertMapping(w3);
        nonEmptyGraph.insertMapping(w4);
        nonEmptyGraph.insertMapping(w5);
    }

    VertexGraph emptyGraph;
    VertexGraph nonEmptyGraph;
    std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>(1, 1);
    std::shared_ptr<Vertex> v2 = std::make_shared<Vertex>(1, 2);
    std::shared_ptr<Vertex> v3 = std::make_shared<Vertex>(2, 1);
    std::shared_ptr<Vertex> v4 = std::make_shared<Vertex>(4, 4);
    std::shared_ptr<Vertex> v5 = std::make_shared<Vertex>(10, 10);
    std::shared_ptr<Vertex> v6 = std::make_shared<Vertex>(20, 20);
    std::shared_ptr<Vertex> v7 = std::make_shared<Vertex>(50, 50);
    std::shared_ptr<Vertex> v8 = std::make_shared<Vertex>(90, 90);

    Wall w1;
    Wall w2;
    Wall w3;
    Wall w4;
    Wall w5;
};

// Test if a mapping is properly inserted into an empty graph
TEST_F(VertexGraphTest, TestInsertMappingIntoEmptyGraph)
{
    emptyGraph.insertMapping(w1);
    EXPECT_TRUE(emptyGraph.contains(w1.getStartVertex()->getId(), w1.getEndVertex()->getId()));
    ASSERT_TRUE(emptyGraph.contains(w1.getEndVertex()->getId(), w1.getStartVertex()->getId()));
}

// Test if a mapping is properly inserted into a non-empty graph where mappings with start vertex are already in the graph
TEST_F(VertexGraphTest, TestInsertMappingStartVertexInGraph)
{
    Wall wall(v1, v7);
    nonEmptyGraph.insertMapping(wall);
    EXPECT_TRUE(nonEmptyGraph.contains(wall.getStartVertex()->getId(), wall.getEndVertex()->getId()));
    ASSERT_TRUE(nonEmptyGraph.contains(wall.getEndVertex()->getId(), wall.getStartVertex()->getId()));
}

// Test if a mapping is properly inserted into a non-empty graph where mappings with end vertex are already in the graph
TEST_F(VertexGraphTest, TestInsertMappingEndVertexInGraph)
{
    Wall wall(v7, v1);
    nonEmptyGraph.insertMapping(wall);
    EXPECT_TRUE(nonEmptyGraph.contains(wall.getStartVertex()->getId(), wall.getEndVertex()->getId()));
    ASSERT_TRUE(nonEmptyGraph.contains(wall.getEndVertex()->getId(), wall.getStartVertex()->getId()));
}

// Test if a mapping is properly inserted into a non-empty graph where mappings with start and end vertices are not in graph
TEST_F(VertexGraphTest, TestInsertMappingStartAndEndVertexNotInGraph)
{
    Wall wall(v7, v8);
    nonEmptyGraph.insertMapping(wall);
    EXPECT_TRUE(nonEmptyGraph.contains(wall.getStartVertex()->getId(), wall.getEndVertex()->getId()));
    ASSERT_TRUE(nonEmptyGraph.contains(wall.getEndVertex()->getId(), wall.getStartVertex()->getId()));
}

// Test if modifyMapping does not modify an existing vertex if the replaced vertex is not in the graph
TEST_F(VertexGraphTest, TestModifyMappingWithVertexNotInGraph)
{
    std::vector<wall_id> modified;
    std::vector<wall_id> removed;
    std::vector<wall_id> actualModified = {};
    std::vector<wall_id> actualRemoved = {};

    nonEmptyGraph.modifyMapping(v1->getId(), v7->getId(), modified, removed);
    EXPECT_FALSE(nonEmptyGraph.contains(v7->getId(), v2->getId()));
    EXPECT_FALSE(nonEmptyGraph.contains(v2->getId(), v7->getId()));
    EXPECT_FALSE(nonEmptyGraph.contains(v7->getId(), v3->getId()));
    EXPECT_FALSE(nonEmptyGraph.contains(v3->getId(), v7->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v1->getId(), v2->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v2->getId(), v1->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v1->getId(), v3->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v3->getId(), v1->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v2->getId(), v3->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v3->getId(), v2->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v3->getId(), v4->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v4->getId(), v3->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v5->getId(), v6->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v6->getId(), v5->getId()));
    EXPECT_EQ(modified, actualModified);
    ASSERT_EQ(removed, actualRemoved);
}

// Test if modifyMapping does not modify an existing vertex if the replaced vertex is the same as existing vertex
TEST_F(VertexGraphTest, TestModifyMappingWithSameVertices)
{
    std::vector<wall_id> modified;
    std::vector<wall_id> removed;
    std::vector<wall_id> actualModified = {};
    std::vector<wall_id> actualRemoved = {};

    nonEmptyGraph.modifyMapping(v1->getId(), v1->getId(), modified, removed);
    EXPECT_FALSE(nonEmptyGraph.contains(v1->getId(), v1->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v1->getId(), v2->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v2->getId(), v1->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v1->getId(), v3->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v3->getId(), v1->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v2->getId(), v3->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v3->getId(), v2->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v3->getId(), v4->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v4->getId(), v3->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v5->getId(), v6->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v6->getId(), v5->getId()));
    EXPECT_EQ(modified, actualModified);
    ASSERT_EQ(removed, actualRemoved);
}

// Test if modifyMapping modifies an existing vertex if the replaced vertex is in the graph but does not have
// a mapping with the existing vertex
TEST_F(VertexGraphTest, TestModifyMappingWithVertexInGraph)
{
    std::vector<wall_id> modified;
    std::vector<wall_id> removed;
    std::vector<wall_id> actualModified = {w5.getId()};
    std::vector<wall_id> actualRemoved = {};

    nonEmptyGraph.modifyMapping(v5->getId(), v1->getId(), modified, removed);
    EXPECT_FALSE(nonEmptyGraph.contains(v5->getId(), v6->getId()));
    EXPECT_FALSE(nonEmptyGraph.contains(v6->getId(), v5->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v1->getId(), v2->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v2->getId(), v1->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v1->getId(), v3->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v3->getId(), v1->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v2->getId(), v3->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v3->getId(), v2->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v3->getId(), v4->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v4->getId(), v3->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v1->getId(), v6->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v6->getId(), v1->getId()));
    EXPECT_EQ(modified, actualModified);
    ASSERT_EQ(removed, actualRemoved);
}

// Test if modifyMapping modifies an existing vertex if the replaced vertex is in the graph and has a mapping
// with the existing vertex
TEST_F(VertexGraphTest, TestModifyMappingWithVerticesSharingWall)
{
    std::vector<wall_id> modified;
    std::vector<wall_id> removed;
    std::vector<wall_id> actualModified = {};
    std::vector<wall_id> actualRemoved = {w4.getId()};

    nonEmptyGraph.modifyMapping(v4->getId(), v3->getId(), modified, removed);
    EXPECT_FALSE(nonEmptyGraph.contains(v3->getId(), v4->getId()));
    EXPECT_FALSE(nonEmptyGraph.contains(v4->getId(), v3->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v1->getId(), v2->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v2->getId(), v1->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v1->getId(), v3->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v3->getId(), v1->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v2->getId(), v3->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v3->getId(), v2->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v5->getId(), v6->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v6->getId(), v5->getId()));
    EXPECT_EQ(modified, actualModified);
    ASSERT_EQ(removed, actualRemoved);
}

// Test if modifyMapping modifies an existing vertex if the replaced vertex is in the graph and has a mapping
// with the existing vertex and has a mapping where a vertex is between the existing vertex and replaced vertex
// Example: E - Existing, R - Replaced, B - "Between"
//  R ____ B
//   |   /
//   |  /
//   | /
//   |/
//   E
TEST_F(VertexGraphTest, TestModifyMappingWithVerticesSharingWallAndSharingAdjacentVertex)
{
    std::vector<wall_id> modified;
    std::vector<wall_id> removed;
    std::vector<wall_id> actualModified = {};
    std::vector<wall_id> actualRemoved = {w1.getId(), w3.getId()};

    nonEmptyGraph.modifyMapping(v2->getId(), v1->getId(), modified, removed);
    EXPECT_FALSE(nonEmptyGraph.contains(v1->getId(), v2->getId()));
    EXPECT_FALSE(nonEmptyGraph.contains(v2->getId(), v1->getId()));
    EXPECT_FALSE(nonEmptyGraph.contains(v2->getId(), v3->getId()));
    EXPECT_FALSE(nonEmptyGraph.contains(v3->getId(), v2->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v1->getId(), v3->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v3->getId(), v1->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v3->getId(), v4->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v4->getId(), v3->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v5->getId(), v6->getId()));
    EXPECT_TRUE(nonEmptyGraph.contains(v6->getId(), v5->getId()));
    EXPECT_EQ(modified, actualModified);
    ASSERT_EQ(removed, actualRemoved);
}

// Test if an empty graph does not contain any mappings with vertex
TEST_F(VertexGraphTest, TestEmptyGraphDoesNotContainVertex)
{
    ASSERT_FALSE(emptyGraph.contains(v1->getId()));
}

// Test if a non-empty graph contains any mappings with vertex
TEST_F(VertexGraphTest, TestNonEmptyGraphContainsVertex)
{
    ASSERT_TRUE(nonEmptyGraph.contains(v1->getId()));
}

// Test if a non-empty graph does not contain any mappings with vertex
TEST_F(VertexGraphTest, TestNonEmptyGraphDoesNotContainVertex)
{
    ASSERT_FALSE(nonEmptyGraph.contains(v7->getId()));
}

// Test if an empty graph does not contain any mappings between two vertices
TEST_F(VertexGraphTest, TestEmptyGraphDoesNotContainMapping)
{
    ASSERT_FALSE(emptyGraph.contains(v1->getId(), v2->getId()));
}

// Test if a non-empty graph contains a mapping between two vertices
TEST_F(VertexGraphTest, TestNonEmptyGraphContainsMapping)
{
    ASSERT_TRUE(nonEmptyGraph.contains(v1->getId(), v2->getId()));
}

// Test if a non-empty graph does not contain a mapping where start vertex is in graph but not end vertex
TEST_F(VertexGraphTest, TestContainsStartVertexButDoesNotContainMapping)
{
    ASSERT_FALSE(nonEmptyGraph.contains(v1->getId(), v7->getId()));
}

// Test if a non-empty graph does not contain a mapping where end vertex is in graph but not start vertex
TEST_F(VertexGraphTest, TestContainsEndVertexButDoesNotContainMapping)
{
    ASSERT_FALSE(nonEmptyGraph.contains(v7->getId(), v1->getId()));
}

// Test if a non-empty graph does not contain any mappings between two vertices
TEST_F(VertexGraphTest, TestNonEmptyGraphDoesNotContainMapping)
{
    ASSERT_FALSE(nonEmptyGraph.contains(v7->getId(), v8->getId()));
}