# Floating-Point Precision

### The Basics

In digital systems, numbers are often represented using binary, where each digit (bit) can be either 0 or 1. The value of a binary number can be understood as a sum of powers of 2, with the power corresponding to the position of the bit. For instance, the number represented by the binary digits (bits) can be expressed as:

$$ \text{value} = \sum_{i=0}^{n-1} b_i \times 2^i $$

where $ b_i $ is the ith bit (0 or 1), and $ n $ is the number of bits.

#### Binary Representations
- **1-bit**: Can represent $ 2^1 = 2 $ states (0 or 1).
- **Nibble**: 4 bits can represent $ 2^4 = 16 $ states. Example: 
  - `0b0000` represents the smallest value (0).
  - `0b1111` represents the largest value (15).

- **Byte**: 8 bits can represent $ 2^8 = 256 $ states. Example:
  - `0b0000_0000` represents the smallest value (0).
  - `0b1111_1111` represents the largest value (255).

#### Hexadecimal Notation
Each hexadecimal digit can represent four binary digits (bits). This is because a single hex digit has 16 possible values (0-15), which matches the 16 possible combinations of four binary digits.

- **1 in Hexadecimal**: 
  - Decimal: $ 1_{10} $
  - Hexadecimal: $ 1_{16} $
  - Binary: $ 0b0001 $

- **Hexadecimal to Binary Conversion**:
  - $ 0x0 = 0b0000 $
  - $ 0x1 = 0b0001 $
  - $ 0x2 = 0b0010 $
  - ...
  - $ 0xF = 0b1111 $

Since each hex digit corresponds to four binary digits, a two-digit hex number represents eight binary digits. For example:

- $ 0x00 = 0b0000\_0000 $

Expanding this:

- $ 0x0000 = 0b0000\_0000\_0000\_0000 $

Here, $ 0x0000 $ represents 16 binary digits (bits), as each hex digit corresponds to four binary digits.

### Summary

Understanding these representations is crucial when converting between different floating-point formats, such as from a 32-bit to a 16-bit floating-point format. This conversion often involves adjusting the number of bits used for the sign, exponent, and mantissa, as well as handling potential loss of precision.

- **Nibble**: 4 bits, can represent 16 values.
- **Byte**: 8 bits, can represent 256 values.
- **Hexadecimal**: Each digit represents 4 binary digits, providing a compact representation for binary numbers.

#### Sign bit
The sign bit is the first, left-most, bit, which results in 0x80, respectively.

We represent the sign bit as $2^1$, e.g. $2^1 = 1 \times 2 = 2$

$$0x80 = 0b1000\_0000 = 128$$

We can follow this logic to validate it by scaling up to 16-bit to verify.

$$0x8000 = 1000\_0000\_0000\_0000 = 32\_768$$

We can scale up to 32-bit as well.

$$0x80000000 = 1000\_0000\_0000\_0000\_0000\_0000\_0000\_0000 = 2\_147\_483\_648$$

If we shift the sign bit over 24 places, we should land on the 8th bit, respectively.

```c
uint32_t sign = (f >> 24) & 0x80;
```

Since a 16-bit sign bit is defined as:

$$0b1\underbrace{......}_{30bits}$$

```c
uint32_t sign = (f >> 16) & 0x8000;
```

It should be represented like this for little endian systems, where the least significant bits are stored first and read right-to-left:

```c
uint32_t sign = (f >> 24) & 0x80;
```

This line of code checks whether the eighth bit in a set of 32 bits is set.

So, full representation for an unsigned range would be 0 - 255. For unsigned, it would be -126 - 127 depending on how we decide to implement it. It's probably best we just follow convention with this.

We can use this logic for the exponent and mantissa as well.

#### Exponent
The exponent is typically the following 8-bits after the sign bit. We decided for a 3-bit exponent.

We represent the exponent as $2^3$, e.g. $2^3 = 1 \times 2 \times 2 \times 2 = 8$

$0x70 = 0b0111\_0000 = 112$

This is where things get a little tricky for me as I don't have much experience with floating point precision and I've made a lot of attempts to understand it. Admittedly, it still eludes me, but I keep at it regardless, revisiting when I have the bandwidth for it.

#### Mantissa
The mantissa is followed 8-bits after the exponent bit set. We decided for a 4-bit mantissa.

We represent the exponent as $2^4$, e.g. $2^4 = 1 \times 2 \times 2 \times 2 \times 2 = 16$

$0x0f = 0b0000\_1111 = 15$

#### Conclusion

To be decided...

Your thought process is on the right track, especially in breaking down the components of the 8-bit floating-point representation into sign, exponent, and mantissa bits. Let's go through each part in detail to clarify and solidify the understanding.

