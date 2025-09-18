#include <iostream>
#include <string>
#include <vector>

#include "bank.h"
#include "bank_customer.h"
#include "seller.h"
#include "buyer.h"
#include "item.h"

using namespace std;

/**
 * @brief Fungsi untuk menyimulasikan proses transaksi pembelian.
 * * @param buyer Objek pembeli.
 * @param seller Objek penjual.
 * @param itemId ID item yang akan dibeli.
 * @param quantity Jumlah item yang akan dibeli.
 */
void prosesPembelian(Buyer& buyer, Seller& seller, int itemId, int quantity) {
    cout << "\n=============================================" << endl;
    cout << "MEMPROSES PEMBELIAN: " << buyer.getName() << " membeli dari " << seller.getName() << endl;
    cout << "=============================================" << endl;

    Item* itemToBuy = seller.findItem(itemId);

    // 1. Validasi apakah item ada
    if (!itemToBuy) {
        cout << "Transaksi Gagal: Item dengan ID " << itemId << " tidak ditemukan di toko " << seller.getName() << "." << endl;
        return;
    }

    // 2. Validasi stok
    if (itemToBuy->getQuantity() < quantity) {
        cout << "Transaksi Gagal: Stok tidak mencukupi. Stok saat ini: " << itemToBuy->getQuantity() << ", permintaan: " << quantity << "." << endl;
        return;
    }

    double totalPrice = itemToBuy->getPrice() * quantity;

    // 3. Validasi saldo pembeli
    if (buyer.getAccount()->getBalance() < totalPrice) {
        cout << "Transaksi Gagal: Saldo " << buyer.getName() << " tidak mencukupi." << endl;
        cout << "Dibutuhkan: $" << totalPrice << ", Saldo: $" << buyer.getAccount()->getBalance() << endl;
        return;
    }

    // 4. Proses transaksi
    cout << "Validasi berhasil. Memulai proses transaksi..." << endl;
    
    // Kurangi saldo pembeli
    buyer.getAccount()->withdraw(totalPrice);

    // Proses penjualan di sisi penjual (akan menambah saldo penjual dan mengurangi stok)
    double salePrice = 0; // Variabel ini akan diisi oleh method sellItem
    seller.sellItem(itemId, quantity, salePrice);

    cout << "\n--- TRANSAKSI BERHASIL ---" << endl;
    cout << buyer.getName() << " berhasil membeli " << quantity << " " << itemToBuy->getName() << " seharga $" << totalPrice << endl;
    cout << "Saldo baru " << buyer.getName() << ": $" << buyer.getAccount()->getBalance() << endl;
    cout << "Saldo baru " << seller.getName() << ": $" << seller.getAccount()->getBalance() << endl;
    cout << "=============================================\n" << endl;
}


int main() {
    // 1. Inisialisasi Bank
    cout << "--- Inisialisasi Sistem ---" << endl;
    Bank aBank("Bank Digital Sejahtera");

    // 2. Membuat dan menambahkan nasabah ke bank
    aBank.addCustomer(BankCustomer(101, "Budi Santoso", 1500.00)); // Penjual
    aBank.addCustomer(BankCustomer(201, "Ani Lestari", 800.00));   // Pembeli
    aBank.addCustomer(BankCustomer(202, "Charlie", 200.00));      // Pembeli lain

    aBank.printAllCustomers();

    // 3. Membuat Penjual dan Pembeli
    // PENTING: Ambil pointer akun dari objek Bank untuk memastikan konsistensi data
    BankCustomer* sellerAccount = aBank.findCustomer(101);
    BankCustomer* buyerAccount1 = aBank.findCustomer(201);
    BankCustomer* buyerAccount2 = aBank.findCustomer(202);

    if (!sellerAccount || !buyerAccount1 || !buyerAccount2) {
        cout << "Error: Gagal menemukan akun nasabah. Program berhenti." << endl;
        return 1;
    }

    Seller seller(1, "Toko Kelontong Budi", sellerAccount);
    Buyer buyer1(1, "Ani", buyerAccount1);
    Buyer buyer2(2, "Charlie", buyerAccount2);

    // 4. Penjual menambahkan barang ke inventaris
    cout << "\n--- Penjual Menambahkan Inventaris ---" << endl;
    seller.addItem(Item(1001, "Buku Tulis", 2.50, 50));
    seller.addItem(Item(1002, "Pensil 2B", 0.50, 100));
    seller.addItem(Item(1003, "Penghapus", 1.00, 75));
    
    seller.printInventory();
    seller.printSellerInfo();
    
    // 5. Skenario Transaksi
    // Ani membeli 10 buku tulis dan 5 pensil
    prosesPembelian(buyer1, seller, 1001, 10);
    prosesPembelian(buyer1, seller, 1002, 5);

    // Charlie mencoba membeli 80 penghapus (stok cukup, tapi uang tidak)
    prosesPembelian(buyer2, seller, 1003, 80);

    // Charlie membeli 20 penghapus (sukses)
    prosesPembelian(buyer2, seller, 1003, 20);

    // 6. Penjual mengupdate harga barang
    cout << "\n--- Penjual Mengupdate Harga Barang ---" << endl;
    seller.updateItemPrice(1001, 2.75); // Harga buku naik
    
    // 7. Tampilkan status akhir
    cout << "\n--- STATUS AKHIR SISTEM ---" << endl;
    seller.printInventory();
    aBank.printAllCustomers();
    aBank.printBankInfo();

    cout << "\n--- SIMULASI SELESAI ---" << endl;

    return 0;
}
