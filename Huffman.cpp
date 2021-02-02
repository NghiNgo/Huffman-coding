#include<bits/stdc++.h>

using namespace std;

#define MAX 128 

//Nút cây Huffman
struct Node {
    char kytu;
    int tanso;
    struct Node* left, * right;
};

//Vun đống
struct Heap {
    int size;
    int cap;
    struct Node** array;
};

//Nút tối thiểu
struct Node* newNode(char kytu, int tanso) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));

    temp->left = temp->right = NULL;
    temp->kytu = kytu;
    temp->tanso = tanso;

    return temp;
}

//Tạo đống
struct Heap* TaoHeap(int cap) {
    struct Heap* heap
        = (struct Heap*)malloc(sizeof(struct Heap));

    heap->size = 0;

    heap->cap = cap;

    heap->array = (struct Node**)malloc(heap->cap * sizeof(struct Node*));
    return heap;
}

//Đổi nút
void swapNode(struct Node** a, struct Node** b) {
    struct Node* t = *a;
    *a = *b;
    *b = t;
}

void heapify(struct Heap* heap, int ind) {
    int min = ind;
    int left = 2 * ind + 1;
    int right = 2 * ind + 2;

    if (left < heap->size && heap->array[left]->tanso < heap->array[min]->tanso)
        min = left;

    if (right < heap->size && heap->array[right]->tanso < heap->array[min]->tanso)
        min = right;

    if (min != ind) {
        swapNode(&heap->array[min], &heap->array[ind]);
        heapify(heap, min);
    }
}

//Kiểm tra đống có bằng 1
int isSizeOne(struct Heap* heap) {
    return (heap->size == 1);
}

//Giải nén nút tối thiểu từ đống
struct Node* extractMin(struct Heap* heap) {
    struct Node* temp = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];

    --heap->size;
    heapify(heap, 0);

    return temp;
}

