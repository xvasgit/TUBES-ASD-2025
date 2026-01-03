#include <iostream>
#include <string>
#include <cctype>
using namespace std;//tes

static const int ALPHABET_SIZE = 37; 
// 0-25  : A-Z
// 26-35 : 0-9
// 36    : spasi

struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
    float emisi;

    TrieNode() {
        isEndOfWord = false;
        emisi = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            children[i] = nullptr;
    }
};

struct BSTNode {
    string nama;
    float emisi;
    BSTNode *left, *right;

    BSTNode(string n, float e) {
        nama = n;
        emisi = e;
        left = right = nullptr;
    }
};

string toUpperCase(string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = toupper(s[i]);
    }
    return s;
}

int charToIndex(char ch) {
    if (isalpha(ch))
        return toupper(ch) - 'A';       // A-Z -> 0-25
    else if (isdigit(ch))
        return 26 + (ch - '0');         // 0-9 -> 26-35
    else if (ch == ' ')
        return 36;                      // spasi
    else
        return -1;                      // karakter lain diabaikan
}

char indexToChar(int idx) {
    if (idx < 26) return 'A' + idx;
    if (idx < 36) return '0' + (idx - 26);
    return ' ';
}

void insertTrie(TrieNode* root,string word, float emisi) {
    TrieNode* curr = root;
    for (int x = 0 ; x < word.length() ; x++) {
        char ch = word[x];
        int index = charToIndex(ch); //perubahan
        if (index == -1) continue;
        if (curr->children[index] == nullptr)
            curr->children[index] = new TrieNode();

        curr = curr->children[index];
    }

    curr->isEndOfWord = true;
    curr->emisi = emisi;
}

BSTNode* insertBST(BSTNode* root, string nama, float emisi) {
    if (root == nullptr) 
    return new BSTNode(nama, emisi);

    if (emisi < root->emisi)
        root->left = insertBST(root->left, nama, emisi);
    else
        root->right = insertBST(root->right, nama, emisi);

    return root;
}

// Fungsi tambahan untuk memastikan data masuk ke kedua struktur data
BSTNode* tambahData(TrieNode* rootTrie, BSTNode* rootBST, string nama, float emisi) {
    insertTrie(rootTrie, nama, emisi);
    rootBST = insertBST(rootBST, nama, emisi);
    return rootBST;
}

TrieNode* search(TrieNode* root, string word) {
    TrieNode* curr = root;

    for (int x = 0 ; x < word.length() ; x++) {
        char ch = word[x];
        int index = charToIndex(ch); //perubahan
        if (index == -1) continue;

        if (curr->children[index] == nullptr)
            return nullptr;

        curr = curr->children[index];
    }

    if (curr->isEndOfWord)
    return curr;
return nullptr;

}

void dfs(TrieNode* node,string prefix) {
    if (node->isEndOfWord) {
        cout << prefix << " - " << node->emisi << " kg CO2e\n";
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != nullptr) {
            dfs(node->children[i], prefix + indexToChar(i));
        }
    }
}

void displayAll(TrieNode* root) {
    dfs(root, "");
}

void startsWith(TrieNode* root,string prefix) {
    TrieNode* curr = root;
    string prefixKapital = toUpperCase(prefix);
    for (int x = 0 ; x < prefix.length() ; x++) {
        char ch = prefix[x];
        int index = charToIndex(ch);
        if (index == -1 || curr->children[index] == nullptr) {
            cout << "Data tidak ditemukan." << endl;
            return;
        }
        curr = curr->children[index];
    }
    dfs(curr, prefixKapital);
}

float sumDFS(TrieNode* node) {
    float total = 0;

    if (node->isEndOfWord) 
        total += node->emisi;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != nullptr)
            total += sumDFS(node->children[i]);
    }
    return total;
}

void sumEmisi(TrieNode* root,string prefix) {
    TrieNode* curr = root;

    for (int x = 0 ; x < prefix.length() ; x++) {
        char ch = prefix[x];
        int index = charToIndex(ch);
        if (index == -1) continue;

        if (curr->children[index] == nullptr) {
            cout << "Tidak ada lokasi dengan awalan '" << prefix << "'\n";
            return;
        }
        curr = curr->children[index];
    }

    float total = sumDFS(curr);
    cout << "Total Emisi untuk prefix '" << prefix
         << "' = " << total << " kg CO2e\n";
}