### 1. **Sign Bit:**
   - **Position:** The sign bit is the most significant bit in the 8-bit format, which means it occupies the leftmost position.
   - **Value:** The sign bit indicates the sign of the number. If the bit is `0`, the number is positive; if it is `1`, the number is negative.
   - **Extraction:**
     - You correctly identified the method to extract the sign bit using `(f >> 7) & 0x1` where `f` is the 8-bit representation. This will correctly shift the sign bit to the least significant position and mask out the other bits.

### 2. **Exponent:**
   - **Position:** In an 8-bit format with a 3-bit exponent, the exponent bits follow the sign bit. These would be the next three bits.
   - **Bias:** The bias for the exponent is calculated as $2^{(N-1)} - 1$, where $N$ is the number of exponent bits. For 3 bits, the bias is $2^{2} - 1 = 3$.
   - **Value Range:** The range of the exponent field after biasing is from -3 to 4 (2^3 - 1 = 7).
   - **Extraction:**
     - The extraction can be done using `(f >> 4) & 0x07`, which shifts the bits right by 4 (to discard the mantissa bits) and masks the 3 bits for the exponent.

### 3. **Mantissa:**
   - **Position:** The mantissa follows the exponent bits and occupies the remaining bits in the 8-bit format. In this case, it uses the last four bits.
   - **Normalization:** In normalized form, the leading bit is implicit and not stored in the representation. However, in subnormal numbers, this implicit bit is not present, and all bits are used for the fraction.
   - **Extraction:**
     - The extraction can be done using `f & 0x0F`, which masks out the first 4 bits, leaving the mantissa bits.

### Example Calculation:
Consider an example byte `0b11001101` (in hexadecimal, 0xCD). Let's break it down:

1. **Sign Bit:** 
   - Extract with `(0xCD >> 7) & 0x1 = 1`. The sign is negative.
2. **Exponent:** 
   - Extract with `(0xCD >> 4) & 0x07 = 0b100 = 4`.
   - The exponent field is 4, but we must subtract the bias (3) to get the actual exponent: $4 - 3 = 1$.
3. **Mantissa:**
   - Extract with `0xCD & 0x0F = 0b1101 = 13`.
   - In normalized form, the implicit leading 1 bit gives us a fraction: $1.1101_2$.

### Summary:
- **Sign:** Negative (1)
- **Exponent:** 1 (after bias adjustment)
- **Mantissa:** 13 (0xD)

#### Formula for the Value:
The formula for computing the value is:
\[ (-1)^{\text{sign}} \times 2^{\text{exponent - bias}} \times 1.\text{mantissa bits} \]

### Moving Forward:
The next step is to implement the encoding and decoding functions based on these details. This will involve handling special cases such as zero, subnormal numbers, infinity, and NaN, as well as ensuring proper normalization of the mantissa.

```c
// Convert a 32-bit floating-point number to a 8-bit floating-point number
float8_t encode_float8(float value) {
    // TODO: This is not right, but uses 16-bit conversion as a guide
    uint32_t f = encode_float32(value);

    uint32_t sign     = (f >> 16) & 0x8000;
    uint32_t exponent = ((f >> 23) & 0xFF) - 127 + 15;
    uint32_t mantissa = (f & 0x007FFFFF) >> 13;

    if (exponent <= 0) {
        if (exponent < -10) {
            return 0;
        }
        mantissa   = (f & 0x007FFFFF) | 0x00800000;
        mantissa >>= (1 - exponent);
        return sign | (mantissa >> 13);
    } else if (exponent == 0xFF - (127 - 15)) {
        if (mantissa) {
            return sign | 0x7E00; // NaN
        } else {
            return sign | 0x7C00; // Infinity
        }
    } else if (exponent > 30) {
        return sign | 0x7C00; // Infinity
    }

    return sign | (exponent << 10) | mantissa;
}

// Convert a 16-bit floating-point number to a 8-bit floating-point number
float decode_float8(float8_t bits) {
    // TODO: This is not right, but uses 16-bit conversion as a guide
    uint32_t sign     = (bits & 0x8000) << 16;
    uint32_t exponent = (bits & 0x7C00) >> 10;
    uint32_t mantissa = (bits & 0x03FF) << 13;

    if (exponent == 0) {
        if (mantissa == 0) {
            return sign ? -0.0f : 0.0f;
        }
        while (!(mantissa & 0x00800000)) {
            mantissa <<= 1;
            exponent--;
        }
        exponent++;
        mantissa &= ~0x00800000;
    } else if (exponent == 0x1F) {
        return sign | (mantissa ? 0x7FC00000 : 0x7F800000);
    }

    exponent = exponent + (127 - 15);
    return decode_float32(sign | (exponent << 23) | mantissa);
}
```
