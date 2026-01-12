#include <iostream>                 // Library untuk input-output (cin, cout)
#include <string>                   // Library untuk tipe data string
#include <cctype>                   // Library untuk fungsi karakter (toupper, isalpha, isdigit)
using namespace std;               

static const int ALPHABET_SIZE = 37; // Ukuran array children Trie (A-Z, 0-9, spasi)
// 0-25  : A-Z                     // Mapping huruf kapital
// 26-35 : 0-9                     // Mapping angka
// 36    : spasi                   // Mapping karakter spasi

struct TrieNode {                  // Struktur node Trie
    TrieNode* children[ALPHABET_SIZE]; // Array pointer ke anak Trie
    bool isEndOfWord;              // Penanda akhir kata
    float emisi;                   // Nilai emisi untuk lokasi

    TrieNode() {                   // Constructor TrieNode
        isEndOfWord = false;        // Awalnya bukan akhir kata
        emisi = 0;                 // Emisi awal 0
        for (int i = 0; i < ALPHABET_SIZE; i++) // Loop inisialisasi children
            children[i] = nullptr; // Semua anak di-set null
    }
};

struct BSTNode {                   // Struktur node Binary Search Tree
    string nama;                   // Nama lokasi
    float emisi;                   // Nilai emisi
    BSTNode *left, *right;         // Pointer anak kiri dan kanan

    BSTNode(string n, float e) {   // Constructor BSTNode
        nama = n;                  // Set nama
        emisi = e;                 // Set emisi
        left = right = nullptr;    // Anak kiri dan kanan kosong
    }
};

string toUpperCase(string s) {     // Fungsi mengubah string menjadi huruf kapital
    for (int i = 0; i < s.length(); i++) { // Loop setiap karakter
        s[i] = toupper(s[i]);      // Ubah karakter ke huruf besar
    }
    return s;                      // Kembalikan string kapital
}

int charToIndex(char ch) {         // Fungsi mapping karakter ke index Trie
    if (isalpha(ch))               // Jika huruf
        return toupper(ch) - 'A';  // A-Z -> 0-25
    else if (isdigit(ch))          // Jika angka
        return 26 + (ch - '0');    // 0-9 -> 26-35
    else if (ch == ' ')            // Jika spasi
        return 36;                 // Spasi -> 36
    else
        return -1;                 // Karakter lain diabaikan
}

char indexToChar(int idx) {        // Fungsi mapping index ke karakter
    if (idx < 26) return 'A' + idx;       // Index 0-25 ke huruf
    if (idx < 36) return '0' + (idx - 26);// Index 26-35 ke angka
    return ' ';                            // Index 36 ke spasi
}

void insertTrie(TrieNode* root,string word, float emisi) { // Insert data ke Trie
    TrieNode* curr = root;         // Pointer traversal Trie
    for (int x = 0 ; x < word.length() ; x++) { // Loop tiap karakter kata
        char ch = word[x];         // Ambil karakter
        int index = charToIndex(ch); // Konversi karakter ke index
        if (index == -1) continue; // Lewati karakter tidak valid
        if (curr->children[index] == nullptr) // Jika node belum ada
            curr->children[index] = new TrieNode(); // Buat node baru

        curr = curr->children[index]; // Pindah ke node berikutnya
    }

    curr->isEndOfWord = true;      // Tandai akhir kata
    curr->emisi = emisi;           // Simpan nilai emisi
}

BSTNode* insertBST(BSTNode* root, string nama, float emisi) { // Insert ke BST
    if (root == nullptr)           // Jika BST kosong
    return new BSTNode(nama, emisi); // Buat node baru

    if (emisi < root->emisi)       // Jika emisi lebih kecil
        root->left = insertBST(root->left, nama, emisi); // Masuk kiri
    else
        root->right = insertBST(root->right, nama, emisi); // Masuk kanan

    return root;                   // Kembalikan root
}

// Fungsi tambahan untuk memastikan data masuk ke kedua struktur data
BSTNode* tambahData(TrieNode* rootTrie, BSTNode* rootBST, string nama, float emisi) {
    insertTrie(rootTrie, nama, emisi); // Tambah ke Trie
    rootBST = insertBST(rootBST, nama, emisi); // Tambah ke BST
    return rootBST;                // Kembalikan root BST
}

TrieNode* search(TrieNode* root, string word) { // Cari kata di Trie
    TrieNode* curr = root;         // Pointer traversal

    for (int x = 0 ; x < word.length() ; x++) { // Loop karakter
        char ch = word[x];         // Ambil karakter
        int index = charToIndex(ch); // Konversi ke index
        if (index == -1) continue; // Lewati karakter tidak valid

        if (curr->children[index] == nullptr) // Jika jalur tidak ada
            return nullptr;        // Kata tidak ditemukan

        curr = curr->children[index]; // Lanjut ke node berikut
    }

    if (curr->isEndOfWord)         // Jika akhir kata valid
    return curr;                   // Kembalikan node
return nullptr;                    // Jika bukan akhir kata
}

