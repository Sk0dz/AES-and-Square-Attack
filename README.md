# AES and Square Attack

## How to build the project

```console
mkdir build
cd build
cmake ..
make
```

The two executables *main* and *main_attack* will be generated. These executables are mainly a test of all the others functions together, not a real implementation of the AES to encrypt your own text.

## Structure of the project

The functions are split into 3 main .c files. The aes.c file contains the function useful for the AES encryption, the gmath.c file contains essentially the multiplication by x and the multiplication in the Rijndael field and the attack.c file contains the function for the square attack.

## Testing with you own values

You can modify the main.c and main_attack.c files with your own values to be sure the implementation works with anything. The variables for the key and the text are easy to find.

## Created By

Created by Victoria Martini, Quentin Bonnel and Paul Bureau under the supervision of Guilhem Castagnos, assistant professor of mathematics at the University of Bordeaux.
