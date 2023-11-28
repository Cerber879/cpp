#include <iostream>
using namespace std;

struct Node {
    int value;
    int height;
    Node* left;
    Node* right;
};

// ������� ��� ��������� ������ ����
int getHeight(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

// ������� ��� ��������� ������� ����
int getBalance(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

// ������� ��� ���������� ������ ����
void updateHeight(Node* node) {
    if (node == nullptr) {
        return;
    }
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
}

// ������� ��� �������� ������ ����
Node* createNode(int value) {
    Node* newNode = new Node();
    newNode->value = value;
    newNode->height = 1;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

// ������� ��� �������� �������
Node* rotateRight(Node* node) {
    Node* newRoot = node->left;
    Node* newSubtree = newRoot->right;

    newRoot->right = node;
    node->left = newSubtree;

    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

// ������� ��� �������� ������
Node* rotateLeft(Node* node) {
    Node* newRoot = node->right;
    Node* newSubtree = newRoot->left;

    newRoot->left = node;
    node->right = newSubtree;

    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

// ������� ��� ������������ ����
Node* balanceNode(Node* node) {
    updateHeight(node);

    int balance = getBalance(node);

    // ����� ������: ����� ��������� ����
    if (balance > 1) {
        if (getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }

    // ������ ������: ������ ��������� ����
    if (balance < -1) {
        if (getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }

    return node;
}

// ������� ��� ���������� ������ �������� � ������
Node* insert(Node* root, int value) {
    // ����������� �������
    if (root == nullptr) {
        return createNode(value);
    }

    if (value < root->value) {
        root->left = insert(root->left, value);
    }
    else if (value > root->value) {
        root->right = insert(root->right, value);
    }
    else {
        // �������� ��� ������������ � ������
        return root;
    }

    return balanceNode(root);
}

// ������� ��� ��������� �������� ��������� �������� � ������
Node* update(Node* root, int oldValue, int newValue) {
    // ����� ���� �� ��������� oldValue
    if (root == nullptr) {
        return nullptr;
    }

    if (oldValue < root->value) {
        root->left = update(root->left, oldValue, newValue);
    }
    else if (oldValue > root->value) {
        root->right = update(root->right, oldValue, newValue);
    }
    else {
        // ������ ���� �� ��������� oldValue
        root->value = newValue;
    }

    return balanceNode(root);
}

// ������� ��� ������ ������ �� ����� � ���� ��������� (������������) ������������������
void printTree(Node* root) {
    if (root != nullptr) {
        printTree(root->left);
        cout << root->value << " ";
        printTree(root->right);
    }
}

int main() {
    setlocale(LC_ALL, "ru");
    Node* root = nullptr;

    // ������ �������������
    int arr[] = { 5, 3, 7, 2, 4, 6, 8 };
    int n = sizeof(arr) / sizeof(arr[0]);

    for (int i = 0; i < n; i++) {
        root = insert(root, arr[i]);
    }

    cout << "���-������: ";
    printTree(root);
    cout << endl;

    int oldValue = 4;
    int newValue = 9;
    root = update(root, oldValue, newValue);

    cout << "���������� ���-������: ";
    printTree(root);
    cout << endl;

    return 0;
}