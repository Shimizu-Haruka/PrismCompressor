#include"prism/util/frequency_table.h"
#include<algorithm>

namespace prism::util{
    void FrequencyTable::build(const ByteBuffer& input)
    {
        clear();
        for(auto c : input){
            counts_[c]++;
        }
    }

    void FrequencyTable::clear()
    {
        std::fill(counts_.begin(), counts_.end(), 0);
    }

    std::size_t FrequencyTable::get(Byte value) const{
        return counts_[value];
    }//得到某个字符的频率

    bool FrequencyTable::empty() const{
        for(auto c : counts_){
            if(c != 0){
                return false;
            }
        }
        return true;
    }
}