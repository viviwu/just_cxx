/**
 ******************************************************************************
 * @file           : tree_node_zip.cpp.c
 * @author         : viviwu
 * @brief          : None
 * @attention      : None
 * @date           : 1/28/24
 ******************************************************************************
 */
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include "tree.pb.h"

class TreeNode : public std::enable_shared_from_this<TreeNode> {
 public:
  std::string text;
  int id;
  std::vector<std::shared_ptr<TreeNode>> children;
  std::weak_ptr<TreeNode> parent;

  TreeNode(const std::string& t, int i) : text(t), id(i) {}

  std::shared_ptr<TreeNode> addChild(const std::string& t, int i) {
    auto child = std::make_shared<TreeNode>(t, i);
    child->parent = shared_from_this();
    children.push_back(child);
    return child;
  }

  void removeChild(const std::shared_ptr<TreeNode>& child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
      (*it)->parent.reset();
      children.erase(it);
    }
  }

  void depthFirstTraversal() {
    std::cout << "Node: " << text << " (ID: " << id << ")" << std::endl;
    for (const auto& child : children) {
      child->depthFirstTraversal();
    }
  }

  void convertToProto(TreeNodeProto& protoNode) const {
    protoNode.set_text(text);
    protoNode.set_id(id);
    std::cout<<protoNode.id()<<" --> "<<protoNode.text()<<std::endl;
    for (const auto& child : children) {
      auto* childProto = protoNode.add_children();
      child->convertToProto(*childProto);
    }
  }

  static std::shared_ptr<TreeNode> convertFromProto(const TreeNodeProto& protoNode) {
    std::cout<<protoNode.id()<<" "<<protoNode.text()<<std::endl;
    auto node = std::make_shared<TreeNode>(protoNode.text(), protoNode.id());
    for (const auto& childProto : protoNode.children()) {
      auto childNode = convertFromProto(childProto);
      childNode->parent = node;
      node->children.push_back(childNode);
    }
    return node;
  }
};

int main() {
  auto root = std::make_shared<TreeNode>("Root", 1);
  auto child1 = root->addChild("Child 1", 2);
  auto child2 = root->addChild("Child 2", 3);
  auto grandchild1 = child1->addChild("Grandchild 1", 4);
  auto grandchild2 = child1->addChild("Grandchild 2", 5);

  // Serialize
  TreeNodeProto protoRoot;
  root->convertToProto(protoRoot);
  std::ofstream output("tree.pb", std::ios::binary | std::ios::out);
  if (protoRoot.SerializeToOstream(&output)) {
    std::cout << "Serialization successful." << std::endl;
  } else {
    std::cerr << "Serialization failed." << std::endl;
    return 1;
  }

  // Deserialize
  TreeNodeProto protoRootLoaded;
  std::ifstream input("tree.pb", std::ios::binary | std::ios::in);
  if (protoRootLoaded.ParseFromIstream(&input)) {
    std::cout << "Deserialization successful." << std::endl;


    std::cout<<protoRootLoaded.id()<<" "<<protoRootLoaded.text()<<std::endl;

    auto loadedRoot = TreeNode::convertFromProto(protoRootLoaded);
    loadedRoot->depthFirstTraversal();  // Test the loaded tree
  } else {
    std::cerr << "Deserialization failed." << std::endl;
    return 1;
  }

  return 0;
}
