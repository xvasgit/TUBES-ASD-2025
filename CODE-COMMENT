#include <iostream>
#include <string>
#include <cctype>   // Untuk fungsi toupper (mengubah huruf kapital)
#include <fstream>  // Untuk operasi file (baca/tulis .txt)
using namespace std;

// Ukuran array child pada Trie
// 26 huruf (A-Z) + 10 angka (0-9) + 1 spasi = 37 slot
static const int ALPHABET_SIZE = 37; 

// ==========================================
// 1. STRUKTUR DATA TRIE
// Digunakan untuk pencarian cepat berdasarkan teks (Prefix/Nama)
// ==========================================
struct TrieNode {
    TrieNode* children[ALPHABET_SIZE]; // Pointer ke karakter berikutnya
    bool isEndOfWord;                  // Penanda bahwa ini adalah akhir dari sebuah kata
    float emisi;                       // Menyimpan data emisi di node terakhir kata

    // Constructor: Inisialisasi node baru (kosongkan anak, set emisi 0)
    TrieNode() {
        isEndOfWord = false;
        emisi = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            children[i] = nullptr;
    }
};

// ==========================================
// 2. STRUKTUR DATA BST (Binary Search Tree)
// Digunakan untuk mengurutkan data berdasarkan nilai Emisi (Ranking)
// ==========================================
struct BSTNode {
    string nama;
    float emisi;
    BSTNode *left, *right; // Pointer ke anak kiri (nilai lebih kecil) dan kanan (lebih besar)

    BSTNode(string n, float e) {
        nama = n;
        emisi = e;
        left = right = nullptr;
    }
};

// ==========================================
// FUNGSI BANTUAN (HELPER)
// ==========================================

// Mengubah seluruh string menjadi HURUF KAPITAL agar seragam
string toUpperCase(string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = toupper(s[i]);
    }
    return s;
}

// Mapping Karakter ke Index Array (0-36)
// A-Z -> index 0-25
// 0-9 -> index 26-35
// Spasi -> index 36
int charToIndex(char ch) {
    if (isalpha(ch))
        return toupper(ch) - 'A';       
    else if (isdigit(ch))
        return 26 + (ch - '0');         
    else if (ch == ' ')
        return 36;                      
    else
        return -1; // Abaikan karakter aneh (simbol dll)
}

// Kebalikannya: Mengubah Index Array kembali menjadi Karakter
// Dipakai saat menampilkan output (DFS)
char indexToChar(int idx) {
    if (idx < 26) return 'A' + idx;
    if (idx < 36) return '0' + (idx - 26);
    return ' ';
}

// ==========================================
// FUNGSI INSERT (MEMASUKKAN DATA)
// ==========================================

// Masukkan data ke TRIE (Logika Prefix)
void insertTrie(TrieNode* root, const string& word, float emisi) {
    TrieNode* curr = root;

    // Loop setiap karakter dalam kata (misal: "LAB")
    for (char ch : word) {
        int index = charToIndex(ch); 
        if (index == -1) continue; // Skip jika karakter tidak valid

        // Jika jalurnya belum ada, buat node baru
        if (curr->children[index] == nullptr)
            curr->children[index] = new TrieNode();

        // Pindah ke node anak tersebut
        curr = curr->children[index];
    }

    // Setelah loop selesai, tandai node terakhir sebagai akhir kata
    curr->isEndOfWord = true;
    curr->emisi = emisi;
}

// Masukkan data ke BST (Logika Sorting Nilai)
BSTNode* insertBST(BSTNode* root, string nama, float emisi) {
    // Jika pohon kosong, buat node akar baru
    if (root == nullptr) 
        return new BSTNode(nama, emisi);

    // Rekursif: Emisi lebih kecil ke KIRI, lebih besar/sama ke KANAN
    if (emisi < root->emisi)
        root->left = insertBST(root->left, nama, emisi);
    else
        root->right = insertBST(root->right, nama, emisi);

    return root;
}

// Wrapper: Satu fungsi untuk memasukkan data ke DUA struktur sekaligus
void tambahData(TrieNode* rootTrie, BSTNode*& rootBST, string nama, float emisi) {
    string namaKapital = toUpperCase(nama); // Standarisasi nama jadi kapital semua
    
    // 1. Simpan ke Trie (agar bisa dicari berdasarkan nama)
    insertTrie(rootTrie, namaKapital, emisi);
    
    // 2. Simpan ke BST (agar bisa diurutkan berdasarkan emisi)
    rootBST = insertBST(rootBST, namaKapital, emisi);
}

// ==========================================
// FUNGSI PENCARIAN (SEARCH & AUTOCOMPLETE)
// ==========================================

// Mencari kata yang PASTI (Exact Match)
TrieNode* search(TrieNode* root, const string& word) {
    TrieNode* curr = root;
    string wordKapital = toUpperCase(word); // Pastikan input user dikapitalkan

    for (char ch : wordKapital) {
        int index = charToIndex(ch);
        if (index == -1) continue;

        // Jika jalur terputus, berarti data tidak ada
        if (curr->children[index] == nullptr)
            return nullptr;

        curr = curr->children[index];
    }

    // Cek apakah ini benar-benar akhir kata yang disimpan
    if (curr->isEndOfWord)
        return curr;
    
    return nullptr;
}

