/*
 * CS:APP Data Lab
 *
 * rax.yang
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */

#endif

#define toBool(x) (!(!(x)))
#define INTMIN    (0X80 << 24)

int negate(int x);
int conditional(int x, int y, int z);

inline static int sub(int x, int y)
{
    // x-y = x+(y的补)，需要注意x-y是可能溢出的，但仅有负数减正数才会溢出
    return x + negate(y);
}

inline static int isLess_SameFlag(x, y)
{
    return toBool((sub(x, y) & INTMIN));
}

inline static int isLess(int x, int y)
{
    int flag_x = x & INTMIN;
    int flag_y = y & INTMIN;
    int isFlagNotEqual = flag_x ^ flag_y;

    return conditional(isFlagNotEqual, toBool(flag_x >> 31), isLess_SameFlag(x, y));
}

inline static int isEqual(int x, int y)
{
    return !(x ^ y);
}

inline static int isNegative(int x)
{
    return toBool(x & INTMIN);
}

// 1
/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y)
{
    return (~(x & y)) & (~((~x) & (~y)));
}
/*
 * tmin - return minimum two's complement integer
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void)
{
    return (0x01 << 31);
}
// 2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x)
{
    // 1.异或的特点：对于x^y，若x和y完全相同，则结果为0，否则结果为1，可用于判断x是否符合某种特定的位模式；
    return !(x ^ (0x7FFFFFFF));
}
/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x)
{
    return !((x & 0xAAAAAAAA) ^ 0xAAAAAAAA);
}
/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x)
{
    // 按位取反再+1，就是变负值
    return (~x) + 1;
}
// 3
/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x)
{
    int isBits_8to31_illegal = (x & 0XFFFFFF00);
    int isBits_4to7_illegal = ((x >> 4) ^ 0x03);
    int isBits_0to4_illegel = (!((x & 0x0A) ^ 0x0A) | !((x & 0x0C) ^ 0x0C));

    // printf("x = %x\r\n", x);
    // printf("isBits_8to31_illegal = %x\r\n", isBits_8to31_illegal);
    // printf("isBits_4to7_illegal = %x\r\n", isBits_4to7_illegal);
    // printf("isBits_0to4_illegel = %x\r\n", isBits_0to4_illegel);

    return !(isBits_8to31_illegal | isBits_4to7_illegal | isBits_0to4_illegel);
}
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z)
{
    int x_is_0 = !x;
    int y_mask = x_is_0 - 1;
    int z_mask = ~y_mask;

    int ret = ((y & y_mask) | (z & z_mask));
    return ret;
}
/*
 * isLessOrEqual - if x <= y  then return 1, else return 0
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y)
{
    return isEqual(x, y) | isLess(x, y);
}
// 4
/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int logicalNeg(int x)
{
    return conditional(x, 0, 1);
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x)
{
    // 1、正数：找到最高位的1即可，注意要在最高位加一个0，因为在补码表示中最高位是负值；
    // 2、负数：对于补码表示的负数，高位连续的1是可被截断的（数值不变），因此只需要找到最高位的0，并在最高位加一个1即可
    // 3、总的来说：正数找到最高位的1，负数找到最高位的0，并同时加上1位即可

    x = conditional(isNegative(x), ~x, x); // 将负数取反，和正数一样找1

    int hbit16 = (toBool(x >> 16)) << 4;
    x = x >> hbit16;

    int hbit8 = (toBool(x >> 8)) << 3;
    x = x >> hbit8;

    int hbit4 = (toBool(x >> 4)) << 2;
    x = x >> hbit4;

    int hbit2 = (toBool(x >> 2)) << 1;
    x = x >> hbit2;

    int hbit1 = (toBool(x >> 1)) << 0;
    x = x >> hbit1;

    int bit0 = toBool(x);

    return hbit16 + hbit8 + hbit4 + hbit2 + hbit1 + bit0 + 1;
}
// float
/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
#define EXP_MASK            0x7F800000
#define FRAC_MASK           0x007FFFFF
#define isSPECIAL_VALUE(uf) (((uf) & (EXP_MASK)) == EXP_MASK)
#define isDENORMALIZED(uf)  (((uf) & (EXP_MASK)) == 0)
#define isFLOAT_ZERO(uf)    (((uf) & 0x7FFFFFFF) == 0)
#define EXP_INCREASE(uf)    ((uf) + (0x01 << 23))
#define GET_SIGN(uf)        ((uf) & INTMIN)

// 非规格化数exp是全0,左移1位之后,符号位必为0,因此或上原来的符号位即可
#define SHIFT_LEFT_EXPECT_SIGN(uf) ((uf) << 1) | (GET_SIGN(uf))

unsigned floatScale2(unsigned uf)
{
    // 1. 特殊值
    if (isSPECIAL_VALUE(uf)) {
        // printf("Special value:%x, ret:%x\r\n", uf, uf);
        return uf;
    }

    // 2. 非规格化数(包括0): 除符号位外, 左移
    if (isDENORMALIZED(uf)) {
        // printf("Denormalized:%x, ret:%x\r\n", uf, SHIFT_LEFT_EXPECT_SIGN(uf));
        return SHIFT_LEFT_EXPECT_SIGN(uf);
    }

    // 3. 规格化数: exp++
    // printf("Normalized:%x, ret:%x\r\n", uf, EXP_INCREASE(uf));
    return EXP_INCREASE(uf);
}

/*
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */

