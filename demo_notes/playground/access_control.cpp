/**
 * @author wuxw
 * @date 10/8/23
 * @desc ...
 */
//
// Created by 朱小争 on 10/8/23.
//

#include <iostream>
#include <string>
#include <unordered_map>

namespace AccessControlModel {

/**
 * 基于角色的访问控制（Role-Based Access Control，RBAC）：
 * RBAC 是一种广泛应用的权限模型，它将权限分配给角色，而不是直接分配给用户。
 * 用户被分配到一个或多个角色，而角色则具有特定的权限。
 * 这种模型简化了权限管理，提高了系统的可维护性和安全性。
 */
namespace RBAC {
// 定义系统权限枚举
enum SystemPermission { Browse = 1 << 0, Add = 1 << 1, Update = 1 << 2, Delete = 1 << 3 };

// 定义角色类
class Role {
private:
    //    std::bitset<4> permissions;
    int permissions;

public:
    Role() { permissions = 0; }

    // 添加权限
    void addPermission(SystemPermission permission) {
        //        permissions.set(permission);
        permissions |= permission;
    }

    // 移除权限
    void removePermission(SystemPermission permission) { permissions &= ~permission; }

    // 判断是否具有某种权限
    bool hasPermission(SystemPermission permission) {
        return (permissions & permission) == permission;
    }
};
}  // namespace RBAC

namespace ABAC {

enum class Attribute { Role, Department, Location };

class User {
private:
    std::unordered_map<Attribute, std::string> attributes;

public:
    User() {}

    void setAttribute(Attribute attribute, const std::string& value) {
        attributes[attribute] = value;
    }

    const std::string& getAttribute(Attribute attribute) const {
        if (attributes.count(attribute) > 0) {
            return attributes.at(attribute);
        }
        static const std::string emptyValue;
        return emptyValue;
    }
};

class Resource {
private:
    std::unordered_map<Attribute, std::string> attributes;

public:
    Resource() {}

    void setAttribute(Attribute attribute, const std::string& value) {
        attributes[attribute] = value;
    }

    const std::string& getAttribute(Attribute attribute) const {
        if (attributes.count(attribute) > 0) {
            return attributes.at(attribute);
        }
        static const std::string emptyValue;
        return emptyValue;
    }
};

class AccessControl {
public:
    static bool checkAccess(const User& user, const Resource& resource) {
        // 检查用户和资源的属性，并根据策略判断是否允许访问
        std::string userRole = user.getAttribute(Attribute::Role);
        std::string userDepartment = user.getAttribute(Attribute::Department);
        std::string userLocation = user.getAttribute(Attribute::Location);

        std::string resourceRole = resource.getAttribute(Attribute::Role);
        std::string resourceDepartment = resource.getAttribute(Attribute::Department);
        std::string resourceLocation = resource.getAttribute(Attribute::Location);

        // 简化的策略：仅允许具有相同部门和位置的用户访问资源
        return (userDepartment == resourceDepartment) && (userLocation == resourceLocation);
    }
};

}  // namespace ABAC
}  // namespace AccessControlModel

using namespace AccessControlModel::RBAC;
using namespace AccessControlModel::ABAC;

int test_rbac() {
    // 创建角色
    Role admin;
    admin.addPermission(Browse);
    admin.addPermission(Add);
    admin.addPermission(Update);
    admin.addPermission(Delete);

    Role guest;
    guest.addPermission(Browse);

    // 判断角色权限
    std::cout << "Admin Permissions:" << std::endl;
    std::cout << "Browse: " << admin.hasPermission(Browse) << std::endl;
    std::cout << "Add: " << admin.hasPermission(Add) << std::endl;
    std::cout << "Update: " << admin.hasPermission(Update) << std::endl;
    std::cout << "Delete: " << admin.hasPermission(Delete) << std::endl;

    std::cout << std::endl;

    std::cout << "Guest Permissions:" << std::endl;
    std::cout << "Browse: " << guest.hasPermission(Browse) << std::endl;
    std::cout << "Add: " << guest.hasPermission(Add) << std::endl;
    std::cout << "Update: " << guest.hasPermission(Update) << std::endl;
    std::cout << "Delete: " << guest.hasPermission(Delete) << std::endl;

    return 0;
}


int test_abac() {
    User user;
    user.setAttribute(Attribute::Role, "Manager");
    user.setAttribute(Attribute::Department, "Sales");
    user.setAttribute(Attribute::Location, "Headquarters");

    Resource resource;
    resource.setAttribute(Attribute::Role, "Manager");
    resource.setAttribute(Attribute::Department, "Sales");
    resource.setAttribute(Attribute::Location, "Headquarters");

    bool hasAccess = AccessControl::checkAccess(user, resource);

    std::cout << "访问权限: " << (hasAccess ? "允许" : "拒绝") << std::endl;

    return 0;
}


int main() {
    test_rbac();
    test_abac();
}
