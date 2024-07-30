# Floating-point Precision

### The Basics
Each bit representation can be considered as power to a base of 2, e.g. $2^n$ where $n$ represents the number of binary digits and $2$ represents the number of states for each binary digit.

If we have 1 binary digit, then we have $2^1 = 1 \times 2 = 2$ states. Typically, we have at least 4-bits which can be represented as a **nibble** and can be defined as $2^4 = 1 \times 2 \times 2 \times 2 \times 2 = 16$ resulting in 16 binary states.

A **byte** can be defined as $2^8 = 256$ which represents 256 possible states, respectively.

We only have 4 bits for each **nibble**:

$$0b0000$$

Following this logic, 8 bits represents each **byte**:

$$0b0000\_0000$$

Each hexadecimal value represents 4 bits, e.g.,

$$1_{10} = 1_{16} = 0001_2$$

or

$$1 = 0x1 = 0b0001$$

Since $0x0$ represents 4 binary digits:

$$0x0 = 0b0000$$

It follows that each hex digit adds an extra group of 4 binary digits, so $0x00$ represents eight binary digits:

$$0x00 = 0b0000\_0000$$

In turn, $0x0000$ represents 16 binary digits or 4 hexadecimal digits times 4 groups of binary digits, resulting in 16 binary digits:

$$0x0000 = 0b0000\_0000\_0000\_0000$$

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
   - **Bias:** The bias for the exponent is calculated as \(2^{(N-1)} - 1\), where \(N\) is the number of exponent bits. For 3 bits, the bias is \(2^{2} - 1 = 3\).
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
   - The exponent field is 4, but we must subtract the bias (3) to get the actual exponent: \(4 - 3 = 1\).
3. **Mantissa:**
   - Extract with `0xCD & 0x0F = 0b1101 = 13`.
   - In normalized form, the implicit leading 1 bit gives us a fraction: \(1.1101_2\).

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
