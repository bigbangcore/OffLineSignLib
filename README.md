# OffLineSignLib
Mainly offer the feature of making new address, signing transaction on off-line.

# Installation

## ubuntu 18.04

- first, install dependency for the project 

```shell
sudo apt install cmake libsodium-dev
```

- second, run INSTALL.sh

```shell
./INSTALL.sh
```

# Features

- Support normal public key address generation.

- Only support public key address signature, DO NOT support template address signature.

- Serialization and Deserialization of Transaction

# Document

See signlib.h, here are comments for exported API.

For Java SDK, pelease read the document [here](java/README.md) to get more details.
