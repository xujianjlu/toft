// Copyright (c) 2013, The Toft Authors.
// All rights reserved.
//
// Author: Ye Shunping <yeshunping@gmail.com>

#include "toft/base/closure.h"
#include "toft/base/stl_util.h"
#include "toft/base/string/format.h"
#include "toft/base/string/number.h"
#include "toft/storage/file/file.h"
#include "toft/storage/sstable/sstable.h"
#include "toft/storage/sstable/sstable_reader.h"
#include "toft/storage/sstable/sstable_writer.h"
#include "toft/storage/sstable/test/test_util.h"
#include "toft/storage/sstable/types.h"

#include "thirdparty/glog/logging.h"

using namespace toft;

DEFINE_int32(test_num, 1000000, "");

void TestSSTableWriter(SSTableWriter *builder,
                       const std::string &sstable_path,
                       int key_mod,
                       SSTableReader::ReadMode type) {
  LOG(INFO)<< "Start build sstable ...";
  for (int i = 0; i < FLAGS_test_num; ++i) {
    std::string key = GenKey(i, key_mod);
    std::string value = GenValue(i, key_mod);
    builder->AddOrDie(key, value);
    if (i % 10000 == 0) {
      LOG(INFO) << "write num:" << i;
    }
  }
  LOG(INFO) << "Start flush ...";
  builder->Flush();
  LOG(INFO) << "Finish flush!";
}

int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, false);

  SSTableWriteOption option;
  std::string path = "/tmp/test_single.sstable";
  option.set_path(path);
  SingleSSTableWriter builder(option);
  TestSSTableWriter(&builder, path, kMaxLength,
                    SSTableReader::ON_DISK);
  LOG(INFO) << "Done";
  return 0;
}
