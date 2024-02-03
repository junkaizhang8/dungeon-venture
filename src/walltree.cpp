#include <memory>
#include "walltree.hpp"

// Return an iterator pointing to the first (in-order) element in the tree.
WallTreeIterator WallTree::begin()
{
    if (root == nullptr)
    {
        return end();
    }

    WallNode *temp = root.get();

    while (temp->left != nullptr)
    {
        temp = temp->left.get();
    }

    return WallTreeIterator(temp);
}

// Return an iterator pointing to the element immediately after the last
// element in the tree.
WallTreeIterator WallTree::end()
{
    return WallTreeIterator(nullptr);
}

// Return an iterator pointing to the first (pre-order) element in the tree.
WallTreePreOrderIterator WallTree::preOrderBegin()
{
    return WallTreePreOrderIterator(root.get(), this);
}

// Return an iterator pointing to the element immediately after the last
// element in the tree.
WallTreePreOrderIterator WallTree::preOrderEnd()
{
    return WallTreePreOrderIterator(nullptr, this);
}

// Insert newWall into the tree.
void WallTree::insert(const std::shared_ptr<Wall> &newWall)
{
    insert(root, nullptr, newWall);
}

void WallTree::remove(wall_id id)
{
    root = remove(root, nullptr, id);
}

std::shared_ptr<Wall> WallTree::search(wall_id id) const
{
    return search(root, id);
}

void WallTree::insert(std::unique_ptr<WallNode> &node, WallNode *const prevNode, const std::shared_ptr<Wall> &newWall)
{
    if (node == nullptr)
    {
        node = std::make_unique<WallNode>(newWall);
        node->parent = prevNode;
        return;
    }

    if (newWall->getId() == node->wall->getId())
    {
        return;
    }

    insert((newWall->getId() <= node->wall->getId()) ? node->left : node->right, node.get(), newWall);

    rebalance(node);
}

std::unique_ptr<WallTree::WallNode> WallTree::remove(std::unique_ptr<WallNode> &node, WallNode *const prevNode, wall_id id)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    if (id < node->wall->getId())
    {
        node->left = remove(node->left, node.get(), id);
    }
    else if (id > node->wall->getId())
    {
        node->right = remove(node->right, node.get(), id);
    }
    else
    {
        // Left child does not exist
        if (node->left == nullptr)
        {
            node = std::move(node->right);

            if (node)
            {
                node->parent = prevNode;
            }
        }
        // Right child does not exist
        else if (node->right == nullptr)
        {
            node = std::move(node->left);

            if (node)
            {
                node->parent = prevNode;
            }
        }
        // Both children exist
        else
        {
            std::shared_ptr<Wall> wall = findMin(node->right);

            node->wall = wall;
            node->right = remove(node->right, node.get(), wall->getId());

            if (node->right)
            {
                node->right->parent = node.get();
            }
        }
    }

    if (node == nullptr)
    {
        return nullptr;
    }

    rebalance(node);

    return std::move(node);
}

std::shared_ptr<Wall> WallTree::search(const std::unique_ptr<WallNode> &node, wall_id id) const
{
    if (node == nullptr)
    {
        return nullptr;
    }

    wall_id wallId = node->wall->getId();

    if (id == wallId)
    {
        return node->wall;
    }

    return search((id <= wallId) ? node->left : node->right, id);
}

std::shared_ptr<Wall> WallTree::findMin(const std::unique_ptr<WallNode> &node) const
{
    if (node == nullptr)
    {
        return nullptr;
    }

    WallNode *temp = node.get();

    while (temp->left)
    {
        temp = temp->left.get();
    }

    return temp->wall;
}

void WallTree::rotateLeft(std::unique_ptr<WallNode> &pivot)
{
    if (pivot == nullptr || pivot->right == nullptr)
    {
        return;
    }

    std::unique_ptr<WallNode> temp = std::move(pivot->right);
    temp->parent = pivot->parent;

    pivot->right = std::move(temp->left);
    pivot->height = max(getHeight(pivot->left), getHeight(pivot->right)) + 1;
    temp->left = std::move(pivot);
    temp->height = max(getHeight(temp->left), getHeight(temp->right)) + 1;
    pivot = std::move(temp);

    pivot->parent = nullptr;

    if (pivot->left)
    {
        pivot->left->parent = pivot.get();
    }

    if (pivot->left->right)
    {
        pivot->left->right->parent = pivot->left.get();
    }
}

