#pragma once

#include "duckdb.hpp"

extern "C" {
#include "postgres.h"
#include "executor/tuptable.h"
}

#include "quack/quack_heap_seq_scan.hpp"

namespace quack {

// DuckDB has date starting from 1/1/1970 while PG starts from 1/1/2000
constexpr int32_t QUACK_DUCK_DATE_OFFSET = 10957;
constexpr int64_t QUACK_DUCK_TIMESTAMP_OFFSET = INT64CONST(10957) * USECS_PER_DAY;

duckdb::LogicalType ConvertPostgresToDuckColumnType(Oid type, int32_t typmod);
Oid GetPostgresDuckDBType(duckdb::LogicalTypeId type);
void ConvertPostgresToDuckValue(Datum value, duckdb::Vector &result, idx_t offset);
void ConvertDuckToPostgresValue(TupleTableSlot *slot, duckdb::Value &value, idx_t col);
void InsertTupleIntoChunk(duckdb::DataChunk &output, PostgresHeapSeqScanThreadInfo &threadScanInfo,
                          PostgresHeapSeqParallelScanState &parallelScanState);

} // namespace quack