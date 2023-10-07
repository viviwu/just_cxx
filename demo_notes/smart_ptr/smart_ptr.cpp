//
// Created by dejavu on 2023/6/12.
//
#include <memory>
#include <iostream>
#include <vector>

class IMNode;
typedef std::weak_ptr<IMNode>  IMNodeWeakPtr;
typedef std::shared_ptr<IMNode>  IMNodePtr;

class IMNode : public std::enable_shared_from_this<IMNode>
{
public:
    std::string title;
    IMNodeWeakPtr parent;

    IMNode(const std::string &name = ""):title(name),
                                         children_(std::vector<IMNodeWeakPtr>(0)){
        std::cout<<__FUNCTION__<<title <<std::endl;
    }

    ~IMNode(){
        std::cout<<__FUNCTION__ <<title <<std::endl;
    }

    std::shared_ptr<IMNode> getShared()
    {
        return shared_from_this();
    }

    void AddChild(IMNodePtr node)
    {
        node->parent = getShared();
        children_.push_back(node);
    }

    int ChildrenCount(){
        return children_.size();
    }

private:
    std::vector<IMNodeWeakPtr> children_;
};

int main()
{
//    std::shared_ptr<IMNode> p1(new IMNode);
    std::shared_ptr<IMNode> p1=std::make_shared<IMNode>("root");
    std::shared_ptr<IMNode> p2 = p1->getShared();
    std::shared_ptr<IMNode> pa=std::make_shared<IMNode>("A");
    std::shared_ptr<IMNode> pb=std::make_shared<IMNode>("B");
    p1->AddChild(pa);
    p2->AddChild(pb);
    std::cout<<"ChildrenCount"<<p2->ChildrenCount() <<std::endl;

    return 0;
}

/*
�����ʾ���У�IMNode �̳��� std::enable_shared_from_this<IMNode>������ζ�� IMNode ��ʵ������ͨ�� shared_from_this() ��������һ�� std::shared_ptr ������� std::shared_ptr ������ IMNode ��ʵ����������Ȩ��

�� IMNode::getShared() �����У����� shared_from_this() ��������һ�� std::shared_ptr ������� std::shared_ptr ������ p1 ���� IMNode ������Ȩ��

��Ҫע����ǣ�Ϊ��ʹ�� std::enable_shared_from_this������ʹ�� std::shared_ptr ���������� IMNode ��ʵ�������ʹ���������͵�����ָ�룬�� std::unique_ptr�����޷�ʹ�� std::enable_shared_from_this��

ʹ�� std::enable_shared_from_this ���Ա�����������ʽ���� std::shared_ptr ������鷳��ʹ������Ӽ���׶�������Ҫע����ǣ�ʹ�� std::enable_shared_from_this ʱ������Ҫ��֤����ʼ���Ǳ� std::shared_ptr ��������������ᵼ��δ�������Ϊ��
 */