# 《深入理解计算机系统》(CSAPP) 学习笔记

### chap-1 计算机系统漫游

#### 1.1 信息和程序

##### （1）信息的表示

1. 信息事实上就是**上下文+数据位**，对机器来说能看到的只有数据位，而数据位表示什么含义则取决于上下文；
2. 举个例子，对于 `int a = 10`，10这个常量的二进制表示为0000 1010，而机器看到这串数字的时候，会观察其上下文 `int a =`，这表明了含义：这个语句是个赋值语句，将常量0000 1010赋值给命名为a的变量；

##### （2）程序

1. 程序的翻译：计算机硬件只能理解机器指令（0和1组成的二进制指令），而人类输入的程序本质上是字符文本（写程序的过程其实和写纯文本没区别，程序只有被翻译后才有意义）；两种语言之间的转换是如何实现的？这就要用到**编译系统(compilation system)**；
2. 编译系统不仅仅是编译器，而是**预处理器、编译器、汇编器和链接器的集合**，整个系统通过以下步骤完成程序的翻译
   ![image-20240910183245406](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202409101832671.png)

#### 1.2 存储器层次结构

##### （1）为什么要有层次地布局存储器？

- 这取决于一个事实性原理：局部性原理——程序会倾向于，在某一段集中的时间，访问某一段连续的数据；
- 因此，如果我们提前将一部分集中的数据读取到离计算机很近的地方（缓存），就能极大地加速程序运行（相对于每次都从较远的内存中取数据）；

##### （2）存储器的布局（多级+缓存）

- 存储器有一个特点，存储容量和单位比特的造价成反比，同时和访问速度也成反比，也就是说容量越大，单位比特的价格越低，同时访问速度越慢，反之亦然；
- 因此形成了以下的多级存储器层次结构，**上层存储器作为低一层存储器的缓存**，既满足了高速需求，也满足了存储大量数据需求；
  ![image-20240910184009393](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202409101840487.png)

#### 1.3 操作系统提供的抽象

##### （1）为什么要有操作系统？

1. 操作系统是计算机硬件和使用者之间插入的一层软件，其通过抽象底层硬件，帮助使用者能够轻松调用底层硬件，而不需要了解硬件细节；
2. 其主要有两个功能：
   1. 防止应用程序滥用硬件，比如访问不该访问的内存；
   2. 为硬件使用者提供易于理解的操作方法，比如我们不需要写入寄存器，调用一个printf就能在屏幕上输出字符串；换一个不同品牌的硬盘，我们的计算机也能正常识别和工作，不需要我们做什么；

##### （2）操作系统提供的抽象

1. **I/O设备的抽象**：操作系统将所有I/O设备抽象为文件，包括键盘、鼠标、显示屏、硬盘和网络等；
2. **虚拟内存**：如果将I/O设备看成地址空间的一部分，那么虚拟内存就是对**存储器（通常是内存）+I/O设备（包括硬盘、网络等）**的统一抽象，为更上一层的进程提供统一的存储映像；
   1. **进程**：进程是对**虚拟内存+CPU**的抽象，可以简单理解为**应用程序本身 = 从虚拟内存映像中取指令 + 由CPU运行指令**；

#### 1.4 Amdahl定律

1. 系统级别的优化必须优化整个系统的大部分组件，如果仅在个别部分进行优化，整体优化会有很大限制；
2. 考虑系统的一个部分，其对系统运行时间的贡献是25%，那么就算这部分的运行效率提高无穷倍，即时间减小为0，整个系统的运行时间也仅仅变为原来的3/4，运行效率是原来的4/3；
3. 下图公式中，Told是系统执行的总时间，α是某部分所需时间占总时间的比例，k是性能提升的比例，Tnew是系统在该部分性能提升后的执行总时间；

   ![image-20240910185634002](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202409101856045.png)

### chap-2 信息的表示和处理

#### 1.1 信息存储

##### （1）字长和基本数据类型的大小

