# 🛒 Retail Management System

A robust, terminal-based C++ application designed to manage retail store operations. This system handles customer accounts, inventory management, secure purchases, and persistent data storage, all from a clean command-line interface.

---

## ✨ Features

* **Secure Authentication:** Users can create accounts and log in using a unique account name and passkey.
* **Persistent Data Storage:** Customer profiles, including wallet funds and reward tokens, are automatically saved to and loaded from a local text file (`client_records.txt`).
* **Dynamic Inventory System:** Browse a categorized store catalog (Apparel, Groceries, Produce) with real-time stock tracking.
* **Wallet & Transactions:** Users have a virtual wallet. Purchases automatically deduct from the wallet and update the store's inventory.
* **Loyalty Rewards:** Built-in reward token system where customers earn 1 token for every $10 spent.
* **Intuitive Dashboard:** A clean, easy-to-navigate CLI dashboard for logged-in users.

---

## 🛠️ Prerequisites

To compile and run this project, you will need a C++ compiler installed on your system. 

* **Windows:** [MinGW](https://www.mingw-w64.org/) or Visual Studio
* **macOS:** Xcode Command Line Tools (`clang++`)
* **Linux:** GCC (`g++`)

---

## 🚀 Getting Started

### 1. Clone the Repository
```bash
git clone [https://github.com/yourusername/retail-management-system.git](https://github.com/yourusername/retail-management-system.git)
cd retail-management-system