#define GET_EXP(uf)  ((uf & EXP_MASK) >> 23)
#define GET_FRAC(uf) (uf & FRAC_MASK)
#define FLOAT_BIAS   127
int floatFloat2Int(unsigned uf)
{
    // 特殊值，返回0x80000000u
    if (isSPECIAL_VALUE(uf)) {
        // printf("special value:%x\r\n", uf);
        return INTMIN;
    }

    // 非规格化值，都小于0，根据舍入规则，返回0
    if (isDENORMALIZED(uf)) {
        // printf("denormalized:%x\r\n", uf);
        return 0;
    }

    // 规格化值，
    // printf("normalized:%x\r\n", uf);
    int ret = 0;
    int E = 0;
    int M = 0;
    int ShiftBitNum = 0;

    E = GET_EXP(uf) - FLOAT_BIAS;
    M = (GET_FRAC(uf) | (0x01 << 23)); // 规格化值需要带上隐含的1
    // printf("E=%d, M=%x\r\n", E, M);

    if (E < 0)
        return 0;
    else if (E > 30)
        return INTMIN;

    ShiftBitNum = E - 23;
    if (ShiftBitNum >= 0)
        ret = (M << ShiftBitNum);
    else
        ret = (M >> negate(ShiftBitNum));

    // 处理负数的情况
    if (isNegative(uf)) {
        ret = negate(ret);
    }
    // printf("ShiftBitNum=%d, ret=%x\r\n", ShiftBitNum, ret);

    return ret;
}
/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while
 *   Max ops: 30
 *   Rating: 4
 */
#define FLOAT_POSITIVE_INF 0x7F800000
#define FRAC_BIT_NUM       23
#define EXP_MIN            (-126)
#define EXP_MAX            127
unsigned floatPower2(int x)
{
    int ret = 0;

    // 太大以至于float无法表示
    if (x > (EXP_MAX)) {
        // printf("too large:%d\r\n", x);
        return FLOAT_POSITIVE_INF;
    }

    // 太小以至于非规格数都无法表示
    if (x < (EXP_MIN - FRAC_BIT_NUM)) {
        // printf("too small:%d\r\n", x);
        return 0;
    }

    // 值很小,需要用非规格数表示
    if (x < EXP_MIN) {
        // printf("denorm:%d\r\n", x);

        int ShiftBitNum = EXP_MIN - x;
        // printf("ShiftBitNum:%d\r\n", ShiftBitNum);

        ret = (0x00800000 >> ShiftBitNum);
        // printf("ret:%x", ret);
        return ret;
    }

    // 用规格数表示即可
    // printf("norm:%d", x);
    ret = ((x + EXP_MAX) << 23);
    // printf("ret:%x", ret);
    return ret;
}
