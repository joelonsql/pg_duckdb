#pragma once

#include "duckdb/common/exception.hpp"
#include "duckdb/common/error_data.hpp"
#include "pgduckdb/pgduckdb_duckdb.hpp"

extern "C" {
#include "postgres.h"
}

#include <vector>
#include <string>
#include <sstream>

namespace pgduckdb {

inline std::vector<std::string>
TokenizeString(char *str, const char delimiter) {
	std::vector<std::string> v;
	std::stringstream ss(str); // Turn the string into a stream.
	std::string tok;
	while (getline(ss, tok, delimiter)) {
		v.push_back(tok);
	}
	return v;
};

/*
 * DuckdbGlobalLock should be held before calling.
 */
template <typename Fn, Fn fn, typename... FuncArgs>
typename std::invoke_result<Fn, FuncArgs...>::type
__PostgresFunctionGuard__(const char *func_name, FuncArgs... args) {
	MemoryContext ctx = CurrentMemoryContext;
	ErrorData *edata = nullptr;
	// clang-format off
	PG_TRY();
	{
		return fn(std::forward<FuncArgs>(args)...);
	}
	PG_CATCH();
	{
		MemoryContextSwitchTo(ctx);
		edata = CopyErrorData();
		FlushErrorState();
	}
	PG_END_TRY();
	// clang-format on
	if (edata) {
		throw duckdb::Exception(duckdb::ExceptionType::EXECUTOR, edata->message);
	}

	std::abort(); // unreacheable
}

#define PostgresFunctionGuard(FUNC, ...) pgduckdb::__PostgresFunctionGuard__<decltype(&FUNC), &FUNC>("##FUNC##", __VA_ARGS__)

template <typename FuncRetT, typename FuncType, typename... FuncArgs>
FuncRetT
DuckDBFunctionGuard(FuncType duckdb_function, const char *function_name, FuncArgs... args) {
	const char *error_message = nullptr;
	try {
		return duckdb_function(args...);
	} catch (duckdb::Exception &ex) {
		duckdb::ErrorData edata(ex.what());
		error_message = pstrdup(edata.Message().c_str());
	} catch (std::exception &ex) {
		const auto msg = ex.what();
		if (msg[0] == '{') {
			duckdb::ErrorData edata(ex.what());
			error_message = pstrdup(edata.Message().c_str());
		} else {
			error_message = pstrdup(ex.what());
		}
	}

	if (error_message) {
		elog(ERROR, "(PGDuckDB/%s) %s", function_name, error_message);
	}

	std::abort(); // Cannot reach.
}

std::string CreateOrGetDirectoryPath(std::string directory_name);

duckdb::unique_ptr<duckdb::QueryResult> DuckDBQueryOrThrow(duckdb::ClientContext &context, const std::string &query);

duckdb::unique_ptr<duckdb::QueryResult> DuckDBQueryOrThrow(duckdb::Connection &connection, const std::string &query);

duckdb::unique_ptr<duckdb::QueryResult> DuckDBQueryOrThrow(const std::string &query);

bool TryDuckDBQuery(duckdb::ClientContext &context, const std::string &query);
bool TryDuckDBQuery(const std::string &query);

} // namespace pgduckdb
