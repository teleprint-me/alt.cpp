# Floating-Point Precision

## The Basics

In digital systems, numbers are often represented using binary, where each digit (bit) can be either 0 or 1. The value of a binary number can be understood as a sum of powers of 2, with the power corresponding to the position of the bit. For instance, the number represented by the binary digits (bits) can be expressed as:

$$ \text{value} = \sum_{i=0}^{n-1} b_i \times 2^i $$

where $ b_i $ is the ith bit (0 or 1), and $ n $ is the number of bits.

### Binary Representations
- **1-bit**: Can represent $ 2^1 = 2 $ states (0 or 1).
- **Nibble**: 4 bits can represent $ 2^4 = 16 $ states. Example: 
  - `0b0000` represents the smallest value (0).
  - `0b1111` represents the largest value (15).

- **Byte**: 8 bits can represent $ 2^8 = 256 $ states. Example:
  - `0b0000_0000` represents the smallest value (0).
  - `0b1111_1111` represents the largest value (255).

### Hexadecimal Notation
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

Understanding these representations is crucial when converting between different floating-point formats, such as from a 32-bit to a 16-bit floating-point format. This conversion often involves adjusting the number of bits used for the sign, exponent, and mantissa, as well as handling potential loss of precision.

- **Nibble**: 4 bits, can represent 16 values.
- **Byte**: 8 bits, can represent 256 values.
- **Hexadecimal**: Each digit represents 4 binary digits, providing a compact representation for binary numbers.

As a simple example, the value $0b1101$ can be calculated as:

$$ 0b1101 = (2^3 \times 1) + (2^2 \times 1) + (2^1 \times 0) + (2^0 \times 1) = 8 + 4 + 1 = 13 $$

This example illustrates the calculation for a 4-bit binary representation in base 2 to its decimal representation in base 10.

## A General Overview

### Formula for the Value:
The value of a floating-point number can be calculated using the formula:

$$ (-1)^{\text{sign}} \times 2^{\text{exponent} - \text{bias}} \times 1.\text{mantissa} $$

Let's consider an 8-bit floating-point format for simplicity, though these principles apply to any floating-point representation. In our defined format:

- The **sign** bit is 1 bit.
- The **exponent** is 3 bits.
- The **mantissa** is 4 bits.

This totals to an 8-bit format.

### 1. **Sign Bit:**
   - **Position:** The sign bit is the most significant bit (MSB) and occupies the leftmost position.
   - **Value:** It indicates the sign of the number. If the bit is `0`, the number is positive; if it is `1`, the number is negative.
   - **Extraction:** To extract the sign bit, shift the number right by 7 bits and mask with `0x1`. For a number `x`, this is done as `(x >> 7) & 0x1`.

### 2. **Exponent:**
   - **Position:** The exponent follows the sign bit. With 3 bits, it occupies positions 2 through 4.
   - **Bias:** The exponent has a bias calculated as $ 2^{(N-1)} - 1 $, where $ N $ is the number of exponent bits. For 3 bits, the bias is $ 2^2 - 1 = 3 $.
   - **Value Range:** The exponent can range from $-\text{bias}$ to $ 2^3 - 1 - \text{bias} $, which is from -3 to 4.
   - **Extraction:** Extract the exponent bits by shifting the number right by 4 and masking with `0x07` (3 bits). For a number `x`, this is done as `(x >> 4) & 0x07`.

### 3. **Mantissa:**
   - **Position:** The mantissa occupies the remaining bits, from positions 5 through 8 in our 8-bit format.
   - **Normalization:** In normalized form, the leading bit is implicit and assumed to be 1. For subnormal numbers, this leading bit is not present, and the value is represented only by the mantissa bits.
   - **Extraction:** Extract the mantissa bits by masking with `0x0F`. For a number `x`, this is done as `x & 0x0F`.

### Example Calculation:
Consider the example byte `0b11001101` (hexadecimal `0xCD`). Let's break it down:

1. **Sign Bit:** 
   - Extract: `(0xCD >> 7) & 0x1 = 1`
   - The sign is `1`, indicating the number is negative.