// DFS (Depth First Search) untuk menelusuri Trie sampai ke ujung
// Digunakan untuk menampilkan semua kata yang berawalan tertentu
void dfs(TrieNode* node, const string& prefix) {
    // Jika node ini menyimpan data, cetak
    if (node->isEndOfWord) {
        cout << prefix << " - " << node->emisi << " kg CO2e\n";
    }

    // Cek ke semua kemungkinan anak (A-Z, 0-9, spasi)
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != nullptr) {
            // Rekursif: Lanjut ke anak dengan menambahkan karakter ke prefix
            dfs(node->children[i], prefix + indexToChar(i));
        }
    }
}

// Menampilkan SEMUA data di database
void displayAll(TrieNode* root) {
    dfs(root, "");
}

// Fitur Autocomplete: Menampilkan data berdasarkan awalan (Prefix)
// Contoh: Input "LAB" -> Output "LAB KIMIA", "LAB FISIKA"
void startsWith(TrieNode* root, const string& prefix) {
    TrieNode* curr = root;
    string prefixKapital = toUpperCase(prefix);

    // 1. Jalan dulu sesuai prefix yang diketik user
    for (char ch : prefixKapital) {
        int index = charToIndex(ch);
        if (index == -1 || curr->children[index] == nullptr) {
            cout << "Data tidak ditemukan." << endl;
            return;
        }
        curr = curr->children[index];
    }
    
    // 2. Setelah sampai di ujung prefix, lakukan DFS untuk cari kelanjutannya
    dfs(curr, prefixKapital);
}

// ==========================================
// HITUNG TOTAL EMISI (AGGREGASI)
// ==========================================

// Fungsi rekursif menjumlahkan semua emisi di bawah node tertentu
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

// Mencari prefix dulu, baru hitung total anak-anaknya
void sumEmisi(TrieNode* root, const string& prefix) {
    TrieNode* curr = root;
    string prefixKapital = toUpperCase(prefix); // Fix: harus di-uppercase

    // Navigasi ke prefix
    for (char ch : prefixKapital) {
        int index = charToIndex(ch);
        if (index == -1) continue;

        if (curr->children[index] == nullptr) {
            cout << "Tidak ada lokasi dengan awalan '" << prefix << "'\n";
            return;
        }
        curr = curr->children[index];
    }

    // Hitung total dari subtree
    float total = sumDFS(curr);
    cout << "Total Emisi untuk prefix '" << prefix
        << "' = " << total << " kg CO2e\n";
}


// ==========================================
// OUTPUT RANKING (MENGGUNAKAN BST)
// ==========================================

// Traversal In-Order (Kiri - Akar - Kanan) -> Hasil Terurut Kecil ke Besar
void cetakTerendah(BSTNode* root, int& count, int limit) {
    if (root == nullptr || count >= limit)
        return;

    cetakTerendah(root->left, count, limit); // Kunjungi yang kecil dulu
    
    if (count < limit) {
        count++;
        cout << count << ". " << root->nama << " (" << root->emisi << " kg CO2e)" << endl;
    }
    
    cetakTerendah(root->right, count, limit); // Baru yang besar
}

// Traversal Reverse In-Order (Kanan - Akar - Kiri) -> Hasil Terurut Besar ke Kecil
void cetakTertinggi(BSTNode* root, int& count, int limit) {
    if (root == nullptr || count >= limit)
        return;

    cetakTertinggi(root->right, count, limit); // Kunjungi yang besar dulu (kanan)
    
    if (count < limit) {
        count++;
        cout << count << ". " << root->nama << " (" << root->emisi << " kg CO2e)" << endl;
    }
    
    cetakTertinggi(root->left, count, limit); // Baru yang kecil (kiri)
}

// ==========================================
// FILE HANDLING (SIMPAN & LOAD TXT)
// ==========================================

// Menyisir Trie dan menulis ke file jika ketemu kata
void saveDFS(TrieNode* node, const string& prefix, ofstream& outFile) {
    if (node->isEndOfWord) {
        // Format di file: NAMA LOKASI|EMISI
        outFile << prefix << "|" << node->emisi << endl;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != nullptr) {
            saveDFS(node->children[i], prefix + indexToChar(i), outFile);
        }
    }
}

// Membuka file untuk ditulis
void saveToFile(TrieNode* root, const string& filename) {
    ofstream outFile(filename); // Membuka file mode write
    if (outFile.is_open()) {
        saveDFS(root, "", outFile);
        outFile.close();
        cout << "Data berhasil disimpan ke " << filename << endl;
    } else {
        cout << "Gagal membuka file\n";
    }
}

// Membaca file baris per baris
void loadFromFile(TrieNode* root, BSTNode*& rootBST, const string& filename) {
    ifstream inFile(filename); // Membuka file mode read
    if (!inFile.is_open()) return;

    string line;
    while (getline(inFile, line)) {
        // Mencari pemisah '|'
        size_t delimiterPos = line.find('|');
        if (delimiterPos != string::npos) {
            // Memecah string: sebelum '|' adalah nama, setelahnya adalah angka
            string nama = line.substr(0, delimiterPos);
            float emisi = stof(line.substr(delimiterPos + 1)); // String to Float
            
            tambahData(root, rootBST, nama, emisi); 
        }
    }
    inFile.close();
}

