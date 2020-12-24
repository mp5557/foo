#include <fstream>
#include <iostream>

#include <array>
#include <map>
#include <memory>
#include <vector>

#undef NDEBUG
#include <assert.h>

class Dumper {
  std::ofstream f_data_;
  std::ofstream f_index_;

 public:
  Dumper(std::string const &fname)
      : f_data_{fname + "-data.bin", std::ios::binary},
        f_index_{fname + "-index.txt"} {}

  void Dump(std::string const &key, char const *data, size_t size) {
    auto pos = f_data_.tellp();
    f_index_ << key << ' ' << pos << '\n';
    f_data_.write(data, size);
  }
};

class Loader {
 public:
  std::map<std::string, std::pair<size_t, size_t>> key_map_;
  std::ifstream f_data_;

 public:
  Loader(std::string const &fname) : f_data_{fname + "-data.bin"} {
    std::ifstream f_index{fname + "-index.txt"};

    std::vector<size_t> pos_list;
    std::vector<std::string> key_list;
    size_t pos;
    std::string key;
    while (f_index >> key >> pos) {
      pos_list.push_back(pos);
      key_list.push_back(key);
    }
    f_data_.seekg(0, std::ios_base::end);
    pos_list.push_back(f_data_.tellg());

    if (pos_list.empty())
      return;

    for (int i = 0; i < key_list.size(); i++) {
      int size = pos_list[i + 1] - pos_list[i];
      key_map_[key_list[i]] = std::make_pair(pos_list[i], size);
    }
  }

  std::vector<std::string> GetAllKeys() const {
    std::vector<std::string> key_list;
    for (auto const &item : key_map_)
      key_list.push_back(item.first);
    return key_list;
  }

  std::vector<char> Load(std::string const &key) {
    auto iter = key_map_.find(key);
    if (iter == key_map_.end())
      return {};

    size_t pos, size;
    std::tie(pos, size) = iter->second;
    f_data_.seekg(pos);

    std::vector<char> value(size);
    f_data_.read(value.data(), size);
    return value;
  }

  size_t Load(std::string const &key, char *data, int capacity) {
    auto iter = key_map_.find(key);
    if (iter == key_map_.end())
      return 0;

    size_t pos, size;
    std::tie(pos, size) = iter->second;
    
    if (size <= capacity) {
      f_data_.seekg(pos);
      f_data_.read(data, size);
    }
    return size;
  }
};

void TestWrite(std::string fname) {
  Dumper dumper(fname);
  std::string data1 = "hello world";
  dumper.Dump("foo", data1.data(), data1.size());

  uint64_t x = 19910413;
  dumper.Dump("foobar", (char*)&x, sizeof(x));
}

void TestRead(std::string fname) {
  Loader loader(fname);

  size_t num;
  size_t size = loader.Load("foobar", (char*)&num, sizeof(size_t));
  std::cout << "num = " << num << " size = " << size << '\n';

  auto data0 = loader.Load("foo");
  std::cout << std::string(data0.data(), data0.size()) << '\n';
}

int main() {
  TestWrite("foo_db");
  TestRead("foo_db");
}