1. 字长：每台计算机的地址长度（地址线的条数、指针数据的标准大小），32位计算机的地址长度为32位，即4字节，因此**32位计算机一个字为4字节**；同理，**64位计算机一个字为8字节**；
2. 数据类型

   - 在不同的程序中，相同的数据类型可能有不同的存储大小，具体取决于编译系统的指令；

     ```shell
     gcc -m32 prog.c # 编译为32位程序
     gcc -m64 prog.c # 编译为64位程序
     ```
   - 64位机器既可以运行64位程序，也可以运行32位程序（向后兼容）， 但32位机器只能运行32位程序；
   - 各种数据大小如图所示，`unsigned`表示无符号数；
     ![image-20240913113351287](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202409131133399.png)

##### （2）字节存储模式

1. 大端存储：高字节先存（低地址），低字节后存（高地址）；
2. 小端存储：低字节先存（低地址），高字节后存（高地址）；
3. Intel兼容机大多采用小端，而IBM多使用大端；手机的ARM处理器可以配置成大端或小端，但主流的手机操作系统Android和iOS都仅支持小端；
4. 对于一个int类型 `0x12345678`，其两种存储模式如下所示，注意两种存储都是**以字节为单位**的；
   ![image-20240913114515685](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202409131145773.png)

##### （3）数据表示

1. **字符串**：ASCII、UTF-8；
2. **代码：**被编译系统转换成指令编码，指令代码取决于不同的处理器架构和操作系统类型；

##### （4）C语言的位级运算和逻辑运算

1. 位级运算：按位与(&)，按位或(|)，按位取反(~)，按位异或(^)；

   - 位级运算的常见应用：掩码运算，比如取整型数 `0x12345678`的最低字节，则可以做运算 `0x12345678 & 0x000000FF`
   - 一个有趣的例子：用异或实现交换两数的值；

   ```c
   // 异或运算实现交换两个数,假设*x=a，*y=b
   void exchange(int *x, int *y){
   	*y = *x ^ *y;
       *x = *x ^ *y;
       *y = *x ^ *y;
   }
   ```

   ![image-20240913210451991](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202409132104095.png)
2. 逻辑运算：与(&&)，或(||)，非(!)；

   - **提前返回**：若第一个条件就能确定整个逻辑运算的返回值，那么就不会判断第二个条件；`a && 1/a` 在a为0时就返回0，因此不会产生除零错误；`p && *p++` 在指针为NULL时返回0，不会访问空指针；

##### （5）C语言的移位运算

1. 左移：`x << n`，将二进制数x左移n位，低位补0
2. 右移：`x >> n`，将二进制数x右移n位；
   1. 逻辑右移：高位补0；
   2. 算数右移：高位补符号位，仔细想想，这样才满足 `右移 = 除2`；

![image-20240913211723518](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202409132117580.png)

3. **无符号数**的右移必然是逻辑的，但C语言对**有符号数**的右移没有明确规定，大多数编译器对**有符号数默认采用算数右移**；
4. 当 `位移量n > 字长w`：C标准规避了这种情况，其对位移量会先进行预处理 `n mod w`，但这种行为在具体的编译系统上未必被实现，因此最好保证位移量小于字长；

##### （6）常见应用

使用移位运算、按位与、按位或运算实现**标志位的设置/读取**

- Set flag

  ```c
  flag = 0x00;
  flag |= (0x01 << 3); // flag = 0x08;
  ```
- Reset flag

  ```c
  flag = 0x08; // 0000 1000
  flag &= (0x00 << 3); // flag = 0x00
  ```
- Read flag

  ```c
  flag = 0x08;
  flag_bit_4 = flag & (0x01 << 3); // 读出第4位, flag_bit_4 = 1
  ```

##### 1.2 整数表示

1. 32位程序上的数据表示如图；

![image-20240913213807522](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202409132138586.png)

2. 64位数据仅有 `long`和 `unsigned long`的数据长度不同；
3. C标准规定的最小长度：
   ![image-20240913214216748](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202409132142809.png)

##### （1）无符号整数（unsigned）表示