2. **Exponent:** 
   - Extract: `(0xCD >> 4) & 0x07 = 0b100 = 4`
   - Actual Exponent: $ 4 - 3 = 1 $

3. **Mantissa:**
   - Extract: `0xCD & 0x0F = 0b1101 = 13`
   - Normalized form includes an implicit leading 1, so the mantissa becomes `1.1101` in binary.

### Summary:

- **Sign:** Negative (1)
- **Exponent:** 1 (after bias adjustment)
- **Mantissa:** 13 (0xD)

Thus, the value is calculated as:

$$ (-1)^1 \times 2^{1} \times 1.1101_2 = -2 \times (1 + 13/16) = -2 \times 1.8125 = -3.625 $$

This example illustrates the breakdown of an 8-bit floating-point format into its sign, exponent, and mantissa components, as well as the calculation of the represented value.

## Mapping Representations in C

### Overview
In hardware, there's no native support for a 16-bit floating-point format, so we often use an integer type to represent these smaller formats. By manipulating the bits, we can encode and decode these representations as needed. This process is straightforward in C and C++ due to their low-level memory access, while languages like Python and JavaScript require more effort to emulate the same functionality.

### Aliasing for Clarity
First, we alias a type for clarity:

```c
typedef uint32_t float32_t;
```

Here, `float32_t` serves as an alias for a 32-bit unsigned integer, which will hold the bit representation of a 32-bit floating-point number.

### Using a Union for Type Mapping
To facilitate conversion between the float and its bit representation, we use a `union`:

```c
// Union to map float to internal integer representation
typedef union {
    float32_t bits; // 32-bit integer representation
    float     value; // Floating-point value
} float_data_t;
```

In this union, `float_data_t`, the `bits` member represents the float as a 32-bit integer, while the `value` member represents the float in its usual floating-point form. The union allows us to access the same data in memory as either a float or its bitwise integer representation.

### Encoding and Decoding Functions
The functions to encode and decode between these representations are straightforward:

```c
// Function to encode a float into its IEEE-754 binary32 representation
float32_t encode_float32(float value) {
    float_data_t f32;
    f32.value = value;
    return f32.bits;
}

// Function to decode an IEEE-754 binary32 representation into a float
float decode_float32(float32_t bits) {
    float_data_t f32;
    f32.bits = bits;
    return f32.value;
}
```

- **`encode_float32` Function**: This function takes a `float` as input and returns its bit representation as a `float32_t`. We store the float in the union's `value` member and then return the `bits` member, which contains the same data in integer form.

- **`decode_float32` Function**: This function takes a `float32_t` representing the bitwise data of a float and converts it back to the `float` type. We assign the integer bits to the union's `bits` member and return the `value` member, retrieving the original floating-point value.

### Purpose and Future Directions
This setup serves as a foundation for converting between higher precision (e.g., 32-bit float) and lower precision formats (e.g., 16-bit float, 8-bit custom formats). Lower precision formats often lead to precision loss due to fewer bits being used to represent the number, which is an important consideration in such conversions.

We will continue to build on this foundation to implement conversions to formats like float16, bfloat16, and custom 8-bit formats, where precision considerations become even more critical.

## The Sign bit
To keep things simple, we'll focus on a 8-bit format. We can use 16-bit (half-precison) and 32-bit (full-precision) for reference.

The sign bit is the first, left-most, bit, which results in $0x80$ for a 8-bit format, respectively.

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

## The exponent bits
The exponent is typically the following 8-bits after the sign bit. We decided for a 3-bit exponent.

We represent the exponent as $2^3$, e.g. $2^3 = 1 \times 2 \times 2 \times 2 = 8$

$0x70 = 0b0111\_0000 = 112$

This is where things get a little tricky for me as I don't have much experience with floating point precision and I've made a lot of attempts to understand it. Admittedly, it still eludes me, but I keep at it regardless, revisiting when I have the bandwidth for it.

## The mantissa bits
The mantissa is followed 8-bits after the exponent bit set. We decided for a 4-bit mantissa.

We represent the exponent as $2^4$, e.g. $2^4 = 1 \times 2 \times 2 \times 2 \times 2 = 16$

$0x0f = 0b0000\_1111 = 15$

#### Conclusion

To be decided...

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
