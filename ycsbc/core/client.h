//
//  client.h
//  YCSB-C
//
//  Created by Jinglei Ren on 12/10/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//
#pragma once

#include <string>
#include <utility>

#include "core_workload.h"
#include "db.h"
#include "utils.h"

namespace ycsbc {

class Client {
 public:
  Client(DB& db, CoreWorkload& wl) : db_(db), workload_(wl) {}

  virtual bool DoInsert();
  virtual bool DoTransaction();

  virtual ~Client() = default;

 protected:
  virtual int TransactionRead();
  virtual int TransactionReadModifyWrite();
  virtual int TransactionScan();
  virtual int TransactionUpdate();
  virtual int TransactionInsert();

  DB& db_;
  CoreWorkload& workload_;
};

inline bool Client::DoInsert() {
  std::string key = workload_.NextSequenceKey();
  std::vector<DB::KVPair> pairs;
  workload_.BuildValues(pairs);
  return db_.Insert(workload_.NextTable(), key, pairs) == DB::kOK;
}

inline bool Client::DoTransaction() {
  int status;
  switch (workload_.NextOperation()) {
    case READ:
      status = TransactionRead();
      break;
    case UPDATE:
      status = TransactionUpdate();
      break;
    case INSERT:
      status = TransactionInsert();
      break;
    case SCAN:
      status = TransactionScan();
      break;
    case READMODIFYWRITE:
      status = TransactionReadModifyWrite();
      break;
    default:
      throw utils::Exception("Operation request is not recognized!");
  }
  assert(status >= 0);
  return (status == DB::kOK);
}

inline int Client::TransactionRead() {
  const std::string& table = workload_.NextTable();
  const std::string& key = workload_.NextTransactionKey();

  std::vector<DB::KVPair> result;
  if (!workload_.read_all_fields()) {
    std::vector<std::string> fields;
    fields.push_back("field" + workload_.NextFieldName());
    return db_.Read(table, key, &fields, result);
  } else {
    return db_.Read(table, key, nullptr, result);
  }
}

inline int Client::TransactionReadModifyWrite() {
  const std::string& table = workload_.NextTable();
  const std::string& key = workload_.NextTransactionKey();

  std::vector<DB::KVPair> result;
  if (!workload_.read_all_fields()) {
    std::vector<std::string> fields;
    fields.push_back("field" + workload_.NextFieldName());
    db_.Read(table, key, &fields, result);
  } else {
    db_.Read(table, key, nullptr, result);
  }

  std::vector<DB::KVPair> values;
  if (workload_.write_all_fields()) {
    workload_.BuildValues(values);
  } else {
    workload_.BuildUpdate(values);
  }
  return db_.Update(table, key, values);

}

inline int Client::TransactionScan() {
  const std::string& table = workload_.NextTable();
  const std::string& key = workload_.NextTransactionKey();
  int len = workload_.NextScanLength();
  std::vector<std::vector<DB::KVPair>> result;
  if (!workload_.read_all_fields()) {
    std::vector<std::string> fields;
    fields.push_back("field" + workload_.NextFieldName());
    return db_.Scan(table, key, len, &fields, result);
  } else {
    return db_.Scan(table, key, len, nullptr, result);
  }
}

inline int Client::TransactionUpdate() {
  const std::string& table = workload_.NextTable();
  const std::string& key = workload_.NextTransactionKey();
  std::vector<DB::KVPair> values;
  if (workload_.write_all_fields()) {
    workload_.BuildValues(values);
  } else {
    workload_.BuildUpdate(values);
  }
  return db_.Update(table, key, values);
}

inline int Client::TransactionInsert() {
  const std::string& table = workload_.NextTable();
  const std::string& key = workload_.NextSequenceKey();
  std::vector<DB::KVPair> values;
  workload_.BuildValues(values);
  return db_.Insert(table, key, values);
}

}  // namespace ycsbc

