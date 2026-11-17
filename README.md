# intel-wifi-cipher-update
A simple C shim to harden intel wifi security from their DES/DES3 defaults to stronger OS-supported cipher suites (for iwd standalone - no NetworkManager!!)

## Instructions:
#### check ell version and config
```bash
pkg-config --modversion ell
pkg-config --variable=libdir ell
```

#### find out ell cipher suport
```bash
grep -n -A25 'enum l_cipher_type' /usr/include/ell/cipher.h
grep -n 'l_cipher_is_supported' /usr/include/ell/cipher.h
```

#### check which ciphers are supported by openssl
```bash
openssl ciphers -v
```

#### (assuming systemd 262) check out what your kernel config supports:
```bash
zgrep -E 'CONFIG_CRYPTO_(USER_API_SKCIPHER|ECB|CBC|DES)=' /proc/config.gz
# you should see sth like that:
# CONFIG_CRYPTO_DES=m
# CONFIG_CRYPTO_CBC=y
# CONFIG_CRYPTO_ECB=y
# CONFIG_CRYPTO_USER_API_SKCIPHER=m
```

#### also check for any cipher-related kernel modules that are loaded:
```bash
lsmod | grep -iE 'algif|des|hash'
```

#### compile and place the new c shim:
```bash
sudo cc -shared -fPIC -Wall -Wextra -o /usr/local/lib/libiwd-no-des-check.so des-check.c -ldl
```

#### if you want you can test for it when connecting:
```bash
sudo tshark -i wlan0 -Y 'tls.handshake.type == 2' -V
```

#### to reconfigure modules accordingly add (e.g. in /etc/modprobe.d/1-install.conf):
```bash
install des3_ede_x86_64           /bin/true
install des_generic               /bin/true
install libdes                    /bin/true
```

#### and (e.g. in /etc/modprobe.d/2-blacklist.conf):
```bash
blacklist des3_ede_x86_64
blacklist des_generic
blacklist libdes
```
