#pragma once

#include"prism/core/codec.h"

namespace prism::algorithms{
    class lz77Compressor : public core::ICodec{
        public:
            ByteBuffer compress(const ByteBuffer& input)const override;
            ByteBuffer decompress(const ByteBuffer& input)const override;
            std::string name() const override;
        private:
        static constexpr std::size_t kWindowSize = 4096;//窗口大小
        static constexpr std::size_t kMinMatchLength = 3;//最小匹配长度
        static constexpr std::size_t kMaxMatchLength = 255;//最大匹配长度
        static constexpr std::size_t kHashSize = 65536;//哈希表大小
        static constexpr std::uint32_t kInvalidPos = 0xffffffff;//没有位置

        static std::uint32_t hash3(const ByteBuffer& input , std::size_t pos);/*算哈希值*/

        static void insertHash(/*把位置传入哈希表*/
            const ByteBuffer& input,/*输入*/
            std::size_t pos,/*位置*/
            std::vector<std::uint32_t>& head,/*哈希表*/
            std::vector<std::uint32_t>& prev/*前驱*/
        );

        static std::size_t findMatch(/*查找匹配*/
            const ByteBuffer& input,
            std::size_t pos,
            std::vector<std::uint32_t>& head,//哈希表
            std::vector<std::uint32_t>& prev,//前驱
            std::size_t& matchOffset//匹配长度
        );

    };
}