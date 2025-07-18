#pragma once

#include <duckdb.h>

#include <cstdint>
#include <format>
#include <memory>
#include <span>
#include <vector>

#include "storage/mapper/duckorm/duckdb_orm.hpp"
#include "storage/mapper/duckorm/duckdb_types.hpp"

namespace puerhlab {
template <typename Derived, typename Mappable, typename ID>
class MapperInterface {
 public:
  duckdb_connection& _conn;

  MapperInterface(duckdb_connection& conn) : _conn(conn) {}
  void Insert(const Mappable&& obj) {
    duckorm::insert(_conn, Derived::TableName(), &obj, Derived::FieldDesc(), Derived::FieldCount());
  }
  void Remove(const ID remove_id) {
    std::string remove_clause = std::format(Derived::PrimeKeyClause(), remove_id);
    duckorm::remove(_conn, Derived::TableName(), remove_clause.c_str());
  }
  void RemoveByClause(const std::string& predicate) {
    duckorm::remove(_conn, Derived::TableName(), predicate.c_str());
  }
  auto Get(const char* where_clause) -> std::vector<Mappable> {
    auto                  raw = duckorm::select(_conn, Derived::TableName(), Derived::FieldDesc(),
                                                Derived::FieldCount(), where_clause);
    std::vector<Mappable> result;
    for (auto& row : raw) {
      result.emplace_back(Derived::FromRawData(std::move(row)));
    }
    return result;
  }
  void Update(const ID target_id, const Mappable&& updated) {
    std::string where_clause = std::format(Derived::PrimeKeyClause(), target_id);
    duckorm::update(_conn, Derived::TableName(), &updated, Derived::FieldDesc(),
                    Derived::FieldCount(), where_clause.c_str());
  }
};

// Don't understand what heck this is... They call it CRTP (C++ Recurring Tremendous Pain, maybe).
template <typename Derived>
struct FieldReflectable {
 public:
  using FieldArrayType = std::span<const duckorm::DuckFieldDesc>;
  static constexpr FieldArrayType FieldDesc() { return Derived::_field_descs; }
  static constexpr uint32_t       FieldCount() { return Derived::_field_count; }
  static constexpr const char*    TableName() { return Derived::_table_name; }
  static constexpr const char*    PrimeKeyClause() { return Derived::_prime_key_clause; }
};
};  // namespace puerhlab