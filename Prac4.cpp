#include <iostream>
#include <string>
#include <stack>

using namespace std;

//структура элемента дерева
struct Tree
{
    string key;
    Tree* left;
    Tree* right;
    Tree(char key) { this->key = key; left = right = nullptr; }
    Tree(string key) { this->key = key; left = right = nullptr; }
};

//вывод дерева рямы обходом
void print_Tree(Tree* p, int level) {
    if (p)
    {
        if (level > 0) {
            for (int i = 0; i < level; i++) cout << "  ";
        }
        cout << p->key << endl;
        print_Tree(p->left, level + 1);
        print_Tree(p->right, level + 1);
    }
}

//поворот дерева справа на лево
Tree* rotateleft(Tree* q) {
    Tree* p = q->right;
    q->right = p->left;
    p->left = q;
    return p;
}

// Функция для проверки символа на оператор
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

//проверка строки на число
bool is_number(const string& s)
{
    return !s.empty() && find_if(s.begin(),
        s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
}

//калькулятор
int calc(int a, char c, int b) {
    switch (c) {
    case '+':
        return a + b;
    case'-':
        return a - b;
    case'*':
        return a * b;
    case'/':
        return a / b;
    default:
        return 0;
    }
}
//дальнейшее заполнение дерева
void createTree(Tree* root) {
    string expr;
    cout << "Введите " << root->key << ":\n";
    getline(cin, expr);
    if (is_number(expr)) {
        root->key = expr;
        return;
    }
    else {
        root->key = expr[1];
        root->left = new Tree{ expr[0] };
        createTree(root->left);
        root->right = new Tree{ expr[2] };
        createTree(root->right);
        return;
    }
}

Tree createExprTree() {
    cout << "Введите формулу(Вид формулы: <формула>::=цифра|<формула><знак операции><формула>):\n";
    cout << "Примечание! При вводе формулы всё кроме символа операции заменяйте на буквы(Пример: x+y)!!!\n";
    string expression;
    //создание корня
    getline(cin, expression);
    Tree root = { expression[1] };
    root.left = new Tree{ expression[0] };
    createTree(root.left);
    root.right = new Tree{ expression[2] };
    createTree(root.right);
    return root;
}

//создание префиксного выражения по дереву
string createPrefixExpr(Tree* root) {
    string s = root->key;
    if (root->left and root->right) {
        s += createPrefixExpr(root->left);
        s += createPrefixExpr(root->right);
        return s;
    }
    else return s;
}

//вычисление выражения по дереву
int prefixCalcTree(Tree* root) {
    int result = -1;
    if (root) {
        if (is_number(root->key)) {
            return stoi(root->key);
        }
        else if (isOperator(root->key[0])) {
            int temp1 = 0, temp2 = 0;
            if (root->left and root->right) {
                temp1 = prefixCalcTree(root->left);
                temp2 = prefixCalcTree(root->right);
            }
            result = calc(temp1, root->key[0], temp2);
        }
    }
    return result;
}

//вычисление выражения по префиксу
int prefixCalc(string s) {
    stack<int> stdig;
    char symb;
    for (int i = s.length(); i > -1; i--) {
        symb = s[i];
        if (isdigit(symb)) {
            stdig.push(symb - '0');
        }
        else if (isOperator(symb)) {
            int t1, t2;
            t1 = stdig.top();
            stdig.pop();
            t2 = stdig.top();
            stdig.pop();
            stdig.push(calc(t1, symb, t2));
        }
    }
    return stdig.top();
}

int main()
{
    setlocale(LC_ALL, "ru");
    Tree root = createExprTree();
    cout << root.key << endl;
    cout << "Дерево:\n";
    print_Tree(&root, 0);
    string prefix = createPrefixExpr(&root);
    cout << "\nПрефиксное выражение:\n" << prefix<< endl;
    int result = prefixCalcTree(root.left) + prefixCalcTree(root.right);
    cout << "Результат выражения по дереву: " << result << endl;
    result = prefixCalc(prefix);
    cout << "Результат выражения по префиксу: " << result << endl;
    cout << "\nПовернутое влево дерево:\n";
    Tree* newroot = rotateleft(&root);
    print_Tree(newroot, 0);
}