1. 定义：二进制位模式到数值的映射（双射）；
   ![image-20240913214422909](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202409132144948.png)
2. 例子
   ![image-20240914221531701](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202409142215752.png)

##### （2）有符号数 - 补码编码

1. 定义：**补码的最高位对应数值被定义为负值**，这也是为什么最高位被称为符号位，因为只要最高位为1，整个位模式表示的必然是负数；
   ![image-20240913214708424](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202409132147458.png)
2. 例子
   <img src="https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202409142215037.png" alt="image-20240914221506932" style="zoom:80%;" />

##### （3）固定位数的数据类型

1. 为了确保不同编译系统/机器之间的可移植性，用确定的位数来表示数据很有必要；IOS C99引入了以下两种数据类型；

   ```c++
   // 通常N仅能取8、16、32
   intN_t  // 表示N位有符号数
   uintN_t // 表示N位无符号数
   ```
2. 固定位数据类型的格式化输出需要用到宏定义；注意在字符串中调用宏定义时，需要为宏定义引上双引号，且在双引号内侧添加空格；

   ```c
   #include <inttypes.h>
   int main()
   {
       uint32_t a = 10;
       uint16_t b = 20;
   
       printf("a=%" PRId32 ",b=%" PRId64 "\n", a, b);
   
       getchar();
       return 0;
   }
   ```

##### （4）有符号数和无符号数之间的转换

1. **基本规则：** 位模式不变，但位模式的解释方式改变，具体来说就是最高位变为负权值；

   ```c
   short val = -12345; // 位模式:1100 1111 1100 0111
   unsigned short uval = (unsigned)val; //显示转换
   printf("val=%d,uval=%u",val,uval);
   
   // 结果输出：val=-12345,uval=53191
   // 可以观察到，53191=-12345+65536，事实上所有的补码表示的负数转换为无符号数时，都会加上2^w，其中w是字长
   // 根本上是因为最高位的权值从-2^(w-1)变为了2^(w-1)，所以整个数值增加了2^w
   ```
2. **显式转换：** 在变量前加上(unsigned)或(int)，将其转换为无符号整型或有符号整型，其他数据类型（short、long）同理；

   - 注意，%d和%u分别表示：以**有符号十进制**和**无符号十进制**的格式来输出某个位模式；
   - 确保该位模式以期望的方式输出是程序员应该保证的事情；
3. **隐式转换：**在C语言的运算中，如果某个运算数是有符号的，而另一个是无符号的，那么**有符号数会被隐式转换为无符号数**，最后的结果也是无符号数；

   ```c
   0 - sizeof(char) < 0 // 返回假，因为sizeof(char)返回的是无符号数，所以最后的结果是0xFFFF FFFF，即INT_MAX
   ```

##### （5）同一个数的位拓展和位截断

1. **位拓展：**
   - 无符号数拓展：拓展后直接在**高位补0**，8位拓展为16位：`0x04 -> 0x0004`；
   - 有符号数拓展：拓展后在**高位补符号位**，8位拓展为16位：`0x80 -> 0xFF80`；
2. **位截断：**

   - 无符号数和有符号数的截断是类似的，将高位截去即可，但有符号数最后的位模式仍要**按照补码来解释**；

   - PS：对于有符号数x，如果x是负数，将其高位连续的1截断若干个（剩余不少于1个），再按照补码解释，其十进制数值是一样的，例如：

     | 二进制 | 十进制（补码表示） |
     | :----: | :----------------: |
     |  1110  |         -2         |
     |  110   |         -2         |
     |   10   |         -2         |

#### 1.3 整数运算（关注溢出）

##### （1）无符号加法

- 向上溢出时，回到0；如 `0xFF + 1 = 0x00`
- 向下溢出时，回到最大值；如 `0x00 - 1 = 0xFF`

##### （2）有符号（补码）加法

- 向上溢出时，回到最小的负数；
- 向下溢出时，回到最大的正数；

##### （3）乘法