void WallTree::rotateRight(std::unique_ptr<WallNode> &pivot)
{
    if (pivot == nullptr || pivot->left == nullptr)
    {
        return;
    }

    std::unique_ptr<WallNode> temp = std::move(pivot->left);

    pivot->left = std::move(temp->right);
    pivot->height = max(getHeight(pivot->left), getHeight(pivot->right)) + 1;
    temp->right = std::move(pivot);
    temp->height = max(getHeight(temp->left), getHeight(temp->right)) + 1;
    pivot = std::move(temp);

    pivot->parent = nullptr;

    
    if (pivot->right)
    {
        pivot->right->parent = pivot.get();
    }

    if (pivot->right->left)
    {
        pivot->right->left->parent = pivot->right.get();
    }
}

void WallTree::rebalance(std::unique_ptr<WallNode> &node)
{
    if (node == nullptr)
    {
        return;
    }

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

    int balance = getBalance(node);

    if (balance > 1)
    {
        // Left Left Case
        if (getBalance(node->left) >= 0)
        {
            rotateRight(node);
            return;
        }

        // Left Right Case
        rotateLeft(node->left);
        rotateRight(node);
        return;
    }

    if (balance < -1)
    {
        // Right Right Case
        if (getBalance(node->right) <= 0)
        {
            rotateLeft(node);
            return;
        }

        // Right Left Case
        rotateRight(node->right);
        rotateLeft(node);
    }
}

// Pre-increment operator
WallTreeIterator &WallTreeIterator::operator++()
{
    const WallTree::WallNode *temp;

    // Set node to be the smallest element if node is empty
    if (node == nullptr)
    {
        return *this;
    }

    // If there is a right subtree, set node to be the smallest node in subtree
    if (node->right != nullptr)
    {
        node = node->right.get();

        while (node->left != nullptr)
        {
            node = node->left.get();
        }
    }
    // If there is no right subtree, then either:
    // 1. set node to be its parent node if node is a left child of parent
    // 2. set node to be the parent node of the root of the right subtree
    else
    {
        temp = node->parent;

        while (temp != nullptr && temp->right && node == temp->right.get())
        {
            node = temp;
            temp = temp->parent;
        }

        node = temp;
    }

    return *this;
}

// Post-increment operator
WallTreeIterator WallTreeIterator::operator++(int)
{
    WallTreeIterator it = *this;
    const WallTree::WallNode *temp;

    // Set node to be the smallest element if node is empty
    if (node == nullptr)
    {
        return *this;
    }

    // If there is a right subtree, set node to be the smallest node in subtree
    if (node->right != nullptr)
    {
        node = node->right.get();

        while (node->left != nullptr)
        {
            node = node->left.get();
        }
    }
    // If there is no right subtree, then either:
    // 1. set node to be its parent node if node is a left child of parent
    // 2. set node to be the parent node of the root of the right subtree
    else
    {
        temp = node->parent;

        while (temp != nullptr && temp->right && node == temp->right.get())
        {
            node = temp;
            temp = temp->parent;
        }

        node = temp;
    }

    return it;
}

// Pre-increment operator
WallTreePreOrderIterator &WallTreePreOrderIterator::operator++()
{
    if (node == nullptr)
    {
        return *this;
    }

    if (node == tree->root.get())
    {
        nodeStack.push(nullptr);

        if (tree->root->right)
        {
            nodeStack.push(tree->root->right.get());
        }
        
        if (tree->root->left)
        {
            nodeStack.push(tree->root->left.get());
        }
    }

    node = nodeStack.top();
    nodeStack.pop();

    if (node)
    {
        if (node->right)
        {
            nodeStack.push(node->right.get());
        }

        if (node->left)
        {
            nodeStack.push(node->left.get());
        }
    }

    return *this;
}

// Post-increment operator
WallTreePreOrderIterator WallTreePreOrderIterator::operator++(int)
{
    WallTreePreOrderIterator &it = *this;

    if (node == nullptr)
    {
        return *this;
    }

    if (node == tree->root.get())
    {
        nodeStack.push(nullptr);

        if (tree->root->right)
        {
            nodeStack.push(tree->root->right.get());
        }
        
        if (tree->root->left)
        {
            nodeStack.push(tree->root->left.get());
        }
    }

    node = nodeStack.top();
    nodeStack.pop();

    if (node)
    {
        if (node->right)
        {
            nodeStack.push(node->right.get());
        }

        if (node->left)
        {
            nodeStack.push(node->left.get());
        }
    }

    return it;
}


// Debugging methods
// void WallTree::draw()
// {
//     draw(root);
// }

// void WallTree::draw(const std::unique_ptr<WallNode> &node)
// {
//     if (node == nullptr)
//     {
//         return;
//     }

//     std::cout << node->wall->getId() << ": ";
//     (node->left) ? std::cout << node->left->wall->getId() << " " : std::cout << "   ";
//     if (node->right) std::cout << node->right->wall->getId();
//     std::cout << std::endl;

//     draw(node->left);
//     draw(node->right);
// }