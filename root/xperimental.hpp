// eXperimental functions/class, to be later implemented into ASWL

#include <bitset>
#include <vector>
#include <ostream>

namespace ASWL::eXperimental {

    template<size_t N> std::vector<unsigned char> Bitset2Bytes(const std::bitset<N>& bs) {

        std::vector<unsigned char> result((N + 7) >> 3);

        for (int j = 0; j < static_cast<int>(N); j++)
            result[j >> 3] |= (bs[j] << (j & 7));

        return result;
    }

    template<size_t N> std::bitset<N> Bytes2Bitset(const std::vector<unsigned char>& buf) {

        assert(buf.size() == ((N + 7) >> 3));
        std::bitset<N> result;

        for (int j = 0; j<int(N); j++)
            result[j] = ((buf[j >> 3] >> (j & 7)) & 1);

        return result;
    }

    struct SetHash {

        SetHash() = default;
        SetHash(int _x, int _y) {

            x = std::bitset<32>(_x);
            y = std::bitset<32>(_y);

            xy = std::bitset<64>(0);

            for (int i = 0; i < xy.size(); i++) {

                if (i < x.size() && x[i])
                    xy.set(i);

                if (i >= y.size() && y[i - y.size()])
                    xy.set(i);
            }

            xy = xy >> 32 | xy << 32;
        };

        std::bitset<32> x;
        std::bitset<32> y;
        std::bitset<64> xy;

        friend bool operator< (const SetHash& left, const SetHash& right) {
            return left.xy.to_ullong() < right.xy.to_ullong();
        };

        friend std::ostream& operator<< (std::ostream& stream, const SetHash& sh) {

            std::vector<unsigned char> v = Bitset2Bytes<64>(sh.xy);

            for (int x = 0; x < v.size(); x++)
                stream << v[x];

            return stream;
        };
    };
}