//Chèn nút mới
void insertHeap(struct Heap* heap, struct Node* Node) {
    ++heap->size;
    int i = heap->size - 1;

    while (i && Node->tanso < heap->array[(i - 1) / 2]->tanso) {
        heap->array[i] = heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    heap->array[i] = Node;
}

void buildHeap(struct Heap* heap) {
    int n = heap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        heapify(heap, i);
}

//Kiểm tra nút có phải lá
int La(struct Node* root) {
    return !(root->left) && !(root->right);
}

struct Heap* createAndBuildHeap(vector<char> kytu, vector<int> tanso, int size) {
    struct Heap* heap = TaoHeap(size);

    for (int i = 0; i < size; ++i)
        heap->array[i] = newNode(kytu[i], tanso[i]);

    heap->size = size;
    buildHeap(heap);

    return heap;
}

struct Node* TaoHuffmanTree(vector<char> kytu, vector<int> tanso, int size) {
    struct Node* left, * right, * goc;

    struct Heap* heap = createAndBuildHeap(kytu, tanso, size);

    while (!isSizeOne(heap)) {
        left = extractMin(heap);
        right = extractMin(heap);

        goc = newNode('#', left->tanso + right->tanso);

        goc->left = left;
        goc->right = right;

        insertHeap(heap, goc);
    }

    return extractMin(heap);
}

//In ký tự va mã hóa của ký tự
void code(struct Node* root, int arr[], int goc, map<char, string>& Ma) {
    if (root->left) {
        arr[goc] = 0;
        code(root->left, arr, goc + 1, Ma);
    }

    if (root->right) {
        arr[goc] = 1;
        code(root->right, arr, goc + 1, Ma);
    }

    if (La(root)) {
        string str;
        for (int i = 0; i < goc; i++) {
            str += to_string(arr[i]);
        }
        cout << "  " << root->kytu << "------->" << str << endl;
        Ma.insert(pair<char, string>(root->kytu, str));
    }
}

//In bảng mã Huffman
void HuffmanCodes(vector<char> kytu, vector<int> tanso, int size, map<char, string>& Ma) {
    struct Node* root = TaoHuffmanTree(kytu, tanso, size);

    int arr[MAX], goc = 0;

    code(root, arr, goc, Ma);
}

//Giải mã chuỗi nhị phân dựa vào cây Huffman được xây ở trên

void DecodeHuffman(vector<char> kytu, vector<int> tanso, int size, string sd) {
    struct Node* root = TaoHuffmanTree(kytu, tanso, size);

    string giaima = "";

    struct Node* current = root;

    int kt = 0;

    while (sd.size()) {
        if (sd[0] == '0') {
            if (current->left != NULL) {
                current = current->left;
                sd = sd.erase(0, 1);
            }
            else {
                kt = 1;
                break;
            }
        }
        else {
            if (current->right != NULL) {
                current = current->right;
                sd = sd.erase(0, 1);
            }
            else {
                kt = 1;
                break;
            }
        }
        if (La(current)) {
            giaima += current->kytu;
            current = root;
        }
    }

    if (!La(current) and current != root)
        kt = 1;

    if (kt == 0)
        cout << giaima << endl;
    else
        cout << "Chuoi nhi phan khong the giai ma bang cay nhi phan duoc xay dung tren!" << endl;
}


int main() {
    cout << " ----------------HUFFMAN ENCODING------------------" << endl;
    //Nhập chuỗi
    string sc, s;
    cout << "Nhap chuoi can ma hoa: " << endl;
    getline(cin, sc);
    s = sc;
    cout << "------------------------------------------------------------------------------------------";

    vector<char> kytu;
    vector<int> tanso;
    int vtri = 0;

    //Đếm ký tự xuất hiện trong chuỗi
    while (s.length() != 0) {
        kytu.push_back(s[0]);
        tanso.push_back(1);
        s.erase(0, 1);
        int i = 0;
        while (i < s.length())
            if (s[i] == kytu[vtri]) {
                tanso[vtri]++;
                s.erase(i, 1);
            }
            else
                i++;
        vtri++;
    }

    //Sắp xếp các ký tự theo tần số tăng dần
    for (int i = 0; i < kytu.size(); i++)
        for (int j = i + 1; j < kytu.size(); j++)
            if (tanso[j] < tanso[i]) {
                swap(tanso[j], tanso[i]);
                swap(kytu[j], kytu[i]);
            }

    vector<int> tansuat;
    int SUM = accumulate(tanso.begin(), tanso.end(), 0);
    for (int i = 0; i < tanso.size(); i++)
        tansuat.push_back(tanso[i]*100/SUM);

    //Xuất các ký tự, tần số và tần suất trong chuỗi vừa nhập:
    cout << endl << "Cac ky tu trong chuoi vua nhap la:" << endl;
    cout << "Ky tu" << "    " << "Tan so" << "    " << "Tan suat (%)" << endl;
    for (int i = 0; i < kytu.size(); i++)
        cout << "  " << kytu[i] << "         " << tanso[i] << "\t\t" << tansuat[i] << endl;
    cout << "------------------------------------------------------------------------------------------";

    //Xuất mã hóa của các ký tự:
    map<char, string> Ma;
    cout << endl << "Ma hoa cua cac ky tu: " << endl;
    cout << "Ky tu" << "    " << "Ma hoa" << endl;
    HuffmanCodes(kytu, tanso, kytu.size(), Ma);
    cout << "------------------------------------------------------------------------------------------";

    //Xuất chuỗi và chuỗi đã được mã hóa:
    cout << endl << "Chuoi duoc nhap vao: " << endl;
    cout << sc << endl;
    cout << endl << "Chuoi duoc ma hoa thanh: " << endl;
    for (int i = 0; i < sc.size() - 1; i++)
        cout << Ma[sc[i]] << "|";
    cout << Ma[sc[sc.size() - 1]] << endl;
    cout << "------------------------------------------------------------------------------------------" << endl;

    //Nhập chuỗi nhị phân và giải mã dựa trên cây Huffman được xây dựng ở câu trên
    int check = 1;
    while (check == 1) {
        cout << endl << "----------------HUFFMAN DECODING------------------" << endl;
        cout << "Nhap chuoi nhi phan can giai ma dua tren cay Huffman vua duoc xay: " << endl;
        string sd;
        cin >> sd;
        cout << endl << "----------------------------------------------------------------------------------------------";
        cout << endl << "Chuoi nhi phan vua nhap la: " << endl << sd << endl;
        cout << endl << "----------------------------------------------------------------------------------------------";
        cout << endl << "---BANG GIAI MA---" << endl;
        for (auto i = Ma.begin(); i != Ma.end(); i++)
            cout << i->second << "\t----->" << i->first << endl;
        cout << "------------------------------------------------------------------------------------------";
        cout << endl << "Chuoi duoc giai ma thanh: " << endl;
        DecodeHuffman(kytu, tanso, kytu.size(), sd);
        cout << "----------------------------------------------------------------------------------------------";
        cout << endl << "1. Tiep tuc decoding." << endl << "2. Ket thuc." << endl;
        cin >> check;
    }

    return 0;
}