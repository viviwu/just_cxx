//
// Created by viviwu on 2023/8/14.
//

#include <iostream>

enum Color: unsigned int {
    Red = 1 << 0,
    Green = 1 << 1,
    Blue = 1 << 2
};

int check_color() {
    //�����ֵ�����:
    Color col = static_cast<Color>(Color::Red | Color::Green);
    if (col & Color::Green) {
        // c contains Green
        std::cout << "c contains Green "<< std::endl;
    }
    std::cout << std::hex << col << std::endl; // 3

    Color c1 = Color::Red;
    Color c2 = Color::Green;

    std::cout << std::hex << (c1 ^ c2) << std::endl; // 3

    return 0;
}


// ����ϵͳȨ��ö��
enum SystemPermission {
    Browse = 1 << 0,
    Add = 1 << 1,
    Update = 1 << 2,
    Delete = 1 << 3
};

// �����ɫ��
class Role {
private:
    int permissions;

public:
    Role() {
        permissions = 0;
    }

    // ���Ȩ��
    void addPermission(SystemPermission permission) {
        permissions |= permission;
    }

    // �Ƴ�Ȩ��
    void removePermission(SystemPermission permission) {
        permissions &= ~permission;
    }

    // �ж��Ƿ����ĳ��Ȩ��
    bool hasPermission(SystemPermission permission) {
        return (permissions & permission) == permission;
    }
};

int main() {
    // ������ɫ
    Role admin;
    admin.addPermission(Browse);
    admin.addPermission(Add);
    admin.addPermission(Update);
    admin.addPermission(Delete);

    Role guest;
    guest.addPermission(Browse);

    // �жϽ�ɫȨ��
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