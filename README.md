# PrismCompressor

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-green.svg)
![License](https://img.shields.io/badge/License-MIT-orange.svg)

一个使用 C++17 从零实现的多算法数据压缩工具与底层压缩框架。

## 目录

- [项目简介](#项目简介)
- [核心特性](#核心特性)
- [架构设计](#架构设计)
- [目录结构](#目录结构)
- [编译与构建](#编译与构建)
- [使用说明](#使用说明)
- [性能基准测试](#性能基准测试)
- [后续规划](#后续规划)
- [开源协议](#开源协议)

## 项目简介

PrismCompressor 是一个面向学习与工程实践的无损压缩项目。该项目没有依赖第三方压缩库，而是从最底层的位操作（BitStream）开始，逐步实现了 RLE、Huffman 和 LZ77 算法，并设计了一套自定义的 `.prism` 文件格式。

项目采用高度模块化的分层架构，通过面向接口编程（多态）将算法与引擎解耦，并提供命令行工具（CLI）以供实际使用。

## 核心特性

- **底层 BitStream**：自实现了 `BitWriter` 和 `BitReader`，支持 MSB-first 的位级别读写与 Padding 处理，为变长编码提供底层支撑。
- **多压缩算法支持**：
  - **RLE (Run-Length Encoding)**：针对连续重复字节的高效游程编码。
  - **Huffman**：基于优先队列构建哈夫曼树，支持变长前缀码与树的序列化/反序列化。
  - **LZ77**：采用滑动窗口与哈希链（Hash Chain）优化的字典压缩算法，时间复杂度接近 O(n)，支持重叠匹配。
- **自适应算法选择**：引擎可根据输入数据特征试跑多种算法，自动选择压缩率最优的方案。
- **自定义文件格式**：设计了包含魔数、版本号、算法标识、原始大小和校验和的 20 字节 `.prism` 头部，保证数据的完整性与自描述性。
- **工程化架构**：采用工厂模式创建算法对象，通过 `ICodec` 统一接口，实现了算法层、格式层与调度层的彻底解耦。

## 架构设计

项目分为五个核心模块，数据流向自下而上：

1. `prism::bitstream` / `prism::util`：底层位操作与频率统计。
2. `prism::algorithms`：具体算法实现（RLE、Huffman、LZ77），均继承自 `ICodec` 接口。
3. `prism::format`：`.prism` 文件格式的打包（Pack）与解包（Unpack）。
4. `prism::core`：核心调度层，包含 `CompressionEngine`、`CompressorFactory` 与 `AlgorithmSelector`。
5. `prism::io` / `app::cli`：磁盘文件读写与用户命令行交互。

## 目录结构

```text
PrismCompressor/
├── include/prism/
│   ├── core/           # 接口、工厂、引擎、算法选择器
│   ├── bitstream/      # 位读写工具
│   ├── algorithms/     # 具体压缩算法
│   ├── format/         # .prism 文件格式
│   ├── io/             # 文件 IO
│   └── util/           # 工具类（频率统计等）
├── src/                # 对应头文件的实现
├── app/cli/            # 命令行程序入口
├── tests/              # 单元测试
├── docs/               # 开发文档与笔记
└── CMakeLists.txt      # 构建脚本
```

## 编译与构建

### 环境要求
- CMake 3.16 或更高版本
- 支持 C++17 的编译器（GCC / Clang / MSVC）

### 构建步骤
```bash
git clone https://github.com/Shimizu-Haruka/PrismCompressor.git
cd PrismCompressor

mkdir build
cd build
cmake ..
cmake --build .
```

构建完成后，可在 `build/` 目录下找到 `prism_cli` 可执行文件。

## 使用说明

命令行工具支持 `compress` 和 `decompress` 两个核心子命令。

**压缩文件：**
```bash
# 指定算法 (rle / huffman / lz77 / adaptive)
./prism_cli compress input.txt output.prism huffman
```

**解压文件：**
```bash
./prism_cli decompress output.prism restored.txt
```

**参数说明：**
- 算法参数为可选，默认使用 `adaptive`（自适应选择）。
- 解压时无需指定算法，引擎会从 `.prism` 文件头中自动读取。

## 性能基准测试

以下数据基于不同格式的物理文件，对比了本工具与主流压缩工具（zip / 7z）的压缩率表现（Compression Ratio = 压缩后大小 / 原始大小）：

### 1. 已高度压缩的数据 (JPG 图像)
对于已被算法处理过的 JPG 格式，再次压缩通常难以获得收益。

| 文件大小 | Prism (自适应) | zip | 7z |
| :--- | :--- | :--- | :--- |
| 178 KB | 99.9748% | 100.0000% | - |
| 148 KB | 99.8126% | 98.9408% | - |
| 2.71 MB | 99.9613% | 99.9894% | 100.7581% |

*结论：对于此类数据，PrismCompressor 保持了极低的头部开销，未使文件显著膨胀，且表现优于 7z。*

### 2. 结构化文档数据 (PDF 文件)
对于 PDF 等包含大量文本与重复结构的文档。

| 文件大小 | Prism (LZ77) | zip (Deflate) | 7z (LZMA) |
| :--- | :--- | :--- | :--- |
| 8.00 MB | 36.4891% | 30.0406% | 23.5152% |

*结论：单靠 LZ77 算法能达到约 36% 的压缩率。与 zip 的差距主要在于缺少熵编码（Huffman）的二次压缩，与 7z 的差距在于滑动窗口大小与概率模型的复杂度。*

## 后续规划

- [ ] 实现 Deflate 风格混合压缩：LZ77 匹配 + Huffman 编码。
- [ ] 引入多线程加速大文件分块压缩。
- [ ] 开发图形界面（GUI）与可视化压缩过程展示。

## 开源协议

本项目采用 [MIT License](LICENSE) 开源协议。
```
