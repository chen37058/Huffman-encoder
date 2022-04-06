#include<iostream>
#include<string>
#include<map>
#include<algorithm>
#include<fstream>
#include<sstream>
using namespace std;

typedef string * HuffmanCode;

typedef struct {
    int weight;
    int parent, lchild, rchild;
}HTNode, * HuffmanTree;

void readFile(stringstream& ss) {
    string filePath;
    cin >> filePath;
    ifstream ifs(filePath, ios::in);
    if (!ifs.is_open()) return;
    ss << ifs.rdbuf();
    ifs.close();
}
void Count(stringstream& ss, map<string, int>& map, int choice) {
    map.clear();
    if (choice == 1) {
        string word;
        for (int i = 1; ss >> word; ++i) {
            auto pos = map.find(word);
            if (pos == map.end()) map.insert(make_pair(word, 1));
            else pos->second += 1;
        }
    }
    else {
        char c;
        ss.unsetf(ios::skipws);
        for (int i = 1; ss >> c; ++i) {
            string s(1, c);
            auto pos = map.find(s);
            if (pos == map.end()) map.insert(make_pair(s, 1));
            else pos->second += 1;
        }
    }   
}
void Select(HuffmanTree HT, int end, int& s1, int& s2) {
    int i;
    int minWeight = 100000;
    for (i = 1; i < end; i++) {
        if (HT[i].weight < minWeight && HT[i].parent == 0) {
            minWeight = HT[i].weight;
            s1 = i;
        }
    }
    minWeight = 100000;
    for (i = 1; i < end; i++) {
        if (HT[i].weight < minWeight && HT[i].parent == 0 & i != s1) {
            minWeight = HT[i].weight;
            s2 = i;
        }
    }
}
void CreateHuffmanTree(HuffmanTree& HT, map<string, int> map) {
    int n, m, i, s1, s2;
    n = map.size();
    if (n <= 1) return;
    m = 2 * n - 1;
    HT = new HTNode[m + 1];
    auto it = map.begin();
    for (i = 1; i <= m; ++i) {
        if (it != map.end()) {
            HT[i].weight = it->second;
            ++it;
        }
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
    }
    for (int i = n + 1; i <= m; i++) {
        Select(HT, i, s1, s2);
        HT[s1].parent = i;
        HT[s2].parent = i;
        HT[i].lchild = s1;
        HT[i].rchild = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
    }
}
void HuffmanCoding(HuffmanTree& HT, HuffmanCode& HC, int n) {
    int i, c, f;   
    HC = new string[n + 1];
    for (i = 1; i <= n; ++i) {
        c = i;
        for (f = HT[c].parent; f != 0; c = f, f = HT[c].parent) {
            if (HT[f].lchild == c) HC[i].append("0");           
            else HC[i].append("1");               
        }
        reverse(HC[i].begin(), HC[i].end());
    }
}
void PaperEncode(HuffmanCode HC, map<string, int>& map, stringstream& ss, int choice) {
    char c;
    string str;
    if (choice == 1) {
        while (ss >> str) {
            auto it = map.begin();
            for (int i = 1; it != map.end(); ++i, ++it)
                if (it->first == str) { cout << HC[i]; break; }
            if (it == map.end()) cout << "#";
        }
    }
    else {
        ss.unsetf(ios::skipws);
        while (ss >> c) {
            string str(1, c);
            auto it = map.begin();
            for (int i = 1; it != map.end(); ++i, ++it)
                if (it->first == str) { cout << HC[i]; break;}
            if (it == map.end()) cout << "#";
        }
    }   
} 
void ShowHuffmanTree(HuffmanTree HT, map<string, int> map) {   
    int m = map.size() * 2 - 1; auto it = map.begin(); string str;
    for (int i = 1; i <= m; ++i) {
        cout << i << "\t";        
        if (it != map.end()) {
            if (it->first == "\n") str = "\\n";
            else str = it->first;
            it++;
            cout << str << " \t";
        }
        else cout << " \t";
        cout<< HT[i].weight << "\t" << HT[i].parent << "\t" << HT[i].lchild << "\t" << HT[i].rchild << endl;
    }
}
void ShowHuffmanCode(map<string, int> map, HuffmanCode HC) {
    int i, n = map.size();
    auto it = map.begin();
    string str;    
    for (i = 1; i <= n; i++, it++) {
        if (it->first == "\n") str = "\\n";
        else str = it->first;
        cout << str << " \t" << HC[i] << endl;
    }
}
int main() {
    map<string, int> m;    
    HuffmanTree HT = NULL;
    HuffmanCode HC = NULL;
    int choice;
    while (true) {
        system("cls");
        int num;
        cout << "请选择功能 1.统计词频并创建哈夫曼树 2.进行哈夫曼编码 3.对文章进行编码 4.退出：";
        cin >> num;
        switch (num) {
            case 1: {
                cout << "请输入文件路径:";
                stringstream ss;
                readFile(ss);
                cout << "请选择编码方式 (1.单词 2.字符):";
                cin >> choice;
                Count(ss, m, choice);
                CreateHuffmanTree(HT, m);
                cout << "创建哈夫曼树成功！下面是哈夫曼树的参数输出：" << endl;
                if (choice == 1) cout << "序号\t单词\t词频\t父结点\t左孩子\t右孩子\n";
                else cout << "序号\t字符\t词频\t父结点\t左孩子\t右孩子\n";
                ShowHuffmanTree(HT, m);
                system("pause");
                break;
            }
            case 2: {
                HuffmanCoding(HT, HC, m.size());     
                if (choice == 1) {
                    cout << "哈夫曼编码成功！下面是每个单词编码：" << endl;
                    cout << "单词\t哈夫曼编码" << endl;
                }
                else {
                    cout << "哈夫曼编码成功！下面是每个字符的编码：" << endl;
                    cout << "字符\t哈夫曼编码" << endl;
                }
                ShowHuffmanCode(m, HC);
                system("pause");
                break;
            }
            case 3: {
                stringstream ss2;
                cout << "请输入要编码的文章的文件路径:";
                readFile(ss2);
                cout << "文章编码成功！下面是文章编码结果(在编码表中不存在的用“#”代替）：" << endl;
                PaperEncode(HC, m, ss2, choice);
                system("pause");
                break;
            }
            case 4: return 0;
        }
    }
}