void cetakTerendah(BSTNode* root, int &count, int limit) {
    if (root == nullptr || count >= limit)
        return;

    cetakTerendah(root->left, count, limit);
    if (count < limit) {
        count++;
        cout << count << ". " << root->nama << " (" << root->emisi << " kg CO2e)" << endl;
    }
    cetakTerendah(root->right, count, limit);
}

void cetakTertinggi(BSTNode* root, int &count, int limit) {
    if (root == nullptr || count >= limit)
        return;

    cetakTertinggi(root->right, count, limit);
    if (count < limit) {
        count++;
        cout << count << ". " << root->nama << " (" << root->emisi << " kg CO2e)" << endl;
    }
    cetakTertinggi(root->left, count, limit);
}

int main() {
    TrieNode* root = new TrieNode();
    BSTNode* rootBST = nullptr;

    int pilihan;
    string nama;
    float emisi;

    rootBST = tambahData(root, rootBST, "Laboratorium Kimia", 50);
    rootBST = tambahData(root, rootBST, "Laboratorium Fisika", 40);
    rootBST = tambahData(root, rootBST, "Laboratorium Bio", 30);
    rootBST = tambahData(root, rootBST, "Kantin Utama", 20);
    rootBST = tambahData(root, rootBST, "Kantin Teknik", 15);
    rootBST = tambahData(root, rootBST, "Gedung A", 25);
    rootBST = tambahData(root, rootBST, "Gedung B", 35);
    rootBST = tambahData(root, rootBST, "Gedung Kuliah Umum", 60);
    rootBST = tambahData(root, rootBST, "Perpustakaan", 10);
    rootBST = tambahData(root, rootBST, "Pusat Riset Energi", 70);
    rootBST = tambahData(root, rootBST, "Ruangan 2702", 12.5);
    rootBST = tambahData(root, rootBST, "Ruangan 2703", 13.0);

    


    while (true) {
        cout << "\n===== DATABASE EMISI LOKASI (TRIE) =====\n";
        cout << "1. Tambah lokasi baru\n";
        cout << "2. Cari lokasi tertentu\n";
        cout << "3. Cari lokasi berdasarkan prefix\n";
        cout << "4. Hitung total emisi berdasarkan prefix\n";
        cout << "5. Tampilkan semua lokasi\n";
        cout << "6. Tampilkan emisi terendah\n";
        cout << "7. Tampilkan emisi tertinggi\n";
        cout << "8. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
        case 1:
            cout << "Nama lokasi: ";
            getline(cin, nama);

            cout << "Emisi (kg CO2e): ";
            cin >> emisi;
            cin.ignore();

            tambahData(root, rootBST, nama, emisi);
            cout << "Berhasil ditambahkan!\n";
            break;

        case 2: {
            cout << "Masukkan nama lokasi: ";
            getline(cin, nama);

            TrieNode* res = search(root, nama);

            if (!res)
                cout << "Lokasi tidak ditemukan!\n";
            else
                cout << "Emisi " << nama << " = "
                     << res->emisi << " kg CO2e\n";
            break;
        }

        case 3:
            cout << "Masukkan prefix: ";
            getline(cin, nama);
            startsWith(root, nama);
            break;

        case 4:
            cout << "Masukkan prefix: ";
            getline(cin, nama);
            sumEmisi(root, nama);
            break;

        case 5:
            cout << "Semua Lokasi:\n";
            displayAll(root);
            break;
        case 6: { 
            int n, counter = 0;
            cout << "Masukkan jumlah data terendah yang ingin ditampilkan: ";
            cin >> n;
            cout << "\n--- " << n << " LOKASI DENGAN EMISI TERENDAH ---\n";
            cetakTerendah(rootBST, counter, n);
            if (counter == 0) 
                cout << "Database kosong.\n";
            break;
        }
        case 7: { 
            int n, counter = 0;
            cout << "Masukkan jumlah data tertinggi yang ingin ditampilkan: ";
            cin >> n;
            cout << "\n--- " << n << " LOKASI DENGAN EMISI TERTINGGI ---\n";
            cetakTertinggi(rootBST, counter, n);
            if (counter == 0) 
                cout << "Database kosong.\n";
            break;
        }
        case 8: {
            cout << "Program selesai.\n";
            return 0;
        }
        default:
            cout << "Pilihan tidak valid!\n";
        }
    }

} 

