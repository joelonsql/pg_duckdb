# pg_duckdb Functions

By default, functions without a schema listed below are installed into `public`. You can choose to install these functions to an alternate location by running `CREATE EXTENSION pg_duckdb WITH SCHEMA schema`.

Note: `ALTER EXTENSION pg_duckdb WITH SCHEMA schema` is not currently supported.

## Data Lake Functions

| Name | Description |
| :--- | :---------- |
| [`read_parquet`](#read_parquet) | Read a parquet file |
| [`read_csv`](#read_csv) | Read a CSV file |
| [`iceberg_scan`](#iceberg_scan) | Read an Iceberg dataset |
| [`iceberg_metadata`](#iceberg_metadata) | Read Iceberg metadata |
| [`iceberg_snapshots`](#iceberg_snapshots) | Read Iceberg snapshot information |

## DuckDB Administration Functions

| Name | Description |
| :--- | :---------- |
| [`duckdb.cache`](#cache) | Caches a Parquet or CSV file to disk |
| [`duckdb.install_extension`](#install_extension) | Installs a DuckDB extension |
| [`duckdb.raw_query`](#raw_query) | Runs a query directly against DuckDB |
| [`duckdb.recycle_ddb`](#recycle_ddb) | TODO |

## Motherduck Functions

| Name | Description |
| :--- | :---------- |
| [`duckdb.force_motherduck_sync`](#force_motherduck_sync) | Forces a full resync of Motherduck databases and schemas to Postgres |

## Detailed Descriptions

#### <a name="read_parquet"></a>`read_parquet(path TEXT or TEXT[], /* optional parameters */)`

Reads a parquet file, either from a remote location (via httpfs) or a local file.

Returns a record set (`SETOF record`). Functions that return record sets need to have their columns and types specified using `AS`. You must specify at least one column and any columns used in your query. For example:

```sql
SELECT COUNT(i) FROM read_parquet('file.parquet') AS (int i);
```
Further information:

* [DuckDB Parquet documentation](https://duckdb.org/docs/data/parquet/overview)
* [DuckDB httpfs documentation](https://duckdb.org/docs/extensions/httpfs/https.html)


##### Required Arguments

| Name | Type | Description |
| :--- | :--- | :---------- |
| path | text or text[] | The path, either to a remote httpfs file or a local file (if enabled), of the parquet file(s) to read. The path can be a glob or array of files to read. |

##### Optional Parameters

Optional parameters mirror [DuckDB's read_parquet function](https://duckdb.org/docs/data/parquet/overview.html#parameters). To specify optional parameter, use `parameter := 'value'`.

#### <a name="read_csv"></a>`read_csv(path TEXT or TEXT[], /* optional parameters */)`

Reads a CSV file, either from a remote location (via httpfs) or a local file.

Returns a record set (`SETOF record`). Functions that return record sets need to have their columns and types specified using `AS`. You must specify at least one column and any columns used in your query. For example:

```sql
SELECT COUNT(i) FROM read_csv('file.csv') AS (int i);
```

Further information:

* [DuckDB CSV documentation](https://duckdb.org/docs/data/csv/overview)
* [DuckDB httpfs documentation](https://duckdb.org/docs/extensions/httpfs/https.html)

##### Required Arguments

| Name | Type | Description |
| :--- | :--- | :---------- |
| path | text or text[] | The path, either to a remote httpfs file or a local file (if enabled), of the CSV file(s) to read. The path can be a glob or array of files to read. |

##### Optional Parameters

Optional parameters mirror [DuckDB's read_csv function](https://duckdb.org/docs/data/csv/overview.html#parameters). To specify optional parameter, use `parameter := 'value'`.

Compatibility notes:

* `columns` is not currently supported.
* `nullstr` must be an array (`TEXT[]`).
