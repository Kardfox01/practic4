#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
#include <fstream>
using namespace std;


using ushort = unsigned short;
using recieved = tuple<string, ushort>;


string read_binary_sequence(string&& filename) {
    ifstream file(filename);
    if (!file)
        return "";

    string buffer(file.tellg(), ' ');
    file.read(buffer.data(), file.tellg());
    file.close();

    return buffer;
}


void write_binary_sequence(string&& filename, string& binary_sequence) {
    ofstream file(filename);
    file.clear();
    file << binary_sequence;
    file.close();
}


string generate_parity_bits(string& data) {
    string parity_bits;

    for (ushort i = 0; i < 9; ++i) {
        ushort parity = 0;
        for (ushort j = 0; j < data.length(); ++j)
            if ((j + 1) & (2 << (i + 1)) != 0)
            parity ^= (int)('0' - data.at(j));
        parity_bits += to_string(parity);
    }

    return parity_bits;
}


string add_parity_bits(string& data) {
    auto parity_bits = generate_parity_bits(data);
    return data + parity_bits;
}


recieved check_and_correct_errors(string& received_data) {
    ushort error_bit = 0;
    for (ushort i = 0; i < 9; ++i) {
        ushort parity = 0;
        for (ushort j = 0; j < received_data.length(); ++j)
            if ((j + 1) & (2 << (i + 1)) != 0)
                parity ^= (int)('0' - received_data.at(j));
        if (parity != 0)
            error_bit += (2 << (i + 1));
    }

    if (error_bit != 0) {
        received_data.data()[error_bit - 1] = 1 - (int)('0' - received_data.at(error_bit - 1));
    }

    return {received_data, error_bit};
}


int main() {
    auto sequence = read_binary_sequence("input3_test.txt");
    sequence = add_parity_bits(sequence);
    write_binary_sequence("output.txt", sequence);

    auto received_sequence = read_binary_sequence("received3_test.txt");
    auto received = check_and_correct_errors(received_sequence);

    cout << get<ushort>(received) << '|' << get<string>(received) << '|' << endl;
}
