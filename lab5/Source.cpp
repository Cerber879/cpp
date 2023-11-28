#include <iostream>
using namespace std;

struct Node {
    int value;
    int height;
    Node* left;
    Node* right;
};

// Функция для получения высоты узла
int getHeight(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

// Функция для получения баланса узла
int getBalance(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

// Функция для обновления высоты узла
void updateHeight(Node* node) {
    if (node == nullptr) {
        return;
    }
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
}

// Функция для создания нового узла
Node* createNode(int value) {
    Node* newNode = new Node();
    newNode->value = value;
    newNode->height = 1;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

// Функция для поворота направо
Node* rotateRight(Node* node) {
    Node* newRoot = node->left;
    Node* newSubtree = newRoot->right;

    newRoot->right = node;
    node->left = newSubtree;

    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

// Функция для поворота налево
Node* rotateLeft(Node* node) {
    Node* newRoot = node->right;
    Node* newSubtree = newRoot->left;

    newRoot->left = node;
    node->right = newSubtree;

    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

// Функция для балансировки узла
Node* balanceNode(Node* node) {
    updateHeight(node);

    int balance = getBalance(node);

    // Левый случай: левое поддерево выше
    if (balance > 1) {
        if (getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }

    // Правый случай: правое поддерево выше
    if (balance < -1) {
        if (getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }

    return node;
}

// Функция для добавления нового элемента в дерево
Node* insert(Node* root, int value) {
    // Рекурсивная вставка
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
        // Значение уже присутствует в дереве
        return root;
    }

    return balanceNode(root);
}

// Функция для изменения значения заданного элемента в дереве
Node* update(Node* root, int oldValue, int newValue) {
    // Поиск узла со значением oldValue
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
        // Найден узел со значением oldValue
        root->value = newValue;
    }

    return balanceNode(root);
}

// Функция для вывода дерева на экран в виде инфиксной (симметричной) последовательности
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

    // Пример использования
    int arr[] = { 5, 3, 7, 2, 4, 6, 8 };
    int n = sizeof(arr) / sizeof(arr[0]);

    for (int i = 0; i < n; i++) {
        root = insert(root, arr[i]);
    }

    cout << "АВЛ-дерево: ";
    printTree(root);
    cout << endl;

    int oldValue = 4;
    int newValue = 9;
    root = update(root, oldValue, newValue);

    cout << "Измененное АВЛ-дерево: ";
    printTree(root);
    cout << endl;

    return 0;
}