- **无符号**
  - 乘法的值也可能溢出，两个w位的整数相乘，可能需要2w位才能表示；
  - 但在C语言中，无符号乘法得到的结果会将高出有效位（w位）的部分截断，本质上相当于取模于2的w次方；
- **补码**
  - 在乘法中，补码乘法和无符号乘法得到的整体位模式虽然不同，但截断后补码的表示仍是符合规则的；
- **移位和乘法**
  - **左移**n位相当于乘2的n次方；**算数右移**n位相当于除2的n次方；
  - 无符号和补码做乘法时，都可能发生溢出，但即使溢出，按照移位得到的位模式也是正确的，例如：
    - (int)0011B 乘 4，相当于3×4 = 12，而4位补码表示[-8, 7]，此时溢出为-4；
    - 按照移位乘法规则，0011左移2位得到1100，其作为补码也表示为-4；
      ![image-20250312165855117](https://cdn.jsdelivr.net/gh/Mryoungg/Typora-image@main/Image%5Cimage-20250312165855117.png)

#### 1.4 浮点数

##### （1）IEEE 浮点表示

1. **浮点数表示**

   1. 浮点数的格式化形式如下，其中s（符号位sign）决定了正负，M（尾数）是数值本身，E（阶码）是幂次；
      $$
      V = (-1)^s × M × 2^E
      $$

   2. 要表示一个浮点数，只需要表示s、M和E即可，浮点类型的位模式也正是被分为这三部分；

2. **浮点数的位模式**
   ![image-20250312195612536](https://cdn.jsdelivr.net/gh/Mryoungg/Typora-image@main/image-20250312195612536.png)

   1. **符号位s**：决定正负，直接编码s；
   2. **阶码字段exp**：编码E，其编码方式见下文；
   3. **尾数字段frac**：编码M，编码方式见下文；

3. **浮点数的三种情况（以32位单精度浮点数为例，exp有8位，frac有23位）**
   ![image-20250312200210315](https://cdn.jsdelivr.net/gh/Mryoungg/Typora-image@main/image-20250312200210315.png)

   1. **规格化**

      - **exp** = [1,254]，其编码 E 的规则为：向下偏移一个偏置量Bias使得其能够表示正、负幂，编码后的E = [-126, 127]；

        - Bias = 2^(k-1)-1，其中k是exp的位数，在单精度中，Bias = 127，即`E = exp - 127`；

      - **frac** = [0, 255/256]，其编码 M 的规则为：M = 1 + frac；

        - 本质上frac表示二进制浮点数，第一个1后面的部分（`这样可以省下一个位）；因此编码为M之后，带上一个**隐含的1**；
   
        - 例如11.1010，可以表示为：
          $$
          11.1010 = (-1)^0×1.11010×2^1
          $$
          此时，M = 1.11010，frac = 0.11010；

   2. **非规格化**
   
      - 对于规格化数值，M总是有隐含的1，因此总有一些很小的小数无法表示（0也无法表示）；因此引入非规格化数，**表示更小的小数和0**；
      - **exp** 恒= 0；此时 exp 不再编码 E ，E 恒= -126；**这样的好处是：从非规格数跳变到规格数时，阶码 E 不会跳变**，下图以**4位 exp + 3位 frac** 为例；
        ![image-20250312215034797](https://cdn.jsdelivr.net/gh/Mryoungg/Typora-image@main/image-20250312215034797.png)
      - **frac** = [0, 255/256]，此时不再带有隐含的1，f = M；

   3. **特殊值**
   
      - **无穷大**：exp恒为全1，frac恒为全0，符号位s决定正、负；
      - **NaN（Not a Number）**：exp恒为全1，frac不为全0，符号位s无含义；
   
   4. **浮点数实例**
      ![image-20250312215745796](https://cdn.jsdelivr.net/gh/Mryoungg/Typora-image@main/image-20250312215745796.png)


##### （2）浮点数的舍入

1. **常见舍入规则**
   ![image-20250312220327208](https://cdn.jsdelivr.net/gh/Mryoungg/Typora-image@main/image-20250312220327208.png)
   - 主要关注**向偶数舍入**，这是浮点数舍入的基本规则；
   - 在向偶数舍入的情况中，当浮点数**并非中间值**时，向最近的符合精度的数舍入；如精度为小数点后2位时时，1.433舍入为1.43，1.436舍入为1.44；
   - 当浮点数**恰为中间值**时，舍入为最接近的偶数（广义上的偶数，考虑小数）；如精度为小数点后2位，1.435舍入为1.44（而非1.43）；
   - **Why：向偶数舍入能得到一组相对没有统计偏差的值；（向上或向下舍入在平均值上会有统计偏差）；**
2. **浮点数舍入实例**
   ![image-20250312220956355](https://cdn.jsdelivr.net/gh/Mryoungg/Typora-image@main/image-20250312220956355.png)

##### （3）浮点数的算数性质

1. **加法**

   1. 满足交换律，有单调性，大部分有逆元（x + (-x) = 0）；

   2. 不满足结合律；

      - 在单精度浮点下， `(3.14 + 1e10) - 1e10 = 0`；而 `3.14 + (1e10 - 1e10) = 3.14`；

      - 因为 (3.14 + 1e10) = 10 000 000 003.14，此时 frac = 0.000 000 000 314（1被隐含），而单精度浮点的最小精度是 2^(-23) = 0.000 000 119...，因此frac被舍入了；

        <img src="https://cdn.jsdelivr.net/gh/Mryoungg/Typora-image@main/image-20250313132856128.png" alt="image-20250313132856128" style="zoom: 80%;" />

2. **乘法**

   1. 满足交换律，有单调性；

   2. 不满足分配律和结合律，对于单精度浮点，有如下情况；因为单精度浮点数最大表示3.4 × 10^38，超过该值则被看作正无穷；
      $$
      (1^{20} × 1^{20}) × 1^{-20} = +∞ \\\\
      1^{20} × (1^{20} × 1^{-20}) = 1^{20}
      $$


##### （4）C语言中的浮点数

1. **类型转换规则**
   1. int转换为float：不会溢出，可能发生舍入；
   2. int/float转换为double：精确保留；
   3. double转换为float：可能溢出为∞（范围，也就是frac不够），也可能发生舍入（精度，也就是exp不够）；
   4. double/float转换为int：向0舍入（直接砍掉小数部分，1.999变为1，-2.33变为-2）；因为浮点数范围更大，所有舍入后可能发生溢出；
2. **浮点数表示范围**
   ![image-20250313144958206](https://cdn.jsdelivr.net/gh/Mryoungg/Typora-image@main/image-20250313144958206.png)

### chap-3 程序的机器级表示

#### 1.1 基础：指令集架构、汇编和C

##### （1）芯片指令集架构的历史

##### （2）汇编&机器码：汇编和反汇编

##### （3）寄存器及访问指令

#### 1.2 控制：程序跳转

##### 1.2.1 条件分支跳转（Conditional branches）

**（1）基本跳转指令**

**（2）条件转移（Condition Move）**

1. 条件转移
   1. what is it？
      - 同时计算两个分支中的表达式，最后基于条件判断，决定使用哪一个
   2. 有什么好处
      - 在程序出现分支跳转时，计算机在预取指时会进行**分支预测**，在多级流水线中，分支预测错误的代价是很高的（正确率很高，但仍会出错）；
      - **条件转移**能够避免分支预测错误带来的开销，因为它同时计算了两个分支的表达式；
2. 不适用于条件转移的情况
   1. 分支中是复杂运算：很消耗资源
   2. 存在风险的运算（空指针访问）
   3. 相互影响的运算：第一个分支运算完后会影响第二个，如`int a = x > 0 ? x+=7 : x*=3`;

##### 1.2.2 循环（Loops）

**（1）do-while & while**

1. do-while和while的区别
2. while循环的两种翻译
   - 先跳转（jump-to-middle）
   - 先测试

**（2）for 循环** 

##### 1.2.3 Switch 语句（Switch statements）

**（1）汇编语言实现：利用符号表**

**（2）Fall-Through 模块的汇编翻译**
<img src="https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202504080015167.png" alt="image-20250408001305525" style="zoom:50%;" />

- w的初始化被推迟了，因为 case 2 中，w的最终值与其初始值无关；

#### 1.3 过程（Procedure）

> 过程具体来说就是：函数、方法，handler等等；抽象出来就是三个元素：输入+处理方法+输出，后续我们都以函数代称；

##### 1.3.0 运行时栈

![image-20250409001039358](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202504090010862.png)

##### 1.3.1 函数的转移控制

> 本质上对函数的控制就是两件事情：控制函数跳转和返回
> ![image-20250409001146577](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202504090011627.png)

- 函数跳转（汇编指令 call）：做了两件事情，push返回地址+jump到被调用函数；

- 函数返回（汇编指令 ret）：只做了一件事，将call时push的返回地址，pop给PC，继续执行后续程序；

- 程序示例：关注PC、SP指针、栈的变化情况

    ```assembly
    ;Call to multstore from main
    400563: e8 d8 ff ff ff callq 400540 <multstore> 
    ;执行callq后，将下一条指令的地址（400568）压栈，并跳转到400540
    400568: 48 8b 54 24 08 mov 	 0x8(%rsp),%rdx
    
    ;Beginning of function multstore
    0000000000400540 <multstore>: 
    400540: 53			 push  %rbx 
    400541: 48 89 d3 	 mov   %rdx,%rbx 
    ;Return from function multstore 
    40054d: c3 retq
    ;返回的时候，跳转到栈中存储的返回地址（400568）
    ```

    ![image-20250409001949655](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202504090019743.png)

##### 1.3.2 传递数据

**（1）传递参数（寄存器+栈）**

1. 寄存器传递参数：最多6个整形和指针参数，浮点参数是另外存的；
    ![image-20250409235141634](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202504092351760.png)
2. 栈传递参数：参数总数超过6个时，将后续参数压入栈中，最后一个参数先压入，第7个参数最后压入；程序示例如下，其中汇编程序的第2行，第6行是从栈中读取参数；
    ![image-20250409235203348](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202504092352458.png)

**（2）传递返回值**

1. 函数通过 `%rax` 传递返回值；
2. 这是一个共识（ABI，应用程序二进制接口），函数调用者默认被调用函数的返回值就在 `%rax` 中

##### 1.3.3 内存管理

1. **局部数据存储：**局部变量，存储的原因通常有如下三点
    1. 寄存器不够放
    1. 对局部变量进行了&（取地址）操作，因此必须给他分配一个内存（栈）上的地址，因为寄存器没有地址；
    1. 该变量可能是数组/结构；
2. **释放：**函数ret时，其栈帧上的所有局部数据都会被释放（本质上是%rsp的位置改变了）；

#### 1.4 数据结构的内存分配和访问

##### 1.4.1 数组存储

##### 1.4.2 指针运算

1. **允许的指针运算：**

    - 指针与常数加减（数组下标填入负值，等价于指针减常数，是合法的）；
    - 指针与指针相减（有意义，可以计算两个指针之间相隔的对象个数）；

    ```
    
    ```

2. **不允许的指针运算**

    - 指针与指针相加（无意义，且可能产生指向非法位置的指针）；

#### 1.5 异构数据结构和字节对齐

##### 1.5.1 结构体&联合

**（1）区别：**结构体给每个成员分配私有空间，联合的空间取决于最大的成员，且所有成员共用这块空间；

**（2）联合的作用**

1. 查看同样位模式的一段数据，在不同数据类型下的解析值；
    ![image-20250412225413828](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202504122254907.png)
2. 查看一段长字节数据的不同部分（注意字节序的影响）
    ![image-20250412225359424](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202504122253790.png)

##### 1.5.2 数据（字节）对齐

> 字节对齐仅针对基本数据结构对象

**（1）基本原则 - 1**

1. 对于任何**K字节**的基本数据类型对象，其地址必须是**K的倍数**；
    ![image-20250410114818346](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202504101148415.png)

2. 实例：

    ```c
    struct S1 { 
    	int i; 
    	char c; 
    	int j; 
    }; 
    ```

    - 未对齐的紧凑存储：无法保证 `i`（4字节）和 `j`（4字节）的对齐要求；
        ![image-20250410114843066](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202504101148104.png)
    - 对齐存储：
        ![image-20250410115006561](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202504101150596.png)
        - 在c之后填充了3字节，保证内部对齐；
        - 同时需要确保：整个结构体的起始地址是4的倍数，如此才能保证`i`的4字节对齐，进而确保后续`j`的对齐；

**（2）延申原则 - 2**

1. 确保整个结构体的大小能够满足：在存储结构体数组时（连续的结构体），仍然确保对齐需求；

2. 实例：

    ```c
    struct S2 { 
    	int i; 
    	int j; 
    	char c; 
    }; 
    ```

    - 紧凑存储：仅需要结构体首地址满足4字节对齐，就能满足整体的对齐需求；
    - **存在问题：存储结构体数组时，无法保证第二个元素（以及之后）的对齐；**
        ![image-20250410121505753](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202504101215789.png)
    - 对齐存储：在末尾补充字节，使整个结构体的大小填充至4字节的倍数；
        ![image-20250410120006749](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202504101200792.png)

**（3）需要字节对齐的原因**

1. 处理器从内存中读取数据时，并非逐字节读取，通常多个字节一起读（现代计算机通常是64字节）;
2. 因此对齐的存储能够保证：每一个基本对象都不会跨越单次读取的边界，即读取每一个基本对象时，都能通过单次读取操作完成；
3. 在x86-64上，数据不对齐时程序也能正常运行，但效率较低；在某些机器上，不对齐的数据会导致内存错误

**（4）字节对齐的小建议**

- 作为程序员，不同的结构体排布会导致不同的字节对齐模式，因此填充的字节数也不同；
- 有一种基于贪心的普遍规律，如果将较大的基本对象放在最前面，所需的填充字节最少；

#### 1.6 浮点代码（不深入探究，参考 CSAPP - 3.11）

##### 1.6.1 浮点数寄存器

##### 1.6.2 浮点数指令

#### 1.7 x86-64 Linux 内存布局

1. **栈（stack）**

    - 存储函数局部变量、多出来的函数参数、寄存器的值和函数返回值；
    - 位于最高地址空间，向低地址增长；
    - 通常限制8M字节；

2. **堆（heap）**

    - 存储程序动态分配的内存；
    - 与栈相向增长；

3. **数据段（.data）**

    - 存储局部变量和静态变量，字符串常量；

4. **文本（程序）段（.text）**

    - 存储机器指令（代码）；

5. **实例**
    ![](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202504121303816.png)

    ![image-20250412125913961](https://gitee.com/yyangyyyy/typora-image/raw/master/Typora_Image/202504121259148.png)

#### 1.8 缓冲区溢出

> 这里讨论的溢出都是基于栈，因为栈中保存了一个重要的参数：**函数返回地址**，这使得栈溢出有极大的危害；

##### 1.8.1 缓冲区溢出实例&危害

1. 实例：
2. 危害：最主要的是函数的ret位置被改变，此时程序的执行流将被改变；

##### 1.8.2 缓冲区溢出的保护措施

**（1）程序员：保证代码健壮性**

- 在向缓冲区中接收数据时，确保检查数据长度符合要求；

**（2）系统：**

1. 栈地址随机化（SALR，Stack Address Layout Randomization）：每次程序运行时，都会给栈分配不同的起始地址；
2. 可执行区域划分：栈区域不给可执行权限；
3. canary（直译为金丝雀，意译为栈哨兵）
    - 在栈中防止一段数据作为哨兵；
    - 在程序执行完后，检查哨兵是否被修改，即可检查栈是否发生溢出（被攻击）；
