# Floating-Point Precision

## Contents

- [The Basics](#the-basics)
- [A General Overview](#a-general-overview)
- [Mapping Representations in C](#mapping-representations-in-c)
- [The Sign Bit](#the-sign-bit)
- [The Exponent Bits](#the-exponent-bits)
- [The Mantissa Bits](#the-mantissa-bits)

## The Basics

In digital systems, numbers are often represented using binary, where each digit (bit) can be either 0 or 1. The value of a binary number can be understood as a sum of powers of 2, with the power corresponding to the position of the bit. For instance, the number represented by the binary digits (bits) can be expressed as:

$$ \text{value} = \sum_{i=0}^{n-1} b_i \times 2^i $$

where $b_i$ is the ith bit (0 or 1), and $n$ is the number of bits.

### Binary Representations
- **1-bit**: Can represent $2^1 = 2$ states (0 or 1).
- **Nibble**: 4 bits can represent $2^4 = 16$ states. Example: 
  - `0b0000` represents the smallest value (0).
  - `0b1111` represents the largest value (15).

- **Byte**: 8 bits can represent $2^8 = 256$ states. Example:
  - `0b0000_0000` represents the smallest value (0).
  - `0b1111_1111` represents the largest value (255).

### Hexadecimal Notation
Each hexadecimal digit can represent four binary digits (bits). This is because a single hex digit has 16 possible values (0-15), which matches the 16 possible combinations of four binary digits.

- **1 in Hexadecimal**: 
  - Decimal: $1_{10}$
  - Hexadecimal: $1_{16}$
  - Binary: $0b0001$

- **Hexadecimal to Binary Conversion**:
  - $0x0 = 0b0000$
  - $0x1 = 0b0001$
  - $0x2 = 0b0010$
  - ...
  - $0xF = 0b1111$

Since each hex digit corresponds to four binary digits, a two-digit hex number represents eight binary digits. For example:

- $0x00 = 0b0000\_0000$

Expanding this:

- $0x0000 = 0b0000\_0000\_0000\_0000$

Here, $0x0000$ represents 16 binary digits (bits), as each hex digit corresponds to four binary digits.

### Example Calculation:
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
   - **Bias:** The exponent has a bias calculated as $2^{(N-1)} - 1$, where $N$ is the number of exponent bits. For 3 bits, the bias is $2^2 - 1 = 3$.
   - **Value Range:** The exponent can range from $-\text{bias}$ to $2^3 - 1 - \text{bias}$, which is from -3 to 4.
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
   - Actual Exponent: $4 - 3 = 1$

3. **Mantissa:**
   - Extract: `0xCD & 0x0F = 0b1101 = 13`
   - Normalized form includes an implicit leading 1, so the mantissa becomes `1.1101` in binary.

We can define the sign, exponent, and mantissa with the following values:

- **Sign:** Negative (1)
- **Exponent:** 1 (after bias adjustment)
- **Mantissa:** 13 (0xD)

Then we can calculate the value as:

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

## The Sign Bit

### Overview
In floating-point representations, the sign bit determines whether the number is positive or negative. We will discuss the extraction of the sign bit in various formats, focusing on an 8-bit format for simplicity and using 16-bit (half-precision) and 32-bit (single-precision) formats for reference.

### Bit Representation

For an 8-bit floating-point format:
- **Sign bit**: The most significant bit (MSB) determines the sign of the number.

Representation:
- **8-bit format**: Sign bit mask is `0x80`, which equals `0b10000000` in binary or `128` in decimal.

For comparison:
- **16-bit format**: Sign bit mask is `0x8000` (`0b1000000000000000` in binary or `32,768` in decimal).
- **32-bit format**: Sign bit mask is `0x80000000` (`0b10000000000000000000000000000000` in binary or `2,147,483,648` in decimal).

### Sign Bit Extraction

The sign bit extraction process involves shifting the number to position the sign bit at the least significant bit (LSB) and then masking it.

#### 8-bit Format Extraction

For an assumed layout:
- **1 bit** for the sign (MSB)
- **3 bits** for the exponent
- **4 bits** for the mantissa

To extract the sign bit:

```c
uint32_t sign = (x >> 7) & 0x1;
```

- **Shift Right by 7 Bits**: Moves the sign bit to the LSB position.
- **Mask with `0x1`**: Ensures only the sign bit remains.

**Example**: If `x = 0b11001101`, shifting right by 7 yields `0b00000001`. Masking with `0x1` results in `0b1` (sign = 1).

#### 16-bit Format Extraction

For a typical layout:
- **1 bit** for the sign (MSB)
- **5 bits** for the exponent
- **10 bits** for the mantissa

To extract the sign bit:

```c
uint32_t sign = (f >> 15) & 0x1;
```

- **Shift Right by 15 Bits**: Positions the sign bit at the LSB.
- **Mask with `0x1`**: Isolates the sign bit.

Alternatively, to directly check the sign bit without shifting:

```c
uint32_t sign = f & 0x8000;
```

- **Mask with `0x8000`**: Isolates the sign bit in the original 16-bit representation.

### Key Considerations

1. **Bit Size and Shift Amount**: The amount shifted corresponds to the position of the sign bit in the representation. For an 8-bit format, the sign bit is at position 7; for a 16-bit format, it is at position 15.

2. **Masking**: After shifting, masking with `0x1` isolates the sign bit. Alternatively, directly masking (e.g., `0x8000` for 16-bit) can be used without shifting.

3. **Endianness**: While the bitwise extraction logic remains consistent, endianness affects the byte order in memory. This section assumes a big-endian representation, where the most significant byte is stored first.

### Conclusion
Extracting the sign bit from a floating-point number involves determining the correct bit position and applying appropriate bitwise operations. This ensures accurate extraction and interpretation of the sign bit across different floating-point formats.

## The Exponent Bits

### Overview

In floating-point numbers, the exponent field follows the sign bit. It determines the scale of the number by representing powers of two. For this section, we focus on an 8-bit floating-point format with a 3-bit exponent, as opposed to standard formats which typically use more bits for the exponent.

### Bit Representation

For our custom 8-bit format:
- **1 bit** for the sign (MSB)
- **3 bits** for the exponent
- **4 bits** for the mantissa

The exponent field's value is stored in a biased format, where a bias is added to the actual exponent value. This helps represent both positive and negative exponents.

### Calculation

Given the layout:
- **3-bit exponent** can represent values from `0` to `7`.
- The exponent is stored as an unsigned integer and uses a bias to include negative exponents. 

**Bias Calculation**: For an `n`-bit exponent, the bias is calculated as $2^{n-1} - 1$.

For a 3-bit exponent:

$$ \text{Bias} = 2^{3-1} - 1 = 3 $$

The actual exponent ($E$) can be calculated by subtracting the bias from the stored exponent value.

**Example**:
- If the exponent field stores the value `5`, the actual exponent is $5 - 3 = 2$.

### Extraction of the Exponent Bits

To extract the exponent bits from the floating-point representation, we isolate the relevant bits.

```c
float32_t exponent = (bits >> 4) & 0x07;
```

#### Breakdown:
1. **Shift Right by 4 Bits**: Shifts the bits right, moving the exponent field to the least significant position.
2. **Mask with `0x07`**: Isolates the 3 bits of the exponent field. `0x07` in binary is `0b00000111`, which corresponds to the lowest three bits set to 1.

**Example**:
- If `bits = 0b01111000`, shifting right by 4 bits results in `0b00000111`. Masking with `0x07` gives `0b00000111`, which is `7` in decimal.

### Exponent Range and Interpretation

For the 3-bit exponent:
- **Unbiased Range**: 0 to 7
- **Bias**: 3

**Exponent Value (E)**:
- The exponent value in the number representation is calculated as $E = \text{exponent bits} - \text{bias}$.
- The actual range of the exponent ($E$) is $-3$ to $4$.

### Special Cases

1. **Exponent of All Zeros (`0b000`)**:
   - Represents a denormalized number (if the mantissa is not zero) or zero (if the mantissa is zero).
   - Exponent value $E = -\text{bias} + 1$.

2. **Exponent of All Ones (`0b111`)**:
   - Represents infinity (if the mantissa is zero) or NaN (if the mantissa is not zero).

### Example Calculation

Let's say we have an 8-bit floating-point number with the following bit pattern: `0b01011100`.

1. **Sign Bit**: `0` (positive number)
2. **Exponent Bits**: `0b101` (after extraction, the value is 5)
3. **Mantissa Bits**: `0b1100`

**Calculating the Exponent**:
- Extracted exponent value: 5
- Bias: 3
- Actual exponent $E$ = 5 - 3 = 2

Thus, the number's scale is $2^2 = 4$.

### Conclusion

Extracting and interpreting the exponent bits involves understanding the bit layout, applying the bias correctly, and recognizing special cases like zeros and NaNs. This is a fundamental aspect of working with floating-point numbers, enabling accurate representation and calculations of a wide range of values.

## The mantissa bits
The mantissa is followed 8-bits after the exponent bit set. We decided for a 4-bit mantissa.

We represent the exponent as $2^4$, e.g. $2^4 = 1 \times 2 \times 2 \times 2 \times 2 = 16$

$0x0f = 0b0000\_1111 = 15$

## 8-bit floating-point implementation in C
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

## Conclusion

To be decided...