void dfs(TrieNode* node,string prefix) { // DFS untuk menampilkan semua data Trie
    if (node->isEndOfWord) {       // Jika akhir kata
        cout << prefix << " - " << node->emisi << " kg CO2e\n"; // Cetak data
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) { // Loop semua kemungkinan anak
        if (node->children[i] != nullptr) { // Jika anak ada
            dfs(node->children[i], prefix + indexToChar(i)); // DFS rekursif
        }
    }
}

void startsWith(TrieNode* root,string prefix) { // Cari data berdasarkan prefix
    TrieNode* curr = root;         // Pointer traversal
    string prefixKapital = toUpperCase(prefix); // Ubah prefix ke kapital
    for (int x = 0 ; x < prefix.length() ; x++) { // Loop karakter prefix
        char ch = prefix[x];      // Ambil karakter
        int index = charToIndex(ch); // Konversi ke index
        if (index == -1 || curr->children[index] == nullptr) { // Jika gagal
            cout << "Data tidak ditemukan." << endl; // Pesan error
            return;                // Keluar fungsi
        }
        curr = curr->children[index]; // Pindah ke node berikut
    }
    dfs(curr, prefixKapital);      // Tampilkan semua data dengan prefix
}

float sumDFS(TrieNode* node) {     // DFS untuk menjumlahkan emisi
    float total = 0;               // Total emisi

    if (node->isEndOfWord)         // Jika akhir kata
        total += node->emisi;      // Tambahkan emisi

    for (int i = 0; i < ALPHABET_SIZE; i++) { // Loop anak
        if (node->children[i] != nullptr)
            total += sumDFS(node->children[i]); // Rekursif jumlahkan
    }
    return total;                  // Kembalikan total
}

void sumEmisi(TrieNode* root,string prefix) { // Hitung total emisi berdasarkan prefix
    TrieNode* curr = root;         // Pointer traversal

    for (int x = 0 ; x < prefix.length() ; x++) { // Loop prefix
        char ch = prefix[x];      // Ambil karakter
        int index = charToIndex(ch); // Konversi ke index
        if (index == -1) continue; // Lewati karakter tidak valid

        if (curr->children[index] == nullptr) { // Jika jalur tidak ada
            cout << "Tidak ada lokasi dengan awalan '" << prefix << "'\n";
            return;                // Keluar fungsi
        }
        curr = curr->children[index]; // Pindah node
    }

    float total = sumDFS(curr);    // Hitung total emisi
    cout << "Total Emisi untuk prefix '" << prefix
         << "' = " << total << " kg CO2e\n"; // Cetak hasil
}

void cetakTerendah(BSTNode* root, int &count, int limit) { // Cetak emisi terendah
    if (root == nullptr || count >= limit) // Basis rekursi
        return;

    cetakTerendah(root->left, count, limit); // Traversal kiri
    if (count < limit) {     // Jika masih belum mencapai limit
        count++;             // Tambah counter
        cout << count << ". " << root->nama << " (" << root->emisi << " kg CO2e)" << endl;
    }
    cetakTerendah(root->right, count, limit); // Traversal kanan
}

void cetakTertinggi(BSTNode* root, int &count, int limit) { // Cetak emisi tertinggi
    if (root == nullptr || count >= limit) // Basis rekursi
        return;

    cetakTertinggi(root->right, count, limit); // Traversal kanan
    if (count < limit) {     // Jika masih dalam limit
        count++;             // Tambah counter
        cout << count << ". " << root->nama << " (" << root->emisi << " kg CO2e)" << endl;
    }
    cetakTertinggi(root->left, count, limit); // Traversal kiri
}

int main() {                 // Fungsi utama
    TrieNode* root = new TrieNode(); // Root Trie
    BSTNode* rootBST = nullptr;      // Root BST

    int pilihan;              // Variabel menu
    string nama;              // Nama lokasi
    float emisi;              // Nilai emisi

    // Data awal dimasukkan ke Trie dan BST
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

    while (true) {            // Loop menu utama
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
        cin >> pilihan;       // Input pilihan menu
        cin.ignore();         // Bersihkan buffer

        switch (pilihan) {    // Percabangan menu
        case 1:
            cout << "Nama lokasi: ";
            getline(cin, nama); // Input nama

            cout << "Emisi (kg CO2e): ";
            cin >> emisi;     // Input emisi
            cin.ignore();     // Bersihkan buffer

            tambahData(root, rootBST, nama, emisi); // Tambah data
            cout << "Berhasil ditambahkan!\n";
            break;

        case 2: {
            cout << "Masukkan nama lokasi: ";
            getline(cin, nama); // Input nama

            TrieNode* res = search(root, nama); // Cari di Trie

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
            startsWith(root, nama); // Tampilkan berdasarkan prefix
            break;

        case 4:
            cout << "Masukkan prefix: ";
            getline(cin, nama);
            sumEmisi(root, nama); // Hitung total emisi
            break;

        case 5:
            cout << "Semua Lokasi:\n";
            dfs(root, "");     // Tampilkan semua data
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
            return 0;         // Keluar program
        }

        default:
            cout << "Pilihan tidak valid!\n"; // Jika input salah
        }
    }
}
