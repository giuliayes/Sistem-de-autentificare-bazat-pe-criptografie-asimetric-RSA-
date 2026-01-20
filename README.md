# 🔐 RSA-Shield | Asymmetric Cryptography Authentication

[![Security](https://img.shields.io/badge/Security-RSA%20%7C%20Cryptography-007acc.svg)](#)
[![Tech](https://img.shields.io/badge/Tech-C%2B%2B%20%7C%20OpenSSL-00a8e8.svg)](#)
[![Status](https://img.shields.io/badge/Status-Secure%20Prototype-003366.svg)](#)

A robust authentication system built on the principles of Asymmetric Cryptography (RSA), ensuring secure user verification without ever transmitting sensitive passwords in plain text.

### 🛡️ Project Overview
This project implements a secure login and registration pipeline using the **RSA (Rivest-Shamir-Adleman)** algorithm. By leveraging public and private key pairs, the system ensures that data integrity and confidentiality are maintained during the authentication process.

The core objective is to demonstrate the practical application of number theory in modern cybersecurity, specifically focusing on key generation, encryption, and digital signatures.



<br clear="left"/>

---

## ✨ Key Features
* **Key Pair Generation:** Automatic creation of unique 2048-bit RSA keys.
* **Encrypted Handshake:** Secure transmission of credentials using public-key encryption.
* **Digital Signatures:** Identity verification through private-key signing to prevent MitM attacks.
* **Secure Storage:** Implementation of hashing and salting techniques for local databases.

## 🛠️ Technical Breakdown
The system manages the cryptographic lifecycle across three main stages:

1.  **Key Exchange** 🔑: Distributing the public key while keeping the private key isolated.
2.  **Encryption Pipeline** 🏗️: Using modular exponentiation to encrypt sensitive buffers.
3.  **Authentication Logic** ✅: Comparing decrypted challenges to verify private key ownership.

---

## 💻 Tech Stack
| Component | Technology |
| :--- | :--- |
| **Language** | C++ |
| **Cryptography** | RSA Algorithm |
| **Libraries** | OpenSSL / Crypto++ |
| **Data Format** | PEM / Base64 Encoding |

---

## 🚀 Usage
1. **Generate Keys:** Run the key generator utility to create your `.pem` files.
2. **Register:** Create a new user identity; the public key is stored on the "server."
3. **Authenticate:** Log in using your private key to sign the authentication challenge.

---
*Developed as a deep dive into Cyber Security and Asymmetric Encryption.*
