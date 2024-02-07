//
// Created by dejavu on 2023/6/12.
//
#include <memory>
#include <iostream>
#include <vector>

// multi-way tree: m-way tree/n-ary tree

class MTNode;
typedef std::weak_ptr<MTNode>  MTNodeWeakPtr;
typedef std::shared_ptr<MTNode>  MTNodePtr;

class MTNode : public std::enable_shared_from_this<MTNode>
{
public:
    std::string title;
    MTNodeWeakPtr parent;

    MTNode(const std::string &name = ""):title(name),
                                         children_(std::vector<MTNodeWeakPtr>(0)){
        std::cout<<__FUNCTION__<<title <<std::endl;
    }

    ~MTNode(){
        std::cout<<__FUNCTION__ <<title <<std::endl;
    }

    std::shared_ptr<MTNode> getShared()
    {
        return shared_from_this();
    }

    void AddChild(MTNodePtr node)
    {
        node->parent = getShared();
        children_.push_back(node);
    }

    int ChildrenCount(){
        return children_.size();
    }

private:
    std::vector<MTNodeWeakPtr> children_;
};

int main()
{
//    std::shared_ptr<MTNode> p1(new MTNode);
    std::shared_ptr<MTNode> p1=std::make_shared<MTNode>("root");
    std::shared_ptr<MTNode> p2 = p1->getShared();
    std::shared_ptr<MTNode> pa=std::make_shared<MTNode>("A");
    std::shared_ptr<MTNode> pb=std::make_shared<MTNode>("B");
    p1->AddChild(pa);
    p2->AddChild(pb);
    std::cout<<"ChildrenCount"<<p2->ChildrenCount() <<std::endl;

    return 0;
}

/*
在这个示例中，MTNode 继承自 std::enable_shared_from_this<MTNode>，这意味着 MTNode 的实例可以通过 shared_from_this() 函数返回一个 std::shared_ptr 对象，这个 std::shared_ptr 对象与 MTNode 的实例共享所有权。

在 MTNode::getShared() 函数中，调用 shared_from_this() 函数返回一个 std::shared_ptr 对象，这个 std::shared_ptr 对象与 p1 共享 MTNode 的所有权。

需要注意的是，为了使用 std::enable_shared_from_this，必须使用 std::shared_ptr 对象来管理 MTNode 的实例。如果使用其他类型的智能指针，如 std::unique_ptr，将无法使用 std::enable_shared_from_this。

使用 std::enable_shared_from_this 可以避免在类中显式传递 std::shared_ptr 对象的麻烦，使代码更加简洁易读。但需要注意的是，使用 std::enable_shared_from_this 时，必须要保证该类始终是被 std::shared_ptr 对象所管理，否则会导致未定义的行为。
 */