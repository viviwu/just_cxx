/**
 ******************************************************************************
 * @file           : tree_node.cpp.c
 * @author         : vivi wu
 * @brief          : None
 * @attention      : None
 * @date           : 2023/12/8
 ******************************************************************************
 */

#include <iostream>
#include <memory>
#include <vector>
#include "geometry.h"

class TreeNode : public std::enable_shared_from_this<TreeNode> {
public:
    XPoint origin={0, 0};
    XSize  size = {10, 3};
    XPoint familyOrigin ;
    XSize  familySize ;

    int value;
    int depth;
    int breadth;
    std::weak_ptr<TreeNode> parent;
    std::vector<std::shared_ptr<TreeNode>> children;

    explicit TreeNode(int val) : value(val), depth(0), breadth(0) {}

    // Calculate origin, familyOrigin, size, and familySize recursively
    void calculateFamilyProperties() {
        updateOriginAndSize();
        updateFamilyOriginAndSize();
        for (const auto& child : children) {
            child->calculateFamilyProperties();
        }
    }

    std::shared_ptr<TreeNode> addChild(int val) {
        auto child = std::make_shared<TreeNode>(val);
        child->parent = shared_from_this();
        child->depth = depth + 1;
        child->breadth = children.size();
        children.push_back(child);

        child->calculateFamilyProperties();
        return child;
    }

    void addChild(std::shared_ptr<TreeNode> child) {
        child->parent = shared_from_this();
        child->depth = depth + 1;
        child->breadth = children.size();
        children.push_back(child);

        child->calculateFamilyProperties();
    }

    void removeChild(std::shared_ptr<TreeNode> child) {
        auto it = std::find(children.begin(), children.end(), child);
        if (it != children.end()) {
            (*it)->parent.reset();
            children.erase(it);
            updateBreadth();
        }
    }

    std::shared_ptr<TreeNode> findChild(int val) {
        for (const auto& child : children) {
            if (child->value == val) {
                return child;
            }
        }
        return nullptr;
    }

    std::shared_ptr<TreeNode> addSibling(int val) {
        if (auto parentNode = parent.lock()) {
            return parentNode->addChild(val);
        }
        return nullptr;
    }

    void removeSibling(std::shared_ptr<TreeNode> sibling) {
        if (auto parentNode = parent.lock()) {
            parentNode->removeChild(sibling);
        }
    }

    bool isRoot() const {
        return parent.expired();
    }

    std::vector<std::shared_ptr<TreeNode>> searchNodesByValue(int val) {
        std::vector<std::shared_ptr<TreeNode>> result;
        searchNodesByValueHelper(val, result);
        return result;
    }

private:
    void updateBreadth() {
        int index = 0;
        for (const auto& child : children) {
            child->breadth = index++;
        }
    }

    void searchNodesByValueHelper(int val, std::vector<std::shared_ptr<TreeNode>>& result) {
        if (value == val) {
            result.push_back(shared_from_this());
        }
        for (const auto& child : children) {
            child->searchNodesByValueHelper(val, result);
        }
    }


    // Update origin and size based on parent's origin and size
    void updateOriginAndSize() {
        if (auto parentNode = parent.lock()) {
            origin.x = parentNode->origin.x + parentNode->size.width;
            origin.y = parentNode->origin.y;
        }
    }

    // Update familyOrigin and familySize based on its own origin and size and its children's family properties
    void updateFamilyOriginAndSize() {
        familyOrigin = origin;
        familySize = size;
        for (const auto& child : children) {
            if (child->familyOrigin.x < familyOrigin.x) {
                familyOrigin.x = child->familyOrigin.x;
            }
            if (child->familyOrigin.y < familyOrigin.y) {
                familyOrigin.y = child->familyOrigin.y;
            }
            if (child->familyOrigin.x + child->familySize.width > familyOrigin.x + familySize.width) {
                familySize.width = child->familyOrigin.x + child->familySize.width - familyOrigin.x;
            }
            if (child->familyOrigin.y + child->familySize.height > familyOrigin.y + familySize.height) {
                familySize.height = child->familyOrigin.y + child->familySize.height - familyOrigin.y;
            }
        }
    }
};

int main_tree_node() {
    auto root = std::make_shared<TreeNode>(1);
    auto child1 = root->addChild(2);
    auto child2 = root->addChild(3);

    auto child3 = child1->addChild(2);
    auto child4 = child1->addChild(5);

    auto child5 = child2->addChild(6);
    auto child6 = child2->addChild(2);

    auto child7 = child3->addChild(8);
    auto child8 = child3->addChild(8);

    auto child9 = std::make_shared<TreeNode>(8);
     child4->addChild(child9);

    auto value = 8;
    std::cout << "Search nodes with value " <<value<<" : "<< std::endl;
    auto nodesWithValue2 = root->searchNodesByValue(value);
    for (const auto& node : nodesWithValue2) {
        std::cout << "Node value: " << node->value << ", Depth: " << node->depth << ", Breadth: " << node->breadth;
        if (auto parentNode = node->parent.lock()) {
            std::cout << ", Parent breadth: " << parentNode->breadth;
        }
        std::cout << std::endl;
    }
    root->calculateFamilyProperties();

    return 0;
}