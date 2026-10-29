# PrismCompressor

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-green.svg)
![HTML5](https://img.shields.io/badge/HTML5-E34F26?logo=html5&logoColor=white)
![JavaScript](https://img.shields.io/badge/JavaScript-F7DF1E?logo=javascript&logoColor=black)

一个使用 C++17 从零实现的多算法数据压缩系统，包含核心压缩引擎、HTTP 后端服务以及全栈 Web 可视化界面。

## 项目简介

PrismCompressor 是一个面向底层工程实践的无损压缩全栈项目。项目没有依赖任何第三方压缩库，从最底层的位操作（BitStream）开始，逐步实现了 RLE、Huffman 和 LZ77 算法，并设计了自定义的 `.prism` 文件格式。

为了提升用户体验，项目基于 `cpp-httplib` 构建了 C++ 高性能 HTTP 服务器，并搭配原生 Web 前端，实现了一套完整的 **“拖拽上传 -> 后端压缩/解压 -> 二进制流式下载”** 的全栈解决方案。

## 核心特性

### 1. 底层算法引擎 (C++)
- **自研 BitStream**：脱离字节对齐限制，手写 `BitWriter` 与 `BitReader` 实现 MSB-first 的位级读写与 Padding 填充，为变长编码提供支撑。
- **多算法支持**：
  - **RLE**：高效游程编码，处理连续重复数据。
  - **Huffman**：基于优先队列构建哈夫曼树，支持前缀码编码与自描述树序列化。
  - **LZ77**：采用滑动窗口与哈希链（Hash Chain）优化，将搜索复杂度从 O(n²) 降至接近 O(n)，支持重叠匹配。
- **自适应选择**：根据数据特征动态评估，自动选择最优算法。

### 2. 全栈 Web 架构 (C++ + Web)
- **HTTP 后端**：使用 `cpp-httplib` 构建，处理跨域资源请求（CORS）与 OPTIONS 预检拦截。
- **二进制传输**：遵循工业级规范，在 HTTP 响应头（Header）中传递元数据（大小/压缩率），在响应体（Body）中传输纯二进制流（`application/octet-stream`）。
- **前端交互**：原生 JS 实现 `FormData` 多部分表单上传、`Blob` 二进制响应接收，以及通过动态 `<a>` 标签与 `URL.createObjectURL` 触发文件下载。

### 3. 自定义文件格式
设计了 20 字节的 `.prism` 文件头（包含魔数、版本号、算法标识、原始大小和校验和），配合小端序读写，保证了压缩数据的完整性与可解码性。

## 架构设计

项目分为五大核心模块，数据流向自下而上，最终通过 HTTP 协议与前端交互：

1. `prism::bitstream` / `prism::util`：底层位操作与频率统计。
2. `prism::algorithms`：具体算法实现（RLE、Huffman、LZ77），均继承自 `ICodec` 接口。
3. `prism::format`：`.prism` 文件格式的打包（Pack）与解包（Unpack）。
4. `prism::core`：核心调度层，包含 `CompressionEngine`、`CompressorFactory` 与 `AlgorithmSelector`。
5. `app::server` / `app::cli`：C++ HTTP 服务器与命令行工具（通过 `cpp-httplib` 支撑前端请求）。

## 目录结构

PrismCompressor/
├── include/prism/
│   ├── core/           # 接口、工厂、引擎、算法选择器
│   ├── bitstream/      # 位读写工具
│   ├── algorithms/     # 具体压缩算法
│   ├── format/         # .prism 文件格式
│   ├── io/             # 文件 IO
│   └── util/           # 工具类（频率统计等）
├── src/                # 对应头文件的实现
├── app/
│   ├── cli/            # 命令行程序入口
│   └── server/         # C++ HTTP 服务器入口
├── web/                # 前端页面 (index.html)
├── tests/              # 单元测试
└── CMakeLists.txt      # 构建脚本
编译与构建
环境要求
CMake 3.16 或更高版本

支持 C++17 的编译器（GCC / Clang / MSVC）

Windows 环境：需要链接系统网络库 ws2_32

构建步骤
bash
git clone https://github.com/Shimizu-Haruka/PrismCompressor.git
cd PrismCompressor

mkdir build
cd build
cmake ..
cmake --build .
使用说明
1. Web 全栈模式 (推荐)
启动后端服务：
在 build 目录下运行：

bash
# Windows
.\prism_server.exe

# Linux / Mac
./prism_server
终端将提示 open http://localhost:8080/hello。请保持终端开启。

启动前端：
直接在浏览器中打开 web/index.html。

选择文件，勾选算法（RLE / Huffman / LZ77）。

点击“压缩”，页面将实时显示原大小、压缩后大小和压缩率。

点击“下载”，即可获得 .prism 压缩文件。

选择 .prism 文件并点击“解压”，即可还原原文件。

2. 命令行模式 (CLI)
bash
# 压缩文件
./prism_cli compress input.txt output.prism huffman

# 解压文件
./prism_cli decompress output.prism restored.txt
性能基准测试
以下数据对比了本工具与主流压缩工具（zip / 7z）的压缩率表现（Compression Ratio = 压缩后大小 / 原始大小）：

1. 已高度压缩的数据 (JPG 图像)
对于已被算法处理过的 JPG 格式，再次压缩通常难以获得收益。

文件大小	Prism (自适应)	zip	7z
178 KB	99.9748%	100.0000%	-
148 KB	99.8126%	98.9408%	-
2.71 MB	99.9613%	99.9894%	100.7581%
结论：对于此类数据，PrismCompressor 保持了极低的头部开销，未使文件显著膨胀，且表现优于 7z。

2. 结构化文档数据 (PDF 文件)
对于 PDF 等包含大量文本与重复结构的文档。

文件大小	Prism (LZ77)	zip (Deflate)	7z (LZMA)
8.00 MB	36.4891%	30.0406%	23.5152%
结论：单靠 LZ77 算法能达到约 36% 的压缩率。与 zip 的差距主要在于缺少熵编码（Huffman）的二次压缩。

后续规划
□ 实现 Deflate 风格混合压缩：LZ77 匹配 + Huffman 编码。
□ 引入多线程加速大文件分块压缩。
□ 前端重构为 Vue 3 + 现代组件库，接入流式传输与进度条。
