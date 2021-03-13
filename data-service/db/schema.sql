CREATE TABLE "holdings" (
	"fund_id"	TEXT,
	"ticker"	TEXT,
	"quantity"	REAL,
	"holding_date"  TEXT,
	PRIMARY KEY("fund_id", "ticker", "holding_date")
);