// ==========================================
// MEMORY MANAGEMENT (RESET)
// ==========================================
// Menghapus Trie dari memori agar bersih (mencegah memory leak)
void deleteTrie(TrieNode* node) {
    if (node == nullptr) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        deleteTrie(node->children[i]);
    }
    delete node;
}

// Menghapus BST dari memori
void deleteBST(BSTNode* node) {
    if (node == nullptr) return;
    deleteBST(node->left);
    deleteBST(node->right);
    delete node;
}


// ==========================================
// MAIN PROGRAM
// ==========================================
int main() {
    TrieNode* root = new TrieNode(); // Inisialisasi Root Trie
    BSTNode* rootBST = nullptr;      // Inisialisasi Root BST (kosong)
    string fileName = "database_emisi.txt";

    int pilihan;
    string nama;
    float emisi;

    // --- CEK FILE DATABASE SAAT APLIKASI MULAI ---
    ifstream cekFile(fileName);
    if (cekFile.is_open()) {
        cekFile.close();
        cout << "[Info] Memuat data dari file database...\n";
        loadFromFile(root, rootBST, fileName); // Load jika file ada
    } else {
        // Jika file tidak ada, isi dengan DATA DUMMY (Bawaan)
        cout << "[Info] File database tidak ditemukan, menggunakan data default.\n";
        tambahData(root, rootBST, "Laboratorium Kimia", 50);
        tambahData(root, rootBST, "Laboratorium Fisika", 40);
        tambahData(root, rootBST, "Laboratorium Bio", 30);
        tambahData(root, rootBST, "Kantin Utama", 20);
        tambahData(root, rootBST, "Kantin Teknik", 15);
        tambahData(root, rootBST, "Gedung A", 25);
        tambahData(root, rootBST, "Gedung B", 35);
        tambahData(root, rootBST, "Gedung Kuliah Umum", 60);
        tambahData(root, rootBST, "Perpustakaan", 10);
        tambahData(root, rootBST, "Pusat Riset Energi", 70);
        tambahData(root, rootBST, "Ruangan 2702", 12.5);
        tambahData(root, rootBST, "Ruangan 2703", 13.0);
    }


    while (true) {
        cout << "\n===== DATABASE EMISI LOKASI (TRIE & BST) =====\n";
        cout << "1. Tambah lokasi baru\n";
        cout << "2. Cari lokasi tertentu (Exact Match)\n";
        cout << "3. Cari lokasi berdasarkan awalan (Autocomplete)\n";
        cout << "4. Hitung total emisi satu gedung (berdasarkan prefix)\n";
        cout << "5. Tampilkan semua lokasi\n";
        cout << "6. Tampilkan Top N Terendah (Ranking BST)\n";
        cout << "7. Tampilkan Top N Tertinggi (Ranking BST)\n";
        cout << "8. Simpan perubahan ke file\n";
        cout << "9. Reset & Reload dari file\n";
        cout << "10. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore(); // Membersihkan buffer enter setelah cin angka

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
            if (!res) cout << "Lokasi tidak ditemukan!\n";
            else cout << "Emisi " << toUpperCase(nama) << " = " << res->emisi << " kg CO2e\n";
            break;
        }

        case 3: // Fitur Trie
            cout << "Masukkan prefix: ";
            getline(cin, nama);
            startsWith(root, nama);
            break;

        case 4: // Fitur Agregasi Trie
            cout << "Masukkan prefix: ";
            getline(cin, nama);
            sumEmisi(root, nama);
            break;

        case 5:
            cout << "Semua Lokasi:\n";
            displayAll(root);
            break;
            
        case 6: { // Fitur Sorting BST Ascending
            int n, counter = 0;
            cout << "Jumlah data terendah ditampilkan: ";
            cin >> n;
            cout << "\n--- " << n << " LOKASI DENGAN EMISI TERENDAH ---\n";
            cetakTerendah(rootBST, counter, n);
            break;
        }
        case 7: { // Fitur Sorting BST Descending
            int n, counter = 0;
            cout << "Jumlah data tertinggi ditampilkan: ";
            cin >> n;
            cout << "\n--- " << n << " LOKASI DENGAN EMISI TERTINGGI ---\n";
            cetakTertinggi(rootBST, counter, n);
            break;
        }
        case 8:
            saveToFile(root, fileName);
            break;

        case 9:
            // Hapus data di RAM dulu
            deleteTrie(root);
            deleteBST(rootBST);
            // Inisialisasi ulang
            root = new TrieNode();
            rootBST = nullptr;
            // Load ulang
            loadFromFile(root, rootBST, fileName);
            cout << "[Sistem] Data berhasil di-reset dan dimuat ulang.\n";
            break;

        case 10:
            cout << "Program selesai.\n";
            return 0;

        default:
            cout << "Pilihan tidak valid!\n";
        }
    }
}
