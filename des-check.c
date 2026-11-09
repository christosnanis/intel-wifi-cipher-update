#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <ell/cipher.h>
#include <dlfcn.h>

bool l_cipher_is_supported(enum l_cipher_type type) {
    static bool (*real_check)(enum l_cipher_type);

    if (type == L_CIPHER_DES ||
        type == L_CIPHER_DES_CBC ||
        type == L_CIPHER_DES3_EDE_CBC) {
        return true;
    }

    if (!real_check) {
        real_check = dlsym(RTLD_NEXT, "l_cipher_is_supported");
    }
    return real_check ? real_check(type) : false;
}
