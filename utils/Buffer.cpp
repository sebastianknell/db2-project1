//
// Created by Sebastian Knell on 23/05/21.
//

#include "Buffer.h"

Buffer::Buffer(char delim, int max_b) {
    delimeter = delim;
    if (max_b < 0) max_b = 0;
    max_bytes = max_b;
    buffer = new char[max_bytes];
    buffer_size = 0;
    next_byte = 0;
}

void Buffer::clear() {
    next_byte = 0;
    buffer_size = 0;
}

bool Buffer::read(ifstream &stream) {
    clear();
    stream.read((char *)&buffer_size, sizeof(buffer_size));
    if (stream.fail()) return false;
    if (buffer_size > max_bytes) return false;
    stream.read(buffer, buffer_size);
    return stream.good();
}

bool Buffer::write(ofstream &stream) const {
    stream.write((char *)&buffer_size, sizeof(buffer_size));
    stream.write(buffer, buffer_size);
    return stream.good();
}

bool Buffer::pack(const char *str, int size) {
    short len = 0;
    if (size >= 0) len = size;
    else len = strlen(str);
    if (len > strlen(str)) return false;
    int start = next_byte;
    next_byte += len + 1;
    if (next_byte > max_bytes) return false;
    memcpy(&buffer[start], str, len);
    buffer[start + len] = delimeter;
    buffer_size = next_byte;
    return true;
}

bool Buffer::unpack(char *str) {
    int len = -1;
    int start = next_byte;
    for (int i = start; i < buffer_size; i++) {
        if (buffer[i] == delimeter) {
            len = i - start;
            break;
        }
    }
    if (len == -1) return false;
    next_byte += len + 1;
    if (next_byte > max_bytes) return false;
    strncpy(str, &buffer[start], len);
    str[len] = 0;
    return true;
}

void Buffer::print_buffer() {
    cout << buffer;
    cout << endl;
}
