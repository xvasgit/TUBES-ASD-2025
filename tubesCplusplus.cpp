#include <iostream>
#include <string>
#include <cctype>
using namespace std;//tes

static const int ALPHABET_SIZE = 37; 
// 0-25  : A-Z
// 26-35 : 0-9
// 36    : spasi

// ----------------------------------------------------------
// TRIE NODE
// ----------------------------------------------------------
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

// ----------------------------------------------------------
// CHAR <-> INDEX MAPPING
// ----------------------------------------------------------
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

// ----------------------------------------------------------
// INSERT
// ----------------------------------------------------------
void insert(TrieNode* root, const string& word, float emisi) {
    TrieNode* curr = root;

    for (char ch : word) {
        int index = charToIndex(ch); //perubahan
        if (index == -1) continue;

        if (curr->children[index] == nullptr)
            curr->children[index] = new TrieNode();

        curr = curr->children[index];
    }

    curr->isEndOfWord = true;
    curr->emisi = emisi;
}

// ----------------------------------------------------------
// SEARCH
// ----------------------------------------------------------
TrieNode* search(TrieNode* root, const string& word) {
    TrieNode* curr = root;

    for (char ch : word) {
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

// ----------------------------------------------------------
// DFS
// ----------------------------------------------------------
void dfs(TrieNode* node, const string& prefix) {
    if (node->isEndOfWord) {
        cout << prefix << " - " << node->emisi << " kg CO2e\n";
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != nullptr) {
            dfs(node->children[i], prefix + indexToChar(i));
        }
    }
}

// ----------------------------------------------------------
// DISPLAY ALL
// ----------------------------------------------------------
void displayAll(TrieNode* root) {
    dfs(root, "");
}

// ----------------------------------------------------------
// STARTSWITH
// ----------------------------------------------------------
void startsWith(TrieNode* root, const string& prefix) {
    TrieNode* curr = root;

    for (char ch : prefix) {
        int index = charToIndex(ch);
        if (index == -1) continue;

        if (curr->children[index] == nullptr) {
            cout << "Tidak ada lokasi dengan awalan '" << prefix << "'\n";
            return;
        }
        curr = curr->children[index];
    }

    dfs(curr, prefix);
}

// ----------------------------------------------------------
// SUM EMISI DFS
// ----------------------------------------------------------
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

void sumEmisi(TrieNode* root, const string& prefix) {
    TrieNode* curr = root;

    for (char ch : prefix) {
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

// ----------------------------------------------------------
// MAIN
// ----------------------------------------------------------
int main() {
    TrieNode* root = new TrieNode();

    // DATA AWAL
    insert(root, "Laboratorium Kimia", 50);
    insert(root, "Laboratorium Fisika", 40);
    insert(root, "Laboratorium Bio", 30);
    insert(root, "Kantin Utama", 20);
    insert(root, "Kantin Teknik", 15);
    insert(root, "Gedung A", 25);
    insert(root, "Gedung B", 35);
    insert(root, "Gedung Kuliah Umum", 60);
    insert(root, "Perpustakaan", 10);
    insert(root, "Pusat Riset Energi", 70);

    // TEST ANGKA + SPASI
    insert(root, "Ruangan 2702", 12.5);
    insert(root, "Ruangan 2703", 13.0);

    int pilihan;
    string nama;
    float emisi;

    while (true) {
        cout << "\n===== DATABASE EMISI LOKASI (TRIE) =====\n";
        cout << "1. Tambah lokasi baru\n";
        cout << "2. Cari lokasi tertentu\n";
        cout << "3. Cari lokasi berdasarkan prefix\n";
        cout << "4. Hitung total emisi berdasarkan prefix\n";
        cout << "5. Tampilkan semua lokasi\n";
        cout << "6. Keluar\n";
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

            insert(root, nama, emisi);
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

        case 6:
            cout << "Program selesai.\n";
            return 0;

        default:
            cout << "Pilihan tidak valid!\n";
        }
    }

} 
